//##########################################################################
//#
//#		FlashFlipDisplay.ino
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
//#	This example shows how to flash (invert) the display
//#	(e.g. to display an alarm)
//# and it shows how to "flip" the display (turn text by 180 degree).
//#
//#-------------------------------------------------------------------------
//#
//#	Dieses Beispiel zeigt, wie das Display blinken kann
//#	(z.B.: für die Ausgabe eines Alarms)
//#	und wie die Ausgabe auf dem Display um 180 Grad gedreht wird (flip).
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

#ifdef ARDUINO_ARCH_AVR
	#include <avr/pgmspace.h>
#endif


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
}


//**************************************************************************
//	loop
//--------------------------------------------------------------------------
//	description
//
void loop()
{
	bool	bInvertDisplay	= false;

	//----------------------------------------------------------------------
	//	print Info
	//
	g_clDisplay.SetCursor( 2, 0 );
	g_clDisplay.Print( F( "Flash-Flip-Demo" ) );

	delay( 2000 );

	//----------------------------------------------------------------------
	//	print message and ...
	//
	g_clDisplay.Clear();
	g_clDisplay.SetInverseFont( true );
	g_clDisplay.SetCursor( 1, 0 );
	g_clDisplay.PrintLn( F( "               " ) );
	g_clDisplay.PrintLn( F( " ! ATTENTION ! " ) );
	g_clDisplay.PrintLn( F( "               " ) );
	g_clDisplay.PrintLn( F( " Flash display " ) );
	g_clDisplay.PrintLn( F( "               " ) );
	g_clDisplay.SetInverseFont( false );

	delay( 2000 );

	//----------------------------------------------------------------------
	//	... flash display
	//
	for( uint8_t idx = 0 ; idx < 10 ; idx++ )
	{
		bInvertDisplay = !bInvertDisplay;

		g_clDisplay.SetInverse( bInvertDisplay );
		
		delay( 250 );
	}

	delay( 3000 );

	//----------------------------------------------------------------------
	//	print info text and ...
	//
	g_clDisplay.Clear();

	g_clDisplay.SetCursor( 3, 0 );
	g_clDisplay.Print( F( "Now flip display" ) );

	delay( 2000 );

	//----------------------------------------------------------------------
	//	... flip the display
	//
	g_clDisplay.Flip( true );

	g_clDisplay.PrintLn( F( "The text now" ) );
	g_clDisplay.PrintLn( F( "is turned by" ) );
	g_clDisplay.PrintLn( F( "180 degree." ) );

	delay( 4000 );

	g_clDisplay.Print( F( "\nand flip back" ) );

	delay( 2000 );

	g_clDisplay.Flip( false );

	g_clDisplay.PrintLn( F( "The text is" ) );
	g_clDisplay.PrintLn( F( "back to normal" ) );

	delay( 5000 );

	g_clDisplay.Clear();
}
