#include <arp.h>
#include <tun_if.h>

extern struct inet_if* device;

static inline struct arp_table_entry* find_arp_entry(struct arp_packet* packet){
  struct arp_inet* data = (struct arp_inet*)packet->addr_data;

  for(int i=0; i<MAX_ARP_TABLE_SIZE; i++){
    if(packet->hw_type == arp_table[i].proto_type && data->src_proto_addr == arp_table[i].src_proto_addr){
      return &arp_table[i];
    }
  }
  return NULL;
}

static inline void add_arp_entry(struct arp_packet* packet){
  struct arp_inet* data = (struct arp_inet*)packet->addr_data;

  arp_table[curr_entry_no].proto_type = ntohs(packet->hw_type);
  arp_table[curr_entry_no].src_proto_addr = ntohl(data->src_proto_addr);
  memcpy(arp_table[curr_entry_no].src_hw_addr, data->src_hw_addr, 6);

  curr_entry_no = ((curr_entry_no + 1) >= MAX_ARP_TABLE_SIZE) ? 0 : (curr_entry_no + 1);

  return;
}

void arp_init(){
  return;
}

void send_arp_reply(struct arp_packet* packet, uint8_t* src, uint8_t* dest){
  char pkt_buf[MAX_ARP_SIZE + ETH_FRAME_SIZE];

  struct eth_frame* frame = (struct eth_frame*) pkt_buf;
  memcpy(frame->data, packet, MAX_ARP_SIZE);
  memcpy(frame->src, src, 6);
  memcpy(frame->dest, dest, 6);
  frame->type = htons(ETH_PROTO_ARP);
  
  write(device->dev_fd, pkt_buf, MAX_ARP_SIZE + ETH_FRAME_SIZE);
  #ifdef DEBUG
  printf("SENT:\n");
  print_packet(pkt_buf, MAX_ARP_SIZE + ETH_FRAME_SIZE);
  #endif
}

int handle_arp_packet(struct eth_frame* frame){
  struct arp_table_entry* entry;
  struct arp_packet* packet = (struct arp_packet*)(frame->data);
  struct arp_inet* data = (struct arp_inet*)(packet->addr_data);

  if(ntohs(packet->hw_type) == HW_TYPE_ETHER){
    char merge = 0;
    entry = find_arp_entry(packet);

    if(entry != NULL){
      merge = 1;
      entry->src_proto_addr = ntohl(data->src_proto_addr);
    }
    if(data->dest_proto_addr == device->ip_addr){
      if(!merge){
        add_arp_entry(packet);
      }
      if(ntohs(packet->opcode) == ARP_REQ){
        memcpy(data->dest_hw_addr, data->src_hw_addr, 6);
        data->dest_proto_addr = data->src_proto_addr;

        memcpy(data->src_hw_addr,  device->hw_addr, 6);
        data->src_proto_addr = device->ip_addr;
        packet->opcode = htons(ARP_RESP);
        send_arp_reply(packet, device->hw_addr, frame->src);
      }
    }
  }

  return 0;
}

int handle_eth_frame(struct eth_frame* frame){
  //enum addr_type addr = frame->dest[0] & 1;
  int ret = -3;
  switch(ntohs(frame->type)){
  case(ETH_PROTO_ARP):
    ret = handle_arp_packet(frame);
    break;
  case(ETH_PROTO_RARP):
    break;
  case(ETH_PROTO_IP):
    break;
  default:
    // TODO logging za neimplementirani protokol
    break;
  }


  return ret;
}
