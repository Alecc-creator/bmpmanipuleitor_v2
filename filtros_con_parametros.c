#include "funciones_grupo.h"
#include "filtros_con_parametros.h"

bool aumentar_contraste (FILE * pf_origen, const char * dest, t_header * header, t_pixel **matriz, const unsigned int porcentaje)
{
    FILE * pf_dest = fopen(dest, "wb");
    if(!pf_dest)
        return false;

    int prom = 0;

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
        {
            prom = matriz[i][j].r + (matriz[i][j].r *(porcentaje / 100.0));
            matriz[i][j].r = (unsigned char)(prom > 255 ? 255 : (prom < 0 ? 0 : prom));

            prom = matriz[i][j].g + ( matriz[i][j].g *(porcentaje / 100.0));
            matriz[i][j].g = (unsigned char)(prom > 255 ? 255 : (prom < 0 ? 0 : prom));

            prom = matriz[i][j].b + (matriz[i][j].b *(porcentaje / 100.0));
            matriz[i][j].b = (unsigned char)(prom > 255 ? 255 : (prom < 0 ? 0 : prom));
        }
    }

    guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    fclose(pf_dest);
    return true;
}

bool reducir_contraste (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz, const unsigned int porcentaje)
{
    FILE * pf_dest = fopen(dest, "wb");
    if(!pf_dest)
        return false;

    int prom=0;

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
        {
            prom = matriz[i][j].r - (matriz[i][j].r *(porcentaje / 100.0));
            matriz[i][j].r = (unsigned char)(prom > 255 ? 255 : (prom < 0 ? 0 : prom));

            prom = matriz[i][j].g - ( matriz[i][j].g *(porcentaje / 100.0));
            matriz[i][j].g = (unsigned char)(prom > 255 ? 255 : (prom < 0 ? 0 : prom));

            prom = matriz[i][j].b - (matriz[i][j].b *(porcentaje / 100.0));
            matriz[i][j].b = (unsigned char)(prom > 255 ? 255 : (prom < 0 ? 0 : prom));
        }
    }

    guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    fclose(pf_dest);
    return true;
}

bool tonalidad_azul (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz, const unsigned int porcentaje)
{
    FILE * pf_dest = fopen(dest, "wb");
    if(!pf_dest)
        return false;

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    int pixelAzul= 0;
    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
        {
            pixelAzul= matriz[i][j].b + (matriz[i][j].b * (porcentaje / 100.0));
            matriz[i][j].b = (unsigned char)(pixelAzul > 255 ? 255 : (pixelAzul < 0 ? 0 : pixelAzul));
        }
    }

    guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    fclose(pf_dest);
    return true;
}

bool tonalidad_verde (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz, const unsigned int porcentaje)
{
    FILE * pf_dest = fopen(dest, "wb");
    if(!pf_dest)
        return false;

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    int pixelVerde= 0;
    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
        {
            pixelVerde= matriz[i][j].g + (matriz[i][j].g*((porcentaje / 100.0)));
            matriz[i][j].g = (unsigned char)(pixelVerde > 255 ? 255 : (pixelVerde < 0 ? 0 : pixelVerde));
        }
    }

    guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    fclose(pf_dest);
    return true;
}

bool tonalidad_roja (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz, const unsigned int porcentaje)
{
    FILE * pf_dest = fopen(dest, "wb");
    if(!pf_dest)
        return false;



    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    int pixelRojo= 0;
    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
        {
            pixelRojo= matriz[i][j].r + (matriz[i][j].r*((porcentaje / 100.0)));
            matriz[i][j].r = (unsigned char)(pixelRojo > 255 ? 255 : (pixelRojo < 0 ? 0 : pixelRojo));
        }
    }

    guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    fclose(pf_dest);
    return true;
}

bool recortar (FILE * pf_origen, const char * dest, t_header * header, t_pixel **matrizOriginal, const unsigned int porcentaje)
{
    t_header nuevo_header;
    copiar_header(header,&nuevo_header);

    nuevo_header.ancho=(unsigned int)((nuevo_header.ancho) * (porcentaje/100.0));
    nuevo_header.alto =(unsigned int)((nuevo_header.alto) * (porcentaje/100.0));
    nuevo_header.padding=(4 - (nuevo_header.ancho * BYTES_X_PIXEL) % 4) % 4;
    nuevo_header.tamImagen =(nuevo_header.ancho * BYTES_X_PIXEL + nuevo_header.padding) * nuevo_header.alto;
    nuevo_header.tamArchivo= (nuevo_header.tamImagen)+(nuevo_header.offsetDatos);

    if(nuevo_header.ancho < 1 || nuevo_header.alto < 1)
    {
        printf("No se puede recortar mas la imagen.\n");
        return false;
    }

    FILE * pf_dest = fopen(dest, "wb");
    if(!pf_dest)
        return false;

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    // Vuelvo para intercambiar tamaño del archivo
    fseek(pf_dest, 2, SEEK_SET);
    fwrite(&nuevo_header.tamArchivo, sizeof(int), 1, pf_dest);

    // Vuelvo para intercambiar alto y ancho
    fseek(pf_dest, 18, SEEK_SET);
    fwrite(&nuevo_header.ancho, sizeof(int), 1, pf_dest);
    fwrite(&nuevo_header.alto, sizeof(int), 1, pf_dest);

    // Vuelvo para intercambiar tamaño de la imagen
    fseek(pf_dest, 34, SEEK_SET);
    fwrite(&nuevo_header.tamImagen, sizeof(int), 1, pf_dest);

    // Vuelvo al offset
    fseek(pf_dest, header->offsetDatos, SEEK_SET);

    guardarMatrizArchivo(matrizOriginal,nuevo_header.alto, nuevo_header.ancho, nuevo_header.padding, pf_dest);

    fclose(pf_dest);
    return true;
}

bool achicar (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz,const unsigned int porcentaje)
{
    t_header nuevo_header;
    copiar_header(header, &nuevo_header);

    nuevo_header.ancho=(unsigned int)((nuevo_header.ancho) * (porcentaje/100.0));
    nuevo_header.alto =(unsigned int)((nuevo_header.alto) * (porcentaje/100.0));
    nuevo_header.padding=(4 - (nuevo_header.ancho * BYTES_X_PIXEL) % 4) % 4;
    nuevo_header.tamImagen =(nuevo_header.ancho * BYTES_X_PIXEL + nuevo_header.padding) * nuevo_header.alto;
    nuevo_header.tamArchivo= (nuevo_header.tamImagen)+(nuevo_header.offsetDatos);

    if(nuevo_header.alto < 1 || nuevo_header.ancho < 1)
    {
        printf("No se puede achicar mas la imagen.\n");
        return false;
    }

    FILE * pf_dest = abrir_archivo(dest, "wb");
    if(!pf_dest)
        return false;

    t_pixel**  matriz_achicada = crearMatriz(nuevo_header.alto,nuevo_header.ancho);
    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    // Vuelvo para intercambiar tamaño del archivo
    fseek(pf_dest, 2, SEEK_SET);
    fwrite(&nuevo_header.tamArchivo, sizeof(int), 1, pf_dest);

    // Vuelvo para intercambiar alto y ancho
    fseek(pf_dest, 18, SEEK_SET);
    fwrite(&nuevo_header.ancho, sizeof(int), 1, pf_dest);
    fwrite(&nuevo_header.alto, sizeof(int), 1, pf_dest);

    // Vuelvo para intercambiar tamaño de la imagen
    fseek(pf_dest, 34, SEEK_SET);
    fwrite(&nuevo_header.tamImagen, sizeof(int), 1, pf_dest);

    // Vuelvo al offset
    fseek(pf_dest, header->offsetDatos, SEEK_SET);

    float saltoFila =(float)header->alto/nuevo_header.alto;
    float saltoCol =(float)header->ancho/nuevo_header.ancho;

    for (int i = 0; i < nuevo_header.alto; i++)
    {
        for (int j = 0; j < nuevo_header.ancho; j++)
        {
            matriz_achicada[i][j] = matriz[(int)(i * saltoFila)][(int)(j * saltoCol)];
        }
    }

    guardarMatrizArchivo(matriz_achicada, nuevo_header.alto,nuevo_header.ancho,nuevo_header.padding, pf_dest);
    liberarMatriz(matriz_achicada,nuevo_header.alto);

    fclose(pf_dest);
    return true;
}

