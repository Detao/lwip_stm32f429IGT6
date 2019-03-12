#include "udp_server.h"
#include "lwip/udp.h"
#include "lwip/err.h"
#include <string.h>
void udp_server_recv (void *arg, struct udp_pcb *pcb, struct pbuf *p,
    const ip_addr_t *addr, u16_t port);

void udp_server_init()
{
	struct udp_pcb *udp_pcb;
	err_t err;
	printf("udp_server_init \r\n");
	udp_pcb = udp_new();
	if(udp_pcb){
	}else{
		printf("udp_pcb create failed\r\n");
	}
	err = udp_bind(udp_pcb,IP_ADDR_ANY,UDP_DEMO_PORT);
	if(err == ERR_OK){
		udp_recv(udp_pcb,udp_server_recv,NULL);
		
	}
	
}

void udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,
    const ip_addr_t *addr, u16_t port)
{
	uint32_t data_len=0;
	struct pbuf* q;
	char *recvdata;
	if(p != NULL){
		recvdata = (char *)mem_malloc(p->tot_len);
		if(recvdata == NULL){
			printf("recvdata create failed");
			return ;
		}
		for(q = p ;q != NULL ;q = q->next){
			memcpy(recvdata+data_len,q->payload,q->len);
			data_len += q->len;
		}
		pcb->remote_ip = *addr;
		pcb->remote_port = port;
		printf("[recv form %s:%d]:%s",ip4addr_ntoa(addr),port,recvdata);
		udp_sendto(pcb,p,addr,port);
		pbuf_free(p);
		mem_free(recvdata);
	}else{
		udp_disconnect(pcb);
	}
		
}