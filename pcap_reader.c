#include <stdio.h>
#include <stdlib.h>
#include "pcap_reader.h"
#include "logger.h"

struct Data_file *pcap_abrir(const char *ruta) 
{
    struct Data_file *Pcap = malloc(sizeof(struct Data_file));

    if (ruta == NULL) 
    {
        fprintf(stderr, "\033[0;31mError crítico\033[0m\n");
        write_log(2, "Error al reservar memoria para leer el pcap");

        Pcap->fichero = NULL;
        Pcap->tamanyo = 0u;

        goto end;
    }

    FILE *pcap = fopen(ruta, "rb");

    if (pcap == NULL) 
    {
        fprintf(stderr, "\033[0;31mEl fichero no se encuentra en el sistema\033[0m\n");
        write_log(2, "Error al intentar abrir el pcap, no se encuentra en el sistema");

        Pcap->fichero = NULL;
        Pcap->tamanyo = 0u;

        goto end;
    }

    fseek(pcap, 0, SEEK_END);

    const long long tamanyo = ftell(pcap);

    if (tamanyo <= 1) 
    {
        fprintf(stderr, "\033[0;31mEl fichero se encuentra vacío, no es un fichero válido para analizar\033[0m\n");
        write_log(1, "El fichero pcap se encuentra vacío, no es válido para analizar");
        fclose(pcap);
        
        Pcap->fichero = NULL;
        Pcap->tamanyo = 0u;

        goto end;
    }

    fseek(pcap, 0, SEEK_SET);

    unsigned char *contenido_pcap = malloc(tamanyo);

    if (contenido_pcap == NULL) 
    {
        fprintf(stderr, "\033[0;31mError crítico durante la ejecución\033[0m\n");
        write_log(2, "Error al asignar memoria para la variable que lee el contenido del pcap");
        fclose(pcap);

        Pcap->fichero = NULL;
        Pcap->tamanyo = 0u;

        goto end;
    }

    fread(contenido_pcap, 1, tamanyo, pcap);

    fclose(pcap);

    Pcap->fichero = contenido_pcap;
    Pcap->tamanyo = (unsigned long long)tamanyo;

    end:
    return Pcap;
}
