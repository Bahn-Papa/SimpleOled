//##########################################################################
//#
//#		DisplayNumberColon.cpp
//#
//#-------------------------------------------------------------------------
//#
//#	The class defined here is the base class vor all large font displays.
//#	Printable characters are only numbers and the colon.
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
//#	File Version:	1		from: 06.10.2026
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

#include "DisplayNumberColon.h"


////////////////////////////////////////////////////////////////////////////
//
//	CLASS: DisplayNumberColonClass
//


//**************************************************************************
//	Constructor
//--------------------------------------------------------------------------
//	description
//
DisplayNumberColonClass::DisplayNumberColonClass()
	: DisplayBaseClass()
{
}


//**************************************************************************
//	Constructor
//--------------------------------------------------------------------------
//	description
//
DisplayNumberColonClass::DisplayNumberColonClass( chip_type_t chipType )
	: DisplayBaseClass( chipType )
{
}


//**************************************************************************
//	DrawDigit
//--------------------------------------------------------------------------
//	description
//
void DisplayNumberColonClass::DrawDigit( uint8_t usColumn, uint8_t usDigit )
{
	switch( usDigit )
	{
		case 0:
			DrawDigit0( usColumn );
			break;

		case 1:
			DrawDigit1( usColumn );
			break;

		case 2:
			DrawDigit2( usColumn );
			break;

		case 3:
			DrawDigit3( usColumn );
			break;

		case 4:
			DrawDigit4( usColumn );
			break;

		case 5:
			DrawDigit5( usColumn );
			break;

		case 6:
			DrawDigit6( usColumn );
			break;

		case 7:
			DrawDigit7( usColumn );
			break;

		case 8:
			DrawDigit8( usColumn );
			break;

		case 9:
			DrawDigit9( usColumn );
			break;

		default:
			ClearDigit( usColumn );
			break;
	}
}


//**************************************************************************
//	ClearDigit
//--------------------------------------------------------------------------
//	description
//
void DisplayNumberColonClass::ClearDigit( uint8_t usColumn )
{
	for( uint8_t idx = 0 ; 8 > idx ; idx++ )
	{
		ClearPageSegment( idx, usColumn );
	}
}


//**************************************************************************
//	DrawColon
//--------------------------------------------------------------------------
//	Draw the colon onto the display starting at the given 'usColumn'.
//
void DisplayNumberColonClass::DrawColon( uint8_t usColumn )
{
	uint8_t usSegmentColon[] = { 0x7E, 0x7E, 0x7E, 0x7E };

	SetDrawPosition( 2, usColumn );
	Draw( usSegmentColon, sizeof( usSegmentColon ) );
	SetDrawPosition( 5, usColumn );
	Draw( usSegmentColon, sizeof( usSegmentColon ) );
}


//**************************************************************************
//	ClearColon
//--------------------------------------------------------------------------
//	Clear the colon from the display starting at the given 'usColumn'.
//
void DisplayNumberColonClass::ClearColon( uint8_t usColumn )
{
	uint8_t usSegmentColon[] = { 0x00, 0x00, 0x00, 0x00 };

	SetDrawPosition( 2, usColumn );
	Draw( usSegmentColon, sizeof( usSegmentColon ) );
	SetDrawPosition( 5, usColumn );
	Draw( usSegmentColon, sizeof( usSegmentColon ) );
}
