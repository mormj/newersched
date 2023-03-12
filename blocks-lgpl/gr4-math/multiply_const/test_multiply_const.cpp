#include <benchmark.hpp>
#include <bm_test_helper.hpp>
#include <graph.hpp>

#include "multiply_const.hpp"

namespace fg = fair::graph;

int main() {
  fg::graph flow_graph;
  auto &src = flow_graph.make_node<test::source<float>>(1000);
  auto &snk = flow_graph.make_node<test::sink<float>>();
  auto &mult = flow_graph.make_node<gr::math::multiply_const<float>>(100.0);

  auto r = flow_graph.connect<"out">(src).template to<"in">(mult);
  r = flow_graph.connect<"out">(mult).template to<"in">(snk);

  auto token = flow_graph.init();
  flow_graph.work(token);
}