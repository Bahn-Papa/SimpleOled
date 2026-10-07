//##########################################################################
//#
//#		Print7Segment.ino
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
//#	This program will write digits on an OLED display demonstrating
//#	some functions of the class 'Display7SegmentClass' and
//#	it's base class'DisplayNumberColonClass'.
//#
//##########################################################################


//==========================================================================
//
//		I N C L U D E S
//
//==========================================================================

#include <Arduino.h>
#include <stdint.h>
#include <Display7Segment.h>


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#ifndef ARDUINO_ARCH_AVR
	#undef F
	#define F( text )	(text)
#endif

#define BLINK_TIME_COLON		500
#define COUNTER_TIME			1000

#define POS_HOUR				29
#define POS_HOUR_TENS			2
#define POS_MINUTE				102
#define POS_MINUTE_TENS			75
#define POS_COLON				61


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

char g_buffer[ 20 ];
uint32_t	g_ulColonTimer		= 0;
uint32_t	g_ulCounterTimer	= 0;
uint8_t		g_usHour			= 0;
uint8_t		g_usMinute			= 0;
bool		g_bColonOn			= false;

DisplayNumberColonClass* g_Display = new Display7SegmentClass( CHIP_TYPE_SSD1306 );


//**************************************************************************
//	test
//--------------------------------------------------------------------------
//	description
//
void test()
{
	Serial.println( "Print Digits" );

	g_Display->DrawDigit2( POS_HOUR_TENS );

	delay( 500 );

	g_Display->DrawDigit4( POS_HOUR );

	delay( 500 );

	g_Display->DrawColon( POS_COLON );

	delay( 500 );

	g_Display->DrawDigit6( POS_MINUTE_TENS );

	delay( 500 );

	g_Display->DrawDigit8( POS_MINUTE );

	delay( 3000 );

	g_Display->ClearDisplay();

	delay( 2000 );
}


//**************************************************************************
//	printMinute
//--------------------------------------------------------------------------
//	description
//
void printMinute( uint8_t usNewMinute )
{
	uint8_t usMinuteTens	= g_usMinute  / 10;
	uint8_t usNewMinuteTens	= usNewMinute / 10;

	if( usMinuteTens != usNewMinuteTens )
	{
		g_Display->DrawDigit(  POS_MINUTE_TENS, usNewMinuteTens );
		g_Display->DrawDigit0( POS_MINUTE );
	}
	else
	{
		g_Display->DrawDigit( POS_MINUTE, (usNewMinute - (usNewMinuteTens * 10)) );
	}

	g_usMinute = usNewMinute;
}


//**************************************************************************
//	printHour
//--------------------------------------------------------------------------
//	description
//
void printHour( uint8_t usNewHour )
{
	uint8_t usHourTens		= g_usHour / 10;
	uint8_t usNewHourTens	= usNewHour / 10;

	if( usHourTens != usNewHourTens )
	{
		g_Display->DrawDigit(  POS_HOUR_TENS, usNewHourTens );
		g_Display->DrawDigit0( POS_HOUR );
	}
	else
	{
		g_Display->DrawDigit( POS_HOUR, (usNewHour - (usNewHourTens * 10)) );
	}

	g_usHour = usNewHour;
}


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
	g_Display->Init( DISPLAY_ADDRESS );

	Serial.println( "SimpleOled Demo: Print7Segment" );
	Serial.println( "setup chip type ssd1306" );

	test();

	g_Display->DrawDigit0( POS_HOUR_TENS );
	g_Display->DrawDigit0( POS_HOUR );
	g_Display->DrawDigit0( POS_MINUTE_TENS );
	g_Display->DrawDigit0( POS_MINUTE );

	g_ulColonTimer		= millis();
	g_ulCounterTimer	= millis();
}


//**************************************************************************
//	loop
//--------------------------------------------------------------------------
//	description
//
void loop()
{
	uint32_t ulActualTime	= millis();

	//--------------------------------------------------------------
	//	handle colon
	//
	if( ulActualTime > g_ulColonTimer )
	{
		g_ulColonTimer = ulActualTime + BLINK_TIME_COLON;

		if( g_bColonOn )
		{
			g_Display->ClearColon( POS_COLON );
			g_bColonOn = false;
		}
		else
		{
			g_Display->DrawColon( POS_COLON );
			g_bColonOn = true;
		}
	}

	//--------------------------------------------------------------
	//	handle hours and minutes
	//
	if( ulActualTime > g_ulCounterTimer )
	{
		g_ulCounterTimer = ulActualTime + COUNTER_TIME;

		if( 60 == (g_usMinute + 1) )
		{
			printHour( g_usHour + 1 );
		}

		printMinute( g_usMinute + 1 );
	}
}
