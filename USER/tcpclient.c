#include "tcp_client.h"

#include "lwip/memp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "stm32f4xx.h"

u8_t  recev_buf[50];
__IO uint32_t message_count=0;

u8_t   data[100];

struct tcp_pcb *tcpclient_pcb;

enum tcpclient_status{
	ES_NOT_CONNECTED = 0,
	ES_CONNECTED,
	ES_RECEIVED,
	ES_CLOSE,
};

struct tcpclient{
	enum tcpclient_status state;
	struct tcp_pcb *pcb;
	struct pbuf *p_tx;
};

struct tcpclient tcpclient;

/*callback function*/
static err_t tcpclient_recv(void *arg,struct tcp_pcb *tcp_pcb ,struct pbuf *p, err_t err);
static err_t tcpclient_sent(void *arg, struct tcp_pcb *tpcb,u16_t len);
static err_t tcpclient_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcpclient_connected(void *arg, struct tcp_pcb *tpcb, err_t err);

/* private function*/
static void tcpclient_connected_close(struct tcp_pcb *tpcb,struct tcpclient *es);
static void tcpclient_send(struct tcp_pcb *tpcb,struct tcpclient *es);

	
void tcpclient_init(void)
{
	struct ip4_addr DestIPaddr;

	tcpclient_pcb = tcp_new();
	if(tcpclient_pcb != NULL){
		IP4_ADDR(&DestIPaddr,DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3);
		tcp_connect(tcpclient_pcb,&DestIPaddr,DEST_PORT,tcpclient_connected);
		
	}else{
		printf("client_pcb create failed\r\n");
	}
	
	
}

static err_t tcpclient_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	err_t ret;
  struct tcpclient *es = NULL;
	
	if(err == ERR_OK){
		es = (struct tcpclient *)mem_malloc(sizeof (struct tcpclient));
		if(es != NULL){
			es->state = ES_CONNECTED;
			es->pcb = tpcb;
			sprintf((char *)data,"sending tcp client message %d",message_count);
			
			es->p_tx = pbuf_alloc(PBUF_TRANSPORT,strlen((char *)data),PBUF_POOL);
			if(es->p_tx){
			pbuf_take(es->p_tx,(char *)data,strlen((char *)data));
			tcp_arg(tpcb ,es);
			tcp_recv(tpcb,tcpclient_recv);
			tcp_sent(tpcb,tcpclient_sent);
			tcp_poll(tpcb,tcpclient_poll,1);
			tcpclient_send(tpcb,es);
			}else{
				tcpclient_connected_close(tpcb,es);
				return ERR_MEM;
			}
			
		}else {
			tcpclient_connected_close(tpcb,es);
			return ERR_MEM;
		}
		
	}else{
		printf("tcp_client conectd failed\r\n");
		ret =err;
	}
	return ret;
}

static err_t tcpclient_recv(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err)
{
	err_t ret;
	char *recv_data;
	struct tcpclient *es;
	LWIP_ASSERT("arg != NULL ",arg != NULL);
	
	es = (struct tcpclient *)arg;
	if(p == NULL){
		es->state = ES_CLOSE;
		if(es->p_tx == NULL){
			tcpclient_connected_close(tpcb,es);
		}else{
			tcpclient_send(tpcb,es);
		}
		ret = ERR_OK;
	}else if(err != ERR_OK){
		pbuf_free(p);
		ret = err;
	}else if(es->state == ES_CONNECTED){
		message_count++;
		tcp_recved(tpcb,p->tot_len);
		recv_data = (char *)malloc(p->len*sizeof(char));
		if(recv_data){
			memcpy(recv_data,p->payload,p->len);
			printf("tcp_rec<< %s",recv_data);
		}
		free(recv_data);
		pbuf_free(p);
		ret = ERR_OK;
	}else{  
		/*data recv when connection alread close */
		tcp_recved(tpcb,p->tot_len);
		pbuf_free(p);
		ret =ERR_OK;
	}
	return ret;
}
static err_t tcpclient_sent(void *arg, struct tcp_pcb *tpcb,
                              u16_t len)
{
	
	struct tcpclient *es;
	LWIP_UNUSED_ARG(len);
	es = (struct tcpclient *)arg;
	
	if(es->p_tx != NULL){
		tcpclient_send(tpcb,es);
	}
	
	return ERR_OK;
}
static err_t tcpclient_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret;
	struct tcpclient *es;
	
	es = (struct tcpclient *)arg;
	
	if(es != NULL){
		
		if(es->p_tx != NULL){
			tcpclient_send(tpcb,es);
		}else{
			if(es->state == ES_CLOSE){
				tcpclient_connected_close(tpcb,es);
			}
			ret =ERR_OK;
		}
	}else{
		tcp_abort(tpcb);
		ret = ERR_ABRT;
	}
	return ret;
}
static void tcpclient_send(struct tcp_pcb *tpcb, struct tcpclient *es)
{
	struct pbuf *ptr;
	err_t err = ERR_OK;
	while((err == ERR_OK)&& \
		    (es->p_tx != NULL)&& \
	      (es->p_tx->len < tcp_sndbuf(tpcb))){
									
		 ptr = es->p_tx;
					
     err = tcp_write(tpcb,ptr->payload,ptr->len,1);
					
		 if(err == ERR_OK){
				es->p_tx = ptr->next;
			 if(es->p_tx != NULL){
				 /**/
				pbuf_ref(es->p_tx);
			 }
			 pbuf_free(ptr);
		 }else if(err == ERR_MEM){
				es->p_tx = ptr;
		 }else{
		 }		 
	}
}
static void tcpclient_connected_close(struct tcp_pcb *tpcb,struct tcpclient* es)
{
	/*remove callback*/
	tcp_recv(tpcb,NULL);
	tcp_sent(tpcb,NULL);
	tcp_poll(tpcb,NULL,0);
	
	if(es != NULL){
		mem_free(es);
	}
	
	tcp_close(tpcb);
}

