#include <pybind11/pybind11.h>

#include "ImgEngine.h"
#include "Sim_ImgEngine.h"

namespace py = pybind11;

// py::class_<Pet>(m, "Pet")
//    .def(py::init<const std::string &>())
//    .def_readwrite("name", &Pet::name);
//
// py::class_<Dog>(m, "Dog", py::base<Pet>() /* <- specify C++ parent type */)
//     .def(py::init<const std::string &>())
//     .def("bark", &Dog::bark);
//
//
// ...or
//
// py::class_<Pet> pet(m, "Pet");
// pet.def(py::init<const std::string &>())
//    .def_readwrite("name", &Pet::name);
//
// py::class_<Dog>(m, "Dog", pet /* <- specify Python parent type */)
//     .def(py::init<const std::string &>())
//     .def("bark", &Dog::bark);


PYBIND11_PLUGIN(dif) {
    py::module m("dif", "Drop In Flight Module");


    class PyImgEngine : public ImgEngine {
    public:
        /* Inherit the constructors */
        using ImgEngine::ImgEngine;

        /* Trampoline (need one for each virtual function) */
        bool init(std::string config) {
            PYBIND11_OVERLOAD_PURE(
                bool, /* Return type */
                ImgEngine,      /* Parent class */
                init,          /* Name of function */
                config      /* Argument(s) */
            );
        }

        bool next2() {
            PYBIND11_OVERLOAD_PURE(
                bool, /* Return type */
                ImgEngine,      /* Parent class */
                next          /* Name of function */
            );
        }

        py::array_t<unsigned short> next() {
            PYBIND11_OVERLOAD_PURE(
                py::array_t<unsigned short>, /* Return type */
                ImgEngine,      /* Parent class */
                next          /* Name of function */
            );
        }

        int getImageSize() {
            PYBIND11_OVERLOAD_PURE(
                int, /* Return type */
                ImgEngine,      /* Parent class */
                getImageSize          /* Name of function */
            );
        }

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
             .def("next2", &ImgEngine::next2)
             .def("next", &ImgEngine::next)
             .def("getPelSize_um", &ImgEngine::getPelSize_um)
             .def("getImageSize", &ImgEngine::getImageSize)
             .def("setColorMode", &ImgEngine::setColorMode)
             .def("getColorMode", &ImgEngine::getColorMode)
             .def("reset", &ImgEngine::reset)
             .def("cur_info", &ImgEngine::cur_info)
             .def("device_info", &ImgEngine::device_info);

    py::enum_<ImgEngine::ColorMode_t>(imgEngine, "ColorMode_t")
       .value("COLOR_MODE_DEFAULT", ImgEngine::ColorMode_t::COLOR_MODE_DEFAULT)
       .value("COLOR_MODE_RGB", ImgEngine::ColorMode_t::COLOR_MODE_RGB)
       .value("COLOR_MODE_RAW8", ImgEngine::ColorMode_t::COLOR_MODE_RAW8)
        .export_values();


   //  py::class_<ImgEngine>(m, "ImgEngine")
   //    .def(py::init<const std::string &, bool>())
   //    .def("getPelSize_um", &ImgEngine::getPelSize_um)
   //    .def("next", &ImgEngine::next)
   //    .def("getName", &ImgEngine::getImageSize)
   //    .def("cur_info", &ImgEngine::cur_info)
   //    .def("device_info", &ImgEngine::device_info);


   py::class_<Sim_ImgEngine> sim_ImgEngine(m, "Sim_ImgEngine", imgEngine);
   sim_ImgEngine.def(py::init<const std::string &, bool>())
                .def("init", &Sim_ImgEngine::init)
                .def("next2", &Sim_ImgEngine::next2)
                .def("next", &Sim_ImgEngine::next)
                .def("getName", &Sim_ImgEngine::getImageSize)
                .def("cur_info", &Sim_ImgEngine::cur_info)
                .def("device_info", &Sim_ImgEngine::device_info);

   py::enum_<Sim_ImgEngine::Record_Mode_t>(sim_ImgEngine, "Record_Mode_t")
      .value("RECORD_MODE_OFF", Sim_ImgEngine::Record_Mode_t::RECORD_MODE_OFF)
      .value("RECORD_MODE_ON", Sim_ImgEngine::Record_Mode_t::RECORD_MODE_ON)
       .export_values();

   // py::class_<Sim_ImgEngine>(m, "Sim_ImgEngine")
   //    .def(py::init<const std::string &, bool>())
   //    .def("init", &Sim_ImgEngine::init)
   //    .def("next", &Sim_ImgEngine::next)
   //    .def("getName", &Sim_ImgEngine::getImageSize)
   //    .def("cur_info", &Sim_ImgEngine::cur_info)
   //    .def("device_info", &Sim_ImgEngine::device_info);

    return m.ptr();
}
