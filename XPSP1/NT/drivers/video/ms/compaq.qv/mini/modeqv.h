/*++

Copyright (c) 1992  Microsoft Corporation
Copyright (c) 1993  Compaq Computer Corporation

Module Name:

    modeqv.h

Abstract:

    This module contains the mode information to set the individual
    modes for the QVision card.  This module also contains the CRTC
    register information for a monitor set.

Environment:

    Kernel mode

Revision History:


   $0005 - MikeD - 02/08/94
	 . modified to work with build 547's new display.cpl
   $0004
      miked: 1/26/1994
	 . Added third party monitor support to force a 76Hz refresh rate

    Adrian Crisan - First version 4/4/93.

--*/

//
// Constants
//

//
// QVision register definitions
//
                                            // port in mono mode
#define ATT_INITIALIZE_PORT_MONO    INPUT_STATUS_1_MONO
                                            // Register to read to reset
                                            // Attribute Controller index/data
					    // toggle in mono mode

#define ATT_INITIALIZE_PORT_COLOR   INPUT_STATUS_1_COLOR
                                            // Register to read to reset
                                            // Attribute Controller index/data
					    // toggle in color mode
// Aries Specific register equates

#define ARIES_CTL_1 	0x63CA		    // Aries BitBlt Control Register 1
#define DAC_CMD_2	0x13C9		    // Aries DAC Command Register 2
#define OVERFLOW_REG_1	0x42		    // Aries Gfx Ctrl Overflow Reg 1
#define OVERFLOW_REG_2	0x51		    // Aries Gfx Ctrl Overflow Reg 2
#define CO_COLOR_WRITE	0x83C8		    // Aries Cursor/Overscan Write Index Reg
#define CO_COLOR_DATA	0x83c9		    // Aries Cursor/Overscan Data Reg
#define HIGH_ADDR_MAP	0x48		    // Index for High Addr Map register


//
// Graphics Miscellaneous Register     - Index 0x06
//
#define MISC_REG	      0x06	    // Graphics Miscellaneous Register
#define MISC_A0000_128K	      0x00	    // 128K video ram starting at A0000
#define MISC_A0000_64K	      0x04	    // 64K  video ram starting at A0000
#define MISC_B0000_32K	      0x08	    // 32K  video ram starting at B0000
#define MISC_B8000_32K	      0x0C	    // 32K  video ram starting at B8000

//
// Register defines for the QVClearScreen code which uses the BitBLT engin
//
#define ROPSELECT_NO_ROPS      0x00
#define PIXELMASK_ONLY	       0x00
#define PLANARMASK_NONE_0XFF   0x04
#define SRC_IS_PATTERN_REGS    0x02
#define PREG_0		       0x33CA
#define PREG_1		       0x33CB
#define PREG_2		       0x33CC
#define PREG_3		       0x33CD
#define PREG_4		       0x33CA
#define PREG_5		       0x33CB
#define PREG_6		       0x33CC
#define PREG_7		       0x33CD
#define BLT_DEST_ADDR_LO       0x63CC
#define BLT_DEST_ADDR_HI       0x63CE
#define BITMAP_WIDTH	       0x23C2
#define BITMAP_HEIGHT	       0x23C4
#define BLT_CMD_0	       0x33CE
#define BLT_CMD_1	       0x33CF
#define XY_SRC_ADDR	       0x40
#define XY_DEST_ADDR	       0x80
#define BLT_FORWARD	       0x00
#define BLT_START	       0x01
#define GLOBAL_BUSY_BIT        0x40

//
// QVision indexed register indexes.
//

#define IND_CURSOR_START        0x0A        // index in CRTC of the Cursor Start
#define IND_CURSOR_END          0x0B        //  and End registers
#define IND_CURSOR_HIGH_LOC     0x0E        // index in CRTC of the Cursor Location
#define IND_CURSOR_LOW_LOC      0x0F        //  High and Low Registers
#define IND_VSYNC_END           0x11        // index in CRTC of the Vertical Sync
                                            //  End register, which has the bit
                                                                                                         //  that protects/unprotects CRTC
                                                                                                         //  index registers 0-7
#define IND_READ_MAP            0x04        // index of Read Map reg in Graph Ctlr
#define IND_BIT_MASK            0x08        // index of Bit Mask reg in Graph Ctlr
#define IND_SYNC_RESET          0x00        // index of Sync Reset reg in Seq
#define IND_MEMORY_MODE         0x04        // index of Memory Mode reg in Seq

#define START_SYNC_RESET_VALUE  0x01        // value for Sync Reset reg to start
                                            //  synchronous reset
#define END_SYNC_RESET_VALUE    0x03        // value for Sync Reset reg to end

#define MEMORY_MAP_BASE 0x23c6              // Juniper memory map base register
#define CTRL_REG_2      0x23c7              // Juniper control register 2
#define CTRL_REG_3      0x63cb              // Juniper control register 3
//
//  Global Variables
//

//
//  Mode commands for inidividual modes.
//
#define VALID_MODE   0x01                   // monitor supports mode index
#define INVALID_MODE 0x00                   // monitor does not support mode index


//////////////////////////////////////////////////////////////
//    Mode index 0
//    Color text mode 3+, 720x400, 9x16 char cell (VGA)
//    BIOS mode: 0x03
//////////////////////////////////////////////////////////////
#define QV_TEXT_720x400x4_INDEX  0

USHORT QV_TEXT_720x400x4[] = {

   OW,                                      // start sync reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (START_SYNC_RESET_VALUE << 8),

   OWM,
   SEQ_ADDRESS_PORT,
   4,
   0x0001,0x0302,0x0003,0x0204,             // program up sequencer

   //
   // Set the Miscellaneous Output register to establish color/mono addressing
   // and select clocks and sync polarities.
   // Synchronous reset must be in effect while this register is being set,
   // because it contains clock control bits.
   //
   OB,                                      // output a byte
   MISC_OUTPUT_REG_WRITE_PORT,              // at this address
   0x67,                                    // with this value


   OW,
   GRAPH_ADDRESS_PORT,
   0x0e06,

   OW,                                      // End sync reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (END_SYNC_RESET_VALUE << 8),

   OW,                                      // unlock extended registers
   GRAPH_ADDRESS_PORT,
   0x050f,

   OW,                                      // enable BitBlt engine
   GRAPH_ADDRESS_PORT,                      // and disable IRQ 9
   0x2810,

   QVNEW+ARIES,                             // make sure this is an aries mode

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_0,                               // 83c6
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_1,
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_2,
   0x01,
   0x20,

   OW,
   CRTC_ADDRESS_PORT_COLOR,
   0x0511,

   //
   // Set all 25 CRT Controller registers.
   //
   METAOUT+INDXOUT,                         // program crtc registers
   CRTC_ADDRESS_PORT_COLOR,
   VGA_NUM_CRTC_PORTS,                      // count
   0,                                       // start index
   0x5F,0x4f,0x50,0x82,
   0x55,0x81,0xbf,0x1f,
   0x00,0x4f,0x0d,0x0e,
   0x00,0x00,0x00,0x00,
   0x9c,0xBe,0x8f,0x28,
   0x1f,0x96,0xb9,0xa3,
   0xFF,

   IB,                                      // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   METAOUT+ATCOUT,                          //
   ATT_ADDRESS_PORT,                        // port
   VGA_NUM_ATTRIB_CONT_PORTS,               // count
   0,                                       // start index
   0x0,0x1,0x2,0x3,0x4,0x5,0x14,0x7,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
   0x04,0x0,0x0F,0x08,0x0,

   METAOUT+INDXOUT,                         //
   GRAPH_ADDRESS_PORT,                      // port
   VGA_NUM_GRAPH_CONT_PORTS,                // count
   0,                                       // start index
   0x00,0x0,0x0,0x0,0x0,0x10,0x0e,0x0,0xFF,

   OB,
   DAC_PIXEL_MASK_PORT,
   0xFF,

   IB,                                      // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   OB,                                      // turn video on.
   ATT_ADDRESS_PORT,
   VIDEO_ENABLE,

   EOD

}; // QV_TEXT_720x400x4

//////////////////////////////////////////////////////////////
//    Mode index 1
//    Color text mode 3, 640x350, 8x14 char cell (EGA)
//    BIOS mode:0x10
//////////////////////////////////////////////////////////////
#define QV_TEXT_640x350x4_INDEX  1

USHORT QV_TEXT_640x350x4[] = {
   OW,                                     // start sync reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (START_SYNC_RESET_VALUE << 8),

   OWM,
   SEQ_ADDRESS_PORT,
   4,
   0x0101,0x0302,0x0003,0x0204,            // program up sequencer

   OB,
   MISC_OUTPUT_REG_WRITE_PORT,
   0xA3,

   OW,
   GRAPH_ADDRESS_PORT,
   0x0e06,

   OW,                                     // end sync reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (END_SYNC_RESET_VALUE << 8),

   OW,                                      // unlock extended registers
   GRAPH_ADDRESS_PORT,
   0x050f,

   OW,                                      // enable BitBlt engine
   GRAPH_ADDRESS_PORT,                      // and disable IRQ 9
   0x2810,

   QVNEW+ARIES,                             // make sure this is an aries mode

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_0,                               // 83c6
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_1,
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_2,
   0x01,
   0x20,

   OW,
   CRTC_ADDRESS_PORT_COLOR,
   0x0511,

   METAOUT+INDXOUT,                        // program crtc registers
   CRTC_ADDRESS_PORT_COLOR,
   VGA_NUM_CRTC_PORTS,                     // count
   0,                                      // start index
   0x5F,0x4f,0x50,0x82,
   0x55,0x81,0xbf,0x1f,
   0x00,0x4d,0x0b,0x0c,
   0x00,0x00,0x00,0x00,
   0x83,0xB5,0x5d,0x28,
   0x1f,0x63,0xba,0xa3,
   0xFF,

   IB,                                     // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   METAOUT+ATCOUT,                         //
   ATT_ADDRESS_PORT,                       // port
   VGA_NUM_ATTRIB_CONT_PORTS,              // count
   0,                                      // start index
   0x00,0x1,0x2,0x3,0x4,0x5,0x14,0x7,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
   0x00,0x00,0x0F,0x00,0x00,

   METAOUT+INDXOUT,                        //
   GRAPH_ADDRESS_PORT,                     // port
   VGA_NUM_GRAPH_CONT_PORTS,               // count
   0,                                      // start index
   0x00,0x00,0x00,0x00,0x00,0x10,0x0e,0x00,0x0FF,

   OB,
   DAC_PIXEL_MASK_PORT,
   0xFF,

   IB,                                     // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   OB,                                     // turn video on.
   ATT_ADDRESS_PORT,
   VIDEO_ENABLE,

   EOD
}; // QV_TEXT_640x350x4


//////////////////////////////////////////////////////////////
//    Mode index 2
//    Standard VGA Color graphics, 640x480x4 - 16 colors
//    BIOS mode: 0x12
//////////////////////////////////////////////////////////////
#define QV_TEXT_640x480x4_INDEX   2

USHORT QV_TEXT_640x480x4[] = {
   OW,                                      // start sync reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (START_SYNC_RESET_VALUE << 8),

   OWM,
   SEQ_ADDRESS_PORT,
   4,
   0x0101,0x0f02,0x0003,0x0604,             // program up sequencer


   OB,
   MISC_OUTPUT_REG_WRITE_PORT,
   0xe3,

   OW,                                      //{ SetGraphCmd,{ "\x05", 0x06, 1 } },
   GRAPH_ADDRESS_PORT,
   0x0506,

   OW,                                      // End Sync reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (END_SYNC_RESET_VALUE << 8),

   OW,                                      // unlock extended registers
   GRAPH_ADDRESS_PORT,
   0x050f,

   OW,                                      // enable BitBlt engine
   GRAPH_ADDRESS_PORT,                      // and disable IRQ 9
   0x2810,

   QVNEW+ARIES,                             // make sure this is an aries mode

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_0,                               // 83c6
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_1,
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_2,
   0x01,
   0x20,

   OW,
   CRTC_ADDRESS_PORT_COLOR,
   0x0511,

   METAOUT+INDXOUT,                         // program crtc registers
   CRTC_ADDRESS_PORT_COLOR,
   VGA_NUM_CRTC_PORTS,                      // count
   0,                                       // start index
   0x5F,0x4F,0x50,0x82,
   0x54,0x80,0x0B,0x3E,
   0x00,0x40,0x00,0x00,
   0x00,0x00,0x00,0x00,
   0xEA,0xBC,0xDF,0x28,
   0x00,0xE7,0x04,0xE3,
   0xFF,

   IB,                                      // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   METAOUT+ATCOUT,                          //
   ATT_ADDRESS_PORT,                        // port
   VGA_NUM_ATTRIB_CONT_PORTS,               // count
   0,                                       // start index
   0x00,0x01,0x02,0x03,0x04,
   0x05,0x14,0x07,0x38,0x39,
   0x3A,0x3B,0x3C,0x3D,0x3E,
   0x3F,0x01,0x00,0x0F,0x00,0x00,

   METAOUT+INDXOUT,                         //
   GRAPH_ADDRESS_PORT,                      // port
   VGA_NUM_GRAPH_CONT_PORTS,                // count
   0,                                       // start index
   0x00,0x0,0x0,0x0,0x0,0x0,0x05,0x0F,0x0FF,

   OB,
   DAC_PIXEL_MASK_PORT,
   0xFF,

   IB,                                      // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   OB,                                      // turn video on.
   ATT_ADDRESS_PORT,
   VIDEO_ENABLE,

   EOD

}; // QV_TEXT_640x480x4

//////////////////////////////////////////////////////////////
//    Mode index 3
//    VGA Color graphics, 640x480x8 - 256 colors
//    BIOS mode: 0x32
//////////////////////////////////////////////////////////////
#define QV_640x480x8_INDEX    3

USHORT QV_640x480x8[] = {
   QVNEW+SETRAM,                                // set aries videoram

   OW,                                          // unlock extended registers
   GRAPH_ADDRESS_PORT,
   0x050f,

   OW,                                          // start sync reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (START_SYNC_RESET_VALUE << 8),

   OWM,
   SEQ_ADDRESS_PORT,
   4,
   0x0101,0xff02,0x0003,0x0e04,                 // program up sequencer


   OW,                                  // set seq memory register 4 to 0x0e
   SEQ_ADDRESS_PORT,
   0x0e04,

   OB,                                          // FIX VGA BUG
   SEQ_ADDRESS_PORT,
   0x07,

   IB,
   MISC_OUTPUT_REG_WRITE_OFFSET,

   OW,                                          // END FIX VGA BUG
   CRTC_ADDRESS_COLOR_OFFSET,
   0x013f,

   OW,                                          // end synch reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (END_SYNC_RESET_VALUE << 8),

   OW,                                          // unlock extended registers
   GRAPH_ADDRESS_PORT,
   0x050f,

   OW,                                          // Set AVGA mode
   GRAPH_ADDRESS_PORT,                          // 3ce
   0x0140,

   OW,                                      // set pixel write mask reg to ff
   SEQ_ADDRESS_PORT,                        // 3c4
   0xFF02,                                  // enable 256 colors again

   OW,                                      // enable BitBlt engine
   GRAPH_ADDRESS_PORT,                      // and disable IRQ 9
   0x2810,

   QVCMD+BYTE2PORT,                         // enable 8BPP Aries Mode
   ARIES_CTL_1,
   0x01,
   0x03,

   QVNEW+ARIES,                             // make sure this is an aries mode

   QVCMD+SETMISCOUT,                        // set MISC OUT registers based
                                            // on monitor type

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_0,
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_1,
   0x01,
   0x40,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_2,
   0x01,
   0x20,

   OW,                                      // unlock the CRTC registers
   CRTC_ADDRESS_PORT_COLOR,
   0x0011,

   QVCMD+SETMONCRTC,                        // Set monitor specific CRTC registers

   QVCMD+SETMONOVFLW,                       // Set monitor specific overflow register

   OW,                                      // write 0 to Overflow_reg_1
   GRAPH_ADDRESS_PORT,                      // register (index 42)
   0x0042,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_WRITE,
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_DATA,
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_DATA,
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_DATA,
   0x01,
   0x00,


   IB,                                      // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   METAOUT+ATCOUT,                          //
   ATT_ADDRESS_PORT,                        // port
   VGA_NUM_ATTRIB_CONT_PORTS,               // count
   0,                                       // start index
   0x00,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
   0x41,0x00,0x0F,0x00,0x00,


   OWM,                                     // program the graphics controller
   GRAPH_ADDRESS_PORT,
   VGA_NUM_GRAPH_CONT_PORTS,
   0x0000, 0x0001, 0x0002, 0x0003,
   0x0004, 0x0005, 0x0506, 0x0f07,
   0xFF08,

   OB,
   DAC_PIXEL_MASK_PORT,
   0xFF,

   IB,                                      // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   OB,                                      // turn video on.
   ATT_ADDRESS_PORT,
   VIDEO_ENABLE,

   EOD
}; // QV_640x480x8

//////////////////////////////////////////////////////////////
//    Mode index 4
//    SVGA color graphics, 800x600x8 - 256 colors
//    BIOS Mode: 0x34.
//////////////////////////////////////////////////////////////
#define QV_800x600x8_INDEX    4

USHORT QV_800x600x8[] = {
   QVNEW+SETRAM,                            // set aries videoram

   OW,                                      // unlock extended registers
   GRAPH_ADDRESS_PORT,
   0x050f,

   OW,                                      // start synch reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (START_SYNC_RESET_VALUE << 8),

   OWM,
   SEQ_ADDRESS_PORT,
   4,
   0x0101,0xff02,0x0003,0x0e04,             // program up sequencer

   OW,                                      // set seq memory register 4 to 0x0e
   SEQ_ADDRESS_PORT,
   0x0e04,

   OB,                                      // FIX VGA BUG
   SEQ_ADDRESS_PORT,
   0x07,

   IB,
   MISC_OUTPUT_REG_WRITE_OFFSET,

   OW,                                      // END FIX VGA BUG
   CRTC_ADDRESS_COLOR_OFFSET,
   0x013f,

   OW,                                      // End sync reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (END_SYNC_RESET_VALUE << 8),

   OW,                                      // unlock extended registers
   GRAPH_ADDRESS_PORT,
   0x050f,

   OW,                                      // Set AVGA mode
   GRAPH_ADDRESS_PORT,                      // 3ce
   0x0140,

   OW,                                      // set pixel write mask reg to ff
   SEQ_ADDRESS_PORT,                        // 3c4
   0xFF02,

   OW,                                      // enable BitBlt engine
   GRAPH_ADDRESS_PORT,                      // and disable IRQ 9
   0x2810,

   QVCMD+BYTE2PORT,                         // enable 8BPP Aries Mode
   ARIES_CTL_1,
   0x01,
   0x03,

   QVNEW+ARIES,                             // make sure this is an aries mode

   QVCMD+SETMISCOUT,                        // set MISC OUT registers based
                                            // on monitor type

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_0,
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_1,
   0x01,
   0x40,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_2,
   0x01,
   0x20,

   OW,                                      // unlock the CRTC registers
   CRTC_ADDRESS_PORT_COLOR,
   0x0011,

   QVCMD+SETMONCRTC,                        // Set monitor specific CRTC registers

   QVCMD+SETMONOVFLW,                       // Set monitor specific overflow register

   OW,                                      // write 0 to Overflow_reg_1
   GRAPH_ADDRESS_PORT,                      // register (index 42)
   0x0042,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_WRITE,
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_DATA,
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_DATA,
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_DATA,
   0x01,
   0x00,


   IB,                                      // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   METAOUT+ATCOUT,                          //
   ATT_ADDRESS_PORT,                        // port
   VGA_NUM_ATTRIB_CONT_PORTS,               // count
   0,                                       // start index
   0x00,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
   0x41,0x00,0x0F,0x00,0x00,


   OWM,                                     // program the graphics controller
   GRAPH_ADDRESS_PORT,
   VGA_NUM_GRAPH_CONT_PORTS,
   0x0000, 0x0001, 0x0002, 0x0003,
   0x0004, 0x0005, 0x0506, 0x0F07,
   0xFF08,

   OB,
   DAC_PIXEL_MASK_PORT,
   0xFF,

   IB,                                      // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   OB,                                      // turn video on.
   ATT_ADDRESS_PORT,
   VIDEO_ENABLE,

   EOD
}; // QV_800x600x8


//////////////////////////////////////////////////////////////
//    Mode index 5
//    SVGA color graphics, 1024x768x8 - 256 colors
//    BIOS mode:0x38
//////////////////////////////////////////////////////////////
#define QV_1024x768x8_INDEX   5

USHORT QV_1024x768x8[] = {
   QVNEW+SETRAM,                            // set aries videoram

   OW,                                      // unlock extended registers
   GRAPH_ADDRESS_PORT,
   0x050f,

   OW,                                      // start synch reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (START_SYNC_RESET_VALUE << 8),

   OWM,
   SEQ_ADDRESS_PORT,
   4,
   0x0101,0xff02,0x0003,0x0e04,             // program up sequencer

   OW,                                      // set seq memory register 4 to 0x0e
   SEQ_ADDRESS_PORT,
   0x0e04,

   OB,                                      // FIX VGA BUG
   SEQ_ADDRESS_PORT,
   0x07,

   IB,
   MISC_OUTPUT_REG_WRITE_OFFSET,

   OW,                                      // END FIX VGA BUG
   CRTC_ADDRESS_COLOR_OFFSET,
   0x013f,

   OW,                                      // end synch reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (END_SYNC_RESET_VALUE << 8),

   OW,                                      // unlock extended registers
   GRAPH_ADDRESS_PORT,
   0x050f,

   OW,                                      // Set AVGA mode
   GRAPH_ADDRESS_PORT,                      // 3ce
   0x0140,

   OW,                                      // set pixel write mask reg to ff
   SEQ_ADDRESS_PORT,                        // 3c4
   0xFF02,

   OW,                                      // enable BitBlt engine
   GRAPH_ADDRESS_PORT,                      // and disable IRQ 9
   0x2810,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   ARIES_CTL_1,                             // 63ca
   0x01,
   0x03,

   QVNEW+ARIES,                             // make sure this is an aries mode

   QVCMD+SETMISCOUT,                        // set MISC_OUT register according
                                            // to the connected monitor.

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_0,                               // 83c6
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_1,                               // 13c8
   0x01,
   0x40,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_2,                               // 13c9
   0x01,
   0x20,

   OW,                                      // unlock the CRTC registers
   CRTC_ADDRESS_PORT_COLOR,
   0x0011,

   QVCMD+SETMONCRTC,                        // Set monitor specific CRTC registers

   QVCMD+SETMONOVFLW,                       // Set monitor specific overflow register

   OW,                                      // Set the Overflow_reg_1
   GRAPH_ADDRESS_PORT,
   0x0042,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_WRITE,                          // 83c8
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_DATA,                           // 83c9
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_DATA,                           // 83c9
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_DATA,                           // 83c9
   0x01,
   0x00,


   IB,                                      // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   METAOUT+ATCOUT,                          //
   ATT_ADDRESS_PORT,                        // port
   VGA_NUM_ATTRIB_CONT_PORTS,               // count
   0,                                       // start index
   0x00,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
   0x41,0x00,0x0F,0x00,0x00,


   OWM,                                     // program the graphics controller
   GRAPH_ADDRESS_PORT,
   VGA_NUM_GRAPH_CONT_PORTS,
   0x0000, 0x0001, 0x0002, 0x0003,
   0x0004, 0x0005, 0x0506, 0x0F07,
   0xFF08,

   OB,
   DAC_PIXEL_MASK_PORT,
   0xFF,

   IB,                                      // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   OB,                                      // turn video on.
   ATT_ADDRESS_PORT,
   VIDEO_ENABLE,

   EOD
}; // QV_1024x768x8


//////////////////////////////////////////////////////////////
//    Mode index 6
//    SVGA color graphics, 1280x1024x8 - 256 colors
//    BIOS mode:0x3A
//////////////////////////////////////////////////////////////
#define QV_1280x1024x8_INDEX   6

USHORT QV_1280x1024x8[] = {
   QVNEW+SETRAM,                            // set aries videoram

   OW,                                      // unlock extended registers
   GRAPH_ADDRESS_PORT,
   0x050f,

   OW,                                      // start synch reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (START_SYNC_RESET_VALUE << 8),

   OWM,
   SEQ_ADDRESS_PORT,
   4,
   0x0101,0xff02,0x0003,0x0e04,             // program up sequencer

   OW,                                      // set seq memory register 4 to 0x0e
   SEQ_ADDRESS_PORT,
   0x0e04,

   OB,                                      // FIX VGA BUG
   SEQ_ADDRESS_PORT,
   0x07,

   IB,
   MISC_OUTPUT_REG_WRITE_OFFSET,

   OW,                                      // END FIX VGA BUG
   CRTC_ADDRESS_COLOR_OFFSET,
   0x013f,

   OW,                                      // end synch reset
   SEQ_ADDRESS_PORT,
   IND_SYNC_RESET + (END_SYNC_RESET_VALUE << 8),

   OW,                                      // unlock extended registers
   GRAPH_ADDRESS_PORT,
   0x050f,

   OW,                                      // Set ORION mode
   GRAPH_ADDRESS_PORT,                      // 3ce
   0x0140,

   OW,                                      // set pixel write mask reg to ff
   SEQ_ADDRESS_PORT,                        // 3c4
   0xFF02,

   OW,                                      // enable BitBlt engine
   GRAPH_ADDRESS_PORT,                      // and disable IRQ 9
   0x2810,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   ARIES_CTL_1,                             // 63ca
   0x01,
   0x03,

   QVNEW+V32,                               // Set board in V32 mode

   QVCMD+SETMISCOUT,                        // set MISC_OUT register according
                                            // to the connected monitor.

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_0,                               // 83c6
   0x01,
   0x80,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_CMD_1,                               // 13c8
   0x01,
   0x40,

   // VidalL 04/25/93
   //
   // IMPORTANT: DAC Command register 2 should really be included in the
   // =========  CRTC parameters because of the following:
   //            Qvision 200 monitors (at 76Hz) require the use of pixel
   //            clock 2, which means value 30.  However, the 68Hz
   //            and 60Hz timings require pixel clock 1, which means
   //            value 20.  So, when 60 and 68Hz timings are used, default
   //            to value 20.  But if a Qvision monitor is being used
   //            (CPQ MON), then must changed this value to 30 !!!
   //            Otherwise, the screen will NOT sync in 1280 resolution
   //
   //            DAC_CMD_2 is set by the QVNEW+V32 command. It must NOT
   //            be set later in this stream !!! Otherwise, 1280 may
   //            may not sync on the monitor.
   //
   // VidalL 04/25/93

//   QVCMD+BYTE2PORT,                         // setup DAC control registers
//   DAC_CMD_2,                               // 13c9
//   0x01,
//   0x30  or  0x20,


   QVCMD+BYTE2PORT,                         // setup DAC control registers
   DAC_STATUS_REG,                          // 13c6
   0x01,
   0x08,

   OW,                                      // unlock the CRTC registers
   CRTC_ADDRESS_PORT_COLOR,
   0x0011,

   QVCMD+SETMONCRTC,                        // Set monitor specific CRTC registers

   QVCMD+SETMONOVFLW,                       // Set monitor specific overflow register

   OW,                                      // Set the Overflow_reg_1
   GRAPH_ADDRESS_PORT,
   0x0142,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_WRITE,                          // 83c8
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_DATA,                           // 83c9
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_DATA,                           // 83c9
   0x01,
   0x00,

   QVCMD+BYTE2PORT,                         // setup DAC control registers
   CO_COLOR_DATA,                           // 83c9
   0x01,
   0x00,


   IB,                                      // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   METAOUT+ATCOUT,                          //
   ATT_ADDRESS_PORT,                        // port
   VGA_NUM_ATTRIB_CONT_PORTS,               // count
   0,                                       // start index
   0x00,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
   0x41,0x00,0x0F,0x00,0x00,


   OWM,                                     // program the graphics controller
   GRAPH_ADDRESS_PORT,
   VGA_NUM_GRAPH_CONT_PORTS,
   0x0000, 0x0001, 0x0002, 0x0003,
   0x0004, 0x0005, 0x0506, 0x0F07,
   0xFF08,

   OB,
   DAC_PIXEL_MASK_PORT,
   0xFF,

   IB,                                      // prepare atc for writing
   INPUT_STATUS_1_COLOR,

   OB,                                      // turn video on.
   ATT_ADDRESS_PORT,
   VIDEO_ENABLE,

   EOD
}; // QV_1280x1024x8


//
//
//    The MonData is an array of bytes of the following format:
//    MONCLASS, ModeIndex, Validity, MiscOut, Overflow,
//    25 CRT register values.
//
//      MONCLASS  = The monitor type from the structure
//      ModeIndex = index of the mode (look above)
//      Validity  = 1 if this MONCLASS supports mode.
//      MiscOut   = Misc Out register value for this mode.
//      Overflow  = Overflow 2 register value for this mode.
//      The rest are register values for consecutive CRT registers.
//
//    If the mode is INVALID_MODE then the CRT registers are
//    not put in the array.  This will allow us to move
//    quickly through an array which contains many invalid modes.
//    We know that if the mode is VALID_MODE, we will have 25
//    register values after the Validity value for that mode.
//
//    An INVALID_MODE does not necessarily mean that the
//    particular mode index is not support on the monitor.
//    It only means that for that particular mode we do not
//    have to program the CRTC registers.
//
//
//$DEL$//USHORT usMonData[] = {
//$DEL$//   Monitor_Vga, QV_TEXT_720x400x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_Vga, QV_TEXT_640x350x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_Vga, QV_TEXT_640x480x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_Vga, QV_640x480x8_INDEX, VALID_MODE, 0xef, 0x08,
//$DEL$//      0x5f, 0x4f, 0x50, 0x81, 0x53, 0x9f, 0x0b, 0x3e, 0x00, 0x40,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xea, 0xbc, 0xdf, 0x80,
//$DEL$//      0x00, 0xe5, 0x03, 0xe3, 0xff,
//$DEL$//   Monitor_Vga, QV_800x600x8_INDEX, INVALID_MODE,
//$DEL$//   Monitor_Vga, QV_1024x768x8_INDEX, INVALID_MODE,
//$DEL$//   Monitor_Vga, QV_1280x1024x8_INDEX, INVALID_MODE,
//$DEL$//
//$DEL$//   Monitor_AG1024, QV_TEXT_720x400x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_AG1024, QV_TEXT_640x350x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_AG1024, QV_TEXT_640x480x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_AG1024, QV_640x480x8_INDEX, VALID_MODE, 0xef, 0x08,
//$DEL$//      0x5f, 0x4f, 0x50, 0x81, 0x53, 0x9f, 0x0b, 0x3e, 0x00, 0x40,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xea, 0xbc, 0xdf, 0x80,
//$DEL$//      0x00, 0xe5, 0x03, 0xe3, 0xff,
//$DEL$//   Monitor_AG1024, QV_800x600x8_INDEX, INVALID_MODE,
//$DEL$//   Monitor_AG1024, QV_1024x768x8_INDEX, VALID_MODE, 0x2b, 0x00,
//$DEL$//      0xa9, 0x7f, 0x7f, 0x0a, 0x87, 0x9f, 0x2e, 0xf5, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xba, 0xff, 0x80,
//$DEL$//      0x00, 0xff, 0x2c, 0xe3, 0xff,
//$DEL$//   Monitor_AG1024, QV_1280x1024x8_INDEX, INVALID_MODE,
//$DEL$//
//$DEL$//   Monitor_Qvision, QV_TEXT_720x400x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_Qvision, QV_TEXT_640x350x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_Qvision, QV_TEXT_640x480x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_Qvision, QV_640x480x8_INDEX, VALID_MODE, 0xef, 0x08,
//$DEL$//      0x5f, 0x4f, 0x50, 0x81, 0x53, 0x9f, 0x0b, 0x3e, 0x00, 0x40,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xea, 0xbc, 0xdf, 0x80,
//$DEL$//      0x00, 0xe5, 0x03, 0xe3, 0xff,
//$DEL$//   Monitor_Qvision, QV_800x600x8_INDEX, VALID_MODE, 0x23, 0x20,
//$DEL$//      0x8b, 0x63, 0x66, 0x8b, 0x6d, 0x1d, 0xe9, 0xf0, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0xba, 0x57, 0x80,
//$DEL$//      0x00, 0x6e, 0xd1, 0xe3, 0xff,
//$DEL$//   Monitor_Qvision, QV_1024x768x8_INDEX, VALID_MODE, 0x2b, 0x00,
//$DEL$//      0x9e, 0x7f, 0x7f, 0x81, 0x83, 0x93, 0x1e, 0xf1, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb3, 0xff, 0x80,
//$DEL$//      0x00, 0xff, 0x1e, 0xe3, 0xff,
//$DEL$//   Monitor_Qvision, QV_1280x1024x8_INDEX, INVALID_MODE,
//$DEL$//
//$DEL$//   Monitor_1280, QV_TEXT_720x400x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_1280, QV_TEXT_640x350x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_1280, QV_TEXT_640x480x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_1280, QV_640x480x8_INDEX, VALID_MODE, 0xf3, 0x28,
//$DEL$//      0x69, 0x4f, 0x55, 0x86, 0x58, 0x80, 0x56, 0xb2, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0xb3, 0xdf, 0x80,
//$DEL$//      0x00, 0x01, 0x32, 0xe3, 0xff,
//$DEL$//   Monitor_1280, QV_800x600x8_INDEX, VALID_MODE, 0x23, 0x20,
//$DEL$//      0x8b, 0x63, 0x66, 0x8b, 0x6d, 0x1d, 0xe9, 0xf0, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0xba, 0x57, 0x80,
//$DEL$//      0x00, 0x6e, 0xd1, 0xe3, 0xff,
//$DEL$//   Monitor_1280, QV_1024x768x8_INDEX, VALID_MODE, 0x2b, 0x00,
//$DEL$//      0x9e, 0x7f, 0x7f, 0x81, 0x83, 0x93, 0x1e, 0xf1, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb3, 0xff, 0x80,
//$DEL$//      0x00, 0xff, 0x1e, 0xe3, 0xff,
//$DEL$//   Monitor_1280, QV_1280x1024x8_INDEX, VALID_MODE, 0x27, 0xe8,
//$DEL$//      0xca, 0x9f, 0x9f, 0x8d, 0xa6, 0x1a, 0x24, 0x5a, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xb5, 0xff, 0x00,
//$DEL$//      0x00, 0xff, 0x24, 0xe3, 0xff,
//$DEL$//
//$DEL$//   Monitor_SVGA, QV_TEXT_720x400x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_SVGA, QV_TEXT_640x350x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_SVGA, QV_TEXT_640x480x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_SVGA, QV_640x480x8_INDEX, VALID_MODE, 0xef, 0x08,
//$DEL$//      0x5f, 0x4f, 0x50, 0x81, 0x53, 0x9f, 0x0b, 0x3e, 0x00, 0x40,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xea, 0xbc, 0xdf, 0x80,
//$DEL$//      0x00, 0xe5, 0x03, 0xe3, 0xff,
//$DEL$//   Monitor_SVGA, QV_800x600x8_INDEX, VALID_MODE, 0xf3, 0x20,
//$DEL$//      0x7f, 0x63, 0x64, 0x80, 0x68, 0x19, 0x73, 0xf0, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0xbc, 0x57, 0x80,
//$DEL$//      0x00, 0x58, 0x72, 0xe3, 0xff,
//$DEL$//   Monitor_SVGA, QV_1024x768x8_INDEX, INVALID_MODE,
//$DEL$//   Monitor_SVGA, QV_1280x1024x8_INDEX, INVALID_MODE,
//$DEL$//
//$DEL$//   Monitor_60Hz, QV_TEXT_720x400x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_60Hz, QV_TEXT_640x350x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_60Hz, QV_TEXT_640x480x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_60Hz, QV_640x480x8_INDEX, VALID_MODE, 0xef, 0x08,
//$DEL$//      0x5f, 0x4f, 0x50, 0x81, 0x53, 0x9f, 0x0b, 0x3e, 0x00, 0x40,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xea, 0xbc, 0xdf, 0x80,
//$DEL$//      0x00, 0xe5, 0x03, 0xe3, 0xff,
//$DEL$//   Monitor_60Hz, QV_800x600x8_INDEX, VALID_MODE, 0xf3, 0x20,
//$DEL$//      0x7f, 0x63, 0x64, 0x80, 0x68, 0x19, 0x73, 0xf0, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0xbc, 0x57, 0x80,
//$DEL$//      0x00, 0x58, 0x72, 0xe3, 0xff,
//$DEL$//   Monitor_60Hz, QV_1024x768x8_INDEX, VALID_MODE, 0xe3, 0x00,
//$DEL$//      0x9b, 0x7f, 0x7f, 0x9e, 0x87, 0x17, 0x31, 0xf5, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0xb4, 0xff, 0x80,
//$DEL$//      0x00, 0xff, 0x31, 0xe3, 0xff,
//$DEL$//   Monitor_60Hz, QV_1280x1024x8_INDEX, VALID_MODE, 0x23, 0xc8,
//$DEL$//      0xed, 0x9f, 0x9f, 0x10, 0xae, 0x90, 0x2a, 0x5a, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xb7, 0xff, 0x00,
//$DEL$//      0x00, 0xff, 0x2a, 0xe3, 0xff,
//$DEL$//
//$DEL$//   Monitor_66Hz, QV_TEXT_720x400x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_66Hz, QV_TEXT_640x350x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_66Hz, QV_TEXT_640x480x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_66Hz, QV_640x480x8_INDEX, INVALID_MODE,
//$DEL$//   Monitor_66Hz, QV_800x600x8_INDEX, INVALID_MODE,
//$DEL$//   Monitor_66Hz, QV_1024x768x8_INDEX, VALID_MODE, 0x2b, 0x00,
//$DEL$//      0xa9, 0x7f, 0x7f, 0x0a, 0x87, 0x9f, 0x2e, 0xf5, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xba, 0xff, 0x80,
//$DEL$//      0x00, 0xff, 0x2c, 0xe3, 0xff,
//$DEL$//   Monitor_66Hz, QV_1280x1024x8_INDEX, INVALID_MODE,
//$DEL$//
//$DEL$//   Monitor_68Hz, QV_TEXT_720x400x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_68Hz, QV_TEXT_640x350x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_68Hz, QV_TEXT_640x480x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_68Hz, QV_640x480x8_INDEX, INVALID_MODE,
//$DEL$//   Monitor_68Hz, QV_800x600x8_INDEX, INVALID_MODE,
//$DEL$//   Monitor_68Hz, QV_1024x768x8_INDEX, INVALID_MODE,
//$DEL$//   Monitor_68Hz, QV_1280x1024x8_INDEX, VALID_MODE, 0x23, 0xe8,
//$DEL$//      0xcf, 0x9f, 0x9f, 0x12, 0xa4, 0x19, 0x2e, 0x5a, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xbb, 0xff, 0x00,
//$DEL$//      0x00, 0xff, 0x2e, 0xe3, 0xff,
//$DEL$//
//$DEL$//   Monitor_72Hz, QV_TEXT_720x400x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_72Hz, QV_TEXT_640x350x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_72Hz, QV_TEXT_640x480x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_72Hz, QV_640x480x8_INDEX, INVALID_MODE,
//$DEL$//   Monitor_72Hz, QV_800x600x8_INDEX, VALID_MODE, 0x23, 0x20,
//$DEL$//      0x8b, 0x63, 0x66, 0x8b, 0x6d, 0x1d, 0xe9, 0xf0, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0xba, 0x57, 0x80,
//$DEL$//      0x00, 0x6e, 0xd1, 0xe3, 0xff,
//$DEL$//   Monitor_72Hz, QV_1024x768x8_INDEX, VALID_MODE, 0x2b, 0x00,
//$DEL$//      0x9e, 0x7f, 0x7f, 0x81, 0x83, 0x93, 0x1e, 0xf1, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb3, 0xff, 0x80,
//$DEL$//      0x00, 0xff, 0x1e, 0xe3, 0xff,
//$DEL$//   Monitor_72Hz, QV_1280x1024x8_INDEX, INVALID_MODE,
//$DEL$//
//$DEL$//   Monitor_75Hz, QV_TEXT_720x400x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_75Hz, QV_TEXT_640x350x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_75Hz, QV_TEXT_640x480x4_INDEX, INVALID_MODE,
//$DEL$//   Monitor_75Hz, QV_640x480x8_INDEX, INVALID_MODE,
//$DEL$//   Monitor_75Hz, QV_800x600x8_INDEX, INVALID_MODE,
//$DEL$//   Monitor_75Hz, QV_1024x768x8_INDEX, VALID_MODE, 0x2b, 0x00,
//$DEL$//      0x97, 0x7f, 0x7f, 0x9a, 0x82, 0x0f, 0x1f, 0xf1, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb4, 0xff, 0x80,
//$DEL$//      0x00, 0xfe, 0x1f, 0xe3, 0x00,
//$DEL$//
//$DEL$//   // Monitor_75Hz, QV_1280x1024x8_INDEX, INVALID_MODE,
//$DEL$//
//$DEL$//   //
//$DEL$//   // $0004 ************  miked 1/26/1994 *****************************
//$DEL$//   //
//$DEL$//   //Added third party monitor support to force a 76Hz refresh rate
//$DEL$//   //using the same timings as the QV200 timings.
//$DEL$//   //
//$DEL$//   //Note: The MonClass entry for 75Hz was used, because it was available
//$DEL$//   //	    and much less overhead than adding a new level to the array
//$DEL$//   //	    of monitor timings; so even though the "Monitor_75Hz" is used
//$DEL$//   //	    here, it really maps to 76Hz.
//$DEL$//   //
//$DEL$//   //	***************************************************************
//$DEL$//   Monitor_75Hz, QV_1280x1024x8_INDEX, VALID_MODE, 0x27, 0xe8,
//$DEL$//      0xca, 0x9f, 0x9f, 0x8d, 0xa6, 0x1a, 0x24, 0x5a, 0x00, 0x60,
//$DEL$//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xb5, 0xff, 0x00,
//$DEL$//      0x00, 0xff, 0x24, 0xe3, 0xff,
//$DEL$//}; // usMonData

//
// $0005 - MikeD - 02/08/94
//  Daytona - new monitor data structure array
//

MONTYPE MonData[] = {
   //
   // Monitor_Vga
   //
   QV_TEXT_720x400x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x350x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x480x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_640x480x8_INDEX, VALID_MODE, 0xef, 0x08,
      0x5f, 0x4f, 0x50, 0x81, 0x53, 0x9f, 0x0b, 0x3e, 0x00, 0x40,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xea, 0xbc, 0xdf, 0x80,
      0x00, 0xe5, 0x03, 0xe3, 0xff,
   QV_800x600x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_1024x768x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_1280x1024x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,

   //
   // Monitor_AG1024
   //
   QV_TEXT_720x400x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x350x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x480x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_640x480x8_INDEX, VALID_MODE, 0xef, 0x08,
      0x5f, 0x4f, 0x50, 0x81, 0x53, 0x9f, 0x0b, 0x3e, 0x00, 0x40,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xea, 0xbc, 0xdf, 0x80,
      0x00, 0xe5, 0x03, 0xe3, 0xff,
   QV_800x600x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_1024x768x8_INDEX, VALID_MODE, 0x2b, 0x00,
      0xa9, 0x7f, 0x7f, 0x0a, 0x87, 0x9f, 0x2e, 0xf5, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xba, 0xff, 0x80,
      0x00, 0xff, 0x2c, 0xe3, 0xff,
   QV_1280x1024x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,

   //
   // Monitor_Qvision
   //
   QV_TEXT_720x400x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x350x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x480x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_640x480x8_INDEX, VALID_MODE, 0xef, 0x08,
      0x5f, 0x4f, 0x50, 0x81, 0x53, 0x9f, 0x0b, 0x3e, 0x00, 0x40,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xea, 0xbc, 0xdf, 0x80,
      0x00, 0xe5, 0x03, 0xe3, 0xff,
   QV_800x600x8_INDEX, VALID_MODE, 0x23, 0x20,
      0x8b, 0x63, 0x66, 0x8b, 0x6d, 0x1d, 0xe9, 0xf0, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0xba, 0x57, 0x80,
      0x00, 0x6e, 0xd1, 0xe3, 0xff,
   QV_1024x768x8_INDEX, VALID_MODE, 0x2b, 0x00,
      0x9e, 0x7f, 0x7f, 0x81, 0x83, 0x93, 0x1e, 0xf1, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb3, 0xff, 0x80,
      0x00, 0xff, 0x1e, 0xe3, 0xff,
   QV_1280x1024x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,

   //
   // Monitor_1280
   //
   QV_TEXT_720x400x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x350x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x480x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_640x480x8_INDEX, VALID_MODE, 0xf3, 0x28,
      0x69, 0x4f, 0x55, 0x86, 0x58, 0x80, 0x56, 0xb2, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0xb3, 0xdf, 0x80,
      0x00, 0x01, 0x32, 0xe3, 0xff,
   QV_800x600x8_INDEX, VALID_MODE, 0x23, 0x20,
      0x8b, 0x63, 0x66, 0x8b, 0x6d, 0x1d, 0xe9, 0xf0, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0xba, 0x57, 0x80,
      0x00, 0x6e, 0xd1, 0xe3, 0xff,
   QV_1024x768x8_INDEX, VALID_MODE, 0x2b, 0x00,
      0x9e, 0x7f, 0x7f, 0x81, 0x83, 0x93, 0x1e, 0xf1, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb3, 0xff, 0x80,
      0x00, 0xff, 0x1e, 0xe3, 0xff,
   QV_1280x1024x8_INDEX, VALID_MODE, 0x27, 0xe8,
      0xca, 0x9f, 0x9f, 0x8d, 0xa6, 0x1a, 0x24, 0x5a, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xb5, 0xff, 0x00,
      0x00, 0xff, 0x24, 0xe3, 0xff,

   //
   // Monitor_SVGA
   //
   QV_TEXT_720x400x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x350x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x480x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_640x480x8_INDEX, VALID_MODE, 0xef, 0x08,
      0x5f, 0x4f, 0x50, 0x81, 0x53, 0x9f, 0x0b, 0x3e, 0x00, 0x40,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xea, 0xbc, 0xdf, 0x80,
      0x00, 0xe5, 0x03, 0xe3, 0xff,
   QV_800x600x8_INDEX, VALID_MODE, 0xf3, 0x20,
      0x7f, 0x63, 0x64, 0x80, 0x68, 0x19, 0x73, 0xf0, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0xbc, 0x57, 0x80,
      0x00, 0x58, 0x72, 0xe3, 0xff,
   QV_1024x768x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_1280x1024x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,

   //
   // Monitor_60Hz
   //
   QV_TEXT_720x400x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x350x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x480x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_640x480x8_INDEX, VALID_MODE, 0xef, 0x08,
      0x5f, 0x4f, 0x50, 0x81, 0x53, 0x9f, 0x0b, 0x3e, 0x00, 0x40,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xea, 0xbc, 0xdf, 0x80,
      0x00, 0xe5, 0x03, 0xe3, 0xff,
   QV_800x600x8_INDEX, VALID_MODE, 0xf3, 0x20,
      0x7f, 0x63, 0x64, 0x80, 0x68, 0x19, 0x73, 0xf0, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0xbc, 0x57, 0x80,
      0x00, 0x58, 0x72, 0xe3, 0xff,
   QV_1024x768x8_INDEX, VALID_MODE, 0xe3, 0x00,
      0x9b, 0x7f, 0x7f, 0x9e, 0x87, 0x17, 0x31, 0xf5, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0xb4, 0xff, 0x80,
      0x00, 0xff, 0x31, 0xe3, 0xff,
   QV_1280x1024x8_INDEX, VALID_MODE, 0x23, 0xc8,
      0xed, 0x9f, 0x9f, 0x10, 0xae, 0x90, 0x2a, 0x5a, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xb7, 0xff, 0x00,
      0x00, 0xff, 0x2a, 0xe3, 0xff,

   //
   // Monitor_66Hz
   //
   QV_TEXT_720x400x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x350x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x480x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_640x480x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_800x600x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_1024x768x8_INDEX, VALID_MODE, 0x2b, 0x00,
      0xa9, 0x7f, 0x7f, 0x0a, 0x87, 0x9f, 0x2e, 0xf5, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xba, 0xff, 0x80,
      0x00, 0xff, 0x2c, 0xe3, 0xff,
   QV_1280x1024x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,

   //
   // Monitor_68Hz
   //
   QV_TEXT_720x400x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x350x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x480x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_640x480x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_800x600x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_1024x768x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_1280x1024x8_INDEX, VALID_MODE, 0x23, 0xe8,
      0xcf, 0x9f, 0x9f, 0x12, 0xa4, 0x19, 0x2e, 0x5a, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xbb, 0xff, 0x00,
      0x00, 0xff, 0x2e, 0xe3, 0xff,

   //
   // Monitor_72Hz
   //
   QV_TEXT_720x400x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x350x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x480x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_640x480x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_800x600x8_INDEX, VALID_MODE, 0x23, 0x20,
      0x8b, 0x63, 0x66, 0x8b, 0x6d, 0x1d, 0xe9, 0xf0, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0xba, 0x57, 0x80,
      0x00, 0x6e, 0xd1, 0xe3, 0xff,
   QV_1024x768x8_INDEX, VALID_MODE, 0x2b, 0x00,
      0x9e, 0x7f, 0x7f, 0x81, 0x83, 0x93, 0x1e, 0xf1, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb3, 0xff, 0x80,
      0x00, 0xff, 0x1e, 0xe3, 0xff,
   QV_1280x1024x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,

   //
   // Monitor_75Hz
   //
   QV_TEXT_720x400x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x350x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x480x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_640x480x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_800x600x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_1024x768x8_INDEX, VALID_MODE, 0x2b, 0x00,
      0x97, 0x7f, 0x7f, 0x9a, 0x82, 0x0f, 0x1f, 0xf1, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb4, 0xff, 0x80,
      0x00, 0xfe, 0x1f, 0xe3, 0x00,
   QV_1280x1024x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,

   //
   // Monitor_76Hz
   //
   QV_TEXT_720x400x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x350x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_TEXT_640x480x4_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_640x480x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_800x600x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_1024x768x8_INDEX, INVALID_MODE, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00,
   QV_1280x1024x8_INDEX, VALID_MODE, 0x27, 0xe8,
      0xca, 0x9f, 0x9f, 0x8d, 0xa6, 0x1a, 0x24, 0x5a, 0x00, 0x60,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xb5, 0xff, 0x00,
      0x00, 0xff, 0x24, 0xe3, 0xff,
}; // MonData

//
//    This array contains a set of boolean values with the
//    following properties.
//    If a monitor supports a specified mode, the value
//    is TRUE.  If the monitor does not support the mode,
//    the value is FALSE.
//    +++++++++++++++++++++++++++++++++++++
//    ++++++++++++++ IMPORTANT ++++++++++++
//    +++++++++++++++++++++++++++++++++++++
//    The NUM_VIDEO_MODES value must be hardcoded because
//    a non constant value cannot be the index of an
//    array.
//    This value must be changed when new modes are added.
//    Currently, the QVISION BIOS does not support 800x600x8 so
//    we set these modes to be invalid if INT10 is chosen
//    as the initialization avenue.
//
#define NUM_VIDEO_MODES    7    // must be hardcoded for the fValidMode array.

BOOLEAN fValidMode[NUM_MONITOR_CLASSES][NUM_VIDEO_MODES] = {

//////////////////////////////////////////////////////////////
//  Monitor_Vga
//////////////////////////////////////////////////////////////
   TRUE,                                    // text 640x350 (EGA)
   TRUE,                                    // text 640x480 (VGA)
   TRUE,                                    // text 720x400 (VGA)
   TRUE,                                    // 640x480x8
#ifdef INIT_INT10                           // The Qvision hardware does not
   FALSE,                                   // support 800x600x8 in BIOS
#else                                       // so we always set this mode
   FALSE,                                   // to be invalid. (for now)
#endif
   FALSE,                                   // 1024x768x8
   FALSE,                                   // 1280x1024x8
//////////////////////////////////////////////////////////////
//  Monitor_AG1024
//////////////////////////////////////////////////////////////
   TRUE,                                    // text 640x350 (EGA)
   TRUE,                                    // text 640x480 (VGA)
   TRUE,                                    // text 720x400 (VGA)
   TRUE,                                    // 640x480x8
#ifdef INIT_INT10                           // The Qvision hardware does not
   FALSE,                                   // support 800x600x8 in BIOS
#else                                       // so we always set this mode
   FALSE,                                   // to be invalid. (for now)
#endif
   TRUE,                                    // 1024x768x8
   FALSE,                                   // 1280x1024x8
//////////////////////////////////////////////////////////////
//  Monitor_Qvision
//////////////////////////////////////////////////////////////
   TRUE,                                    // text 640x350 (EGA)
   TRUE,                                    // text 640x480 (VGA)
   TRUE,                                    // text 720x400 (VGA)
   TRUE,                                    // 640x480x8
#ifdef INIT_INT10                           // The Qvision hardware does not
   FALSE,                                   // support 800x600x8 in BIOS
#else                                       // so we always set this mode
   TRUE,                                    // to be invalid. (for now)
#endif
   TRUE,                                    // 1024x768x8
   FALSE,                                   // 1280x1024x8
//////////////////////////////////////////////////////////////
//  Monitor_1280
//////////////////////////////////////////////////////////////
   TRUE,                                    // text 640x350 (EGA)
   TRUE,                                    // text 640x480 (VGA)
   TRUE,                                    // text 720x400 (VGA)
   TRUE,                                    // 640x480x8
#ifdef INIT_INT10                           // The Qvision hardware does not
   FALSE,                                   // support 800x600x8 in BIOS
#else                                       // so we always set this mode
   TRUE,                                    // to be invalid. (for now)
#endif
   TRUE,                                    // 1024x768x8
   TRUE,                                    // 1280x1024x8
//////////////////////////////////////////////////////////////
//  Monitor_SVGA
//////////////////////////////////////////////////////////////
   TRUE,                                    // text 640x350 (EGA)
   TRUE,                                    // text 640x480 (VGA)
   TRUE,                                    // text 720x400 (VGA)
   TRUE,                                    // 640x480x8
#ifdef INIT_INT10                           // The Qvision hardware does not
   FALSE,                                   // support 800x600x8 in BIOS
#else                                       // so we always set this mode
   TRUE,                                    // to be invalid. (for now)
#endif
   FALSE,                                   // 1024x768x8
   FALSE,                                   // 1280x1024x8
//////////////////////////////////////////////////////////////
//    Monitor_60Hz
//////////////////////////////////////////////////////////////
   TRUE,                                    // text 640x350 (EGA)
   TRUE,                                    // text 640x480 (VGA)
   TRUE,                                    // text 720x400 (VGA)
   TRUE,                                    // 640x480x8
#ifdef INIT_INT10                           // The Qvision hardware does not
   FALSE,                                   // support 800x600x8 in BIOS
#else                                       // so we always set this mode
   TRUE,                                    // to be invalid. (for now)
#endif
   TRUE,                                    // 1024x768x8
   TRUE,                                    // 1280x1024x8
//////////////////////////////////////////////////////////////
//  Monitor_66Hz
//////////////////////////////////////////////////////////////
   TRUE,                                    // text 640x350 (EGA)
   TRUE,                                    // text 640x480 (VGA)
   TRUE,                                    // text 720x400 (VGA)
   FALSE,                                   // 640x480x8
#ifdef INIT_INT10                           // The Qvision hardware does not
   FALSE,                                   // support 800x600x8 in BIOS
#else                                       // so we always set this mode
   FALSE,                                   // to be invalid. (for now)
#endif
   TRUE,                                    // 1024x768x8
   FALSE,                                   // 1280x1024x8
//////////////////////////////////////////////////////////////
//    Monitor_68Hz
//////////////////////////////////////////////////////////////
   TRUE,                                    // text 640x350 (EGA)
   TRUE,                                    // text 640x480 (VGA)
   TRUE,                                    // text 720x400 (VGA)
   FALSE,                                   // 640x480x8
#ifdef INIT_INT10                           // The Qvision hardware does not
   FALSE,                                   // support 800x600x8 in BIOS
#else                                       // so we always set this mode
   FALSE,                                   // to be invalid. (for now)
#endif
   FALSE,                                   // 1024x768x8
   TRUE,                                    // 1280x1024x8
//////////////////////////////////////////////////////////////
//  Monitor_72Hz
//////////////////////////////////////////////////////////////
   TRUE,                                    // text 640x350 (EGA)
   TRUE,                                    // text 640x480 (VGA)
   TRUE,                                    // text 720x400 (VGA)
   FALSE,                                   // 640x480x8
#ifdef INIT_INT10                           // The Qvision hardware does not
   FALSE,                                   // support 800x600x8 in BIOS
#else                                       // so we always set this mode
   TRUE,                                    // to be invalid. (for now)
#endif
   TRUE,                                    // 1024x768x8
   FALSE,                                   // 1280x1024x8
//////////////////////////////////////////////////////////////
//    Monitor_75Hz
//////////////////////////////////////////////////////////////
   TRUE,                                    // text 640x350 (EGA)
   TRUE,                                    // text 640x480 (VGA)
   TRUE,                                    // text 720x400 (VGA)
   FALSE,                                   // 640x480x8
#ifdef INIT_INT10                           // The Qvision hardware does not
   FALSE,                                   // support 800x600x8 in BIOS
#else                                       // so we always set this mode
   FALSE,                                   // to be invalid. (for now)
#endif
   TRUE,				    // 1024x768x8
   FALSE,				    // 1280x1024x8
//////////////////////////////////////////////////////////////
//    Monitor_76Hz
//////////////////////////////////////////////////////////////
   TRUE,                                    // text 640x350 (EGA)
   TRUE,                                    // text 640x480 (VGA)
   TRUE,                                    // text 720x400 (VGA)
   FALSE,                                   // 640x480x8
#ifdef INIT_INT10                           // The Qvision hardware does not
   FALSE,                                   // support 800x600x8 in BIOS
#else                                       // so we always set this mode
   FALSE,                                   // to be invalid. (for now)
#endif
   FALSE,				    // 1024x768x8
   TRUE					    // 1280x1024x8

}; // fValidMode


