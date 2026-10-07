
#pragma once

//##########################################################################
//#
//#		DisplayNumberColon.h
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

#include "DisplayBase.h"


//==========================================================================
//
//		C L A S S   D E F I N I T I O N S
//
//==========================================================================


////////////////////////////////////////////////////////////////////////////
//	CLASS: Display7SegmentClass
//
class DisplayNumberColonClass : public DisplayBaseClass
{
	public:
		DisplayNumberColonClass();
		DisplayNumberColonClass( chip_type_t chip_type );

		void DrawDigit(  uint8_t usColumn, uint8_t usDigit );
		void ClearDigit( uint8_t usColumn );

		virtual void DrawDigit0(  uint8_t usColumn ) = 0;
		virtual void DrawDigit1(  uint8_t usColumn ) = 0;
		virtual void DrawDigit2(  uint8_t usColumn ) = 0;
		virtual void DrawDigit3(  uint8_t usColumn ) = 0;
		virtual void DrawDigit4(  uint8_t usColumn ) = 0;
		virtual void DrawDigit5(  uint8_t usColumn ) = 0;
		virtual void DrawDigit6(  uint8_t usColumn ) = 0;
		virtual void DrawDigit7(  uint8_t usColumn ) = 0;
		virtual void DrawDigit8(  uint8_t usColumn ) = 0;
		virtual void DrawDigit9(  uint8_t usColumn ) = 0;

		void DrawColon(  uint8_t usColumn );
		void ClearColon( uint8_t usColumn );

	protected:
		virtual void ClearPageSegment( uint8_t usPage, uint8_t usColumn ) = 0;
};
