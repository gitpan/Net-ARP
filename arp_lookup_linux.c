/*
Perl ARP Extension
Lookup the MAC address of an ip address
Linux code

Programmed by Bastian Ballmann
Last update: 16.01.2004

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

/*
 * Old part, by Bastian Ballmann
 *

#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

int arp_lookup_linux(u_char *dev, u_char *ip, char *mac)
{
  struct sockaddr_in sin;
  struct arpreq arp;
  int sockfd;
  unsigned char *hw_addr;

  strcpy(mac,"unknown");

  sin.sin_family = AF_INET;
  inet_aton(ip, &sin.sin_addr);

  memcpy(&arp.arp_pa, &sin, sizeof arp.arp_pa);
  strcpy(arp.arp_dev, dev);

  // Open a socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
      perror("socket");
      exit(1);
    }

  // Get the interface hardware address
  if (ioctl(sockfd, SIOCGARP, &arp))
    {
      hw_addr = (unsigned char *) arp.arp_ha.sa_data;
      sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x", 
	      hw_addr[0], hw_addr[1], hw_addr[2], 
	      hw_addr[3], hw_addr[4], hw_addr[5]);
    }

  close(sockfd);
}
*/

// ##########################################################

/*
 * New part, by Alexander Müller
 */


#include <stdio.h>

#define _PATH_PROCNET_ARP "/proc/net/arp"

int arp_lookup_linux(char *dev, char *ip, char *mac)
{
	FILE *fp;
	char ipaddr[100];
	char line[200];
	char hwa[100];
	char mask[100];
	char device[100];
	int num, type, flags;

	if ((fp = fopen(_PATH_PROCNET_ARP, "r")) == NULL) {
		perror(_PATH_PROCNET_ARP);
		return (-1);
	}

	/* Bypass header -- read until newline */
	if (fgets(line, sizeof(line), fp) != (char *) NULL)
	{
		/* Read the ARP cache entries. */
		while (fgets(line, sizeof(line), fp))
		{
			num = sscanf(line, "%s 0x%x 0x%x %100s %100s %100s\n", ipaddr, &type, &flags, hwa, mask, device);

			if (num < 4)
				break;
			if (strcmp(dev, device) == 0 && strcmp(ip, ipaddr) == 0)
			{
				strcpy(mac, hwa);
				break;
			}
			strcpy(mac, "unknown");
		}
	}
}
