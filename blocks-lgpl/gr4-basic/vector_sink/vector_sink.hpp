#pragma once

#include <constants.hpp>
#include <graph.hpp>
#include <vector>

namespace fg = fair::graph;
using namespace fair::literals;


namespace gr {
namespace basic {

inline static std::size_t n_samples_consumed = 0_UZ;

template <typename T>
class vector_sink : public fg::node<vector_sink<T>>
{
private:
    std::vector<T> _data;

    size_t _vlen;
    size_t _reserve_items;

public:
    struct block_args {
        size_t vlen = 1;
        size_t reserve_items = 1024;
    };

    fg::IN<T> in;

    vector_sink() : vector_sink(block_args{}) {}
    vector_sink(const block_args& args)
        : _vlen(args.vlen), _reserve_items(args.reserve_items)
    {
    }

    // template <fair::meta::t_or_simd<T> V>
    // [[nodiscard]] constexpr auto process_one(V a) const noexcept
    [[nodiscard]] constexpr auto process_one(T a) noexcept
    {
        n_samples_consumed++;
        _data.push_back(a);
    }

    std::vector<T>& data() { return _data; }
};


} // namespace basic
} // namespace gr

ENABLE_REFLECTION_FOR_TEMPLATE(gr::basic::vector_sink, in);