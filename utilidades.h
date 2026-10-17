#ifndef UTILIDADES_H_INCLUDED
#define UTILIDADES_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#define MAX_BMP 2
#define BYTES_X_PIXEL 3

typedef struct
{
    unsigned int tamArchivo;
    unsigned int offsetDatos;
    unsigned int ancho;
    unsigned int alto;
    unsigned int compresion;
    unsigned int tamImagen;
    unsigned int padding;
    unsigned short bits;
    char firma[2];
} t_header;

typedef struct
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
} t_pixel;

/**
 * @brief Carga la informacion de la imagen en una estructura.
 *
 * @param nombreArch Ruta del archivo.
 * @param header Estructura a cargar.
 */
void cargar_header (FILE * pf, t_header * header);

/**
 * @brief Comprueba e imprime si el archivo es valido para procesar.
 *
 * @details Abre el archivo binario y verifica:
 * - Firma "BM"
 * - Profundidad de 24 bits
 * - Sin compresion
 *
 * @param header Estructura con la informacion del archivo.
 *
 * @return true Si cumple las 3 condiciones
 * @return false Si no se puede abrir o no cumple alguna condicion.
 */
bool comando_validar (t_header * header);

/**
 * @brief Imprime por consola la informacion del archivo.
 *
 * @param header Estructura con la informacion del archivo.
 */
void mostrar_info (t_header * header);

/**
 * @brief Imprime por consola los comandos disponibles
 */
void mostrar_comandos ();

#endif // UTILIDADES_H_INCLUDED
