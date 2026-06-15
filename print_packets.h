#include "pcap_reader.h"

#ifndef PRINT_PACKETS_H 
#define PRINT_PACKETS_H

unsigned char print_header(struct Header_global *header);

void print_packet(struct Record_packet *paquete);

#endif 
