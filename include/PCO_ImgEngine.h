/*
 * PCO_ImgEngine.h
 *
 *  Created on: Aug 24, 2015
 *      Author: trafferty
 */

#ifndef INCLUDE_PCOIMGENGINE_H_
#define INCLUDE_PCOIMGENGINE_H_

#include <string>
#include <memory>
#include <vector>

// pco sdk:
#include "Cpco_com.h"
#include "Cpco_grab_usb.h"
#include "Ccambuf.h"

//#define PCO_ERRT_H_CREATE_OBJECT
//#pragma GCC diagnostic ignored "-Wwrite-strings"
//#include "PCO_errt.h"
//#pragma GCC diagnostic pop

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "ImgEngine.h"

class PCO_ImgEngine: public ImgEngine {
public:

   PCO_ImgEngine(std::string name, bool debug = false);
   virtual ~PCO_ImgEngine();

   bool init(const std::map<std::string, double> &config);

   py::array_t<unsigned short> next();

   std::tuple<int, int> getImageSize();

   std::string cur_info();

   std::string device_info();

protected:
   int m_device_num;
   int m_exposure_us;
   int m_delay_us;
   double m_conversion_factor;
   unsigned int m_height_px;
   unsigned int m_width_px;
   int m_offset_x_px;
   int m_offset_y_px;
   int m_framerate_hz;
   int m_defaultImageDataFormat;
   int m_lostframes;
   bool m_isColor;

   int m_binh, m_binv;
   int m_trigger_mode;
   int m_doubleImage_mode;
   int m_timestamp_mode;
   int m_bit_shift;
   SC2_Camera_Description_Response m_description;

   std::unique_ptr<CPco_com_usb>  m_Camera;
   std::unique_ptr<CPco_grab_usb> m_Grabber;
   std::unique_ptr<CCambuf> m_Cbuf;
   std::shared_ptr<CPco_Log> m_CPco_Log;

   Record_Mode_t m_Record_Mode;

   bool setImageParams();

   int image_num_from_timestamp(void *buf, int shift);
   std::string getPCOErrorStr(DWORD dwerr);
   void printPCOErrorMsg(std::string funcName, DWORD dwerr);
};

#endif /* INCLUDE_XIMEAIMGENGINE_H_ */
