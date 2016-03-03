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

bool Sim_ImgEngine::init(std::string config)
{
   // if (!getAttributeValue_Int(config, "device_num", m_device_num) )
   // {
   //     m_Log->LogError("Could not get device_num from config: ");
   //     printJSON(config);
   //     return false;
   // }
   //
   // if (!getAttributeValue_Int(config, "exposure_us", m_exposure_us) )
   // {
   //     m_Log->LogError("Could not get exposure_ms from config: ");
   //     printJSON(config);
   //     return false;
   // }
   //
   // if (!getAttributeValue_Int(config, "delay_us", m_delay_us) )
   // {
   //     m_Log->LogWarn("Could not get delay_us from config: ");
   //     printJSON(config);
   // }
   //
   // if (!getAttributeValue_Double(config, "conversion_factor", m_conversion_factor) )
   // {
   //     m_Log->LogWarn("Could not get conversion_factor from config: ");
   //     printJSON(config);
   // }
   //
   // if (!getAttributeValue_Int(config, "height_px", (int&)m_height_px) )
   // {
   //     m_Log->LogError("Could not get height_px from config: ");
   //     printJSON(config);
   //     return false;
   // }
   //
   // if (!getAttributeValue_Int(config, "width_px", (int&)m_width_px) )
   // {
   //     m_Log->LogError("Could not get width_px from config: ");
   //     printJSON(config);
   //     return false;
   // }
   //
   // if (!getAttributeValue_Int(config, "offset_x_px", m_offset_x_px) )
   // {
   //     m_Log->LogError("Could not get offset_x_px from config: ");
   //     printJSON(config);
   //     return false;
   // }
   //
   // if (!getAttributeValue_Int(config, "offset_y_px", m_offset_y_px) )
   // {
   //     m_Log->LogError("Could not get offset_y_px from config: ");
   //     printJSON(config);
   //     return false;
   // }
   //
   // if (!getAttributeValue_Double(config, "pel_size_um", m_pelSize) )
   // {
   //     m_Log->LogWarn("Could not get pel_size_um from config: ");
   //     printJSON(config);
   // }
   //
   // if (!getAttributeValue_Int(config, "binh", m_binh) )
   // {
   //     m_Log->LogWarn("Could not get binh from config: ");
   //     printJSON(config);
   // }
   //
   // if (!getAttributeValue_Int(config, "binv", m_binv) )
   // {
   //     m_Log->LogWarn("Could not get binv from config: ");
   //     printJSON(config);
   // }
   //
   // if (!getAttributeValue_Int(config, "trigger_mode", m_trigger_mode) )
   // {
   //     m_Log->LogWarn("Could not get trigger_mode from config: ");
   //     printJSON(config);
   // }
   //
   // if (!getAttributeValue_Int(config, "doubleImage_mode", m_doubleImage_mode ) )
   // {
   //     m_Log->LogWarn("Could not get doubleImage_mode from config: ");
   //     printJSON(config);
   // }
   //
   // if (!getAttributeValue_Int(config, "timestamp_mode", m_timestamp_mode ) )
   // {
   //     m_Log->LogWarn("Could not get timestamp_mode from config: ");
   //     printJSON(config);
   // }

   m_Log->LogInfo("Sim driver loaded.  std::string config:", config);
   std::cerr << device_info();

   return true;
}

bool Sim_ImgEngine::setImageParams()
{

   return true;
}

bool Sim_ImgEngine::next2()
{

   m_Log->LogInfo("Next 2!!!");


   return true;
}

py::array_t<unsigned short> Sim_ImgEngine::next()
{
   int rows = {500};
   int cols = {500};

   //auto v = new Matrix(rows, cols);
   ////memcpy(v.data(), info.ptr, sizeof(float) * v.rows() * v.cols())

   m_Log->LogInfo("Inside next...rows=", rows, ", cols=", cols);

   unsigned short* m_data = new unsigned short[rows*cols];
   memset(m_data, 0, sizeof(unsigned short) * rows * cols);

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

int Sim_ImgEngine::getImageSize()
{
   return 5;
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
