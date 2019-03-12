#include "lwip/netif.h"   //网络接口管理函数和结构定义
#include "lwip/init.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "ethernetif.h"
#include "lwip/tcp.h"
#include <string.h>
#include "lwipdemo.h"
#include "stm32f4xx.h"



#define MAX_DHCP_TRIES        4
struct netif gnetif;


__IO uint8_t DHCP_state;

void lwip_dhcp_process_handle();


void lwip_init_task(void)
{

  ip4_addr_t ipaddr;
  ip4_addr_t netmask;
  ip4_addr_t gw;
  /* Initilialize the LwIP stack without RTOS */
  lwip_init();

#ifdef USE_DHCP
	ipaddr.addr=0;
	netmask.addr= 0;
	gw.addr = 0;
#else
  /* IP addresses initialization without DHCP (IPv4) */
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#endif
  /* add the network interface (IPv4/IPv6) without RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /* Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }
#ifdef USE_DHCP
	DHCP_state  = DHCP_START;
#else
	printf("Static IP Address\r\n");
	printf("IP: %d.%d.%d.%d \r\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
	printf("NETMASK: %d.%d.%d.%d \r\n",NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
	printf("Gateway: %d.%d.%d.%d \r\n",GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#endif
}

void lwip_Process(void)
{
	ethernetif_input(&gnetif);
  
  sys_check_timeouts();
#ifdef USE_DHCP
	if ((DHCP_state != DHCP_ADDRESS_ASSIGNED) && 
        (DHCP_state != DHCP_TIMEOUT) &&
          (DHCP_state != DHCP_LINK_DOWN))
    {
			//printf("\nFine DHCP periodic process every 500ms\n");
      /* process DHCP state machine */
       lwip_dhcp_process_handle();    
    }
#endif
}

void lwip_dhcp_process_handle()
{
	struct ip4_addr ip_addr;
	struct ip4_addr netmask_addr;
	struct ip4_addr gw;
	struct dhcp *dhcp;
	switch(DHCP_state){
		
		case DHCP_START:
		{
			DHCP_state = DHCP_WAIT_ADDRESS;
			dhcp_start(&gnetif);
		  printf("looking for dhcpserver please waiting.....\r\n");
		}
			break;
			
		case DHCP_WAIT_ADDRESS:
		{
			dhcp = netif_dhcp_data(&gnetif);
			if(dhcp_supplied_address(&gnetif)){
				
				DHCP_state = DHCP_ADDRESS_ASSIGNED;
				
				printf("\n  IP address assigned \n");
				printf("    by a DHCP server   \r\n");
		    printf("IP address: %s\r\n",ip4addr_ntoa(&dhcp->offered_ip_addr));
				                      
				printf("NETMASK: %s \r\n",ip4addr_ntoa(&dhcp->offered_sn_mask));
				printf("Gateway: %s  \r\n",ip4addr_ntoa(&dhcp->offered_gw_addr));
				//dhcp_stop(&gnetif);
			}else{
				dhcp = netif_dhcp_data(&gnetif);
				if(dhcp->tries > MAX_DHCP_TRIES){
					DHCP_state = DHCP_TIMEOUT;
					/* Stop DHCP */
          dhcp_stop(&gnetif);

          /* Static address used */
          IP4_ADDR(&ip_addr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
          IP4_ADDR(&netmask_addr, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
          IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
          netif_set_addr(&gnetif, &ip_addr , &netmask_addr, &gw);


          printf("\n    DHCP timeout    \n");
          printf("  Static IP address   \n");
		      printf("IP: %d.%d.%d.%d\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
					printf("NETMASK: %d.%d.%d.%d\n",NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
					printf("Gateway: %d.%d.%d.%d\n",GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
				}
			}
		}
			
			break;
	  default:
			 break;

	}
}


