/*
Perl ARP Extension
Create and send an arp packet

Programmed by Bastian Ballmann
Last update: 31.12.2004

This program is free software; you can redistribute 
it and/or modify it under the terms of the 
GNU General Public License version 2 as published 
by the Free Software Foundation.

This program is distributed in the hope that it will 
be useful, but WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS 
FOR A PARTICULAR PURPOSE. 
See the GNU General Public License for more details. 
*/

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"
#include <stdio.h>           
#include <stdlib.h>          
#include <string.h>          
#include <errno.h>           
#include <net/ethernet.h>    
#include <net/if.h>
#include <arpa/inet.h>       
#include "arp.h"

MODULE = Net::ARP		PACKAGE = Net::ARP		

void
send_packet(dev, sip, dip, smac, dmac, type)
	unsigned char *dev;
	unsigned char *sip;
	unsigned char *dip;
	unsigned char *smac;
	unsigned char *dmac;
	unsigned char *type;

	CODE:
	  int uid;
	  unsigned int packetsize = sizeof(struct arphdr) + sizeof(struct ether_header);
	  unsigned char packet[packetsize];
	  struct ether_header *ethhdr = (struct ether_header *)packet;
	  struct arphdr *arp = (struct arphdr *)(packet + sizeof(struct ether_header));
	  u_short op;	

	  // Are you root?
	  uid = getuid();
	  if(uid != 0) 
	  { 
	    printf("You must have UID 0 instead of %d.\n",uid); 
	    exit(1); 
	  }

	  // Initialize packet buffer 
	  memset(packet,0,packetsize);

	  // What's the ARP operation type?
	  if(!strcmp(type,"request"))
	  {
	    op = ARPOP_REQUEST;
	  }
	  else if(!strcmp(type,"reply"))
	  {
	    op = ARPOP_REPLY;
	  }
	  else if(!strcmp(type,"revrequest"))
	  {
	    op = ARPOP_REVREQUEST;
	  }
	  else if(!strcmp(type,"revreply"))
	  {
	    op = ARPOP_REVREPLY;
	  }
	  else if(!strcmp(type,"invrequest"))
	  {
	    op = ARPOP_INVREQUEST;
	  }
	  else if(!strcmp(type,"invreply"))
	  {
	    op = ARPOP_INVREPLY;
	  }
	  else
	  {
	    op = ARPOP_REPLY;
	  }

	  // Ethernet header
	  memcpy(ethhdr->ether_dhost,(u_char *)ether_aton(dmac),ETHER_ADDR_LEN); // Destination MAC
	  memcpy(ethhdr->ether_shost,(u_char *)ether_aton(smac),ETHER_ADDR_LEN); // Source MAC
	  ethhdr->ether_type = htons(ETHERTYPE_ARP);                             // ARP protocol

	  // ARP header
	  arp->hw_type = htons(ARPHDR_ETHER);                                    // Hardware address type
	  arp->proto_type = htons(ETH_P_IP);                                     // Protocol address type
	  arp->ha_len = ETH_ALEN;                                                // Hardware address length
	  arp->pa_len = IP_ALEN;                                                 // Protocol address length
	  arp->opcode = htons(op);                                               // ARP operation
	  memcpy(arp->source_add,(u_char *)ether_aton(smac),ETH_ALEN);           // Source MAC
	  *(u_long *)arp->source_ip = inet_addr(sip);                            // Source IP

	  if(strcmp(dmac,"ff:ff:ff:ff:ff:ff"))
          	memcpy(arp->dest_add,(u_char *)ether_aton(dmac),ETH_ALEN);       // Destination MAC

	  *(u_long *)arp->dest_ip = inet_addr(dip);                              // Destination IP


	  // Run packet!! Run!
	  // FreeBSD code
	  if(SOCK_TYPE == SOCK_RAW)
	  {
	     send_packet_bsd(dev,packet,packetsize);	
	  }

	  // Linux code
	  else
	  {
	     send_packet_linux(dev,packet,packetsize);
	  }

char *
get_mac(dev, mac)
	unsigned char *dev;
	unsigned char *mac;
	CODE:
          char tmp[20];

	  if(SOCK_TYPE == SOCK_RAW)
	  {
	    get_mac_bsd(dev,tmp);
	  }
	  else
	  {
	    get_mac_linux(dev,tmp);
	  }

	  mac = tmp;

	OUTPUT:
	mac


char *
arp_lookup(dev, ip, mac)
	unsigned char *dev;
	unsigned char *ip;
	unsigned char *mac;
	CODE:
	  char tmp[20];

	  if(SOCK_TYPE == SOCK_RAW)
	  {
	    arp_lookup_bsd(dev,ip,tmp);
	  }
	  else
	  {
	    arp_lookup_linux(dev,ip,tmp);
	  }

	  mac = tmp;

	OUTPUT:
	mac
