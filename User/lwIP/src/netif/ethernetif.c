/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "lwip/opt.h"

#if 1 /* don't build, this is only a skeleton, see previous comment */

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "lwip/etharp.h"
#include "netif/ppp/pppoe.h"

#include "ENC28J60/enc28j60.h"
#include <string.h>

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
};


static unsigned char ENC28J60_MACID[6] = {0x04,0x02,0x35,0x00,0x00,0x01};

/* Forward declarations. */
static void  ethernetif_input(struct netif *netif);

extern struct netif enc28j60_netif;



/****************************************************************************
* ��    �ƣ�void ENC28J60_PacketSend (struct pbuf *p)
* ��    �ܣ�����һ������	���pbuf�����ݵķ���																	 
* ��ڲ�����
* ���ڲ���: 
* ˵    ��������uip�������������LwIP�����ݰ����ͺͽ���
* ���÷�������pbuf�е����ݿ�����ȫ������MyDatabuf�У�Ȼ���������ĺ���enc28j60PacketSend��������
****************************************************************************/ 
//��̫������֡����󳤶�1500�������������������ڴ濪�����������������ü�
static unsigned char  ENC28J60_SendBuf[1500]; 
static err_t ENC28J60_PacketSend (struct pbuf *p)
{
	struct pbuf *q = NULL;
	unsigned int templen = 0;

	for(q=p ; q!=NULL ; q=q->next)
	{
		memcpy(&ENC28J60_SendBuf[templen], q->payload, q->len);	 //��pbuf�е����ݿ�����ȫ������MyDatabuf��
		templen += 	q->len ;

		if(templen > 1500 || templen > p->tot_len)	 	//��Ч��У�飬��ֹ�������
		{
			LWIP_PLATFORM_DIAG(("ENC28J60_PacketSend: error,tmplen=%"U32_F",tot_len=%"U32_F"\n\t", templen, p->tot_len));
			return ERR_BUF;
		}
	}
	
	//������ϣ�����������ݵķ��͹���
	if(templen == p->tot_len)
	{
		enc28j60PacketSend(templen, ENC28J60_SendBuf);		   //�����������ͺ���
		return ERR_OK; 
	}
	
	LWIP_PLATFORM_DIAG(("ENC28J60_PacketSend: length mismatch ,tmplen=%"U32_F",tot_len=%"U32_F"\n\t", templen, p->tot_len));
	
	return ERR_BUF;
}

/****************************************************************************
* ��    �ƣ�struct pbuf *ENC28J60_PacketReceive(struct netif *netif)
* ��    �ܣ����LwIP��Ҫ�����ݰ�����																	 
* ��ڲ�����
* ���ڲ���: 
* ˵    ��������uip�������������LwIP�����ݰ����ͺͽ���
* ���÷��������������ݿ�����ȫ������MyRecvbuf�У�����װ��pbuf
****************************************************************************/ 
//��̫������֡����󳤶�1500�������������������ڴ濪�����������������ü�
static unsigned char  ENC28J60_RecvBuf[1500]; 
struct pbuf *ENC28J60_PacketReceive(void)
{
	struct pbuf *p = NULL;	
	unsigned int recvlen;
	unsigned int i;
	struct pbuf *q = NULL;
    
	recvlen = enc28j60PacketReceive(1500, ENC28J60_RecvBuf);

	if(!recvlen)	       //�������ݳ���Ϊ0��ֱ�ӷ��ؿ�
	{
	    return NULL;
	}
	
	//�����ں�pbuf�ռ䣬ΪRAM����
	p = pbuf_alloc(PBUF_RAW, recvlen, PBUF_RAM);
	
	if(!p)			       //����ʧ�ܣ��򷵻ؿ�
	{
	    LWIP_PLATFORM_DIAG(("ENC28J60_PacketReceive: pbuf_alloc fail ,len=%"U32_F"\n\t", recvlen));
		return NULL;
	}
    //����ɹ����������ݵ�pbuf��
	q = p;
	i = 0;
	while(q != NULL)
	{   
		memcpy(q->payload, &ENC28J60_RecvBuf[i], q->len);
		i += q->len;
		q = q->next;
		if(i >= recvlen)  break;
	}
		
	return p;
}


/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{
//	struct ethernetif *ethernetif = netif->state;

	if(netif == &enc28j60_netif) {
		/* set MAC hardware address length */
		netif->hwaddr_len = ETHARP_HWADDR_LEN;

		/* set MAC hardware address */
		netif->hwaddr[0] = ENC28J60_MACID[0];
		netif->hwaddr[1] = ENC28J60_MACID[1];
		netif->hwaddr[2] = ENC28J60_MACID[2];
		netif->hwaddr[3] = ENC28J60_MACID[3];
		netif->hwaddr[4] = ENC28J60_MACID[4];                
		netif->hwaddr[5] = ENC28J60_MACID[5];

		/* maximum transfer unit */
		netif->mtu = 1500;

		/* device capabilities */
		/* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
		netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
		
#if LWIP_IPV6 && LWIP_IPV6_MLD
		/*
		* For hardware/netifs that implement MAC filtering.
		* All-nodes link-local is handled by default, so we must let the hardware know
		* to allow multicast packets in.
		* Should set mld_mac_filter previously. */
		if (netif->mld_mac_filter != NULL) {
			ip6_addr_t ip6_allnodes_ll;
			ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
			netif->mld_mac_filter(netif, &ip6_allnodes_ll, NETIF_ADD_MAC_FILTER);
		}
#endif /* LWIP_IPV6 && LWIP_IPV6_MLD */

		/* Do whatever else is needed to initialize interface. */
		//enc28j60Init(MyMacID); 
		ENC28J60_Init(ENC28J60_MACID);
	}
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
#if 0  //LwIP�Ƽ��ı�̿��
#if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

  for (q = p; q != NULL; q = q->next) {
    /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
    send data from(q->payload, q->len);
  }

  signal that packet should be sent();

  MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
  if (((u8_t*)p->payload)[0] & 1) {
    /* broadcast or multicast packet*/
    MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
  } else {
    /* unicast packet */
    MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
  }
  /* increase ifoutdiscards or ifouterrors on error */

#if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

  LINK_STATS_INC(link.xmit);

  return ERR_OK;
#endif
  
	if(netif == &enc28j60_netif) {
		return ENC28J60_PacketSend(p);	 
	}
	while(1);
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif)
{
#if  0   //LwIP�Ƽ��ı�̿��
  struct ethernetif *ethernetif = netif->state;
  struct pbuf *p, *q;
  u16_t len;

  /* Obtain the size of the packet and put it into the "len"
     variable. */
  len = ;

#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

  if (p != NULL) {

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    /* We iterate over the pbuf chain until we have read the entire
     * packet into the pbuf. */
    for (q = p; q != NULL; q = q->next) {
      /* Read enough bytes to fill this pbuf in the chain. The
       * available data in the pbuf is given by the q->len
       * variable.
       * This does not necessarily have to be a memcpy, you can also preallocate
       * pbufs for a DMA-enabled MAC and after receiving truncate it to the
       * actually received size. In this case, ensure the tot_len member of the
       * pbuf is the sum of the chained pbuf len members.
       */
      read data into(q->payload, q->len);
    }
    acknowledge that packet has been read();

    MIB2_STATS_NETIF_ADD(netif, ifinoctets, p->tot_len);
    if (((u8_t*)p->payload)[0] & 1) {
      /* broadcast or multicast packet*/
      MIB2_STATS_NETIF_INC(netif, ifinnucastpkts);
    } else {
      /* unicast packet*/
      MIB2_STATS_NETIF_INC(netif, ifinucastpkts);
    }
#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.recv);
  } else {
    drop packet();
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
    MIB2_STATS_NETIF_INC(netif, ifindiscards);
  }

  return p;
#endif
  
	//return NULL;
	if(netif == &enc28j60_netif) {
		return ENC28J60_PacketReceive();
	}
	while(1);
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
static void 
ethernetif_input(struct netif *netif)
{
	struct pbuf *p;

	/* move received packet into a new pbuf */
	p = low_level_input(netif);
	
	/* no packet could be read, silently ignore this */
	while(p != NULL) {
		/* full packet send to tcpip_thread to process */
		if(netif->input(p, netif) != ERR_OK) { 
			LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
			pbuf_free(p);
		}
		
		p = low_level_input(netif);
	}
}

void process_mac(void)
{
	ethernetif_input(&enc28j60_netif);
//	ethernetif_input(&other_ethernetif);
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
	struct ethernetif *ethernetif;

	LWIP_ASSERT("netif != NULL", (netif != NULL));

	ethernetif = mem_malloc(sizeof(struct ethernetif));
	if (ethernetif == NULL) {
		LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
		return ERR_MEM;
	}

#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

	/*
	 * Initialize the snmp variables and counters inside the struct netif.
	 * The last argument should be replaced with your link speed, in units
	 * of bits per second.
	 */
	MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

	netif->state = ethernetif;
	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;
	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */
	netif->output = etharp_output;
#if LWIP_IPV6
	netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
	netif->linkoutput = low_level_output;

	ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

	/* initialize the hardware */
	low_level_init(netif);

	return ERR_OK;
}

#endif /* 0 */
