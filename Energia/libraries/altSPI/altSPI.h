///
/// @mainpage	SPI for LM4F
///
/// @details	Test of the 4 SPI ports on Stellaris LaunchPad
/// @n          Works fine on all ports, SPI=SPI2, SPI1, SP0 and SPI3
///
/// @author	reaper7
/// @date	Jul 08, 2013
/// @version	101
///
/// @see Based on SPI Master library for Arduino.
///
/// @copyright Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2
/// or the GNU Lesser General Public License version 2.1, both as
/// published by the Free Software Foundation.
///
#ifndef _altSPI_H_INCLUDED
#define _altSPI_H_INCLUDED
#include <Energia.h>
// Board check
#if defined(__LM4F120H5QR__)
#else
#error Board not supported.
#endif
#include <stdio.h>
#define SPI_CLOCK_DIV2 2
#define SPI_CLOCK_DIV4 4
#define SPI_CLOCK_DIV8 8
#define SPI_CLOCK_DIV16 16
#define SPI_CLOCK_DIV32 32
#define SPI_CLOCK_DIV64 64
#define SPI_CLOCK_DIV128 128
#define SPI_MODE0 0x00
#define SPI_MODE1 0x80
#define SPI_MODE2 0x40
#define SPI_MODE3 0xC0
#define BOOST_PACK_SPI 2
class altSPIClass {
private:
	unsigned long slaveSelect;
	unsigned long SSIModule;	
public:
  altSPIClass(void);
  altSPIClass(unsigned long);
  void begin(); // Default
  void begin(unsigned long);
  void end();
  void end(unsigned long);
  void setBitOrder(uint8_t);
  void setBitOrder(unsigned long, uint8_t);
  void setDataMode(uint8_t);                  //
  void setClockDivider(uint8_t);
  uint8_t transfer(uint8_t);
//  int bits(int);                          // new
//  uint16_t trans16(uint16_t);             // new Oct 21, 13, jss
  uint8_t transfer(unsigned long, uint8_t);
  uint8_t transfer(unsigned long, uint8_t, uint8_t);
  uint8_t trans2ByteA(uint8_t);         //  new function
  uint8_t trans2ByteB(uint8_t);         //  new function
                                         // transfers 2 bytes
  // Stellarpad-specific functions
  void setModule(unsigned long);                // module
  void setModule(unsigned long, unsigned long); // module, sspin
};
// SPI ports
extern altSPIClass SPI0;
extern altSPIClass SPI1;
extern altSPIClass SPI2;
extern altSPIClass SPI3;
// default SPI port on LM4F = SPI port 2
#define altSPI SPI2
#endif
