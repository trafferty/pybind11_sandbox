/*
 * XimeaImgEngine.cpp
 *
 *  Created on: Sep 24, 2015
 *      Author: trafferty
 */

// PCO sdk:
#include "VersionNo.h"
#include "Cpco_com.h"
#include "Cpco_grab_usb.h"
#include "file12.h"
#define PCO_ERRT_H_CREATE_OBJECT
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "PCO_errt.h"
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop

#include "PCO_ImgEngine.h"

PCO_ImgEngine::PCO_ImgEngine(std::string name, bool debug):
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

   m_Camera(nullptr),
   m_Grabber(nullptr),
   m_Cbuf(nullptr),
   m_CPco_Log(nullptr)
{
   m_Camera = std::unique_ptr<CPco_com_usb>(new CPco_com_usb);
   m_Grabber = std::unique_ptr<CPco_grab_usb>(new CPco_grab_usb(m_Camera.get()));
   m_Cbuf = std::unique_ptr<CCambuf>(new CCambuf);
}

PCO_ImgEngine::~PCO_ImgEngine()
{
   DWORD err;

   m_Log->LogInfo("Shutting down PCO camera...");
   // err = m_Camera->PCO_SetRecordingState(0);
   // if(err!=PCO_NOERROR)
   //    printPCOErrorMsg("PCO_SetRecordingState()", err);

   // m_Grabber->Close_Grabber();
   // m_Camera->Close_Cam();
   m_Cbuf->FreeBuffer();

   m_Log->LogDebug("Lostframe count: ", m_lostframes);
}

bool PCO_ImgEngine::init(const std::map<std::string, double> &config)
{
   DWORD err;

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



   // ********************************************************
   if (m_Debug)
   {
      m_CPco_Log = std::shared_ptr<CPco_Log>(new CPco_Log("/tmp/pco_camera_grab.log"));
      m_Camera->SetLog(m_CPco_Log.get());
      m_Grabber->SetLog(m_CPco_Log.get());
   }

#if 0
   err = m_Camera->Open_Cam(m_device_num);
   if(err!=PCO_NOERROR)
   {
      m_Log->LogError("Failed to get handle to camera!  Check connection?");
      printPCOErrorMsg("Open_Cam()", err);
      m_Camera = nullptr;
      return false;
   }

   err = m_Grabber->Open_Grabber(m_device_num);
   if(err!=PCO_NOERROR)
   {
      m_Log->LogError("Failed to get handle to grabber!  Check connection?");
      printPCOErrorMsg("Open_Grabber()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   err = m_Camera->PCO_SetBinning((DWORD)m_binh, (DWORD)m_binv);
   if(err!=PCO_NOERROR)
   {
      m_Log->LogError("Failed to set cam binning: ", m_binh, "x", m_binv);
      printPCOErrorMsg("PCO_SetBinning()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   err = m_Camera->PCO_GetCameraDescriptor(&m_description);
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("PCO_GetCameraDescriptor()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   err = m_Camera->PCO_SetCameraToCurrentTime();
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("PCO_SetCameraToCurrentTime()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }


   //set RecordingState to STOP
   err = m_Camera->PCO_SetRecordingState(0);
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("PCO_SetRecordingState()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   err = m_Camera->PCO_SetTimestampMode(2);
   if(err!=PCO_NOERROR)
   {
      m_Log->LogError("PCO_SetTimestampMode() Error: ", std::hex, err, std::dec );
      printPCOErrorMsg("Open_Cam()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   //set camera timebase to ms
   err = m_Camera->PCO_SetTimebase(1,1);
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("PCO_SetTimebase()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   err = m_Camera->PCO_SetDelayExposure(m_delay_us, m_exposure_us);
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("PCO_SetDelayExposure()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   if(m_description.wNumADCsDESC > 1)
   {
      err = m_Camera->PCO_SetADCOperation(2);
      if(err!=PCO_NOERROR)
      {
         printPCOErrorMsg("PCO_SetADCOperation()", err);
         m_Camera->Close_Cam();
         m_Camera = nullptr;
         return false;
      }
   }

   err = m_Camera->PCO_SetBitAlignment(BIT_ALIGNMENT_MSB);
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("PCO_SetBitAlignment()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   err = m_Camera->PCO_SetTriggerMode(m_trigger_mode);
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("PCO_SetTriggerMode()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   err = m_Camera->PCO_SetTimestampMode(m_timestamp_mode);
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("PCO_SetTimestampMode()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   if (m_doubleImage_mode == 1)
   {
      //m_height_px *= 2;
      err = m_Camera->PCO_SetDoubleImageMode(1);
      if(err!=PCO_NOERROR)
      {
         printPCOErrorMsg("PCO_SetDoubleImageMode()", err);
         m_Camera->Close_Cam();
         m_Camera = nullptr;
         return false;
      }
   }


   //prepare Camera for recording
   err = m_Camera->PCO_ArmCamera();
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("PCO_ArmCamera()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   unsigned short int act_align;
   err = m_Camera->PCO_GetBitAlignment(&act_align);
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("PCO_GetBitAlignment()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   m_bit_shift=0;
   if(act_align==BIT_ALIGNMENT_MSB)
   {
      m_bit_shift = 16 - m_description.wDynResDESC;
   }

   err = m_Camera->PCO_GetActualSize(&m_width_px, &m_height_px);
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("PCO_GetActualSize()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

   if (m_doubleImage_mode == 1)
   {
      m_height_px *= 2;
   }

   err = m_Grabber->Set_Grabber_Size(m_width_px, m_height_px);
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("Set_Grabber_Size()", err);
      m_Camera->Close_Cam();
      m_Camera = nullptr;
      return false;
   }

#endif

   if (m_doubleImage_mode == 1)
   {
      m_height_px *= 2;
   }

   m_description.wDynResDESC = 16;

   err = m_Cbuf->Allocate(m_width_px, m_height_px, m_description.wDynResDESC, 0, IN_BW);
   if(err!=PCO_NOERROR)
   {
      printPCOErrorMsg("m_Cbuf->Allocate()", err);
      // m_Camera->Close_Cam();
      // m_Camera = nullptr;
      return false;
   }

   m_Log->LogInfo("PCO driver loaded.  Stats:");
   std::cerr << device_info();

   m_Record_Mode = RECORD_MODE_OFF;

   if (m_Record_Mode == RECORD_MODE_OFF)
   {
      //  err = m_Camera->PCO_SetRecordingState(1);
      //  if(err!=PCO_NOERROR)
      //  {
      //     printPCOErrorMsg("PCO_SetRecordingState()", err);
      //     m_Camera->Close_Cam();
      //     m_Camera = nullptr;
      //     return false;
      //  }
       m_Record_Mode = RECORD_MODE_ON;
       m_Log->LogInfo("Acquisition started");
   }
   return true;
}

bool PCO_ImgEngine::setImageParams()
{

   return true;
}

py::array_t<unsigned short> PCO_ImgEngine::next()
{
   int rows = m_height_px;
   int cols = m_width_px;

   m_Log->LogInfo("Inside next...rows=", rows, ", cols=", cols);

   unsigned short* m_data = (unsigned short*)m_Cbuf->Get_actadr();

   // m_Log->LogInfo("Memsetting...");
   // memset(m_data, 0, sizeof(unsigned short) * rows * cols);

   // m_Log->LogInfo("Manipulating...");
   // unsigned short t=rows;
   // for (size_t idx = (rows * cols) - cols; idx < (rows * cols); t--, idx++)
   //    m_data[idx] = t;

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

std::tuple<int, int> PCO_ImgEngine::getImageSize()
{
   return std::make_tuple(m_width_px, m_height_px);
}

std::string PCO_ImgEngine::cur_info()
{
   return std::to_string(img_idx);
}

std::string PCO_ImgEngine::device_info()
{
   std::stringstream ss;
   DWORD err(0);
   char infostr[100];
//   char firmware_version[100];
//   int firmware_version_strlen;
   SHORT ccdtemp,camtemp,pstemp;
   WORD trigger_mode, doubleImage_mode;
   WORD camtype;
   DWORD serialnumber, pixelrate, delay_us, exposure_us;

   // err += m_Camera->PCO_GetCameraType(&camtype,&serialnumber);
   // err += m_Camera->PCO_GetInfo(1,infostr,sizeof(infostr));
   // err += m_Camera->PCO_GetTemperature(&ccdtemp,&camtemp,&pstemp);
   // err += m_Camera->PCO_GetTriggerMode(&trigger_mode);
   // err += m_Camera->PCO_GetDoubleImageMode(&doubleImage_mode);
   // err += m_Camera->PCO_GetPixelRate(&pixelrate);
   // err += m_Camera->PCO_GetDelayExposure(&delay_us, &exposure_us);
   //err += m_Camera->PCO_GetFirmwareVersion(firmware_version, &firmware_version_strlen);

   std::stringstream pel_rates;
   for(int x=0;x<4;x++)
   {
      if(m_description.dwPixelRateDESC[x]!=0)
      {
         pel_rates << m_description.dwPixelRateDESC[x];
         pel_rates << ", ";
      }
   }

   if (err > 0)
   {
      m_Log->LogWarn("Warning!  Not all PCO settings obtained correctly...");
   }

   ss << " (" << m_Name << ") Device num           : " << m_device_num << std::endl;
   ss << " (" << m_Name << ") Camera name          : " << "infostr" << std::endl;
   ss << " (" << m_Name << ") Camera type          : " << std::hex << camtype << std::dec << std::endl;
   ss << " (" << m_Name << ") Camera S/N           : " << serialnumber << std::endl;
   //ss << " (" << m_Name << ") Camera FW Version    : " << firmware_version << std::endl;
   ss << " (" << m_Name << ") Camera temp (C)      : " << camtemp << std::endl;
   ss << " (" << m_Name << ") Actual Width         : " << m_width_px << std::endl;
   ss << " (" << m_Name << ") Actual Height        : " << m_height_px << std::endl;
   ss << " (" << m_Name << ") Bin Hor              : " << m_binh << std::endl;
   ss << " (" << m_Name << ") Bin Vert             : " << m_binv << std::endl;
   ss << " (" << m_Name << ") Delay (us)           : " << m_delay_us << std::endl;
   ss << " (" << m_Name << ") Exposure (us)        : " << m_exposure_us << std::endl;
   ss << " (" << m_Name << ") Bit shift            : " << m_bit_shift << std::endl;
   ss << " (" << m_Name << ") Trigger Mode         : " << m_trigger_mode << std::endl;
   ss << " (" << m_Name << ") DoubleImage Mode     : " << m_doubleImage_mode << std::endl;
   ss << " (" << m_Name << ") Timestamp Mode       : " << m_timestamp_mode << std::endl;
   ss << " (" << m_Name << ") Possible pixel rates : " << pel_rates.str() << std::endl;
   ss << " (" << m_Name << ") Current pixel rate   : " << pixelrate << std::endl;
   ss << " (" << m_Name << ") MinDelay (us)        : " << m_description.dwMinDelayDESC * 1000    << std::endl;
   ss << " (" << m_Name << ") MaxDelay (us)        : " << m_description.dwMaxDelayDESC * 1000     << std::endl;
   ss << " (" << m_Name << ") MinExposure (us)     : " << m_description.dwMinExposureDESC / 1000  << std::endl;
   ss << " (" << m_Name << ") MaxExposure (us)     : " << m_description.dwMaxExposureDESC * 1000 << std::endl;

   return ss.str();
}

int PCO_ImgEngine::image_num_from_timestamp(void *buf, int shift)
{
   unsigned short *b;
   int y;
   int image_num=0;
   b=(unsigned short *)(buf);
   y=100*100*100;
   for(;y>0;y/=100)
   {
      image_num += ((((*b>>shift)&0x00F0)>>4)*10 + (*b>>shift&0x000F))*y;
      b++;
   }
   return image_num;
}

std::string PCO_ImgEngine::getPCOErrorStr(DWORD dwerr)
{
   char pbuf[500];
   PCO_GetErrorText(dwerr, pbuf, 499);
   std::string errStr(pbuf);
   return errStr;
}

void PCO_ImgEngine::printPCOErrorMsg(std::string funcName, DWORD dwerr)
{
   std::stringstream ss;
   ss << "PCO Error - ";
   ss << funcName;
   ss << " Error code: ";
   ss << std::hex << dwerr << std::dec;
   ss << ", " << getPCOErrorStr(dwerr);
   m_Log->LogError(ss.str() );

}
