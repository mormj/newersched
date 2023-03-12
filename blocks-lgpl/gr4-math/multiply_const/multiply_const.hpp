#include <graph.hpp>

namespace fg = fair::graph;



namespace gr {
namespace math {

inline constexpr std::size_t N_MAX = std::numeric_limits<std::size_t>::max();

template <typename T>
class multiply_const : public fg::node<multiply_const<T>, fg::IN<T, 0, N_MAX, "in">, fg::OUT<T, 0, N_MAX, "out">> {
public:
  explicit multiply_const(T k) : _k(k) {}
//   fg::IN<T> in;
//   fg::OUT<T> out;

  template <fair::meta::t_or_simd<T> V>
  [[nodiscard]] constexpr V process_one(V a) const noexcept {
    return a * _k;
  }

private:
  T _k = static_cast<T>(1.0f);
};

} // namespace math
} // namespace gr