#include <stdlib.h>
#include <stdio.h>
#include <net/if.h>
#include "type_link.h"
#include "logger.h"

void type_link(const unsigned int type_link) 
{
    switch (type_link) 
    {
        case 1u: 

            fprintf(stdout, "\033[0;32m(Ethernet Cable)\033[0m\n");

            break;

        case 105u: 

            fprintf(stdout, "\033[0;32m(Wireless Raw)\033[0m\n");

            break;

        case 127u: 

            fprintf(stdout, "\033[0;32m(Wireless RadioTap)\033[0m\n");

            break;

        case 113u: 

            fprintf(stdout, "\033[0;32m(Linux Virtual - Cooked v1)\033[0m\n");

            break;

        case 276u: 

            fprintf(stdout, "\033[0;32m(Linux Virtual - Cooked v2)\033[0m\n");

            break;

        case 108u: 

            fprintf(stdout, "\033[0;32m(Loopback Localhost)\033[0m\n");

            break;

        case 101u: 

            fprintf(stdout, "\033[0;32m(Raw IP - VPN/Celular)\033[0m\n");

            break;

        default:
    
            fprintf(stdout, "\033[0;32m(Protocolo Exótico)\033[0m\n");

            break;
    }

    return;
}

void protocolo(const unsigned short *protocolo) 
{
    if (protocolo == NULL) 
    {
        fprintf(stderr, "\033[0;31mError crítico\n");
        write_log(2, "Error al recibir el protocolo en una función de análisis de protocolos");
        
        return;
    }

    const unsigned char *little_endian = (void *)protocolo;
    little_endian++;

    const unsigned short protocolo_corregido = (unsigned short)(*(little_endian -1u) << 8u) | (unsigned short)(*little_endian); 

    if (protocolo_corregido == 0x0800u) fprintf(stdout, "\033[0;32mIPv4\033[0m\n");
    else if (protocolo_corregido == 0x0806u) fprintf(stdout, "\033[0;32mARP\033[0m\n");
    else if (protocolo_corregido == 0x86DDu) fprintf(stdout, "\033[0;32mIPv6\033[0m\n");
    else if (protocolo_corregido == 0x8100u) fprintf(stdout, "\033[0;32mVLAN tag 802.1Q\033[0m\n");
    else if (protocolo_corregido == 0x0842) fprintf(stdout, "\033[0;32mWake-on-LAN\033[0m\n");
    else fprintf(stdout, "\033[0;32mProtocolo exótico\033[0m\n");

    return;
}

void type_packet(const unsigned char *type) 
{
    if (type == NULL) 
    {
        fprintf(stderr, "\033[0;31mError crítico\n");
        write_log(2, "Error al recibir el tipo de paquete en una función de análisis de tipos");

        return;
    }

    if (*type == 0u) fprintf(stdout, "\033[0;32mUnicast dirigido al host\033[0m\n");
    else if (*type == 1u) fprintf(stdout, "\033[0;32mBroadcast\033[0m\n");
    else if (*type == 2u) fprintf(stdout, "\033[0;32mMulticast\033[0m\n");
    else if (*type == 3u) fprintf(stdout, "\033[0;32mRecibido de otro host (Promiscuo)\033[0m\n");
    else if (*type == 4u) fprintf(stdout, "\033[0;32mEnviado por este host\033[0m\n");
    else 
    {
        fprintf(stdout, "\033[0;32mTipo de paquete exótico o desconocido\033[0m\n");
        write_log(1, "El tipo de paquete es desconocido");
    }
    return;
}

void type_arp(const unsigned short *arp) 
{
    if (arp == NULL) 
    {
        fprintf(stderr, "\033[0;31mError crítico\033[0m\n");
        write_log(2, "Error al recibir un dato en una función");

        return;
    }

    const unsigned char *little_endian = (unsigned char *)arp;

    const unsigned short arp_le = (unsigned short)(*little_endian << 8u) | (unsigned short)(*(little_endian + 1u)); 

    if (arp_le == 1u) fprintf(stdout, "\033[0;32mARPHRD_ETHER\033[0m\n");
    else if (arp_le == 772u) fprintf(stdout, "\033[0;32mARPHRD_LOOPBACK\033[0m\n");
    else if (arp_le == 778u) fprintf(stdout, "\033[0;32mARPHRD_TUNNEL\033[0m\n");
    else if (arp_le == 803u) fprintf(stdout, "\033[0;32mARPHRD_IEEE80211\033[0m\n");
    else if (arp_le == 824u) fprintf(stdout, "\033[0;32mARPHRD_IEEE80211_RADIOTAP\033[0m\n");
    else if (arp_le == 65534u) fprintf(stdout, "\033[0;32mARPHRD_NONE\033[0m\n");
    else if (arp_le == 65535u) fprintf(stdout, "\033[0;32mARPHRD_VOID\033[0m\n");
    else 
    {
        fprintf(stdout, "\033[0;32mDesconocido\033[0m\n");
        write_log(1, "El tipo ARP es desconocido");
    }

    return;
}

void header_sll2(struct Sll2_header *sll2) 
{
    printf("\n");

    if (sll2 == NULL) 
    {
        fprintf(stderr, "\033[0;31mError al leer la información de la cabecera del payload... puede estar corrupta\033[0m\n");
        write_log(2, "Error en una función al recibir la información de la cabecera del payload");
        return;
    }

    char nombre_if[IF_NAMESIZE];

    fprintf(stdout, "\033[0;32m%-28s\033[0m", "Protocolo: ");
    protocolo(&sll2->protocol);
    fprintf(stdout, "\033[0;32m%-28s\033[0m", "Tipo de paquete: ");
    type_packet(&sll2->type_packet);
    fprintf(stdout, "\033[0;32m%-28s\033[0m", "Nombre de interfaz: ");
    fprintf(stdout, "\033[0;32m%s\033[0m\n", (if_indextoname(sll2->indice_interfaz, nombre_if) == NULL) ? "Interfaz desconocida" : if_indextoname(sll2->indice_interfaz, nombre_if));
    fprintf(stdout, "\033[0;32m%-28s\033[0m", "Tipo de ARP: ");
    type_arp(&sll2->tipo_arp);
    fprintf(stdout, "\033[0;32m%-28s%hhu\033[0m\n", "Longitud en bytes de ADDR: ", sll2->length_direction);
    fprintf(stdout, "\033[0;32m%-29s\033[0m", "Dirección MAC: ");

    unsigned char *mac = (unsigned char *)&sll2->direccion;
    unsigned char *mac_limit = mac + sll2->length_direction;

    while (mac < mac_limit)
    {
        if (mac == mac_limit - 1u)
        {
            fprintf(stdout, "\033[0;32m%02X\033[0m\n", *mac);

            break;
        }

        fprintf(stdout, "\033[0;32m%02X:\033[0m", *mac);

        mac++;

    }
    
    write_log(0, "Paquete analizado sin problema aparente");

    return;
}
