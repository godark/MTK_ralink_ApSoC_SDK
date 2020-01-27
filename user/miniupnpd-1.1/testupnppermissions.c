/* $Id: //WIFI_SOC/release/SDK_4_1_0_0/source/user/miniupnpd-1.1/testupnppermissions.c#1 $ */
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "upnppermissions.h"

void
print_upnpperm(const struct upnpperm * p)
{
	switch(p->type)
	{
	case UPNPPERM_ALLOW:
		printf("allow ");
		break;
	case UPNPPERM_DENY:
		printf("deny ");
		break;
	default:
		printf("error ! ");
	}
	printf("%hu-%hu ", p->eport_min, p->eport_max);
	printf("%s/", inet_ntoa(p->address));
	printf("%s ", inet_ntoa(p->mask));
	printf("%hu-%hu", p->iport_min, p->iport_max);
	putchar('\n');
}

int main(int argc, char * * argv)
{
	int i, r;
	struct upnpperm p;
	openlog("testupnppermissions", LOG_PERROR, LOG_USER);
	for(i=0; i<argc; i++)
		printf("%2d '%s'\n", i, argv[i]);
	memset(&p, 0, sizeof(struct upnpperm));
	r = read_permission_line(&p, argv[1]);
	printf("r=%d\n", r);
	if(r==0)
	{
		print_upnpperm(&p);
	}
	return 0;
}

