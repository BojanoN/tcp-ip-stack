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
