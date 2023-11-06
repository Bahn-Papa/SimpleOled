
#pragma once

//##########################################################################
//#
//#		SimpleOled.h
//#
//#-------------------------------------------------------------------------
//#
//#	This class is used to control an OLED display with an sh1106 or ssd1306
//#	chipset via the I²C bus.
//#	Supported are only simple text output and some auxiliary functions,
//#	e.g.: clear display, clear line, position cursor, etc.
//#	If no display is connected nothing will be send over the I2C bus.
//#
//#-------------------------------------------------------------------------
//#
//#		MIT License
//#
//#		Copyright (c) 2023	Michael Pfeil
//#							Am Kuckhof 8
//#							D - 52146 Würselen
//#							GERMANY
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	5		Date: 06.11.2023
//#
//#	Implementation:
//#		-	add ESP32 support
//#		-	rework of the initialize sequence
//#			new functions
//#				Initsh1106()
//#				Initssd1306()
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	4		Date: 31.10.2023
//#
//#	Implementation:
//#		-	change default chip type to ssd1306
//#		-	do not send anything over the I2C bus when no display is
//#			connected
//#			add variable
//#				m_bDisplayConnected
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	3		Date: 21.09.2023
//#
//#	Implementation:
//#		-	add support for two chips (sh1106 and ssd1306) finished
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	2		Date: 20.09.2023
//#
//#	Implementation:
//#		-	prepare to distinguage between sh1106 and ssd1306
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	1		Date: 19.09.2023
//#
//#	Implementation:
//#		-	First implementation of the class 'SimpleDisplayClass'.
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
	CHIP_TYPE_SH1106	= 0,
	CHIP_TYPE_SSD1306

} chip_type_t;


//----------------------------------------------------------------------
//	The different print modes
//
//	PM_OVERWRITE_SAME_LINE:
//		if the text output comes to the end of a line then continue
//		with the output in the same line and overwrite an existing text.
//
//	PM_OVERWRITE_NEXT_LINE:
//		if the text output comes to the end of a line then continue
//		with the output in the next line and perhaps overwrite an
//		existing text.
//		if it was the last line of the display then jumpt to the first
//		line and continue the output there.
//
//	PM_SCROLL_LINE:
//		if the text output comes to the end of a line then continue
//		with the output in the next line.
//		If it was the last line of the display then scroll all lines
//		up one line, discarding the first line, clear the last line
//		and continue the output in the cleared last line.
//
typedef enum print_mode
{
	PM_OVERWRITE_SAME_LINE	= 1,
	PM_OVERWRITE_NEXT_LINE,
	PM_SCROLL_LINE

} print_mode_t;


//==========================================================================
//
//		C L A S S   D E F I N I T I O N S
//
//==========================================================================


////////////////////////////////////////////////////////////////////////////
//	CLASS: SimpleDisplayClass
//
class SimpleDisplayClass
{
	public:
		SimpleDisplayClass();

		uint8_t Init( chip_type_t chipType = CHIP_TYPE_SSD1306, uint8_t address = DISPLAY_ADDRESS );

		uint8_t MaxTextLines( void );
		uint8_t MaxTextColumns( void );

		void PrintChar( uint8_t usCharIdx );

#ifdef ARDUINO_ARCH_AVR
		void Print(   const __FlashStringHelper* cstrText );
		void PrintLn( const __FlashStringHelper* cstrText );
#endif

		void Print(   char* strText );
		void PrintLn( char* strText );

		inline void Print( const char* strText )
		{
			Print( (char *)strText );
		};

		inline void PrintLn( const char* strText )
		{
			PrintLn( (char *)strText );
		};


		void Clear( void );
		void ClearLine( uint8_t usLineToClear );
		inline void ClearLine( void )
		{
			ClearLine( m_usTextLine );
		};

		void SetCursor( uint8_t usTextLine, uint8_t usTextColumn );

		inline void Home( void )
		{
			SetCursor( 0, 0 );
		};

		void SetInverse( bool bInverse );
		void Flip( bool bFlip );

		inline void SetInverseFont( bool bInverse )
		{
			m_bInverse = bInverse;
		};

		inline void SetPrintMode( print_mode_t printMode )
		{
			m_PrintMode = printMode;
		};

		void SetDisplayColumnOffset( uint8_t usOffset );


	private:
		chip_type_t		m_ChipType;
		bool			m_bDisplayConnected;
		print_mode_t	m_PrintMode;
		uint8_t			m_usAddress;
		uint8_t			m_usDisplayColumnOffset;
		uint8_t			m_usTextLine;
		uint8_t			m_usTextColumn;
		uint8_t			m_usLineOffset;
		bool			m_bInverse;

		void Initsh1106( void );
		void Initssd1306( void );
		void NextLine( bool bShiftLine );
		void SendCommand( uint8_t usOpCode );
		void SendCommand( uint8_t usOpCode, uint8_t usParameter );
		void ShiftDisplayOneLine( void );
};


//==========================================================================
//
//		E X T E R N   G L O B A L   V A R I A B L E S
//
//==========================================================================

extern SimpleDisplayClass	g_clDisplay;
