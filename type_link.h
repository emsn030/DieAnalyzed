#ifndef TYPE_LINK_H
#define TYPE_LINK_H

void type_link(const unsigned int type_link);

struct Sll2_header
{
    unsigned short protocol;
    unsigned short reserved;
    unsigned int indice_interfaz;
    unsigned short tipo_arp;
    unsigned char type_packet;
    unsigned char length_direction;
    unsigned char direccion[8];
};

void header_sll2(struct Sll2_header *sll2);

void protocolo(const unsigned short *protocolo);

void type_packet(const unsigned char *type);

void type_arp(const unsigned short *arp);

#endif 
