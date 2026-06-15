#include <stdio.h>
#include "pcap_reader.h"
#include "print_packets.h"
#include "payload_parser.h"
#include "type_link.h"
#include "logger.h"

int main(int argc, char **argv) 
{
    printf("\n");

    if (argc != 2) 
    {
        fprintf(stderr, "\033[0;31mEl programa necesita un argumento\033[0m\n");
        
        goto end;
    }

    unsigned char error = crear_archivo_logs(*argv);

    if (error) goto end;

    const char *argumento = *(argv + 1);

    struct Data_file *fichero = pcap_abrir(argumento);

    if (fichero == NULL) 
    {
        fprintf(stderr, "\033[0;31mFallo crítico\033[0m\n");
        write_log(2, "No se puede leer el pcap");

        return 1;
    }

    if (fichero->fichero == NULL) 
    {
        fprintf(stderr, "\033[0;31mFallo crítico\033[0m\n");
        write_log(1, "No se pueden leer los datos del pcap");
        
        return 1;
    }

    struct Header_global *header = (struct Header_global *)fichero->fichero;

    unsigned char fail = print_header(header);

    if (fail) goto end;
    
    unsigned char *cursor = fichero->fichero + sizeof(struct Header_global);
    unsigned char *limit = (void *)(fichero->fichero + fichero->tamanyo);

    while (cursor < limit) 
    {
        struct Record_packet *paquete = (struct Record_packet *)cursor;
        
        const unsigned int caplength = paquete->caplength;
        
        print_packet(paquete);
        
        cursor += sizeof(struct Record_packet);
        
        struct Sll2_header *sll2 = (struct Sll2_header *)cursor;
        
        header_sll2(sll2);
        
        cursor += caplength;

        printf("\n");
    }
    
    cerrar_archivo_log();

    end: 
    return 0;
}

