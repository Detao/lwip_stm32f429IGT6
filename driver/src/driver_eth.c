//#include "driver_eth.h"

//#include "driver_control.h"

//Driver_t eth_handle;

//static int Init(const char *dev_name)
//{
//	/* Initialize Tx Descriptors list: Chain Mode */
//  ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
//  /* Initialize Rx Descriptors list: Chain Mode  */
//  ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);
//	
//	#ifdef CHECKSUM_BY_HARDWARE
//  /* Enable the TCP, UDP and ICMP checksum insertion for the Tx frames */
//  for(i=0; i<ETH_TXBUFNB; i++)
//    {
//      ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
//    }
//#endif
//	return 0;
//}

//static DIRVER_HANDLE Open()
//{
//	 /* Enable MAC and DMA transmission and reception */
//  ETH_Start();
//	return (DIRVER_HANDLE)100;
//}
//static int Close(Driver_t dev)
//{
//	return 0;
//}

//static int Read(Driver_t dev,char *buf,unsigned int num)
//{
//	 /* get received frame */
//  frame = ETH_Get_Received_Frame();
//	 /* Obtain the size of the packet and put it into the "len" variable. */
//  len = frame.length;
//  buffer = (u8 *)frame.buffer;
//	return 0;
//}
//static int IoCtrl(Driver_t dev,char *buf,unsigned int num)
//{
//	return 0;
//}
//static int Flush(Driver_t dev_hanlde)
//{
//	return 0;
//}


//const Driver ETH_Driver = 
//{
//	.dev_name = "ETH_Driver",
//	.Flush = Flush,
//	.Read = Read,
//	.Close = Close,
//	.Open = Open,
//	.Init = Init,
//	.IoCtrl = IoCtrl,
//};