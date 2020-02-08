#pragma once

#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAC_ADDR_SIZE 6

struct inet_if{
  int dev_fd;
  char name[IFNAMSIZ];
  unsigned char hw_addr[6];
  uint32_t ip_addr;
  uint16_t mtu;
};

static int tun_alloc(char* dev);
static int tun_free(int dev_fd);
int if_init(struct inet_if* iface, char* hw_addr, char* ip_addr);
void if_free(struct inet_if* iface);
