//##########################################################################
//#
//#		Display7Segment.cpp
//#
//#-------------------------------------------------------------------------
//#
//#	The class defined here will draw numbers and a colon as 7 segment
//#	characters. The 7 segment numbers have the height off all display lines.
//#	In this case: 64 pixels height.
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

#include "Display7Segment.h"


//==========================================================================
//
//		G L O B A L   V A R I A B L E S
//
//==========================================================================

uint8_t g_usSegmentClear[]	= { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t	g_usSegmentTop[]	= {	0x06, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
								0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x06 };

uint8_t g_usSegmentBottom[]	= { 0x60, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
								0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x60 };

uint8_t g_usUpperStart[]	= { 0xE0, 0xF0, 0xF0, 0xE0 };
uint8_t g_usLowerStart[]	= { 0xFE, 0xFF, 0xFF, 0xFE };
uint8_t g_usSegmentMiddle[]	= { 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t g_usSegmentEnd[]	= { 0x07, 0x0F, 0x0F, 0x07 };


////////////////////////////////////////////////////////////////////////////
//
//	CLASS: Display7SegmentClass
//


//**************************************************************************
//	Constructor
//--------------------------------------------------------------------------
//	description
//
Display7SegmentClass::Display7SegmentClass()
	: DisplayNumberColonClass()
{
}


//**************************************************************************
//	Constructor
//--------------------------------------------------------------------------
//	description
//
Display7SegmentClass::Display7SegmentClass( chip_type_t chipType )
	: DisplayNumberColonClass( chipType )
{
}


//**************************************************************************
//	DrawDigit0
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawDigit0( uint8_t usColumn )
{
	ClearDigit( usColumn );

	DrawTopSegment(		0, usColumn +  4 );
	DrawUpperSegment(	0, usColumn		 );
	DrawUpperSegment(	0, usColumn + 17 );
	DrawLowerSegment(	4, usColumn		 );
	DrawLowerSegment(	4, usColumn + 17 );
	DrawBottomSegment(	7, usColumn +  4 );
}


//**************************************************************************
//	DrawDigit1
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawDigit1( uint8_t usColumn )
{
	ClearDigit( usColumn );

	DrawUpperSegment(	0, usColumn + 17 );
	DrawLowerSegment(	4, usColumn + 17 );
}


//**************************************************************************
//	DrawDigit2
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawDigit2( uint8_t usColumn )
{
	ClearDigit( usColumn );

	DrawTopSegment(		0, usColumn +  4 );
	DrawUpperSegment(	0, usColumn + 17 );
	DrawBottomSegment(	3, usColumn +  4 );
	DrawLowerSegment(	4, usColumn		 );
	DrawBottomSegment(	7, usColumn +  4 );
}


//**************************************************************************
//	DrawDigit3
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawDigit3( uint8_t usColumn )
{
	ClearDigit( usColumn );

	DrawTopSegment(		0, usColumn +  4 );
	DrawUpperSegment(	0, usColumn + 17 );
	DrawBottomSegment(	3, usColumn +  4 );
	DrawLowerSegment(	4, usColumn + 17 );
	DrawBottomSegment(	7, usColumn +  4 );
}


//**************************************************************************
//	DrawDigit4
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawDigit4( uint8_t usColumn )
{
	ClearDigit( usColumn );

	DrawUpperSegment(	0, usColumn		 );
	DrawUpperSegment(	0, usColumn + 17 );
	DrawBottomSegment(	3, usColumn +  4 );
	DrawLowerSegment(	4, usColumn + 17 );
}


//**************************************************************************
//	DrawDigit5
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawDigit5( uint8_t usColumn )
{
	ClearDigit( usColumn );

	DrawTopSegment(		0, usColumn +  4 );
	DrawUpperSegment(	0, usColumn		 );
	DrawBottomSegment(	3, usColumn +  4 );
	DrawLowerSegment(	4, usColumn + 17 );
	DrawBottomSegment(	7, usColumn +  4 );
}


//**************************************************************************
//	DrawDigit6
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawDigit6( uint8_t usColumn )
{
	ClearDigit( usColumn );

	DrawTopSegment(		0, usColumn +  4 );
	DrawUpperSegment(	0, usColumn		 );
	DrawBottomSegment(	3, usColumn +  4 );
	DrawLowerSegment(	4, usColumn		 );
	DrawLowerSegment(	4, usColumn + 17 );
	DrawBottomSegment(	7, usColumn +  4 );
}


//**************************************************************************
//	DrawDigit7
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawDigit7( uint8_t usColumn )
{
	ClearDigit( usColumn );

	DrawTopSegment(		0, usColumn +  4 );
	DrawUpperSegment(	0, usColumn + 17 );
	DrawLowerSegment(	4, usColumn + 17 );
}


//**************************************************************************
//	DrawDigit8
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawDigit8( uint8_t usColumn )
{
	ClearDigit( usColumn );

	DrawTopSegment(		0, usColumn +  4 );
	DrawUpperSegment(	0, usColumn		 );
	DrawUpperSegment(	0, usColumn + 17 );
	DrawBottomSegment(	3, usColumn +  4 );
	DrawLowerSegment(	4, usColumn		 );
	DrawLowerSegment(	4, usColumn + 17 );
	DrawBottomSegment(	7, usColumn +  4 );
}


//**************************************************************************
//	DrawDigit9
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawDigit9( uint8_t usColumn )
{
	ClearDigit( usColumn );

	DrawTopSegment(		0, usColumn +  4 );
	DrawUpperSegment(	0, usColumn		 );
	DrawUpperSegment(	0, usColumn + 17 );
	DrawBottomSegment(	3, usColumn +  4 );
	DrawLowerSegment(	4, usColumn + 17 );
	DrawBottomSegment(	7, usColumn +  4 );
}


//**************************************************************************
//	ClearPageSegment (protected)
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::ClearPageSegment( uint8_t usPage, uint8_t usColumn )
{
	SetDrawPosition( usPage, usColumn );
	Draw( g_usSegmentClear, sizeof( g_usSegmentClear ) );
}


//**************************************************************************
//	DrawTopSegment (protected)
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawTopSegment( uint8_t usPage, uint8_t usColumn )
{
	SetDrawPosition( usPage, usColumn );
	Draw( g_usSegmentTop, sizeof( g_usSegmentTop ) );
}


//**************************************************************************
//	DrawTopSegment (protected)
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawBottomSegment( uint8_t usPage, uint8_t usColumn )
{
	SetDrawPosition( usPage, usColumn );
	Draw( g_usSegmentBottom, sizeof( g_usSegmentBottom ) );
}


//**************************************************************************
//	DrawTopSegment (protected)
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawUpperSegment( uint8_t usPage, uint8_t usColumn )
{
	SetDrawPosition( usPage, usColumn );
	Draw( g_usUpperStart, sizeof( g_usUpperStart ) );
	SetDrawPosition( usPage + 1, usColumn );
	Draw( g_usSegmentMiddle, sizeof( g_usSegmentMiddle ) );
	SetDrawPosition( usPage + 2, usColumn );
	Draw( g_usSegmentMiddle, sizeof( g_usSegmentMiddle ) );
	SetDrawPosition( usPage + 3, usColumn );
	Draw( g_usSegmentEnd, sizeof( g_usSegmentEnd ) );
}


//**************************************************************************
//	DrawTopSegment (protected)
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::DrawLowerSegment( uint8_t usPage, uint8_t usColumn )
{
	SetDrawPosition( usPage, usColumn );
	Draw( g_usLowerStart, sizeof( g_usLowerStart ) );
	SetDrawPosition( usPage + 1, usColumn );
	Draw( g_usSegmentMiddle, sizeof( g_usSegmentMiddle ) );
	SetDrawPosition( usPage + 2, usColumn );
	Draw( g_usSegmentMiddle, sizeof( g_usSegmentMiddle ) );
	SetDrawPosition( usPage + 3, usColumn );
	Draw( g_usSegmentEnd, sizeof( g_usSegmentEnd ) );
}


//**************************************************************************
//	ClearHorizontalSegment (protected)
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::ClearHorizontalSegment( uint8_t usPage, uint8_t usColumn )
{
	SetDrawPosition( usPage, usColumn );
	Draw( g_usSegmentClear, sizeof( g_usSegmentTop ) );
}


//**************************************************************************
//	ClearUpperSegment (protected)
//--------------------------------------------------------------------------
//	description
//
void Display7SegmentClass::ClearVerticalSegment( uint8_t usPage, uint8_t usColumn )
{
	SetDrawPosition( usPage, usColumn );
	Draw( g_usSegmentClear, sizeof( g_usLowerStart ) );
	SetDrawPosition( usPage + 1, usColumn );
	Draw( g_usSegmentClear, sizeof( g_usSegmentMiddle ) );
	SetDrawPosition( usPage + 2, usColumn );
	Draw( g_usSegmentClear, sizeof( g_usSegmentMiddle ) );
	SetDrawPosition( usPage + 3, usColumn );
	Draw( g_usSegmentClear, sizeof( g_usSegmentEnd ) );
}
