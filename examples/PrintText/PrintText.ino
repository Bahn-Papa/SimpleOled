//##########################################################################
//#
//#		PrintText.ino
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
//#	This program will write Text on an OLED display and demonstrates
//#	some options of the SimpleOled library, like
//#		- Cursor positioning
//#		- normal and inverse text font
//#
//#-------------------------------------------------------------------------
//#
//#	Dieses Programm schreibt Text auf ein OLED Display und zeigt dabei
//#	einige Möglichkeiten aus der SimpleOled Library:
//#		- Cursor positionieren
//#		- normale und inverse Textausgabe
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include <Arduino.h>
#include <stdint.h>
#include <SimpleOled.h>


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#ifndef ARDUINO_ARCH_AVR
	#undef F
	#define F( text )	(text)
#endif


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

char g_buffer[ 20 ];


//**************************************************************************
//	setup
//--------------------------------------------------------------------------
//	description
//
void setup()
{
	Serial.begin( 115200 );

	//------------------------------------------------------------------
	//	default settings:
	//		chip type:	ssd1306
	//		I2C adr:	60 (0x3C)
	//
	g_clDisplay.Init();

	//------------------------------------------------------------------
	//	alternate settings
	//
//	g_clDisplay.Init( CHIP_TYPE_SH1106, DISPLAY_ADDRESS );

	Serial.println( "SimpleOled Demo: PrintText" );
	Serial.println( "setup chip type sh1106" );
}


//**************************************************************************
//	loop
//--------------------------------------------------------------------------
//	description
//
void loop()
{
	Serial.println( "Print Text Demo" );

	g_clDisplay.Print( F( "Print Text Demo" ) );

	delay( 2000 );

	//----------------------------------------------------------------------
	//	positioning the cursor to the beginning of the third line
	//	print text in normal font mode
	//
	Serial.println( "Normal Text" );

	g_clDisplay.SetCursor( 2, 0 );
	g_clDisplay.PrintLn( F( "Normal Text\n" ) );

	delay( 1000 );

	//----------------------------------------------------------------------
	//	now print some text in inverse font mode
	//
	Serial.println( "Inverse Font" );

	g_clDisplay.SetInverseFont( true );
	g_clDisplay.Print( F( "Inverse Font" ) );

	delay( 3000 );

	//----------------------------------------------------------------------
	//	some additional text to demonstrate how to clear a line
	//
	Serial.println( "Clear this line" );

	g_clDisplay.SetCursor( 6, 0 );
	g_clDisplay.Print( F( "Clear this Line" ) );
	g_clDisplay.SetInverseFont( false );

	delay( 2000 );

	//----------------------------------------------------------------------
	//	this will clear the line where the cursor is in
	//
	g_clDisplay.ClearLine();

	delay( 1000 );

	//----------------------------------------------------------------------
	//	and now clear the hole display
	//	first some text ...
	//
	Serial.println( "Clear Display" );

	g_clDisplay.Print( F( "Clear Display" ) );

	delay( 2000 );

	//----------------------------------------------------------------------
	//	... and now clear the display
	//
	g_clDisplay.Clear();

	delay( 500 );

	//----------------------------------------------------------------------
	//	another example for positioning the cursor and writing some text
	//
	Serial.println( "Print stars" );

	g_clDisplay.SetInverseFont( true );
	g_clDisplay.SetCursor( 0, 0 );
	g_clDisplay.PrintChar( '*' );
	g_clDisplay.SetCursor( 0, 15 );
	g_clDisplay.PrintChar( '*' );
	g_clDisplay.SetCursor( 7, 0 );
	g_clDisplay.PrintChar( '*' );
	g_clDisplay.SetCursor( 7, 15 );
	g_clDisplay.PrintChar( '*' );
	g_clDisplay.SetInverseFont( false );

	delay( 500 );

	//----------------------------------------------------------------------
	//	In this example you can see how to print text with numbers in it.
	//	First prepare the text (in this case with function sprintf).
	//	Then print the text.
	Serial.println( "Print lines" );

	for( uint8_t idx = 0 ; idx < g_clDisplay.MaxTextLines() ; idx++ )
	{
		g_clDisplay.SetCursor( idx, 4 );

		sprintf( g_buffer, "Zeile %d", idx );
		g_clDisplay.Print( g_buffer );

		delay( 250 );
	}

	delay( 2000 );

	g_clDisplay.Clear();

	delay( 500 );

	//----------------------------------------------------------------------
	//	the last example for printing text.
	//
	Serial.println( "Print columns" );

	g_clDisplay.SetCursor( 4, 0 );
	g_clDisplay.Print( F( "Column" ) );
	g_clDisplay.SetCursor( 3, 0 );
	
	for( uint8_t idx = 0 ; idx < g_clDisplay.MaxTextColumns() ; idx++ )
	{
		if( 9 < idx )
		{
			g_clDisplay.SetCursor( 2, idx );
			g_clDisplay.Print( "1" );

			sprintf( g_buffer, "%d", idx - 10 );
			g_clDisplay.SetCursor( 3, idx );
		}
		else
		{
			sprintf( g_buffer, "%d", idx );
		}

		g_clDisplay.Print( g_buffer );
		
		delay( 250 );
	}

	delay( 2000 );

	g_clDisplay.Clear();

	delay( 2000 );
}
