#include "utilidades.h"

void cargar_header (FILE * pf, t_header * header)
{
    long posInicial = ftell(pf);

    fread(header->firma, sizeof(char), 2, pf);
    fread(&header->tamArchivo, sizeof(unsigned int), 1, pf);

    fseek(pf, 10, SEEK_SET);
    fread(&header->offsetDatos, sizeof(unsigned int), 1, pf);

    fseek(pf, 18, SEEK_SET);
    fread(&header->ancho, sizeof(unsigned int), 1, pf);
    fread(&header->alto, sizeof(unsigned int), 1, pf);

    fseek(pf, 28, SEEK_SET);
    fread(&header->bits, sizeof(unsigned short), 1, pf);
    fread(&header->compresion, sizeof(unsigned int), 1, pf);
    fread(&header->tamImagen, sizeof(unsigned int), 1, pf);

    header->padding =  (4 - ((header->ancho * BYTES_X_PIXEL) % 4)) % 4;
    if(header->tamImagen == 0)
        header->tamImagen = (header->ancho * BYTES_X_PIXEL + header->padding) * header->alto;

    fseek(pf, posInicial, SEEK_SET);
}

bool comando_validar (t_header * header)
{
    if(header->firma[0] != 'B' || header->firma[1] != 'M')
    {
        printf("Signature BMP invalido\n");
        return false;
    }

    printf("Signature BMP valido\n");

    if(header->bits != 24)
    {
        printf("ERROR: Profundidad de color incorrecta (%hu bits, esperado 24 bits)\n", header->bits);
        return false;
    }

    printf("Profundidad de 24 bits confirmada\n");

    if(header->compresion != 0)
    {
        printf("Compresion: Comprimido\n");
        return false;
    }

    printf("Compresion: No comprimido\n");

    printf("ARCHIVO VALIDO - Listo para procesar\n");
    return true;
}

void mostrar_info (t_header * header)
{
    printf("Tamaño del archivo: %d bytes\n", header->tamArchivo);
    printf("Dimensiones: %dx%d pixeles\n", header->ancho, header->alto);
    printf("Profundida de color: %d bits\n", header->bits);

    printf("Offset de datos: %d\n", header->offsetDatos);
    printf("Tamaño de imagen: %d bytes\n", header->tamImagen);
    printf("Padding por fila: %d bytes\n", header->padding);
}

void mostrar_comandos ()
{
    printf("BMPMANIPULEITOR - Manipulador de imágenes BMP 24 bits\n\n");
    puts("GRUPO: RECURSIVIDAD");
    puts("Integrantes:");
    printf("\t1. 41202088 - CUEVAS CARDOZO, Alejandro\n");
    printf("\t2. 40239700 - DEDO, Juan Pablo Lujan\n");
    printf("\t3. 45148037 - VELAZ, Pedro Ezequiel\n\n");

    printf("Uso: bmpmanipuleitor.exe [OPCIONES]\n\n");
    printf("EJEMPLOS:\n");
    printf("\tbmpmanipuleitor.exe --negativo foto.bmp\n");
    printf("\tbmpmanipuleitor.exe --info imagen.bmp --validar\n");
    printf("\tbmpmanipuleitor.exe foto.bmp --verbose --escala-de-grises --aumentar-contraste=25\n\n");

    printf("Filtros basicos\n");
    printf("\t--negativo: Invertir colores\n");
    printf("\t--escala-de-grises: Convertir a escala de grises promediando RGB\n");
    printf("\t--espejar-horizontal: Voltear imagen horizontalmente\n");
    printf("\t--espejar-vertical: Voltear imagen verticalmente\n");

    printf("\nFiltros con parámetros (0-100%%)\n");
    printf("\t--aumentar-contraste=X: Aumenta el contraste en un X%%\n");
    printf("\t--reducir-contraste=X: Reduce el contraste en un X%%\n");
    printf("\t--tonalidad-azul=X: Aumenta en un X%% la intensidad del color azul\n");
    printf("\t--tonalidad-verde=X: Aumenta en un X%% la intensidad del color verde\n");
    printf("\t--tonalidad-roja=X: Aumenta en un X%% la intensidad del color rojo\n");
    printf("\t--recortar=X: Mantener solo X%% del tamaño original comenzando desde la\nesquina inferior izquierda, el resto se descarta (rango válido: 1-100)\n");
    printf("\t--achicar=X: Reducir el tamaño re escalando imagen al X%% (rango válido: 1-100)\n");

    printf("\nRotaciones\n");
    printf("\t--rotar-derecha: sentido horario\n");
    printf("\t--rotar-izquierda: sentido anti horario\n");

    printf("\nConcatenaciones\n");
    printf("\t--concatenar-horizontal: una al lado de la otra, primero la primer imagen.\n");
    printf("\t--concatenar-vertical: una arriba de la otra, primero la primer imagen.\n");

    printf("\n--comodin: Aplica un filtro VHS a la imagen\n");
}
