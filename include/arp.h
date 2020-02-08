#pragma once

#include <stdint.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <arpa/inet.h>

#include <util.h>

#define MAX_ETH_HEADER_LEN 1518

#define ETH_PROTO_ARP  0x0806
#define ETH_PROTO_RARP 0x8035
#define ETH_PROTO_IP   0x0800

#define ARP_REQ   0x1
#define ARP_RESP  0x2
#define RARP_REQ  0x3
#define RARP_RESP 0x4

#define MAX_ARP_SIZE 28
#define ETH_FRAME_SIZE 14

#define HW_TYPE_ETHER 0x1

#define MAX_ARP_TABLE_SIZE 64

enum addr_type {
                PHYS = 0,
                MULTICAST = 1
};

struct arp_table_entry{
  uint32_t src_proto_addr;
  uint8_t src_hw_addr[6];
  uint16_t proto_type;
};

struct arp_packet{
  uint16_t hw_type;
  uint16_t proto_type;
  uint8_t  hw_addr_len;
  uint8_t  proto_addr_len;
  uint16_t opcode;
  uint8_t  addr_data[];
}__attribute__((packed));

struct arp_inet{
  uint8_t src_hw_addr[6];
  uint32_t src_proto_addr;
  uint8_t dest_hw_addr[6];
  uint32_t dest_proto_addr;
}__attribute__((packed));

struct eth_frame{
  uint8_t  dest[6];
  uint8_t  src[6];
  uint16_t type;
  uint8_t  data[];
}__attribute__((packed));


// TODO: implementation idea, circular buffer
static struct arp_table_entry  arp_table[MAX_ARP_TABLE_SIZE] = {0};
static int curr_entry_no = 0;


static inline struct arp_table_entry* find_arp_entry(struct arp_packet* packet);

static inline void add_arp_entry(struct arp_packet* packet);

void arp_init();

void arp_response(struct arp_packet* packet, uint8_t* src, uint8_t* dest);

int handle_arp_packet(struct eth_frame* frame);

int handle_eth_frame(struct eth_frame* frame);
