#pragma once
#include <stdio.h>
#include <stdint.h>

void print_packet(char* packet, int size);
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
