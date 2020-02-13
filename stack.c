#include <signal.h>
#include <stdlib.h>

#include <tun_if.h>
#include <arp.h>
#include <util.h>
#include <ip.h>

#define MTU 1500

struct inet_if iface;


void handler(int sig){
  if_free(&iface);
  ip_exit();

  exit(0);
}


int main(void){

  char packet_buff[MTU] = {0};

  int recvd;

  int err = if_init(&iface, "00:00:DE:AD:BE:EF", "10.0.0.2");
  if(err){
    fprintf(stderr, "Failed to initialize TUN device\n");
    return -1;
  }

  signal(SIGINT, handler);

  LOG_DEBUG("DEV_FD: %d\n", iface.dev_fd);
  LOG_DEBUG("DEV_NAME: %s\n", iface.name);

  arp_init(&iface);
  ip_init(&iface);

  while(1){
    recvd = read(iface.dev_fd, packet_buff, MTU);
    LOG_DEBUG("BYTES_READ: %d\n", recvd);
    print_packet(packet_buff, recvd);
    handle_eth_frame((struct eth_frame *) packet_buff);
  }

  if_free(&iface);

  return 0;
}


