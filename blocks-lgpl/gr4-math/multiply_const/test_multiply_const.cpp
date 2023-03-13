// #include <benchmark.hpp>
// #include <bm_test_helper.hpp>
#include <graph.hpp>
#include <iostream>

// FIXME: PATHS ////////////////////////////////////////////
////////////////////////////////////////////////////////////
#include <constant_source/constant_source.hpp>
#include <vector_sink/vector_sink.hpp>
////////////////////////////////////////////////////////////
#include "multiply_const.hpp"

namespace fg = fair::graph;

int main() {
  fg::graph flow_graph;
  auto &src = flow_graph.make_node<gr::basic::constant_source<float>>(gr::basic::constant_source<float>::block_args{11.0, 1, 1000});
  auto &snk = flow_graph.make_node<gr::basic::vector_sink<float>>();
  auto &mult = flow_graph.make_node<gr::math::multiply_const<float>>(100.0);

  auto r = flow_graph.connect<"out">(src).template to<"in">(mult);
  r = flow_graph.connect<"out">(mult).template to<"in">(snk);




  auto token = flow_graph.init();
  flow_graph.work(token);

  auto d = snk.data();

  std::cout << d.size() << std::endl;

  for (auto a : d) {
    std::cout << a << ",";
  }
  std::cout << std::endl;

}