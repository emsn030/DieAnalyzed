#ifndef LOGGER_H
#define LOGGER_H 

unsigned char crear_archivo_logs(const char *nombre);
void crear_carpetas(const char *ruta);
void cerrar_archivo_log(void);
void write_log(const unsigned char nivel, const char *mensaje);

#endif 
