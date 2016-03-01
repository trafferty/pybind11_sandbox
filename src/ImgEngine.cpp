/*
 * ImgEngine.cpp
 *
 *  Created on: Aug 24, 2015
 *      Author: trafferty
 */

#include "ImgEngine.h"

ImgEngine::ImgEngine(std::string name, bool debug) :
   img_idx(0),
   m_Debug(debug),
   m_Name(name),
   m_pelSize(1.0),
   m_colorMode(ColorMode_t::COLOR_MODE_MONO)
{
   m_Log = std::unique_ptr<Logger>(new Logger(m_Name, m_Debug));
}

ImgEngine::~ImgEngine()
{
}

double ImgEngine::getPelSize_um()
{
   return m_pelSize;
}

void ImgEngine::setColorMode(ColorMode_t colorMode)
{
   m_colorMode = colorMode;
}

ImgEngine::ColorMode_t ImgEngine::getColorMode()
{
   return m_colorMode;
}

bool ImgEngine::reset(ColorMode_t colorMode)
{
   m_colorMode = colorMode;
   return true;
}
