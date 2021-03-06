/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 * Improvements for StellarPad LM4F
 * ---
 * Four SPI ports for StellarPad - reaper7 - Jul 08, 2013
 * trans2Byte commands for transfering 16 bits - JackSummers -Feb28,14
 */

#include "wiring_private.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "altSPI.h"


//#define NOT_ACTIVE 0xA

#define SSIBASE g_ulSSIBase[SSIModule]
#define SSELPIN g_u8SlaveSelPins[SSIModule]

static const unsigned long g_ulSSIBase[4] =
{
    SSI0_BASE, SSI1_BASE, SSI2_BASE, SSI3_BASE
};

//*****************************************************************************
//
// The list of SSI peripherals.
//
//*****************************************************************************
static const unsigned long g_ulSSIPeriph[4] =
{
    SYSCTL_PERIPH_SSI0, SYSCTL_PERIPH_SSI1,
    SYSCTL_PERIPH_SSI2, SYSCTL_PERIPH_SSI3
};

//*****************************************************************************
//
// The list of SSI gpio configurations.
//
//*****************************************************************************
static const unsigned long g_ulSSIConfig[4][4] =
{
  {GPIO_PA2_SSI0CLK, GPIO_PA3_SSI0FSS, GPIO_PA4_SSI0RX, GPIO_PA5_SSI0TX},
  {GPIO_PF2_SSI1CLK, GPIO_PF3_SSI1FSS, GPIO_PF0_SSI1RX, GPIO_PF1_SSI1TX},
  {GPIO_PB4_SSI2CLK, GPIO_PB5_SSI2FSS, GPIO_PB6_SSI2RX, GPIO_PB7_SSI2TX},
  {GPIO_PD0_SSI3CLK, GPIO_PD1_SSI3FSS, GPIO_PD2_SSI3RX, GPIO_PD3_SSI3TX}
};

//*****************************************************************************
//
// The list of SSI gpio port bases.
//
//*****************************************************************************
static const unsigned long g_ulSSIPort[4] =
{
  GPIO_PORTA_BASE, GPIO_PORTF_BASE, GPIO_PORTB_BASE, GPIO_PORTD_BASE
};

//*****************************************************************************
//
// The list of SSI gpio configurations.
//
//*****************************************************************************
static const unsigned long g_ulSSIPins[4] =
{
	GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5,
	GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
	GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
	GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
};

//*****************************************************************************
//
// The list of SlaveSelect Default Pins.
//
//*****************************************************************************
static const uint8_t g_u8SlaveSelPins[4] =
{
  PA_3, PF_3, PB_5, PD_1
};

altSPIClass::altSPIClass(void)
{
	SSIModule = BOOST_PACK_SPI;
	slaveSelect = SSELPIN;
}

altSPIClass::altSPIClass(unsigned long module)
{
	SSIModule = module;
	slaveSelect = SSELPIN;
}
  
void altSPIClass::begin(unsigned long ssPin) {

	unsigned long initialData = 0;
/*
  if(SSIModule == NOT_ACTIVE) {
    SSIModule = BOOST_PACK_SPI;
  }
*/
	ROM_SysCtlPeripheralEnable(g_ulSSIPeriph[SSIModule]);
	ROM_SSIDisable(SSIBASE);
	ROM_GPIOPinConfigure(g_ulSSIConfig[SSIModule][0]);
	ROM_GPIOPinConfigure(g_ulSSIConfig[SSIModule][1]);
	ROM_GPIOPinConfigure(g_ulSSIConfig[SSIModule][2]);
	ROM_GPIOPinConfigure(g_ulSSIConfig[SSIModule][3]);
	ROM_GPIOPinTypeSSI(g_ulSSIPort[SSIModule], g_ulSSIPins[SSIModule]);

	/*
	  Polarity Phase        Mode
	     0 	   0   SSI_FRF_MOTO_MODE_0
	     0     1   SSI_FRF_MOTO_MODE_1
	     1     0   SSI_FRF_MOTO_MODE_2
	     1     1   SSI_FRF_MOTO_MODE_3
	*/

	slaveSelect = ssPin;
	pinMode(slaveSelect, OUTPUT);

	/*
	 * Default to
	 * System Clock, SPI_MODE_0, MASTER,
	 * 8MHz bit rate, and 8 bit data
	*/
	ROM_SSIClockSourceSet(SSIBASE, SSI_CLOCK_SYSTEM);
   ROM_SSIConfigSetExpClk(SSIBASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 16000000, 8);
 //   ROM_SSIConfigSetExpClk(SSIBASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 16000000, 16);
    ROM_SSIEnable(SSIBASE);

	//clear out any initial data that might be present in the RX FIFO
	while(ROM_SSIDataGetNonBlocking(SSIBASE, &initialData));

}

void altSPIClass::begin() {
	begin(SSELPIN);
}

void altSPIClass::end(unsigned long ssPin) {
	ROM_SSIDisable(SSIBASE);
}

void altSPIClass::end() {
	end(slaveSelect);
}

void altSPIClass::setBitOrder(unsigned long ssPin, uint8_t bitOrder)
{
}

void altSPIClass::setBitOrder(uint8_t bitOrder)
{
}

void altSPIClass::setDataMode(uint8_t mode) {

	HWREG(SSIBASE + SSI_O_CR0) &=
			~(SSI_CR0_SPO | SSI_CR0_SPH);

	HWREG(SSIBASE + SSI_O_CR0) |= mode;

}

void altSPIClass::setClockDivider(uint8_t divider) {

  //value must be even
  HWREG(SSIBASE + SSI_O_CPSR) = divider;

}

uint8_t altSPIClass::transfer(unsigned long ssPin, uint8_t data, uint8_t transferMode) {

	unsigned long rxData;

	digitalWrite(ssPin, LOW);

	ROM_SSIDataPut(SSIBASE, data);

	while(ROM_SSIBusy(SSIBASE));

	if(transferMode == SPI_LAST)
		digitalWrite(ssPin, HIGH);
	else
		digitalWrite(ssPin, LOW);

	ROM_SSIDataGet(SSIBASE, &rxData);

	return (uint8_t) rxData;

}

uint8_t altSPIClass::transfer(unsigned long ssPin, uint8_t data) {

  return transfer(ssPin, data, SPI_LAST);

}

uint8_t altSPIClass::transfer(uint8_t data) {

  return transfer(slaveSelect, data, SPI_LAST);

}

uint8_t altSPIClass::trans2ByteA(uint8_t data) {

  return transfer(slaveSelect, data, SPI_CONTINUE);

}
uint8_t altSPIClass::trans2ByteB(uint8_t data) {

  return transfer(slaveSelect, data, SPI_LAST);

}
void altSPIClass::setModule(unsigned long module) {
  SSIModule = module;
  begin(SSELPIN);
}

void altSPIClass::setModule(unsigned long module, unsigned long ssPin) {
  SSIModule = module;
  begin(ssPin);
}

//SPIClass SPI;
altSPIClass SPI0(0);
altSPIClass SPI1(1);
altSPIClass SPI2(2);
altSPIClass SPI3(3);
