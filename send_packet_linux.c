/*
Perl ARP Extension
Send the packet
Linux code

Programmed by Bastian Ballmann
Last update: 01.12.2004

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

#include <sys/types.h>
#include <sys/socket.h>      
#include "arp.h"

int send_packet_linux(u_char *dev, u_char *packet, u_int packetsize)
{
  struct sockaddr addr;
  int sock;

  if( (strlen(dev) == 0) ||
      (packetsize == 0) )
    return -1;

  // Create socket descriptor
  if( ( sock = socket(AF_INET,SOCK_TYPE,htons(ETH_P_ALL))) < 0 ) 
    { 
      perror("socket"); 
      exit(1); 
    }

  // Set dev and send the packet
  strncpy(addr.sa_data,dev,sizeof(addr.sa_data));
  if( (sendto(sock,packet,packetsize,0,&addr,sizeof(struct sockaddr))) < 0 )
    {
      perror("send");
      exit(1);
    }

  close(sock);
  return 0;
}
