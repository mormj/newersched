

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include "multiply_const.hpp"

using namespace gr::math;

template <class T>
void bind_multiply_const_template(py::module &m, const char *classname) {
  // using block_class = gr::math::multiply_const<T>;

  py::class_<gr::math::multiply_const<T>, gr::sync_block, gr::block, gr::node,
             std::shared_ptr<gr::math::multiply_const<T>>>
      multiply_const_class(m, classname);

  multiply_const_class
      .def(py::init(
               [](flowgraph& fg, T k) {
                 return ::gr::math::multiply_const<T>::make({k,}, impl);
               }),

           py::arg("k"))


      .def("set_k", &gr::math::multiply_const<T>::set_k)

      .def("k", &gr::math::multiply_const<T>::k)
}

void bind_multiply_const(py::module &m) {

  bind_multiply_const_template<std::complex<float>>(m, "multiply_const_cc");
  bind_multiply_const_template<float>(m, "multiply_const_ff");
  bind_multiply_const_template<int32_t>(m, "multiply_const_ii");
  bind_multiply_const_template<int16_t>(m, "multiply_const_ss");
}