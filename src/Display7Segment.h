
#pragma once

//##########################################################################
//#
//#		Display7Segment.h
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

#include "DisplayNumberColon.h"


//==========================================================================
//
//		C L A S S   D E F I N I T I O N S
//
//==========================================================================


////////////////////////////////////////////////////////////////////////////
//	CLASS: Display7SegmentClass
//
class Display7SegmentClass : public DisplayNumberColonClass
{
	public:
		Display7SegmentClass();
		Display7SegmentClass( chip_type_t chip_type );

		virtual void DrawDigit0(  uint8_t usColumn );
		virtual void DrawDigit1(  uint8_t usColumn );
		void DrawDigit2(  uint8_t usColumn );
		void DrawDigit3(  uint8_t usColumn );
		void DrawDigit4(  uint8_t usColumn );
		void DrawDigit5(  uint8_t usColumn );
		void DrawDigit6(  uint8_t usColumn );
		void DrawDigit7(  uint8_t usColumn );
		void DrawDigit8(  uint8_t usColumn );
		void DrawDigit9(  uint8_t usColumn );

	protected:
		void ClearPageSegment( uint8_t usPage, uint8_t usColumn );

		void DrawTopSegment(    uint8_t usPage, uint8_t usColumn );
		void DrawBottomSegment( uint8_t usPage, uint8_t usColumn );
		void DrawUpperSegment(  uint8_t usPage, uint8_t usColumn );
		void DrawLowerSegment(  uint8_t usPage, uint8_t usColumn );

		void ClearHorizontalSegment( uint8_t usPage, uint8_t usColumn );
		void ClearVerticalSegment(   uint8_t usPage, uint8_t usColumn );
};
