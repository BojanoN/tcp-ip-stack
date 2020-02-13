#pragma once

#include <stdint.h>
#include <arp.h>

#define IP_MAX_LEN 65535
#define MAX_PACKETS 24

#define PROTO_ICMP 0x01
#define PROTO_TCP 0x06
#define PROTO_UDP 0x11

struct ip_hdr{
  unsigned int version : 4;
  unsigned int ihl : 4;
  unsigned int tos : 5;
  unsigned int ecn : 3;
  uint16_t len;
  uint16_t id;
  unsigned int flags : 3;
  unsigned int frag_offset : 13;
  uint8_t ttl;
  uint8_t proto;
  uint16_t checksum;
  uint32_t src;
  uint32_t dest;
  uint32_t opt;
  uint8_t  data[];
}__attribute__((packed));

static uint8_t* pkt_buffer[MAX_PACKETS] = {0};
static uint32_t pkt_buffer_id[MAX_PACKETS] = {0};

int ip_init();
void ip_exit();
uint16_t ip_checksum(struct ip_hdr* hdr);
void handle_ip_packet(struct eth_frame* hdr);
void ip_send(uint32_t dest, uint32_t opt, uint8_t* data, uint32_t len);
