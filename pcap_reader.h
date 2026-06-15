#ifndef PCAP_READER_H
#define PCAP_READER_H 

struct Header_global 
{
    unsigned int magic_number;
    unsigned short mayor_version;
    unsigned short minor_version;
    unsigned int time_zone;
    unsigned int timestamp;
    unsigned int max_length;
    unsigned int type_link;
};

struct Record_packet 
{
    unsigned int timestamp_second; 
    unsigned int timestamp_micro_second;
    unsigned int caplength;
    unsigned int origin_length;
};

struct Data_file
{
    unsigned char *fichero;
    unsigned long long tamanyo;
};

struct Data_file *pcap_abrir(const char *ruta);

#endif 
