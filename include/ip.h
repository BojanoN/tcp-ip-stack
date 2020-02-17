#pragma once

#include <stdint.h>
#include <arp.h>

#define IP_MAX_LEN 65535
#define MAX_PACKETS 24

#define PROTO_ICMP 0x01
#define PROTO_TCP 0x06
#define PROTO_UDP 0x11

#define IP_OFFSET_MASK 0x1fff

struct ip_hdr{
  unsigned int ihl : 4;
  unsigned int version : 4;
  uint16_t tos;
  uint16_t len;
  uint16_t id;
  uint16_t frag_offset;
  uint8_t ttl;
  uint8_t proto;
  uint16_t checksum;
  uint32_t src;
  uint32_t dest;
  uint32_t opt;
  uint8_t  data[];
};

static uint8_t* pkt_buffer[MAX_PACKETS] = {0};
static uint32_t pkt_buffer_id[MAX_PACKETS] = {0};

int ip_init();
void ip_exit();
uint16_t ip_checksum(void* hdr, uint8_t len);
void handle_ip_packet(struct eth_frame* hdr);
void ip_send(uint32_t dest, uint32_t opt, uint8_t* data, uint32_t len);
