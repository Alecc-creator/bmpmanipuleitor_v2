#ifndef FUNCIONES_GRUPO_H_INCLUDED
#define FUNCIONES_GRUPO_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "utilidades.h"
#include "filtros_basicos.h"
#include "rotaciones.h"
#include "filtros_con_parametros.h"
#include "concatenaciones.h"
#include "comodin.h"
#include "matriz.h"
#include "codigos.h"

#define TAM_MAX_NOMBRE 256

typedef struct
{
    FILE* archivo;
    t_header header;
    t_pixel** matriz;
} t_ImagenBMP;

extern bool verbose;

int procesar_imagen (int argc, char* argv[]);

/**
 * @brief Busca en argv archivos con extension bmp y los almacena.
 *
 * @param argv Array de strings recibido por main.
 * @param bmpEncontrados Array para guardar los argumentos con extension bmp.
 *
 * @return Cantidad de archivos .bmp encontrados.
 */
int encontrarImagenes (char* argv[], char * bmpEncontrados[MAX_BMP]);

/**
 * @brief Comprueba si el archivo es valido para procesar.
 *
 * @details Abre el archivo binario y verifica:
 * - Firma "BM".
 * - Dimensiones minimas de 1x1.
 * - Profundidad de 24 bits.
 * - Sin compresion.
 *
 * @param header Estructura con la informacion del archivo.
 *
 * @return true Si cumple las 4 condiciones.
 * @return false Si no se puede abrir o no cumple alguna condicion.
 */
int validar_bmp (t_header * header);

/**
 * @brief Reserva memoria para una matriz dinamica.
 *
 * @param alto Cantidad de filas.
 * @param ancho Cantidad de columnas.
 *
 * @return Direccion de memoria del inicio de la matriz.
 */
t_pixel** crearMatriz (int alto, int ancho);

/**
 * @brief Libera la memoria reservada para la matriz.
 *
 * @param mat Direccion de memoria del inicio de la matriz.
 * @param filas Cantida de filas a liberar.
 */
void liberarMatriz (t_pixel** mat, int filas);

/**
 * @brief Escribe en un archivo binario la matriz dada.
 *
 * @param matriz Direccion de memoria del inicio de la matriz.
 * @param filas Cantidad de filas de la matriz.
 * @param col Cantidad de columnas de la matriz.
 * @param padding Cantidad de bytes de padding por fila.
 * @param pf Puntero al archivo a escribir.
 */
int guardarMatrizArchivo (t_pixel ** matriz, int filas, int col, int padding, FILE * pf);

/**
 * @brief Copia una cantidad dada de bytes de una archivo a otro.
 *
 * @param origen Puntero al archivo desde donde se copia.
 * @param dest Puntero al archivo donde se escribe.
 */
int copiar_bytes (FILE * origen, FILE * dest, int offsetDatos);

/**
 * @brief Genera un cadena de caracteres con el prefijo + filtro + nombre del archivo.
 *
 * @param prefijo String con el que comienza la cadena.
 * @param opcion String del filtro que se aplica.
 * @param nombreArch String con el que finaliza la cadena.
 * @param resultado String en donde se guarda la concatenacion.
 */

void generarNombreArchivo (const char * prefijo, const char * nombreArch, const char * opcion, char * resultado, int porcentaje);

int limpiarNombreArchivo(char* cad_nombre);

bool esParametroUtilidad (const char * opcion);

int buscarPorcentaje (const char * parametro);

FILE* abrir_archivo (const char * path, const char * metodo);

int cargarMatriz (FILE * pf, t_pixel ** mat, t_header * header);

void copiar_header(t_header* original, t_header* nuevo);

void copiarMatriz (t_pixel** matOriginal, t_pixel** matCopia, int filas, int col);

int cargarImagen(const char* nombre, t_ImagenBMP* imagen);

void liberarImagen(t_ImagenBMP* imagen);

int ejecutarFiltro(const char* opcion, t_ImagenBMP* imagen, t_ImagenBMP* imagen2, const char* nombreSalida, t_pixel** matrizCopia, int porcentaje, int* flagFunciones, int cantBMP, bool* filtroValido);

#endif // FUNCIONES_GRUPO_H_INCLUDED
