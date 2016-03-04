/*
 * ImgEngine.h
 *
 *  Created on: Aug 24, 2015
 *      Author: trafferty
 */

#ifndef INCLUDE_IMGENGINE_H_
#define INCLUDE_IMGENGINE_H_

#include <string>
#include <memory>

//#include "opencv2/core.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

#include "CNT_JSON.h"
#include "Logger.h"
#include "matrix.h"

class ImgEngine {
public:
   enum ColorMode_t
   {
       COLOR_MODE_DEFAULT = 0, COLOR_MODE_RGB, COLOR_MODE_MONO, COLOR_MODE_RAW8
   };
   enum Record_Mode_t
   {
      RECORD_MODE_OFF = 0, RECORD_MODE_ON
   };

   ImgEngine(std::string name, bool debug = false);
   virtual ~ImgEngine();

   //virtual bool init(cJSON* config) = 0;
   virtual bool init(const std::map<std::string, double> &config) = 0;

   //virtual bool next(cv::Mat& frame) = 0;

   virtual py::array_t<unsigned short> next() = 0;

   virtual std::tuple<int, int> getImageSize();

   virtual std::string cur_info() = 0;

   virtual std::string device_info() = 0;

   virtual double getPelSize_um();

   virtual void setColorMode(ColorMode_t colorMode);
   virtual ColorMode_t getColorMode();

   virtual bool reset(ColorMode_t colorMode);

   uint img_idx;

   std::string getName();

protected:
    bool m_Debug;
    std::string m_Name;
    std::unique_ptr<Logger> m_Log;
    double m_pelSize;
    ColorMode_t m_colorMode;
};

#endif /* INCLUDE_IMGENGINE_H_ */
