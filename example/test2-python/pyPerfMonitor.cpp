#include <pybind11/pybind11.h>
#include <PerfMonitor.h>

namespace py = pybind11;
// in python, the PerfMonitor class should be imported as
//  import pyPerfMonitor as pmlib

using namespace pm_lib;

PYBIND11_MODULE(pyPerfMonitor, m) {

    py::class_<PerfMonitor>(m, "PerfMonitor")
	.def(py::init<>())
    //  pmlib.doc() = "pybind11 binding to pmlib";
    .def("initialize", &PerfMonitor::initialize, py::arg())
    .def("start", &PerfMonitor::start,  py::arg())
	.def("stop",  &PerfMonitor::stop, py::arg("label")="default", py::arg("flopPerTask")=0.0, py::arg("iterationCount")=1)
	//	if only the first string argument is used.
    //	.def("stop",  &stop,   py::arg(), py::arg(), py::arg());
    .def("report", &PerfMonitor::report, py::arg())
	//	.def("reportfile", &PerfMonitor::reportfile, py::arg())
    .def("setProperties", &PerfMonitor::setProperties, py::arg(), py::arg(), py::arg());
        // only the first string argument is used.
}


