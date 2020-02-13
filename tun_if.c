#include <tun_if.h>
#include <util.h>

struct inet_if* device = NULL;

static int tun_alloc(char* dev){

  int fd, err;
  struct ifreq ifr;

  if( (fd = open("/dev/net/tap", O_RDWR)) < 0){
    perror("Error opening device");
    return fd;
  }

  memset(&ifr, 0, sizeof(dev));

  ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
  if( *dev )
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

  if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0){
    perror("IOCTL error");
    close(fd);
    return err;
  }

  strcpy(dev, ifr.ifr_name);

  return fd;
}

static int tun_free(int dev_fd){

  int ret = close(dev_fd);
  return ret;
}

int if_init(struct inet_if* iface, char* hw_addr, char* ip_addr){

  LOG_DEBUG("Initializing TAP device...");

  char* p = strdup(hw_addr);
  char* tok;
  char cmd_buf[64];

  int dev_fd = tun_alloc(iface->name);

  if(dev_fd < 0){
    return -1;
  }

  sprintf(cmd_buf, "ip link set dev %s up", iface->name);
  system(cmd_buf);

  sprintf(cmd_buf, "ip route add dev %s %s" , iface->name, ip_addr);
  system(cmd_buf);

  iface->dev_fd = dev_fd;
  inet_pton(AF_INET, ip_addr, &iface->ip_addr);

  int i=0;
  while((tok = strsep(&p, ":")) != NULL){
    unsigned char num = (unsigned char)strtol(tok, NULL, 16);
    iface->hw_addr[i++] = num;
  }
  iface->mtu = 1500;
  device = iface;

  LOG_DEBUG("Initialized TAP device!");

  return 0;
}

void if_free(struct inet_if* iface){
  LOG_DEBUG("Deallocating TAP device...");

  tun_free(iface->dev_fd);
  memset(&iface->name, 0, IFNAMSIZ);
  memset(&iface->hw_addr, 0, MAC_ADDR_SIZE);

  LOG_DEBUG("Deallocated TAP device.");
}
