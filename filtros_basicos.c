#include "filtros_basicos.h"
#include "funciones_grupo.h"

int negativo(FILE* pf_origen, const char* dest, t_header* header, t_pixel** matriz)
{
    if(pf_origen == NULL || dest == NULL || header == NULL || matriz == NULL)
        return ERROR_ARGUMENTOS;

    FILE* pf_dest = abrir_archivo(dest, "wb");

    if(pf_dest == NULL)
        return ERROR_ARCHIVO;

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
        {
            matriz[i][j].b = 255 - matriz[i][j].b;
            matriz[i][j].g = 255 - matriz[i][j].g;
            matriz[i][j].r = 255 - matriz[i][j].r;
        }
    }

    int ret = guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    fclose(pf_dest);

    if(ret != EXITO)
        return ret;

    return EXITO;
}

int escala_de_grises(FILE* pf_origen, const char* dest, t_header* header, t_pixel** matriz)
{
    if(pf_origen == NULL || dest == NULL || header == NULL || matriz == NULL)
        return ERROR_ARGUMENTOS;

    FILE* pf_dest = abrir_archivo(dest, "wb");
    if(pf_dest == NULL)
        return ERROR_ARCHIVO;

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    unsigned char gris;

    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
        {
            gris = (matriz[i][j].b + matriz[i][j].g + matriz[i][j].r) / 3;

            matriz[i][j].b = gris;
            matriz[i][j].g = gris;
            matriz[i][j].r = gris;
        }
    }

    int ret = guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    if(fclose(pf_dest) != 0 && ret == EXITO)
        ret = ERROR_ARCHIVO;

    if(ret != EXITO)
        return ret;

    return EXITO;
}

int espejar_horizontal (FILE * pf_origen, const char * dest, t_header * header, t_pixel **matrizOriginal, t_pixel **matriz)
{
    if(pf_origen == NULL || dest == NULL || header == NULL || matrizOriginal == NULL || matriz == NULL)
        return ERROR_ARGUMENTOS;

    FILE* pf_dest = abrir_archivo(dest, "wb");
    if(pf_dest == NULL)
        return ERROR_ARCHIVO;

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
            matriz[i][header->ancho - j - 1] = matrizOriginal[i][j];
    }

    int ret = guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    if(fclose(pf_dest) != 0 && ret == EXITO)
        ret = ERROR_ARCHIVO;

    if(ret != EXITO)
        return ret;

    return EXITO;
}

int espejar_vertical (FILE * pf_origen, const char * dest, t_header * header, t_pixel **matrizOriginal, t_pixel **matriz)
{
    if(pf_origen == NULL || dest == NULL || header == NULL || matrizOriginal == NULL || matriz == NULL)
        return ERROR_ARGUMENTOS;

    FILE* pf_dest = abrir_archivo(dest, "wb");
    if(pf_dest == NULL)
        return ERROR_ARCHIVO;

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
            matriz[header->alto - i - 1][j] = matrizOriginal[i][j];
    }

    int ret = guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    if(fclose(pf_dest) != 0 && ret == EXITO)
        ret = ERROR_ARCHIVO;

    if(ret != EXITO)
        return ret;

    return EXITO;
}
