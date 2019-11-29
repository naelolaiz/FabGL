/*
  Created by Fabrizio Di Vittorio (fdivitto2013@gmail.com) - <http://www.fabgl.com>
  Copyright (c) 2019 Fabrizio Di Vittorio.
  All rights reserved.

  This file is part of FabGL Library.

  FabGL is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FabGL is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FabGL.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once



/**
 * @file
 *
 * @brief This file contains fabgl::SSD1306Controller definition.
 */


#include <stdint.h>
#include <stddef.h>

#include "freertos/FreeRTOS.h"

#include "esp32-hal.h"

#include "fabglconf.h"
#include "fabutils.h"
#include "displaycontroller.h"
#include "comdrivers/tsi2c.h"




namespace fabgl {




class SSD1306Controller : public DisplayController {

public:

  // unwanted methods
  SSD1306Controller(SSD1306Controller const&)   = delete;
  void operator=(SSD1306Controller const&)      = delete;


  SSD1306Controller();

  ~SSD1306Controller();

  void begin(I2C * i2c, gpio_num_t resetGPIO, int address);

  void end();

  void setResolution(char const * modeline, int viewPortWidth = -1, int viewPortHeight = -1, bool doubleBuffered = false);

  bool available() { return m_screenBuffer != nullptr; }

  // abstract method of DisplayController
  void suspendBackgroundPrimitiveExecution();

  // abstract method of DisplayController
  void resumeBackgroundPrimitiveExecution();

  // abstract method of DisplayController
  NativePixelFormat nativePixelFormat() { return NativePixelFormat::Mono; }

  // abstract method of DisplayController
  int getViewPortWidth()  { return m_viewPortWidth; }

  // abstract method of DisplayController
  int getViewPortHeight() { return m_viewPortHeight; }

  // abstract method of DisplayController
  int getScreenWidth()    { return m_screenWidth; }

  // abstract method of DisplayController
  int getScreenHeight()   { return m_screenHeight; }

  void setScreenCol(int value);

  void setScreenRow(int value);

  int screenCol() { return m_screenCol; }

  int screenRow() { return m_screenRow; }

  void readScreen(Rect const & rect, RGB888 * destBuf);


private:

  // abstract method of DisplayController
  PixelFormat getBitmapSavePixelFormat() { return PixelFormat::RGBA2222; }


  bool SSD1306_sendData(uint8_t * buf, int count, uint8_t ctrl);
  bool SSD1306_sendCmd(uint8_t c);
  bool SSD1306_sendCmd(uint8_t c1, uint8_t c2);
  bool SSD1306_sendCmd(uint8_t c1, uint8_t c2, uint8_t c3);

  void SSD1306_hardReset();
  bool SSD1306_softReset();

  void SSD1306_sendScreenBuffer();

  void allocScreenBuffer();

  static void updateTaskFunc(void * pvParameters);

  // abstract method of DisplayController
  void setPixelAt(PixelDesc const & pixelDesc);

  // abstract method of DisplayController
  void clear();

  // abstract method of DisplayController
  void drawEllipse(Size const & size);

  void VScroll(int scroll);
  
  void HScroll(int scroll);

  // abstract method of DisplayController
  void drawGlyph(Glyph const & glyph, GlyphOptions glyphOptions, RGB888 penColor, RGB888 brushColor);

  void drawGlyph_full(Glyph const & glyph, GlyphOptions glyphOptions, RGB888 penColor, RGB888 brushColor);

  void drawGlyph_light(Glyph const & glyph, GlyphOptions glyphOptions, RGB888 penColor, RGB888 brushColor);

  // abstract method of DisplayController
  void swapBuffers();

  // abstract method of DisplayController
  void invertRect(Rect const & rect);

  // abstract method of DisplayController
  void copyRect(Rect const & source);

  // abstract method of DisplayController
  void swapFGBG(Rect const & rect);

  // abstract method of DisplayController
  void drawLine(int X1, int Y1, int X2, int Y2, RGB888 color);

  // abstract method of DisplayController
  void fillRow(int y, int x1, int x2, RGB888 color);

  // abstract method of DisplayController
  void drawBitmap_Mask(int destX, int destY, Bitmap const * bitmap, uint8_t * saveBackground, int X1, int Y1, int XCount, int YCount);

  // abstract method of DisplayController
  void drawBitmap_RGBA2222(int destX, int destY, Bitmap const * bitmap, uint8_t * saveBackground, int X1, int Y1, int XCount, int YCount);

  // abstract method of DisplayController
  void drawBitmap_RGBA8888(int destX, int destY, Bitmap const * bitmap, uint8_t * saveBackground, int X1, int Y1, int XCount, int YCount);

  void copyRow(int x1, int x2, int srcY, int dstY);

  uint8_t preparePixel(RGB888 const & rgb);


  I2C *              m_i2c;
  uint8_t            m_i2cAddress;
  gpio_num_t         m_resetGPIO;

  uint8_t *          m_screenBuffer;

  int16_t            m_screenWidth;
  int16_t            m_screenHeight;
  int16_t            m_screenCol;
  int16_t            m_screenRow;

  int16_t            m_viewPortWidth;
  int16_t            m_viewPortHeight;

  TaskHandle_t       m_updateTaskHandle;

  int                m_updateTaskFuncSuspended;             // 0 = enabled, >0 suspended

};


} // end of namespace






