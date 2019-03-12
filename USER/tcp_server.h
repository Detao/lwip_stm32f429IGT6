#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#define TCP_SERVER_PROT 5555


#define TCP_PRIO_MIN    1
#define TCP_PRIO_NORMAL    64
#define TCP_PRIO_MAX   127


enum tcp_server_status{
	ES_TCPSERVER_NONE =0,
	ES_TCPSERVER_ACCEPTED,
	ES_TCPSERVER_CLOSING,
};

struct tcp_server{
	enum tcp_server_status state;
	struct tcp_pcb *pcb;
	struct pbuf *p;
};

void tcp_server_init(void);
#endif /*__TCP_SERVER_H__*/

