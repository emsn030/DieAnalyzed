#include <stdio.h>
#include "print_packets.h"
#include "type_link.h"
#include "pcap_reader.h"
#include "logger.h"

unsigned char print_header(struct Header_global *header) 
{
    if (header == NULL) 
    {
        fprintf(stderr, "\033[0;31mFallo crítico al intentar leer el header\033[0m\n");
        write_log(2, "Error en una función al recibir la información del pcap");

        return 1u;
    }

    if (header->magic_number != 0xA1B2C3D4 && header->magic_number != 0xD4C3B2A1) 
    {
        fprintf(stderr, "\033[0;31mEl archivo no es un pcap\033[0m\n");
        write_log(2, "El fichero no es un pcap");

        return 1;
    }

    fprintf(stdout, "\033[0;32m%-16s0x%X %s\033[0m\n", "Magic number: ", header->magic_number, (header->magic_number != 0xA1B2C3D4) ? "(PCAP big-endian)" : "(PCAP little-endian)");
    fprintf(stdout, "\033[0;32m%-17s%hu.%hu\033[0m\n", "Versión: ", header->mayor_version, header->minor_version);
    fprintf(stdout, "\033[0;32m%-16s\033[0m", "Tipo de enlace: "); 
    type_link(header->type_link);

    return 0u;
}

void print_packet(struct Record_packet *paquete) 
{
    printf("\n");

    if (paquete == NULL) 
    {
        fprintf(stderr, "\033[0;31mEl paquete parece estar currupto\033[0m\n");
        write_log(2, "Error de una función al recibir el header del paquete");

        return;
    }

    static unsigned int num_paquete = 0u;

    fprintf(stdout, "\033[0;32m%-23s%u\033[0m\n", "Número de paquete: ", num_paquete);
    fprintf(stdout, "\033[0;32m%-22s%u.%u\033[0m\n", "Timestamp: ", paquete->timestamp_second, paquete->timestamp_micro_second);
    fprintf(stdout, "\033[0;32m%-22s%u\033[0m\n", "Longitud capturada: ", paquete->caplength);
    fprintf(stdout, "\033[0;32m%-22s%u\033[0m\n", "Longitud original: ", paquete->origin_length);

    num_paquete++;

    return;
}

