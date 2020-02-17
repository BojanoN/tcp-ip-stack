#include <util.h>


void print_packet(char* packet, int size){
  for(int i=0; i<size; i++){
    printf("%02x ", (unsigned char)packet[i]);
    if((i+1)%16 == 0){
      printf("\n");
    }
  }
  printf("\n\n");
}

uint32_t htonl(uint32_t hostlong){
  return ((hostlong & 0x000000FF) << 24) |
    ((hostlong & 0x0000FF00) << 8) |
    ((hostlong & 0x00FF0000) >> 8) |
    ((hostlong & 0xFF000000) >> 24);
}

uint16_t htons(uint16_t hostshort){
  return ((hostshort & 0xFF) << 8) |
    ((hostshort & 0xFF00) >> 8);
}

uint32_t ntohl(uint32_t netlong){
  return htonl(netlong);
}

uint16_t ntohs(uint16_t netshort){
  return htons(netshort);
}
