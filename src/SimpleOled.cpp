//##########################################################################
//#
//#		SimpleOled.cpp
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
//#	File Version:	6		Date: 07.11.2023
//#
//#	Bug Fix:
//#		-	did not start with chip type sh1106
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	5		Date: 06.11.2023
//#
//#	Implementation:
//#		-	add ESP32 support
//#		-	rework of the initialize sequence
//#			change in function
//#				Init()
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
//#			change in functions
//#				nearly all functions
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
//#		-	add op codes for the different chips
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

#include <Arduino.h>
#include <Wire.h>

#ifdef ARDUINO_ARCH_AVR
	#include <avr/pgmspace.h>
#endif

#include "SimpleOled.h"
#include "font.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

//#define	PRINT_DEBUG_INFO


#define	TEXT_LINES						8
#define TEXT_COLUMNS					16

#define PIXELS_CHAR_HEIGHT				8
#define PIXELS_CHAR_WIDTH				8

#define DISPLAY_COLUMN_OFFSET_MAX		3
#define DISPLAY_COLUMN_OFFSET_MIN		0
#define DISPLAY_COLUMN_OFFSET_DEFAULT	2


//--------------------------------------------------------------------------
//	Definitions for I²C protocol
//

//----	common command codes (for both chips)  -------------------------
#define	OPC_COLUMN_ADDRESS_LOW			0x00
#define OPC_COLUMN_ADDRESS_HIGH			0x10
#define OPC_DISPLAY_START_LINE			0x40
#define OPC_SET_CONTRAST				0x81
#define OPC_SEG_ROTATION_RIGHT			0xA0
#define OPC_SEG_ROTATION_LEFT			0xA1
#define	OPC_ENTIRE_DISPLAY_NORMAL		0xA4
#define	OPC_ENTIRE_DISPLAY_ON			0xA5
#define OPC_MODE_NORMAL					0xA6
#define OPC_MODE_INVERSE				0xA7
#define OPC_SET_MULTIPLEX_RATIO			0xA8
#define OPC_DISPLAY_OFF					0xAE
#define OPC_DISPLAY_ON					0xAF
#define	OPC_PAGE_ADDRESS				0xB0
#define OPC_OUTPUT_SCAN_NORMAL			0xC0
#define OPC_OUTPUT_SCAN_INVERSE			0xC8
#define OPC_DISPLAY_LINE_OFFSET			0xD3
#define OPC_CLK_DIV_OSC_FREQ			0xD5
#define OPC_DIS_PRE_CHARGE_PERIOD		0xD9
#define OPC_SET_COM_PINS				0xDA
#define OPC_SET_VCOM_DESELECT_LEVEL		0xDB

#define OPC_NOP							0xE3

//----	sh1106 specific command codes  ---------------------------------
#define OPC_DC_DC_PUMP_VOLTAGE_6_4		0x30
#define OPC_DC_DC_PUMP_VOLTAGE_7_4		0x31
#define OPC_DC_DC_PUMP_VOLTAGE_8_0		0x32
#define OPC_DC_DC_PUMP_VOLTAGE_9_0		0x33
#define OPC_DC_DC_CONTROL_MODE			0xAD

//----	ssd1306 specific command codes  --------------------------------
#define OPC_MEMORY_ADR_MODE				0x20
#define OPC_DEACTIVATE_SCROLL			0x2E
#define OPC_CHARGE_PUMP_SETTING			0x8D

//----	prefix codes  --------------------------------------------------
#define	PREFIX_NEXT_COMMAND				0x80
#define PREFIX_LAST_COMMAND				0x00
#define PREFIX_DATA						0x40

//----	Masks to prepare commands  -------------------------------------
#define MASK_PAGE_ADDRESS				0x0F
#define	MASK_COLUMN_ADDRESS_LOW			0x0F
#define MASK_COLUMN_ADDRESS_HIGH		0xF0

//----	Idx into command buffer  ---------------------------------------
#define	IDX_PAGE_ADDRESS				1
#define IDX_COLUMN_ADDRESS_LOW			3
#define IDX_COLUMN_ADDRESS_HIGH			5

//----	memory addressing modes  ---------------------------------------
#define ADR_MODE_HORIZONTAL				0x00
#define ADR_MODE_VERTICAL				0x01
#define ADR_MODE_PAGE					0x02

//----	DC DC control modes  -------------------------------------------
#define DC_DC_OFF						0x8A
#define DC_DC_ON						0x8B

//----	Clock divide ratio values  -------------------------------------
#define CLOCK_DIV_RATIO_1				0x00
#define CLOCK_DIV_RATIO_2				0x01
#define CLOCK_DIV_RATIO_3				0x02
#define CLOCK_DIV_RATIO_4				0x03
#define CLOCK_DIV_RATIO_5				0x04
#define CLOCK_DIV_RATIO_6				0x05
#define CLOCK_DIV_RATIO_7				0x06
#define CLOCK_DIV_RATIO_8				0x07
#define CLOCK_DIV_RATIO_9				0x08
#define CLOCK_DIV_RATIO_10				0x09
#define CLOCK_DIV_RATIO_11				0x0A
#define CLOCK_DIV_RATIO_12				0x0B
#define CLOCK_DIV_RATIO_13				0x0C
#define CLOCK_DIV_RATIO_14				0x0D
#define CLOCK_DIV_RATIO_15				0x0E
#define CLOCK_DIV_RATIO_16				0x0F

//----	Oscilator frequence variation  ---------------------------------
#define OSC_FREQ_VARIATION_M_25			0x00
#define OSC_FREQ_VARIATION_M_20			0x10
#define OSC_FREQ_VARIATION_M_15			0x20
#define OSC_FREQ_VARIATION_M_10			0x30
#define OSC_FREQ_VARIATION_M_5			0x40
#define OSC_FREQ_VARIATION_P_M_0		0x50
#define OSC_FREQ_VARIATION_P_5			0x60
#define OSC_FREQ_VARIATION_P_10			0x70
#define OSC_FREQ_VARIATION_P_15			0x80
#define OSC_FREQ_VARIATION_P_20			0x90
#define OSC_FREQ_VARIATION_P_25			0xA0
#define OSC_FREQ_VARIATION_P_30			0xB0
#define OSC_FREQ_VARIATION_P_35			0xC0
#define OSC_FREQ_VARIATION_P_40			0xD0
#define OSC_FREQ_VARIATION_P_45			0xE0
#define OSC_FREQ_VARIATION_P_50			0xF0

//----	Pre Charge periods  --------------------------------------------
#define PRE_CHARGE_PERIOD_DCLK_1		0x01
#define PRE_CHARGE_PERIOD_DCLK_2		0x02
#define PRE_CHARGE_PERIOD_DCLK_3		0x03
#define PRE_CHARGE_PERIOD_DCLK_4		0x04
#define PRE_CHARGE_PERIOD_DCLK_5		0x05
#define PRE_CHARGE_PERIOD_DCLK_6		0x06
#define PRE_CHARGE_PERIOD_DCLK_7		0x07
#define PRE_CHARGE_PERIOD_DCLK_8		0x08
#define PRE_CHARGE_PERIOD_DCLK_9		0x09
#define PRE_CHARGE_PERIOD_DCLK_10		0x0A
#define PRE_CHARGE_PERIOD_DCLK_11		0x0B
#define PRE_CHARGE_PERIOD_DCLK_12		0x0C
#define PRE_CHARGE_PERIOD_DCLK_13		0x0D
#define PRE_CHARGE_PERIOD_DCLK_14		0x0E
#define PRE_CHARGE_PERIOD_DCLK_15		0x0F

//----	Dis Charge periods  --------------------------------------------
#define DIS_CHARGE_PERIOD_DCLK_1		0x10
#define DIS_CHARGE_PERIOD_DCLK_2		0x20
#define DIS_CHARGE_PERIOD_DCLK_3		0x30
#define DIS_CHARGE_PERIOD_DCLK_4		0x40
#define DIS_CHARGE_PERIOD_DCLK_5		0x50
#define DIS_CHARGE_PERIOD_DCLK_6		0x60
#define DIS_CHARGE_PERIOD_DCLK_7		0x70
#define DIS_CHARGE_PERIOD_DCLK_8		0x80
#define DIS_CHARGE_PERIOD_DCLK_9		0x90
#define DIS_CHARGE_PERIOD_DCLK_10		0xA0
#define DIS_CHARGE_PERIOD_DCLK_11		0xB0
#define DIS_CHARGE_PERIOD_DCLK_12		0xC0
#define DIS_CHARGE_PERIOD_DCLK_13		0xD0
#define DIS_CHARGE_PERIOD_DCLK_14		0xE0
#define DIS_CHARGE_PERIOD_DCLK_15		0xF0


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

SimpleDisplayClass	g_clDisplay	= SimpleDisplayClass();

uint8_t	g_arusPositionCommandBuffer[] =
	{
		PREFIX_NEXT_COMMAND,
		OPC_PAGE_ADDRESS,
		PREFIX_NEXT_COMMAND,
		OPC_COLUMN_ADDRESS_LOW,
		PREFIX_LAST_COMMAND,
		OPC_COLUMN_ADDRESS_HIGH
	};


////////////////////////////////////////////////////////////////////////////
//
//	CLASS: SimpleDisplayClass
//


//**************************************************************************
//	Constructor
//--------------------------------------------------------------------------
//	description
//
SimpleDisplayClass::SimpleDisplayClass()
{
	m_bDisplayConnected = false;
}


//**************************************************************************
//	Init
//--------------------------------------------------------------------------
//	The Function initializes the class, sets the display in default
//	operation mode, switches the display 'on', clears the display and
//	sets the cursor to home position (top left corner).
//
uint8_t SimpleDisplayClass::Init( chip_type_t chipType, uint8_t address )
{
	uint8_t	usError;


	//------------------------------------------------------------------
	//	set initial values for internal variables
	//
	m_ChipType		= chipType;
	m_PrintMode		= PM_SCROLL_LINE;
	m_usTextLine	= 0;
	m_usTextColumn	= 0;
	m_usLineOffset	= 0;
	m_bInverse		= false;


	//------------------------------------------------------------------
	//	Check the given address
	//
	if( !(DISPLAY_ADDRESS == address) || (SECOND_DISPLAY_ADDRESS == address) )
	{
		//----------------------------------------------------------
		//	no valid address
		//
		return( 1 );
	}

	//------------------------------------------------------------------
	//	Initialize the I2C
	//
	Wire.begin();

	//------------------------------------------------------------------
	//	Check if Display can be connected under the given address
	//
	Wire.beginTransmission( address );
	usError = Wire.endTransmission();

	if( 0 == usError )
	{
		//----------------------------------------------------------
		//	YES the display can be connected with the given address
		//	so initialize the display
		//
		m_usAddress			= address;
		m_bDisplayConnected	= true;

		if( CHIP_TYPE_SSD1306 == m_ChipType )
		{
			m_usDisplayColumnOffset	= 0;

			Initssd1306();
		}
		else
		{
			m_usDisplayColumnOffset	= DISPLAY_COLUMN_OFFSET_DEFAULT;

			Initsh1106();
		}

		SendCommand( OPC_PAGE_ADDRESS );
		SendCommand( OPC_COLUMN_ADDRESS_LOW );
		SendCommand( OPC_COLUMN_ADDRESS_HIGH );
		SendCommand( OPC_DISPLAY_ON );
		SendCommand( OPC_SEG_ROTATION_RIGHT );
		SendCommand( OPC_OUTPUT_SCAN_NORMAL );

		Clear();
	}
	
	return( usError );
}


//**************************************************************************
//	MaxTextLines
//--------------------------------------------------------------------------
//	The function returns the maximum number of text lines
//	that are possible on the display.
//
uint8_t SimpleDisplayClass::MaxTextLines( void )
{
	return( TEXT_LINES );
}


//**************************************************************************
//	MaxTextColumns
//--------------------------------------------------------------------------
//	The function returns the maximum number of text columns
//	that are possible on the display.
//
uint8_t SimpleDisplayClass::MaxTextColumns( void )
{
	return( TEXT_COLUMNS );
}


//**************************************************************************
//	SetCursor
//--------------------------------------------------------------------------
//	The function sets the cursor to the given line and column
//	valid values are:
//		line:	0 -  7
//		column:	0 - 15
//
void SimpleDisplayClass::SetCursor( uint8_t usTextLine, uint8_t usTextColumn )
{
	uint8_t		usAddressLow;
	uint8_t		usAddressHigh;


	if( m_bDisplayConnected && (TEXT_LINES > usTextLine) && (TEXT_COLUMNS > usTextColumn) )
	{
		//------------------------------------------------------------------
		//	store the new cursor position
		//
		m_usTextLine	= usTextLine;
		m_usTextColumn	= usTextColumn;

		//------------------------------------------------------------------
		//	take care of the display line shift
		//	and correct the text line accordingly
		//
		usTextLine += m_usLineOffset;

		if( TEXT_LINES <= usTextLine )
		{
			usTextLine -= TEXT_LINES;
		}

		//------------------------------------------------------------------
		//	preparation for the command that will be send to the display
		//
		usTextLine &= MASK_PAGE_ADDRESS;
		g_arusPositionCommandBuffer[ IDX_PAGE_ADDRESS ] = OPC_PAGE_ADDRESS | usTextLine;

		//------------------------------------------------------------------
		//	calculate bit column
		//	the calculated bit column is the start column of a character
		//
		usTextColumn <<= 3;		//	multiply by 8
		usTextColumn  += m_usDisplayColumnOffset;

		//------------------------------------------------------------------
		//	preparation for the commands that will be send to the display
		//
		usAddressLow	 = usTextColumn & MASK_COLUMN_ADDRESS_LOW;
		g_arusPositionCommandBuffer[ IDX_COLUMN_ADDRESS_LOW  ] = OPC_COLUMN_ADDRESS_LOW | usAddressLow;

		usAddressHigh	 = usTextColumn & MASK_COLUMN_ADDRESS_HIGH;
		usAddressHigh >>= 4;
		g_arusPositionCommandBuffer[ IDX_COLUMN_ADDRESS_HIGH ] = OPC_COLUMN_ADDRESS_HIGH | usAddressHigh;

		//------------------------------------------------------------------
		//	now send the commands to position the cursor to the display
		//
		Wire.beginTransmission( m_usAddress );
		Wire.write( g_arusPositionCommandBuffer, sizeof( g_arusPositionCommandBuffer ) );
		Wire.endTransmission();
	}
}


//**************************************************************************
//	PrintChar
//--------------------------------------------------------------------------
//	This function will print the given character on the display starting at
//	the actual cursor position.
//	If the text contains a new line character ('\n') then the output will
//	continue at the beginning of the next line.
//	If the text ouput reaches the end of the line then depending of the
//	PrintMode the cursor will be set to the beginning of the (next) line
//	and the text output will continue there.
//
void SimpleDisplayClass::PrintChar( uint8_t usCharIdx )
{

#ifdef ARDUINO_ARCH_AVR
	const uint8_t *	pusActualColumn;
#endif

	uint16_t		uiHelper;
	uint8_t			usLetterColumn;

	if( m_bDisplayConnected )
	{
		if( '\n' == usCharIdx )
		{
			NextLine( true );
		}
		else if( (' ' <= usCharIdx) && (128 > usCharIdx) )
		{
			//--------------------------------------------------------------
			//	if we reached the end of the line then depending of the
			//	PrintMode continue in the 'next line'
			//
			if( TEXT_COLUMNS <= m_usTextColumn )
			{
				NextLine( false );
			}

			//--------------------------------------------------------------
			//	this is a printable character, so calculate the pointer
			//	into the font array to that position where the bitmap of
			//	this character starts
			//
#ifdef PRINT_DEBUG_INFO
			Serial.print( "PrintChar( " );
			Serial.print( (char)usCharIdx );
#endif

			uiHelper   = usCharIdx - 32;
			uiHelper <<= 3;	//	mit 8 multiplizieren

#ifdef PRINT_DEBUG_INFO
			Serial.print( " ): Idx: " );
			Serial.print( uiHelper );
			Serial.print( " => " );
#endif

#ifdef ARDUINO_ARCH_AVR
			pusActualColumn = &font8x8_simple[ 0 ] + uiHelper;
#endif

			//--------------------------------------------------------------
			//	transmit the bitmap of the character to the display
			//
			Wire.beginTransmission( m_usAddress );

			Wire.write( PREFIX_DATA );

			for( uint8_t idx = 0 ; PIXELS_CHAR_WIDTH > idx ; idx++ )
			{

#ifdef ARDUINO_ARCH_AVR
				usLetterColumn = pgm_read_byte( pusActualColumn );
				pusActualColumn++;
#else
				usLetterColumn = (uint8_t)font8x8_simple[ uiHelper ];
				uiHelper++;
#endif

#ifdef PRINT_DEBUG_INFO
				Serial.print( " " );
				Serial.print( usLetterColumn, HEX );
				Serial.print( " " );
#endif

				if( m_bInverse )
				{
					usLetterColumn = ~usLetterColumn;
				}

				Wire.write( usLetterColumn );
			}

			Wire.endTransmission();

#ifdef PRINT_DEBUG_INFO
			Serial.println();
#endif

			//--------------------------------------------------------------
			//	one character printed, so move cursor
			//
			m_usTextColumn++;
		}
	}
}


#ifdef ARDUINO_ARCH_AVR

//**************************************************************************
//	Print
//--------------------------------------------------------------------------
//	This function will print the given text on the display starting at
//	the actual cursor position.
//	If the text contains a new line character ('\n') then the output will
//	continue at the beginning of the next line.
//	If the text ouput reaches the end of the line then depending of the
//	PrintMode the cursor will be set to the beginning of the (next) line
//	and the text output will continue there.
//
void SimpleDisplayClass::Print( const __FlashStringHelper* cstrText )
{
	if( m_bDisplayConnected )
	{
		PGM_P	pText		= reinterpret_cast<PGM_P>( cstrText );
		uint8_t	usCharIdx	= pgm_read_byte( pText++ );

		while( 0x00 != usCharIdx )
		{
			PrintChar( usCharIdx );

			usCharIdx = pgm_read_byte( pText++ );
		}
	}
}


//**************************************************************************
//	Print
//--------------------------------------------------------------------------
//	This function will print the given text on the display starting at
//	the actual cursor position and then sets the cursor to the beginning
//	of the next line.
//	If the text contains a new line character ('\n') then the output will
//	continue at the beginning of the next line.
//	If the text ouput reaches the end of the line then depending of the
//	PrintMode the cursor will be set to the beginning of the (next) line
//	and the text output will continue there.
//
void SimpleDisplayClass::PrintLn( const __FlashStringHelper* cstrText )
{
	if( m_bDisplayConnected )
	{
		Print( cstrText );
		NextLine( true );
	}
}

#endif


//**************************************************************************
//	Print
//--------------------------------------------------------------------------
//	This function will print the given text on the display starting at
//	the actual cursor position.
//	If the text contains a new line character ('\n') then the output will
//	continue at the beginning of the next line.
//	If the text ouput reaches the end of the line then depending of the
//	PrintMode the cursor will be set to the beginning of the (next) line
//	and the text output will continue there.
//
void SimpleDisplayClass::Print( char* strText )
{
	if( m_bDisplayConnected )
	{
		uint8_t	usCharIdx	= *strText++;

		while( 0x00 != usCharIdx )
		{
			PrintChar( usCharIdx );

			usCharIdx = *strText++;
		}
	}
}


//**************************************************************************
//	Print
//--------------------------------------------------------------------------
//	This function will print the given text on the display starting at
//	the actual cursor position and then sets the cursor to the beginning
//	of the next line.
//	If the text contains a new line character ('\n') then the output will
//	continue at the beginning of the next line.
//	If the text ouput reaches the end of the line then depending of the
//	PrintMode the cursor will be set to the beginning of the (next) line
//	and the text output will continue there.
//
void SimpleDisplayClass::PrintLn( char* strText )
{
	if( m_bDisplayConnected )
	{
		Print( strText );
		NextLine( true );
	}
}


//**************************************************************************
//	Clear
//--------------------------------------------------------------------------
//	The function deletes all text shown on the display.
//
void SimpleDisplayClass::Clear( void )
{
	if( m_bDisplayConnected )
	{
		for( uint8_t usTextLine = 0 ; usTextLine < TEXT_LINES ; usTextLine++ )
		{
			ClearLine( usTextLine );
		}

		//------------------------------------------------------------------
		//	Set the display line offset back to the default value '0'.
		//	That means beginn to display the display with the top line.
		//
		m_usLineOffset = 0;

		SendCommand( OPC_DISPLAY_LINE_OFFSET, 0 );

		//------------------------------------------------------------------
		//	set the cursor to home position
		//
		SetCursor( 0, 0 );
	}
}


//**************************************************************************
//	ClearLine
//--------------------------------------------------------------------------
//	The function deletes the text line at the given cursor position and
//	sets the cursor to the bginning of that line.
//
void SimpleDisplayClass::ClearLine( uint8_t usLineToClear )
{
	uint8_t		usLoop1End;
	uint8_t		usLoop2End;


	if( m_bDisplayConnected )
	{
		//--------------------------------------------------------------
		//	at the end of the function the cursor will be positioned to
		//	the beginning of the line that will be cleared
		//
		m_usTextLine	= usLineToClear;
		m_usTextColumn	= 0;

		//--------------------------------------------------------------
		//	take care of the display line shift
		//	and correct the line to clear accordingly
		//
		usLineToClear += m_usLineOffset;

		if( TEXT_LINES <= usLineToClear )
		{
			usLineToClear -= TEXT_LINES;
		}

		//--------------------------------------------------------------
		//	preparation for the command that will be send to the
		//	display: set cursor to actual line first column
		//
		usLineToClear &= MASK_PAGE_ADDRESS;
		g_arusPositionCommandBuffer[ IDX_PAGE_ADDRESS ] = OPC_PAGE_ADDRESS | usLineToClear;
		g_arusPositionCommandBuffer[ IDX_COLUMN_ADDRESS_LOW  ] = OPC_COLUMN_ADDRESS_LOW;
		g_arusPositionCommandBuffer[ IDX_COLUMN_ADDRESS_HIGH ] = OPC_COLUMN_ADDRESS_HIGH;

		//--------------------------------------------------------------
		//	now send the commands to position the cursor to the display
		//
		Wire.beginTransmission( m_usAddress );
		Wire.write( g_arusPositionCommandBuffer, sizeof( g_arusPositionCommandBuffer ) );
		Wire.endTransmission();

		//--------------------------------------------------------------
		//	split the number of bytes to be send to clear the display
		//	to less than 31 ?? (I2C buffer size)
		//
		if( CHIP_TYPE_SSD1306 == m_ChipType )
		{
			//------------------------------------------------------
			//	ssd1306 has 128 pixel columns (8 x 16 = 128)
			//
			usLoop1End	= 8;
			usLoop2End	= 16;
		}
		else
		{
			//------------------------------------------------------
			//	sh1106 has 132 pixel columns (6 x 22 = 132)
			//
			usLoop1End	= 6;
			usLoop2End	= 22;
		}

		for( uint8_t idx1 = 0 ; idx1 < usLoop1End ; idx1++ )
		{
			Wire.beginTransmission( m_usAddress );

			Wire.write( PREFIX_DATA );

			for( uint8_t idx2 = 0 ; idx2 < usLoop2End ; idx2++ )
			{
				Wire.write( 0x00 );
			}

			Wire.endTransmission();
		}

		//--------------------------------------------------------------
		//	set cursor to first text position of this line
		//
		g_arusPositionCommandBuffer[ IDX_COLUMN_ADDRESS_LOW  ] =	  OPC_COLUMN_ADDRESS_LOW
																	| m_usDisplayColumnOffset;

		Wire.beginTransmission( m_usAddress );
		Wire.write( g_arusPositionCommandBuffer, sizeof( g_arusPositionCommandBuffer ) );
		Wire.endTransmission();
	}
}


//**************************************************************************
//	SetInverse
//--------------------------------------------------------------------------
//	This function inverses the display, means every OLED pixel that is 'on'
//	will be turned 'off' and vice versa.
//
void SimpleDisplayClass::SetInverse( bool bInverse )
{
	if( m_bDisplayConnected )
	{
		if( bInverse )
		{
			SendCommand( OPC_MODE_INVERSE );
		}
		else
		{
			SendCommand( OPC_MODE_NORMAL );
		}
	}
}


//**************************************************************************
//	Flip
//--------------------------------------------------------------------------
//	This function will turn the output on the display by 180 degree
//	and clears the display.
//
void SimpleDisplayClass::Flip( bool bFlip )
{
	if( m_bDisplayConnected )
	{
		if( bFlip )
		{
			SendCommand( OPC_SEG_ROTATION_LEFT );
			SendCommand( OPC_OUTPUT_SCAN_INVERSE );
		}
		else
		{
			SendCommand( OPC_SEG_ROTATION_RIGHT );
			SendCommand( OPC_OUTPUT_SCAN_NORMAL );
		}
		
		Clear();
	}
}


//**************************************************************************
//	SetDisplayColumnOffset
//--------------------------------------------------------------------------
//	With this function it is possible to adjust the display in left right
//	direction in a small range.
//
//	Background:
//	The OLED display with sh1106 chip has 132 columns of OLED pixels.
//	The font I use has 8 pixels per character.
//	So 128 pixels are used for one text line. This leads to a left over
//	of 4 pixels that can be used to adjust the text output on the display.
//
//	The OLED disable with ssd1306 chip has 128 columns of OLED pixels.
//	So the column offset is allways '0'.
//
void SimpleDisplayClass::SetDisplayColumnOffset( uint8_t usOffset )
{
//-----------------------------------------------------------------------
//	suppress compiler warning
//
//	if( (DISPLAY_COLUMN_OFFSET_MIN <= usOffset) && (DISPLAY_COLUMN_OFFSET_MAX >= usOffset) )
//
	if( DISPLAY_COLUMN_OFFSET_MAX >= usOffset )
	{
		if( CHIP_TYPE_SSD1306 == m_ChipType )
		{
			m_usDisplayColumnOffset = 0;
		}
		else
		{
			m_usDisplayColumnOffset = usOffset;
		}
	}
}


//**************************************************************************
//	Initsh1106 (private)
//--------------------------------------------------------------------------
//	This function will send the initialize sequence to a display with
//	a ssd1306 chip type.
//
void SimpleDisplayClass::Initsh1106( void )
{
	SendCommand( OPC_DISPLAY_OFF );
	SendCommand( OPC_ENTIRE_DISPLAY_NORMAL );
	SendCommand( OPC_CLK_DIV_OSC_FREQ, (OSC_FREQ_VARIATION_P_M_0 | CLOCK_DIV_RATIO_1) );
	SendCommand( OPC_SET_MULTIPLEX_RATIO, 0x3F );
	SendCommand( OPC_DISPLAY_LINE_OFFSET, 0 );
	SendCommand( OPC_DISPLAY_START_LINE );
	SendCommand( OPC_CHARGE_PUMP_SETTING, 0x14 );
	SendCommand( OPC_DC_DC_CONTROL_MODE, DC_DC_ON );
	SendCommand( OPC_DIS_PRE_CHARGE_PERIOD, (DIS_CHARGE_PERIOD_DCLK_2 | PRE_CHARGE_PERIOD_DCLK_2) );
	SendCommand( OPC_SET_VCOM_DESELECT_LEVEL, 0x35 );
	SendCommand( OPC_DC_DC_PUMP_VOLTAGE_8_0 );
	SendCommand( OPC_SET_CONTRAST, 0xFF );
	SendCommand( OPC_MODE_NORMAL );
	SendCommand( OPC_SET_COM_PINS, 0x12 );
}


//**************************************************************************
//	Initssd1306 (private)
//--------------------------------------------------------------------------
//	This function will send the initialize sequence to a display with
//	a ssd1306 chip type.
//
void SimpleDisplayClass::Initssd1306( void )
{
	SendCommand( OPC_DISPLAY_OFF );
	SendCommand( OPC_CLK_DIV_OSC_FREQ, (OSC_FREQ_VARIATION_P_15 | CLOCK_DIV_RATIO_1) );
	SendCommand( OPC_SET_MULTIPLEX_RATIO, 0x3F );
	SendCommand( OPC_DISPLAY_LINE_OFFSET, 0 );
	SendCommand( OPC_DISPLAY_START_LINE );
	SendCommand( OPC_CHARGE_PUMP_SETTING, 0x14 );
	SendCommand( OPC_MEMORY_ADR_MODE, ADR_MODE_PAGE );
	SendCommand( OPC_SET_COM_PINS, 0x12 );
	SendCommand( OPC_SET_CONTRAST, 0xCF );
//	SendCommand( OPC_DIS_PRE_CHARGE_PERIOD, (DIS_CHARGE_PERIOD_DCLK_2 | PRE_CHARGE_PERIOD_DCLK_2) );
	SendCommand( OPC_DIS_PRE_CHARGE_PERIOD, (DIS_CHARGE_PERIOD_DCLK_15 | PRE_CHARGE_PERIOD_DCLK_1) );
	SendCommand( OPC_SET_VCOM_DESELECT_LEVEL, 0x40 );
//	SendCommand( OPC_DEACTIVATE_SCROLL );	//	I think this is not needed, because we are in Page mode
	SendCommand( OPC_ENTIRE_DISPLAY_NORMAL );
	SendCommand( OPC_MODE_NORMAL );
}


//**************************************************************************
//	SendCommand (private)
//--------------------------------------------------------------------------
//	This function will send a one byte command to the display.
//	A one byte command exists of
//		-	1 byte prefix
//		-	1 byte command code
//
void SimpleDisplayClass::SendCommand( uint8_t usOpCode )
{
	Wire.beginTransmission( m_usAddress );

	Wire.write( PREFIX_LAST_COMMAND );
	Wire.write( usOpCode );

	Wire.endTransmission();
}


//**************************************************************************
//	SendCommand (private)
//--------------------------------------------------------------------------
//	This function will send a two byte command to the display.
//	A two byte command exists of
//		-	1 byte prefix
//		-	1 byte command code
//		-	1 byte parameter
//
void SimpleDisplayClass::SendCommand( uint8_t usOpCode, uint8_t usParameter )
{
	Wire.beginTransmission( m_usAddress );

	Wire.write( PREFIX_LAST_COMMAND );
	Wire.write( usOpCode );
	Wire.write( usParameter );

	Wire.endTransmission();
}


//**************************************************************************
//	NextLine (private)
//--------------------------------------------------------------------------
//	The function will set the cursor to the beginning of the 'next print
//	line'. Which will be the 'next print line' depends on the print mode
//	and the function parameter.
//
void SimpleDisplayClass::NextLine( bool bShiftLine )
{
	m_usTextColumn	= 0;

	if( PM_SCROLL_LINE == m_PrintMode )
	{
		//------------------------------------------------------------------
		//	PrintMode is scroll line
		//	if		the cursor is in the last line of the display,
		//	then	stay there and shift all other lines one up
		//	else	set cursor to the next line
		//
		if( (TEXT_LINES - 1) == m_usTextLine )
		{
			ShiftDisplayOneLine();
		}
		else
		{
			m_usTextLine++;
		}
	}
	else if( bShiftLine || (PM_OVERWRITE_NEXT_LINE == m_PrintMode) )
	{
		//------------------------------------------------------------------
		//	if PrintMode is overwrite next line or bShiftLine is 'true'
		//	set cursor to the next line
		//
		m_usTextLine++;
	}

	//----------------------------------------------------------------------
	//	check if the cursor is set to the allowed line range
	//
	if( TEXT_LINES <= m_usTextLine )
	{
		m_usTextLine = 0;
	}

	//----------------------------------------------------------------------
	//	now set the cursor to the new position and
	//	if required clear the line
	//
	SetCursor( m_usTextLine, m_usTextColumn );

	if( PM_OVERWRITE_NEXT_LINE < m_PrintMode )
	{
		ClearLine();
	}
}


//**************************************************************************
//	SetDisplayLineOffset (private)
//--------------------------------------------------------------------------
//	Die Funktion
//
void SimpleDisplayClass::ShiftDisplayOneLine( void )
{
	m_usLineOffset++;
	
	if( TEXT_LINES <= m_usLineOffset )
	{
		m_usLineOffset = 0;
	}

	SendCommand( OPC_DISPLAY_LINE_OFFSET, (m_usLineOffset << 3) );
}
