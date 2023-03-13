#pragma once

#include <constants.hpp>
#include <graph.hpp>

namespace fg = fair::graph;
using namespace fair::literals;

namespace gr {
namespace basic {

inline static std::size_t n_samples_produced = 0_UZ;

template <typename T>
class constant_source : public fg::node<constant_source<T>>
{
private:
    T _value;
    std::size_t _n_items_max;

    std::size_t _n_items_produced = 0;

public:
    struct block_args {
        T value;
        size_t vlen = 1;
        size_t n_items_max = 0;
    };
    // port(std::string port_name, std::int16_t priority = 0, std::size_t min_samples =
    // 0_UZ, std::size_t max_samples = SIZE_MAX) noexcept
    fg::OUT<T> out;

    // constant_source() = delete;

    constant_source(const block_args& args)
        : _value(args.value), _n_items_max(args.n_items_max)
    {
    }

    friend constexpr std::size_t available_samples(const constant_source& self) noexcept
    {
        return self._n_samples_max - self._n_items_produced;
    }

    [[nodiscard]] constexpr T process_one() const noexcept
    {
        n_samples_produced++;
        T x{ _value };
        return x;
    }

    fair::graph::work_return_t work()
    {
        const std::size_t n_to_publish = _n_items_max - _n_items_produced;
        if (n_to_publish > 0) {
            auto& port = output_port<"out">(this);
            auto& writer = port.writer();

            std::size_t n_write = std::clamp(
                n_to_publish, 0UL, std::min(writer.available(), port.max_buffer_size()));
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

            _n_items_produced += n_write;
            return fair::graph::work_return_t::OK;
        }
        else {
            return fair::graph::work_return_t::DONE;
        }
    }
};

} // namespace basic
} // namespace gr

// ENABLE_REFLECTION(gr::basic::constant_source<float>::block_args);
ENABLE_REFLECTION_FOR_TEMPLATE(gr::basic::constant_source, out);