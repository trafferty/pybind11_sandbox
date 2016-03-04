/*
 * Sim_ImgEngine.cpp
 *
 *  Created on: Sep 24, 2015
 *      Author: trafferty
 */
#include <unistd.h>

#include "Sim_ImgEngine.h"
#include "ip_routines.h"

//#include "matrix.h"

Sim_ImgEngine::Sim_ImgEngine(std::string name, bool debug):
   ImgEngine(name, debug),
   m_device_num(0),
   m_exposure_us(5000),
   m_delay_us(0),
   m_conversion_factor(1.0),
   m_height_px(512),
   m_width_px(512),
   m_offset_x_px(0),
   m_offset_y_px(0),
   m_framerate_hz(10),
   m_defaultImageDataFormat(0),
   m_lostframes(0),
   m_isColor(false),
   m_binh(1),
   m_binv(1),
   m_trigger_mode(0),
   m_doubleImage_mode(0),
   m_timestamp_mode(0),
   m_bit_shift(0),
   m_sim_drops_start(0)
{
}

Sim_ImgEngine::~Sim_ImgEngine()
{
   m_Log->LogInfo("Shutting down Sim camera...");
   m_Log->LogDebug("Lostframe count: ", m_lostframes);
}

bool Sim_ImgEngine::init(const std::map<std::string, double> &config)
{
#if 0
for (auto item : config)
    std::cout << "key: " << item.first << ", value=" << item.second << std::endl;
#else
   try {
      m_device_num = (int)config.at("device_num");
      m_exposure_us = (int)config.at("exposure_us");
      m_delay_us = (int)config.at("delay_us");
      m_conversion_factor = config.at("conversion_factor");
      m_height_px = (int)config.at("height_px");
      m_width_px = (int)config.at("width_px");
      m_pelSize = (int)config.at("pel_size_um");
      m_binh = (int)config.at("binh");
      m_binv = (int)config.at("binv");
      m_trigger_mode = (int)config.at("trigger_mode");
      m_doubleImage_mode = (int)config.at("doubleImage_mode");
      m_timestamp_mode = (int)config.at("timestamp_mode");
   }
   catch (const std::out_of_range& oor) {
       std::cerr << "Out of Range error: " << oor.what() << '\n';
     }
#endif
   m_Log->LogInfo("Sim driver loaded. ");
   std::cerr << device_info();

   return true;
}

bool Sim_ImgEngine::setImageParams()
{

   return true;
}

py::array_t<unsigned short> Sim_ImgEngine::next()
{
   int rows = m_height_px;
   int cols = m_width_px;

   m_Log->LogInfo("Inside next...rows=", rows, ", cols=", cols);

   unsigned short* m_data = new unsigned short[rows*cols];
   memset(m_data, 0, sizeof(unsigned short) * rows * cols);

   unsigned short t=rows;
   for (size_t idx = (rows * cols) - cols; idx < (rows * cols); t--, idx++)
      m_data[idx] = t;

   m_Log->LogInfo("Alloc'd m_data");

   py::array_t<unsigned short> nextImg =
     py::array(py::buffer_info(
      m_data,            /* Pointer to data (nullptr -> ask NumPy to allocate!) */
      sizeof(unsigned short),     /* Size of one item */
      py::format_descriptor<unsigned short>::value(), /* Buffer format */
      2,          /* How many dimensions? */
      { rows, cols },  /* Number of elements for each dimension */
      { sizeof(unsigned short) * rows,            /* Strides (in bytes) for each index */
              sizeof(unsigned short) }
   ));

   m_Log->LogInfo("Alloc'd nextImg");

   auto buf3 = nextImg.request();
   unsigned short *ptr3 = (unsigned short *) buf3.ptr;

   m_Log->LogInfo("got buf3...shape: ", buf3.shape[0], "x", buf3.shape[1]);
   m_Log->LogInfo("now some idxing...");

   for (size_t idx = 0; idx < buf3.shape[0]; idx++)
      ptr3[idx] = idx+1;

   m_Log->LogInfo("returning...");
   return nextImg;
}

std::tuple<int, int> Sim_ImgEngine::getImageSize()
{
   return std::make_tuple(m_width_px, m_height_px);
}

std::string Sim_ImgEngine::cur_info()
{
   return std::to_string(img_idx);
}

std::string Sim_ImgEngine::device_info()
{
   std::stringstream ss;

   ss << " (" << m_Name << ") Device num           : " << m_device_num << std::endl;
   ss << " (" << m_Name << ") Camera type          : " << "Simulated" << std::endl;
   ss << " (" << m_Name << ") Actual Width         : " << m_width_px << std::endl;
   ss << " (" << m_Name << ") Actual Height        : " << m_height_px << std::endl;
   ss << " (" << m_Name << ") Bin Hor              : " << m_binh << std::endl;
   ss << " (" << m_Name << ") Bin Vert             : " << m_binv << std::endl;
   ss << " (" << m_Name << ") Exposure (us)        : " << m_exposure_us << std::endl;
   ss << " (" << m_Name << ") Bit shift            : " << m_bit_shift << std::endl;
   ss << " (" << m_Name << ") Trigger Mode         : " << m_trigger_mode << std::endl;
   ss << " (" << m_Name << ") DoubleImage Mode     : " << m_doubleImage_mode << std::endl;
   ss << " (" << m_Name << ") Timestamp Mode       : " << m_timestamp_mode << std::endl;

   return ss.str();
}
