#pragma once

#include <constants.hpp>
#include <graph.hpp>
#include <vector>

namespace fg = fair::graph;
using namespace fair::literals;


namespace gr {
namespace basic {

inline static std::size_t n_samples_produced = 0_UZ;

template <typename T>
class vector_source : public fg::node<vector_source<T>>
{
private:
    std::vector<T> _data;
    std::size_t _n_samples_max;

public:
    struct block_args {
        std::vector<T> data;
        bool repeat = false;
        size_t vlen = 1;
    };
//port(std::string port_name, std::int16_t priority = 0, std::size_t min_samples = 0_UZ, std::size_t max_samples = SIZE_MAX) noexcept
    fg::OUT<T> out;

    vector_source() = delete;

    vector_source(const block_args& args)
        : _data(args.data), _n_samples_max(args.data.size())
    {
    }

    friend constexpr std::size_t available_samples(const vector_source& self) noexcept
    {
        return self._n_samples_max - n_samples_produced;
    }

    [[nodiscard]] constexpr T process_one() const noexcept
    {
        n_samples_produced++;
        T x{};
        return x;
    }

    fair::graph::work_return_t work()
    {
        const std::size_t n_to_publish = _n_samples_max - n_samples_produced;
        if (n_to_publish > 0) {
            auto& port = output_port<"out">(this);
            auto& writer = port.writer();

            std::size_t n_write =
                std::clamp(n_to_publish,
                            0UL,
                            std::min(writer.available(), port.max_buffer_size()));
            if (n_write == 0_UZ) {
                return fair::graph::work_return_t::INSUFFICIENT_INPUT_ITEMS;
            }

            writer.publish( //
                [this](std::span<T> output) {
                    for (auto& val : output) {
                        val = process_one();
                    }
                },
                n_write);

            return fair::graph::work_return_t::OK;
        }
        else {
            return fair::graph::work_return_t::DONE;
        }
    }
};

} // namespace basic
} // namespace gr

ENABLE_REFLECTION_FOR_TEMPLATE_FULL(
    (typename T, std::size_t min, std::size_t count, bool use_bulk_operation),
    (gr::basic::vector_source<T>),
    out);