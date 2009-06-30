/**
 * \file udpd.c
 *
 * \date 08.05.09
 * \author sikmir
 */

#include "misc.h"
#include "udpd.h"
#include "net.h"
#include "in.h"
#include "shell.h"
#include "socket.h"

#define COMMAND_NAME "udpd"
static char available_keys[] = {
    #include "udpd_keys.inc"
};

static void show_help() {
	printf(
    #include "udpd_help.inc"
	);
}

int udpd_shell_handler(int argsc, char **argsv) {
	SHELL_KEY keys[MAX_SHELL_KEYS];
	char *key_value;
	int keys_amount;
	unsigned char addr[4];
	unsigned long ip_addr;
	int port;

	keys_amount = parse_arg(COMMAND_NAME, argsc, argsv, available_keys,
			sizeof(available_keys), keys);

	if (!get_key('a', keys, keys_amount, &key_value)) {
		ip_addr = INADDR_ANY;
	} else if (NULL == ipaddr_scan(key_value, addr)) {
	        LOG_ERROR("wrong ip addr format (%s)\n", key_value);
	        show_help();
	        return -1;
	} else {
		ip_addr = inet_addr(addr);
	}
        if (!get_key('p', keys, keys_amount, &key_value)) {
                port = 666;
        } else if (1 != sscanf(key_value, "%d", &port)) {
                LOG_ERROR("enter validly port '-p'\n");
                show_help();
                return -1;
        }
	if (get_key('h', keys, keys_amount, &key_value)) {
		show_help();
		return 0;
	}
	int sock, length, n;
	struct sockaddr_in server;
	char buf[64];
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0) {
		LOG_ERROR("opening socket\n");
		return -1;
	}
	length = sizeof(server);
	memset(&server, 0, length);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=ip_addr;
	server.sin_port=htons(port);

	if(bind(sock, (struct sockaddr *)&server, length)) {
		LOG_ERROR("binding socket\n");
		return -1;
	}
	while (1) {
		n = recv(sock, buf, sizeof(buf), 0);
		if(n < 0) {
			LOG_ERROR("can't recv\n");
			continue;
		}
		printf("buf=%s\n", buf);
		n = send(sock, buf, sizeof(buf), 0);
		if(n < 0) {
			LOG_ERROR("can't send\n");
		}
		sleep(1);
	}
	close(sock);
	return 0;
}
