
#pragma once

//##########################################################################
//#
//#		DisplayBase.h
//#
//#	The class defined here will act as a base class for all derived
//#	display classes (like 'SimpleDisplayClass') that have one of the
//#	controllers: ssd1306 or sh1106.
//#
//#	The class will contain all functions to setup and communicate with
//#	such a display over I2C bus.
//#
//#-------------------------------------------------------------------------
//#
//#		MIT License
//#
//#		Copyright (c) 2026	Michael Pfeil
//#							Am Kuckhof 8
//#							D - 52146 Würselen
//#							GERMANY
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	3		from: 19.10.2026
//#
//#	Implementation:
//#		-	move the functionality to invert drawing (means black will get
//#			white and vice versa) from the class 'SimpleOledClass' to
//#			'DisplayBaseClass'.
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	2		from: 11.10.2026
//#
//#	Implementation:
//#		-	consolidated version works with Arduino chips and Arduino IDE
//#			as well as with ESP32 chips. The distinguishing feature is a
//#			definition set by the Arduino IDE: ARDUINO_ARCH_AVR
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		from: 02.10.2026
//#
//#	Implementation:
//#		-	first implementation
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include <stdint.h>


#ifndef ARDUINO_ARCH_AVR
	#include "driver/i2c_master.h"
#endif


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define DISPLAY_ADDRESS_DEFAULT			255
#define	DISPLAY_ADDRESS					60
#define SECOND_DISPLAY_ADDRESS			61


//==========================================================================
//
//		T Y P E   D E F I N I T I O N S
//
//==========================================================================

typedef enum chip_type
{
	CHIP_TYPE_UNKNOWN	= 0,
	CHIP_TYPE_SH1106	= 1106,
	CHIP_TYPE_SSD1306	= 1306

} chip_type_t;


//==========================================================================
//
//		C L A S S   D E F I N I T I O N S
//
//==========================================================================


////////////////////////////////////////////////////////////////////////////
//	CLASS: DisplayBaseClass
//
class DisplayBaseClass
{
	public:
		DisplayBaseClass();
		DisplayBaseClass( chip_type_t chipType );

#ifndef ARDUINO_ARCH_AVR
		DisplayBaseClass( i2c_master_bus_handle_t bus );
		DisplayBaseClass( i2c_master_bus_handle_t bus, chip_type_t chipType );
#endif

		uint8_t Init( uint8_t address );
		uint8_t Init( chip_type_t chipType, uint8_t address );

#ifndef ARDUINO_ARCH_AVR
		uint8_t	Init(	i2c_master_bus_handle_t	bus,
						chip_type_t				chipType,
						uint8_t					address		);
		uint8_t	Init(	i2c_master_bus_handle_t	bus,
						uint8_t					address		);
#endif

		void SetDisplayColumnOffset( uint8_t usOffset );
		void SetDisplayLineOffset( uint8_t usOffset );

		void ClearDisplay( void );
		void ClearPage( uint8_t usPage );

		void SetDrawPosition( uint8_t usPage, uint8_t usColumn );
		void Draw( uint8_t *pBuffer, uint8_t usSize );

		void SetInverse( bool bInverse );
		void Flip( bool bFlip );

		inline void SetDrawInverse( bool bInverse )
		{
			m_bDrawInverse = bInverse;
		};

		inline chip_type_t GetChipType( void )
		{
			return( m_ChipType );
		};


	protected:
		chip_type_t				m_ChipType;
		bool					m_bDisplayConnected;
		bool					m_bDrawInverse;
		uint8_t					m_usAddress;
		uint8_t					m_usDisplayColumnOffset;

#ifndef ARDUINO_ARCH_AVR
		i2c_master_bus_handle_t	m_Bus;
		i2c_master_dev_handle_t m_Display;

		bool CheckConnection( void );
#endif

		void Initsh1106( void );
		void Initssd1306( void );
		void SendCommand( uint8_t usOpCode );
		void SendCommand( uint8_t usOpCode, uint8_t usParameter );
};
