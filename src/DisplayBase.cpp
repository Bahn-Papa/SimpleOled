//##########################################################################
//#
//#		DisplayBase.cpp
//#
//#	The class defined here will act as a base class for all derived
//#	display classes (like 'SimpleDisplayClass') that have one of the
//#	controllers: ssd1306 or sh1106.
//#
//#	The class will contain all functions to setup and communicate with
//#	such a display over I2C bus.
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
//#	File Version:	1		from: 02.10.2026
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

#include <Arduino.h>
#include <Wire.h>

#include "DisplayBase.h"


//==========================================================================
//
//		D E F I N I T I O N S
//
//==========================================================================

#define DISPLAY_COLUMN_OFFSET_MAX		3
#define DISPLAY_COLUMN_OFFSET_MIN		0
#define DISPLAY_COLUMN_OFFSET_DEFAULT	2

#define DISPLAY_PAGES					8


//--------------------------------------------------------------------------
//	Definitions for I²C protocol
//
#define I2C_BUFFER_SIZE					30


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
//	CLASS: DisplayBaseClass
//


//**************************************************************************
//	Constructor
//--------------------------------------------------------------------------
//	description
//
DisplayBaseClass::DisplayBaseClass()
{
	m_ChipType			= CHIP_TYPE_UNKNOWN;
	m_bDisplayConnected = false;
}


//**************************************************************************
//	Constructor
//--------------------------------------------------------------------------
//	description
//
DisplayBaseClass::DisplayBaseClass( chip_type_t chipType )
{
	m_ChipType			= chipType;
	m_bDisplayConnected = false;
}


//**************************************************************************
//	Init
//--------------------------------------------------------------------------
//	The Function initializes the class, sets the display in default
//	operation mode, switches the display 'on', clears the display and
//	sets the cursor to home position (top left corner).
//
uint8_t DisplayBaseClass::Init( chip_type_t chipType, uint8_t address )
{
	uint8_t	usError;


	//------------------------------------------------------------------
	//	set initial values for internal variables
	//
	m_ChipType	= chipType;


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

		ClearDisplay();
	}
	
	return( usError );
}


//**************************************************************************
//	Init
//--------------------------------------------------------------------------
//	The Function initializes the class, sets the display in default
//	operation mode, switches the display 'on', clears the display and
//	sets the cursor to home position (top left corner).
//
uint8_t DisplayBaseClass::Init( uint8_t address )
{
	if( CHIP_TYPE_UNKNOWN == m_ChipType )
	{
		m_ChipType = CHIP_TYPE_SSD1306;
	}

	return( Init( m_ChipType, address ) );
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
//	The OLED display with ssd1306 chip has 128 columns of OLED pixels.
//	So the column offset is allways '0'.
//
void DisplayBaseClass::SetDisplayColumnOffset( uint8_t usOffset )
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
//	SetDisplayLineOffset
//--------------------------------------------------------------------------
//	Die Funktion
//
void DisplayBaseClass::SetDisplayLineOffset( uint8_t usOffset )
{
	SendCommand( OPC_DISPLAY_LINE_OFFSET, usOffset );
}


//**************************************************************************
//	ClearDisplay
//--------------------------------------------------------------------------
//	This function clears all pixels of the display.
//
void DisplayBaseClass::ClearDisplay( void )
{
	if( m_bDisplayConnected )
	{
		for( uint8_t usPage = 0 ; usPage < DISPLAY_PAGES ; usPage++ )
		{
			ClearPage( usPage );
		}
	}
}


//**************************************************************************
//	ClearPage
//--------------------------------------------------------------------------
//	This function will clear all pixels of the given display page
//
void DisplayBaseClass::ClearPage( uint8_t usPage )
{
	uint8_t		usLoop1End;
	uint8_t		usLoop2End;

	if( m_bDisplayConnected )
	{
		SetDrawPosition( usPage, 0 );

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

		//--------------------------------------------------------------
		//	now send the data to clear the page
		//
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
	}
}


//**************************************************************************
//	SetDrawPosition
//--------------------------------------------------------------------------
//	This function will set the pointer into the display ram where the
//	next display data will be stored.
//
void DisplayBaseClass::SetDrawPosition( uint8_t usPage, uint8_t usColumn )
{
	uint8_t		usAddressLow;
	uint8_t		usAddressHigh;

	if( m_bDisplayConnected && (DISPLAY_PAGES > usPage) )
	{
		//------------------------------------------------------------------
		//	preparation for the command that will be send to the display
		//
		usPage &= MASK_PAGE_ADDRESS;
		g_arusPositionCommandBuffer[ IDX_PAGE_ADDRESS ] = OPC_PAGE_ADDRESS | usPage;

		//------------------------------------------------------------------
		//	correct the bit column
		usColumn += m_usDisplayColumnOffset;

		//------------------------------------------------------------------
		//	preparation for the commands that will be send to the display
		//
		usAddressLow = usColumn & MASK_COLUMN_ADDRESS_LOW;
		g_arusPositionCommandBuffer[ IDX_COLUMN_ADDRESS_LOW  ] = OPC_COLUMN_ADDRESS_LOW | usAddressLow;

		usAddressHigh	= usColumn & MASK_COLUMN_ADDRESS_HIGH;
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
//	Draw
//--------------------------------------------------------------------------
//	This function sends the display data to the display
//
void DisplayBaseClass::Draw( uint8_t *pBuffer, uint8_t usSize )
{
	uint8_t idx;

	if( m_bDisplayConnected )
	{
		//------------------------------------------------------
		//	if the amount of data is bigger than the buffer,
		//	then transfer slices of data that fit into the buffer
		//
		while( I2C_BUFFER_SIZE < usSize )
		{
			Wire.beginTransmission( m_usAddress );

			Wire.write( PREFIX_DATA );

			for( idx = 0 ; I2C_BUFFER_SIZE > idx ; idx++ )
			{
				Wire.write( pBuffer[ idx ] );
			}

			Wire.endTransmission();

			usSize -= I2C_BUFFER_SIZE;
		}

		//------------------------------------------------------
		//	now transfer the rest of the data
		//
		Wire.beginTransmission( m_usAddress );

		Wire.write( PREFIX_DATA );

		for( idx = 0 ; usSize > idx ; idx++ )
		{
			Wire.write( pBuffer[ idx ] );
		}

		Wire.endTransmission();
	}
}


//**************************************************************************
//	SetInverse
//--------------------------------------------------------------------------
//	This function inverses the display, means every OLED pixel that is 'on'
//	will be turned 'off' and vice versa.
//
void DisplayBaseClass::SetInverse( bool bInverse )
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
void DisplayBaseClass::Flip( bool bFlip )
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
		
		ClearDisplay();
	}
}


//**************************************************************************
//	Initsh1106 (protected)
//--------------------------------------------------------------------------
//	This function will send the initialize sequence to a display with
//	a ssd1306 chip type.
//
void DisplayBaseClass::Initsh1106( void )
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
//	Initssd1306 (protected)
//--------------------------------------------------------------------------
//	This function will send the initialize sequence to a display with
//	a ssd1306 chip type.
//
void DisplayBaseClass::Initssd1306( void )
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
//	SendCommand (protected)
//--------------------------------------------------------------------------
//	This function will send a one byte command to the display.
//	A one byte command exists of
//		-	1 byte prefix
//		-	1 byte command code
//
void DisplayBaseClass::SendCommand( uint8_t usOpCode )
{
	Wire.beginTransmission( m_usAddress );

	Wire.write( PREFIX_LAST_COMMAND );
	Wire.write( usOpCode );

	Wire.endTransmission();
}


//**************************************************************************
//	SendCommand (protected)
//--------------------------------------------------------------------------
//	This function will send a two byte command to the display.
//	A two byte command exists of
//		-	1 byte prefix
//		-	1 byte command code
//		-	1 byte parameter
//
void DisplayBaseClass::SendCommand( uint8_t usOpCode, uint8_t usParameter )
{
	Wire.beginTransmission( m_usAddress );

	Wire.write( PREFIX_LAST_COMMAND );
	Wire.write( usOpCode );
	Wire.write( usParameter );

	Wire.endTransmission();
}
