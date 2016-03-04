#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "ImgEngine.h"
#include "Sim_ImgEngine.h"
#include "PCO_ImgEngine.h"

namespace py = pybind11;

PYBIND11_PLUGIN(dif) {
    py::module m("dif", "Drop In Flight Module");

    class PyImgEngine : public ImgEngine {
    public:
        /* Inherit the constructors */
        using ImgEngine::ImgEngine;

        /* Trampoline (need one for each virtual function) */
        bool init(const std::map<std::string, double> &config) {
            PYBIND11_OVERLOAD_PURE(
                bool, /* Return type */
                ImgEngine,      /* Parent class */
                init,          /* Name of function */
                config      /* Argument(s) */
            );
        }

        py::array_t<unsigned short> next() {
            PYBIND11_OVERLOAD_PURE(
                py::array_t<unsigned short>, /* Return type */
                ImgEngine,      /* Parent class */
                next          /* Name of function */
            );
        }

      //   std::tuple<int, int> getImageSize() {
      //       PYBIND11_OVERLOAD_PURE(
      //           std::tuple<int, int> , /* Return type */
      //           ImgEngine,      /* Parent class */
      //           getImageSize          /* Name of function */
      //       );
      //   }

        std::string cur_info() {
            PYBIND11_OVERLOAD_PURE(
                std::string, /* Return type */
                ImgEngine,      /* Parent class */
                cur_info          /* Name of function */
            );
        }

        std::string device_info() {
            PYBIND11_OVERLOAD_PURE(
                std::string, /* Return type */
                ImgEngine,      /* Parent class */
                device_info          /* Name of function */
            );
        }
    };

   py::class_<PyImgEngine> imgEngine(m, "ImgEngine");
   imgEngine
      .alias<ImgEngine>()
      .def(py::init<const std::string &, bool>())
      .def("init", &ImgEngine::init)
      .def("next", &ImgEngine::next)
      .def("getPelSize_um", &ImgEngine::getPelSize_um)
      .def("getImageSize", &ImgEngine::getImageSize)
      .def("setColorMode", &ImgEngine::setColorMode)
      .def("getColorMode", &ImgEngine::getColorMode)
      .def("reset", &ImgEngine::reset)
      .def("cur_info", &ImgEngine::cur_info)
      .def("getName", &ImgEngine::getName)
      .def("device_info", &ImgEngine::device_info)
      .def_readwrite("img_idx", &ImgEngine::img_idx);

   py::enum_<ImgEngine::ColorMode_t>(imgEngine, "ColorMode_t")
      .value("COLOR_MODE_DEFAULT", ImgEngine::ColorMode_t::COLOR_MODE_DEFAULT)
      .value("COLOR_MODE_RGB", ImgEngine::ColorMode_t::COLOR_MODE_RGB)
      .value("COLOR_MODE_RAW8", ImgEngine::ColorMode_t::COLOR_MODE_RAW8)
      .export_values();

   py::enum_<ImgEngine::Record_Mode_t>(imgEngine, "Record_Mode_t")
      .value("RECORD_MODE_OFF", ImgEngine::Record_Mode_t::RECORD_MODE_OFF)
      .value("RECORD_MODE_ON", ImgEngine::Record_Mode_t::RECORD_MODE_ON)
      .export_values();

   py::class_<Sim_ImgEngine> sim_ImgEngine(m, "Sim_ImgEngine", imgEngine);
   sim_ImgEngine.def(py::init<const std::string &, bool>())
                .def("init", &Sim_ImgEngine::init)
                .def("next", &Sim_ImgEngine::next)
                .def("getImageSize", &Sim_ImgEngine::getImageSize)
                .def("getName", &Sim_ImgEngine::getName)
                .def("cur_info", &Sim_ImgEngine::cur_info)
                .def("device_info", &Sim_ImgEngine::device_info);

   py::class_<PCO_ImgEngine> pco_ImgEngine(m, "PCO_ImgEngine", imgEngine);
   pco_ImgEngine.def(py::init<const std::string &, bool>())
              .def("init", &PCO_ImgEngine::init)
              .def("next", &PCO_ImgEngine::next)
              .def("getImageSize", &PCO_ImgEngine::getImageSize)
              .def("getName", &PCO_ImgEngine::getName)
              .def("cur_info", &PCO_ImgEngine::cur_info)
              .def("device_info", &PCO_ImgEngine::device_info);


    return m.ptr();
}
