//***************************************************************************
//	PCI Interface(DACK) header
//
//***************************************************************************

#ifndef __CDACK_H__
#define __CDACK_H__

#include "decoder.h" // HACK?

#define PALETTE_Y	0x01
#define PALETTE_Cb	0x02
#define PALETTE_Cr	0x03


NTSTATUS PCIF_RESET( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_AMUTE_ON( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_AMUTE_OFF( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_AMUTE2_ON( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_AMUTE2_OFF( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_VSYNC_ON( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_VSYNC_OFF( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_PACK_START_ON( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_PACK_START_OFF( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_SET_DIGITAL_OUT( PHW_DEVICE_EXTENSION pHwDevExt, UCHAR mode );
void PCIF_SET_DMA0_SIZE( PHW_DEVICE_EXTENSION pHwDevExt, ULONG dmaSize );
void PCIF_SET_DMA1_SIZE( PHW_DEVICE_EXTENSION pHwDevExt, ULONG dmaSize );
void PCIF_SET_DMA0_ADDR( PHW_DEVICE_EXTENSION pHwDevExt, ULONG dmaAddr );
void PCIF_SET_DMA1_ADDR( PHW_DEVICE_EXTENSION pHwDevExt, ULONG dmaAddr );
void PCIF_DMA0_START( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_DMA1_START( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_SET_PALETTE( PHW_DEVICE_EXTENSION pHwDevExt, UCHAR select, PUCHAR pPalette );
void PCIF_GET_PALETTE( PHW_DEVICE_EXTENSION pHwDevExt, UCHAR select, PUCHAR pPalette );
void PCIF_CHECK_SERIAL( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_DMA_ABORT( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_ALL_IFLAG_CLEAR( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_ASPECT_0403( PHW_DEVICE_EXTENSION pHwDevExt );
void PCIF_ASPECT_1609( PHW_DEVICE_EXTENSION pHwDevExt );

#endif	// __CACK_H__
