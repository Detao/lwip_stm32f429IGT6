#ifndef __TCPCLIENT__H__
#define __TCPCLIENT_H__

#include "lwip/tcp.h"

#define DEST_IP_ADDR0 192
#define DEST_IP_ADDR1 168
#define DEST_IP_ADDR2 1
#define DEST_IP_ADDR3 114
#define DEST_PORT 6000





void tcpclient_init(void);
#endif /*__TCPCLIENT_H__*/
