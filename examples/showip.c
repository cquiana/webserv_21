#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int ac, char **av)
{
	struct addrinfo hints, *res, *p;

	int status;
	char ipstr[INET6_ADDRSTRLEN];
	if (ac != 2)
	{
		fprintf(stderr, "usage: showip hostname\n");
		return 1;
	}
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((status = getaddrinfo(av[1], NULL, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}

	printf("IP address for %s:\n", av[1]);
	for(p = res; p != NULL; p = p->ai_next)
	{
		void *addr;
		char *ipver;
		//получаем указатель на адрес в разных протоколах
		if (p->ai_family == AF_INET)
		{
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		}
		else
		{
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
		// преобразуем  IP и выводим его
		inet_ntop(p->ai_family, addr, ipstr, INET6_ADDRSTRLEN);
		printf(" %s: %s\n", ipver, ipstr);
	}
	freeaddrinfo(res);

	return 0;
}
