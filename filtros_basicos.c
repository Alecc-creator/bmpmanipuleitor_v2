#include "filtros_basicos.h"
#include "funciones_grupo.h"

bool negativo (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz)
{
    FILE * pf_dest = abrir_archivo(dest, "wb");
    if(!pf_dest)
        return false;

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

    guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    fclose(pf_dest);
    return true;
}

bool escala_de_grises (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz)
{
    FILE * pf_dest = abrir_archivo(dest, "wb");
    if(!pf_dest)
        return false;

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    unsigned char gris;

    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
        {
            gris = ((matriz[i][j].b + matriz[i][j].g + matriz[i][j].r) / 3);
            matriz[i][j].b = gris;
            matriz[i][j].g = gris;
            matriz[i][j].r = gris;
        }
    }

    guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    fclose(pf_dest);
    return true;
}

bool espejar_horizontal (FILE * pf_origen, const char * dest, t_header * header, t_pixel **matrizOriginal, t_pixel **matriz)
{
    FILE * pf_dest = abrir_archivo(dest, "wb");
    if(!pf_dest)
        return false;

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
        {
            matriz[i][header->ancho - j - 1] = matrizOriginal[i][j];
        }
    }

    guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    fclose(pf_dest);
    return true;
}

bool espejar_vertical (FILE * pf_origen, const char * dest, t_header * header, t_pixel **matrizOriginal, t_pixel **matriz)
{
    FILE * pf_dest = abrir_archivo(dest, "wb");
    if(!pf_dest)
        return false;

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
        {
            matriz[header->alto - i - 1][j] = matrizOriginal[i][j];
        }
    }

    guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    fclose(pf_dest);
    return true;
}
