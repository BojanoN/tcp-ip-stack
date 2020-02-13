#pragma once
#include <stdio.h>
#include <stdint.h>

#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#define LOG_DEBUG(fmt, ...) do { if(DEBUG) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__ , ##__VA_ARGS__); } while(0);



void print_packet(char* packet, int size);
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
