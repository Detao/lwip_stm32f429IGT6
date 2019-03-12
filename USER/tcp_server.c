#include "tcp_server.h"
#include "lwip/tcp.h"
#include "lwip/mem.h"
#include <stdlib.h>
#include <string.h>

/*callbacke function */

static err_t tcp_server_accept(void *arg ,struct tcp_pcb *tpcb,err_t err);
static err_t tcp_server_recv(void *arg ,struct tcp_pcb *tpcb,struct pbuf *p ,err_t err);
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb ,u16_t len);
static err_t tcp_server_poll(void *arg ,struct tcp_pcb *tpcb );
static void tcp_server_error(void *arg ,err_t err);

/*private functions*/
static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server *es);
static void tcp_server_connection_close(struct tcp_pcb *tpcb,struct tcp_server *es);

struct tcp_pcb *tcp_server_pcb;
struct tcp_pcb *tcp_server_conn_pcb;

void tcp_server_init(void)
{
	err_t err;

  printf("tcp_server_init \r\n");
	tcp_server_pcb = tcp_new();
	
	if(tcp_server_pcb !=  NULL){
		err = tcp_bind(tcp_server_pcb ,IP_ADDR_ANY,TCP_SERVER_PROT);
		if(err == ERR_OK){
			tcp_server_conn_pcb = tcp_listen(tcp_server_pcb);
			tcp_accept(tcp_server_conn_pcb,tcp_server_accept);
		}else{
			printf("tcp_bind failed\r\n");
		}
	}else{
		printf("tcp_server_pcb create failed\r\n");
	}
	
}

static err_t tcp_server_accept(void *arg , struct tcp_pcb *tpcb, err_t err)
{
	err_t ret ;
	struct tcp_server *es;
	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);
	printf("[acccept ] callback \r\n");
	tcp_setprio(tpcb,TCP_PRIO_MIN);
	
	es = (struct tcp_server *)mem_malloc(sizeof(struct tcp_server));
	
	if(es != NULL){
		es->pcb  = tpcb;
		es->p = NULL;
		es->state = ES_TCPSERVER_ACCEPTED;
		
		tcp_arg(tpcb, es);
		tcp_recv(tpcb,tcp_server_recv);
		tcp_err(tpcb ,tcp_server_error);
		tcp_sent(tpcb,tcp_server_sent);
		tcp_poll(tpcb,tcp_server_poll,1);
		printf("IP:%d.%d.%d.%d connected",tpcb->remote_ip.addr&0xff,  \
																			tpcb->remote_ip.addr>>8&0xff, \
																			tpcb->remote_ip.addr>>16&0xff, \
																			tpcb->remote_ip.addr>>24 &0xff);
		
		ret =ERR_OK;
		
	}else{
		ret = ERR_MEM;
	}
	
	return ret;
}
static err_t tcp_server_recv(void *arg ,struct tcp_pcb *tpcb,struct pbuf *p ,err_t err)
{
	err_t ret;
	char *recvdata;
	struct pbuf* q;
	struct tcp_server *es;
	int data_len;
	LWIP_ASSERT("[tcp_server] arg != NULL",arg != NULL);
	es = (struct tcp_server*) arg;
	if(p == NULL){
		/*recv data is NULL*/
		es->state = ES_TCPSERVER_CLOSING;
		es->p = p;
		ret = ERR_OK;
	}else if(err != ERR_OK){
		if(p)pbuf_free(p);
		ret = err;
		/*data is not null but has err*/
	}else if(es->state == ES_TCPSERVER_ACCEPTED){
		recvdata = (char *)mem_malloc(p->tot_len);
		if(recvdata == NULL){
			printf("[tcp_server recved] recvdata create failed\r\n");
			return ERR_MEM;
		}
		data_len = 0;
		for(q = p;q != NULL; q=q->next ){
			memcpy(recvdata+data_len,q->payload,q->len);
			data_len += q->len;
		}
		printf("recv<< %s\r\n",recvdata);
		es->p = pbuf_alloc(PBUF_TRANSPORT,data_len,PBUF_POOL);
		if(es->p){
			pbuf_take(es->p,recvdata,data_len);
			tcp_server_send(tpcb,es);
		}else{
			printf("[tcp_server send] sedbuf create failed");
			tcp_server_connection_close(tpcb,es);
		}
		tcp_recved(tpcb,p->tot_len);
		pbuf_free(p);
		mem_free(recvdata);
		/*connecting*/
		ret = ERR_OK;
	}else{
		/*server close*/
		tcp_recved(tpcb,p->tot_len);
		es->p = NULL;
		pbuf_free(p);
		ret = ERR_OK;
		
	}
	
	return ret;
}
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb ,u16_t len)
{
	err_t ret = ERR_OK;
	struct tcp_server * es;
	LWIP_UNUSED_ARG(len);
	es = (struct tcp_server *)arg;
	if(es->p){
		tcp_server_send(tpcb,es);
	}
	
	return ret ;
}
static err_t tcp_server_poll(void *arg ,struct tcp_pcb *tpcb )
{
	err_t ret;
	struct tcp_server *es;
	es = (struct tcp_server *)arg;
	if(es != NULL){
		if(es->p != NULL){
			tcp_server_send(tpcb ,es);
		}else if(es->state == ES_TCPSERVER_CLOSING){
			tcp_server_connection_close(tpcb,es);
		}
		ret = ERR_OK;
	}else{
		tcp_abort(tpcb);
		ret = ERR_ABRT;
	}
	return ret;
}
static void tcp_server_error(void *arg ,err_t err)
{

	LWIP_UNUSED_ARG(err);
	printf("[tcp_error]%x\r\n",(uint32_t)arg);
	if(arg != NULL) mem_free(arg);

}

static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server *es)
{
	err_t err=ERR_OK;
	uint16_t len;
	struct pbuf *ptr;
	while((err==ERR_OK)&&(es->p != NULL)&&(es->p->len<= tcp_sndbuf(tpcb))){
		ptr = es->p;
		err = tcp_write(tpcb,ptr->payload,ptr->len,1);
		if(err == ERR_OK){
			len = ptr->len;
			es->p = ptr->next;
			if(es->p != NULL) pbuf_ref(es->p);
			pbuf_free(ptr);
			tcp_recved(tpcb,len);
		}else if(err == ERR_MEM) es->p =ptr;
	}
}

static void tcp_server_connection_close(struct tcp_pcb *tpcb,struct tcp_server *es)
{
	tcp_close(tpcb);
	tcp_arg(tpcb,NULL);
	tcp_sent(tpcb,NULL);
	tcp_recv(tpcb,NULL);
	tcp_err(tpcb,NULL);
	tcp_poll(tpcb,NULL,0);
	if(es) mem_free(es);
}