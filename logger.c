#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static FILE *archivo_log = NULL;

void write_log(const unsigned char nivel, const char *mensaje) 
{
    if (archivo_log == NULL) 
    {
        fprintf(stderr, "\033[0;31mNo se pueden escribir logs\033[0m\n");

        return;
    }

    if (nivel == 0u) fprintf(archivo_log, "\033[0;32m[%s] %s\033[0m\n", "Normal", mensaje);    
    else if (nivel == 1) fprintf(archivo_log, "\033[0;33m[%s] %s\033[0m\n", "Warning", mensaje);  
    else if (nivel == 2) fprintf(archivo_log, "\033[0;31m[%s] %s\033[0m\n", "Critical", mensaje);
    else fprintf(stderr, "\033[0;31mEl nivel de log no es correcto\033[0m\n");

    return;
}

void crear_carpetas(const char *ruta) 
{
    if (ruta == NULL) 
    {
        fprintf(stderr, "\033[0;31mHa ocurrido un error crítico\033[0m\n");

        return;
    }

    const char *inicio_ruta = ruta;
    unsigned short contador = 0u;

    while (*ruta != '\0') 
    {
        contador++;
        ruta++;
    }

    ruta = inicio_ruta;
    
    char *carpetas = malloc(contador+1u);

    if (carpetas == NULL) 
    {
        fprintf(stderr, "\033[0;31mHa ocurrido un error crítico\033[0m\n");

        return;
    }

    char *posicion_actual_carpetas = carpetas;
    char *inicio_carpetas = carpetas;

    while (*ruta != '\0') 
    {
        *carpetas = *ruta;

        if (ruta == inicio_ruta) 
        {
            ruta++;
            carpetas++;
            continue;
        }

        if (*ruta == '/') 
        {
            *carpetas = '\0';
            posicion_actual_carpetas = carpetas;
            carpetas = inicio_carpetas;

            mkdir(carpetas, 0755);

            carpetas = posicion_actual_carpetas;
            *carpetas = '/';
        }

        carpetas++;
        ruta++;
    }

    return;
}

unsigned char crear_archivo_logs(const char *nombre) 
{
    if (nombre == NULL) 
    {
        fprintf(stderr, "\033[0;31mHa ocurrido un error crítico\033[0m\n");

        return 1;
    }

    const char *avanzar_nombre = nombre;
    const char *sacar_nombre = nombre;

    while (*avanzar_nombre != '\0') 
    {
        if (*avanzar_nombre == '/') sacar_nombre = avanzar_nombre;
        avanzar_nombre++;
    }

    if (*sacar_nombre == '/') sacar_nombre++;

    const char *home = getenv("HOME");

    if (home == NULL) 
    {
        fprintf(stderr, "\033[0;31mError crítico, no se enuentran variables de entorno\033[0m\n");

        return 1;
    }

    const char *home_local = "/.local";
    const char *home_local_log = "/log/";
    const char *ptr_home = home;
    const char *ptr_home_local = home_local;
    const char *ptr_home_local_log = home_local_log;
    unsigned short tamanyo_ruta = 0u;

    while (*home != '\0') 
    {
        tamanyo_ruta++;
        home++;
    }
    
    home = ptr_home;

    while (*home_local != '\0') 
    {
        tamanyo_ruta++;
        home_local++;
    }

    home_local = ptr_home_local;

    while (*home_local_log != '\0') 
    {
        tamanyo_ruta++;
        home_local_log++;
    }

    home_local_log = ptr_home_local_log;

    char *ruta = malloc(tamanyo_ruta+1u);

    if (ruta == NULL) 
    {
        fprintf(stderr, "\033[0;31mHa ocurrido un error crítico\033[0m\n");

        return 1;
    }

    char *inicio_ruta = ruta;
    
    while (*home != '\0') 
    {
        *ruta = *home;
        ruta++;
        home++;
    }

    home = ptr_home;

    while (*home_local != '\0') 
    {
        *ruta = *home_local;
        ruta++;
        home_local++;
    }

    home_local = ptr_home_local;
    
    while (*home_local_log != '\0') 
    {
        *ruta = *home_local_log;
        ruta++;
        home_local_log++;
    }

    home_local_log = ptr_home_local_log;
    *ruta = '\0';
    ruta = inicio_ruta;
    
    const char *extension = ".log";
    unsigned short contador = 0u;

    while (*ruta != '\0') 
    {
        contador++;
        ruta++;
    }
    
    ruta = inicio_ruta;
    avanzar_nombre = sacar_nombre;

    while (*avanzar_nombre != '\0') 
    {
        contador += 2u;
        avanzar_nombre++;
    }

    const char *inicio_extension = extension;

    while (*extension != '\0') 
    {
        contador++;
        extension++;
    }

    extension = inicio_extension;
    char *ruta_total = malloc(contador+3u);

    if (ruta_total == NULL) 
    {
        fprintf(stderr, "\033[0;31mHa ocurrido un error crítico\033[0m\n");

        return 1;
    }

    char *inicio_ruta_total = ruta_total;

    while (*ruta != '\0') 
    {
        *ruta_total = *ruta;
        ruta++;
        ruta_total++;
    }    

    avanzar_nombre = sacar_nombre;
   
    while (*avanzar_nombre != '\0') 
    {
        *ruta_total = *avanzar_nombre;
        avanzar_nombre++;
        ruta_total++;
    }

    avanzar_nombre = sacar_nombre;
    *ruta_total = '/';
    ruta_total++;

    while (*avanzar_nombre != '\0') 
    {
        *ruta_total = *avanzar_nombre;
        avanzar_nombre++;
        ruta_total++;
    }

    while (*extension != '\0') 
    {
        *ruta_total = *extension;
        extension++;
        ruta_total++;
    }

    *ruta_total = '\0';
    ruta_total = inicio_ruta_total;

    if (archivo_log == NULL) 
    {
        crear_carpetas(ruta_total);

        archivo_log = fopen(ruta_total, "a");

        if (archivo_log == NULL) 
        {
            fprintf(stderr, "\033[0;31mNo se ha podido crear el fichero de logs\033[0m\n");
            free(ruta_total);

            return 1;
        }
    }

    free(inicio_ruta);
    free(ruta_total);

    return 0;

}

void cerrar_archivo_log(void) 
{
    if (archivo_log != NULL) 
    {
        fclose(archivo_log);
    }

    return;
}

