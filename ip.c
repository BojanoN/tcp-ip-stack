#include <malloc.h>
#include <arpa/inet.h>

#include <ip.h>
#include <icmp.h>


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
  device = NULL;
  for(int i=0; i<MAX_PACKETS; i++){
    free(pkt_buffer[i]);
  }
}

/*
 * Compute IPv4 header checksum.
 * Zeroes checksum field.
 */
uint16_t ip_checksum(struct ip_hdr* hdr){
  uint32_t sum = 0;
  uint32_t len = hdr->ihl << 1;
  uint16_t* arr = (uint16_t *)hdr;

  hdr->checksum = 0;

  while(len > 0){
    sum += *arr++;
  }

  sum = (sum>>16) + (sum & 0xffff);
  sum += (sum>>16);

  return (~sum);
}

void handle_ip_packet(struct ip_hdr* hdr){

  uint16_t checksum = ntohs(hdr->checksum);
  uint32_t id = ntohl(hdr->id);

#ifdef DEBUG
  printf("Packet checksum: %d\n",ip_checksum(hdr));
#endif

  if(checksum != ip_checksum(hdr)){
    printf("Invalid packet checksum, packet with ID: %d is being dropped...\n", id);
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
