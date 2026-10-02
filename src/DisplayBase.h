
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


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

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
		DisplayBaseClass( chip_type_t chip_type );

		uint8_t Init( chip_type_t chipType, uint8_t address );
		uint8_t Init( uint8_t address );

		void SetDisplayColumnOffset( uint8_t usOffset );
		void SetDisplayLineOffset( uint8_t usOffset );

		void ClearDisplay( void );
		void ClearPage( uint8_t usPage );

		void SetDrawPosition( uint8_t usPage, uint8_t usColumn );
		void Draw( uint8_t *pBuffer, uint8_t usSize );

		void SetInverse( bool bInverse );
		void Flip( bool bFlip );


	protected:
		chip_type_t		m_ChipType;
		bool			m_bDisplayConnected;
		uint8_t			m_usAddress;
		uint8_t			m_usDisplayColumnOffset;

		void Initsh1106( void );
		void Initssd1306( void );
		void SendCommand( uint8_t usOpCode );
		void SendCommand( uint8_t usOpCode, uint8_t usParameter );
};
