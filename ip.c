#include <malloc.h>
#include <arpa/inet.h>
#include <assert.h>

#include <ip.h>
#include <icmp.h>
#include <util.h>

extern struct inet_if* device;

int ip_init(){

  for(int i=0; i<MAX_PACKETS; i++){
    uint8_t* buf = (uint8_t *)malloc(sizeof(uint8_t) * IP_MAX_LEN);

    if(buf == NULL){
      printf("IP memory allocation error!\n");
      i-=1;
      while(i >= 0){
        free(pkt_buffer[i--]);
      }
      return -1;
    }

    pkt_buffer[i] = buf;
  }
  return 0;
}

void ip_exit(){
  LOG_DEBUG("Deallocating IP layer..");
  device = NULL;
  for(int i=0; i<MAX_PACKETS; i++){
    free(pkt_buffer[i]);
  }
  LOG_DEBUG("Deallocated IP layer");
}

/*
 * Compute IPv4 header checksum.
 */
uint16_t ip_checksum(void* hdr, uint8_t len){
  uint32_t sum = 0;
  uint16_t* arr = hdr;

  while(len > 1){
    sum += *arr++;
    len -= sizeof(uint16_t);
  }

  if(len > 0){
    sum += *((uint8_t *)arr);
  }

  sum = (sum>>16) + (sum & 0xffff);
  sum += (sum>>16);

  return (~sum);
}

void handle_ip_packet(struct eth_frame* eth_hdr){

  struct ip_hdr* hdr = (struct ip_hdr*) &eth_hdr->data;

  uint16_t checksum = ntohs(hdr->checksum);
  uint32_t id = ntohl(hdr->id);

  LOG_DEBUG("Packet checksum: %u", checksum);

  uint16_t calc_sum = ip_checksum(hdr, hdr->ihl << 2);

  LOG_DEBUG("Calculated checksum: %u", calc_sum);

  if(calc_sum != 0){
    printf("Invalid packet checksum, packet with ID: %u is being dropped...\n", id);
    return;
  }

  // TODO: fragmentation

  switch(hdr->proto){
  case(PROTO_ICMP):
    handle_icmp_packet(hdr);
    break;
  case(PROTO_TCP):
    break;
  case(PROTO_UDP):
    break;
  default:
    break;
  }

  return;
}
