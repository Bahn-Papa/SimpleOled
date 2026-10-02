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
//#		Copyright (c) 2026	Michael Pfeil
//#							Am Kuckhof 8
//#							D - 52146 Würselen
//#							GERMANY
//#
//#-------------------------------------------------------------------------
//#
//#	File Version:	7		Date: 02.10.2026
//#
//#	Implementation:
//#		-	splitt 'SimpleDisplayClass' into two classes
//#			 -	'DisplayBaseClass'
//#			 -	'SimpleDisplayClass'
//#			this is done in preparation for new display classes
//#			other than 'SimpleDisplayClass'
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


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

SimpleDisplayClass	g_clDisplay	= SimpleDisplayClass();

uint8_t	g_arusDataBuffer[ PIXELS_CHAR_WIDTH ];


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
	m_PrintMode		= PM_SCROLL_LINE;
	m_usTextLine	= 0;
	m_usTextColumn	= 0;
	m_usLineOffset	= 0;
	m_bInverse		= false;

	return( DisplayBaseClass::Init( chipType, address ) );
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
		//	calculate bit column
		//	the calculated bit column is the start column of a character
		//
		usTextColumn <<= 3;		//	multiply by 8

		//------------------------------------------------------------------
		//	now send the commands to position the cursor to the display
		//
		SetDrawPosition( usTextLine, usTextColumn );
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

				g_arusDataBuffer[ idx ] = usLetterColumn;
			}

			Draw( g_arusDataBuffer, PIXELS_CHAR_WIDTH );

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
		ClearDisplay();

		//------------------------------------------------------------------
		//	Set the display line offset back to the default value '0'.
		//	That means beginn to display the display with the top line.
		//
		m_usLineOffset = 0;

		SetDisplayLineOffset( 0 );

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

		ClearPage( usLineToClear );

		//--------------------------------------------------------------
		//	set cursor to first text position of this line
		//
		SetDrawPosition( usLineToClear, 0 );
	}
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
//	ShiftDisplayOneLine (private)
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

	SetDisplayLineOffset( (m_usLineOffset << 3) );
}
