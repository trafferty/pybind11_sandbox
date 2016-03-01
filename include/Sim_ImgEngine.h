/*
 * Sim_ImgEngine.h
 *
 *  Created on: Aug 24, 2015
 *      Author: trafferty
 */

#ifndef INCLUDE_SIMIMGENGINE_H_
#define INCLUDE_SIMIMGENGINE_H_

#include <string>
#include <memory>
#include <vector>

// #include "opencv2/core.hpp"
// #include "opencv2/opencv.hpp"

#include "ImgEngine.h"

class Sim_ImgEngine: public ImgEngine {
public:
    enum Record_Mode_t
    {
       RECORD_MODE_OFF = 0, RECORD_MODE_ON
    };

    Sim_ImgEngine(std::string name, bool debug = false);
   virtual ~Sim_ImgEngine();

   bool init(std::string config);

   bool next();

   int getImageSize();

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

   Record_Mode_t m_Record_Mode;

   bool setImageParams();

   unsigned int m_sim_drops_start;

};

#endif /* INCLUDE_SIMIMGENGINE_H_ */
