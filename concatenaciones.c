#include "concatenaciones.h"
#include "funciones_grupo.h"

int concatenar_vertical(FILE * pf_origen, FILE * pf_origen_2, const char * dest, t_header * header, t_header * header_2, t_pixel ** matriz, t_pixel ** matriz_2)
{
    FILE *pf_dest = abrir_archivo(dest, "wb");
    if(!pf_dest)
        return ERROR_ARCHIVO;

    t_header nuevo_header;
    copiar_header(header, &nuevo_header);

    nuevo_header.alto = header->alto + header_2->alto;

    if(header->ancho >= header_2->ancho)
        nuevo_header.ancho = header->ancho;
    else
        nuevo_header.ancho = header_2->ancho;

    nuevo_header.padding = (4 - (nuevo_header.ancho * BYTES_X_PIXEL) % 4) % 4;
    nuevo_header.tamImagen = (nuevo_header.ancho * BYTES_X_PIXEL + nuevo_header.padding) * nuevo_header.alto;
    nuevo_header.tamArchivo = nuevo_header.tamImagen + nuevo_header.offsetDatos;

    int ret = copiar_bytes(pf_origen, pf_dest, header->offsetDatos);
    if(ret != EXITO)
    {
        fclose(pf_dest);
        return ret;
    }

    if(fseek(pf_dest, 2, SEEK_SET) != 0 ||
       fwrite(&nuevo_header.tamArchivo, sizeof(nuevo_header.tamArchivo), 1, pf_dest) != 1 ||
       fseek(pf_dest, 18, SEEK_SET) != 0 ||
       fwrite(&nuevo_header.ancho, sizeof(nuevo_header.ancho), 1, pf_dest) != 1 ||
       fwrite(&nuevo_header.alto, sizeof(nuevo_header.alto), 1, pf_dest) != 1 ||
       fseek(pf_dest, 34, SEEK_SET) != 0 ||
       fwrite(&nuevo_header.tamImagen, sizeof(nuevo_header.tamImagen), 1, pf_dest) != 1 || fseek(pf_dest, header->offsetDatos, SEEK_SET) != 0)
    {
        fclose(pf_dest);
        return ERROR_ARCHIVO;
    }

    t_pixel **matriz_concatenada = crearMatriz(nuevo_header.alto, nuevo_header.ancho);

    if(!matriz_concatenada)
    {
        fclose(pf_dest);
        return ERROR_MEMORIA;
    }

    if(header->ancho >= header_2->ancho)
        iniciarConcatenacionVer_grande(matriz, matriz_2, matriz_concatenada, header, header_2, &nuevo_header);
    else
        iniciarConcatenacionVer_chica(matriz, matriz_2, matriz_concatenada, header, header_2, &nuevo_header);

    ret = guardarMatrizArchivo(matriz_concatenada, nuevo_header.alto, nuevo_header.ancho, nuevo_header.padding, pf_dest);
    liberarMatriz(matriz_concatenada, nuevo_header.alto);
    if(ret != EXITO)
    {
        fclose(pf_dest);
        return ret;
    }

    if(fclose(pf_dest) != 0)
        return ERROR_ARCHIVO;

    return EXITO;
}

void iniciarConcatenacionVer_grande (t_pixel** mat_origen, t_pixel** mat_origen_2, t_pixel** mat_destino, t_header* header_origen, t_header* header_origen_2, t_header* header_destino)
{
    unsigned int i, j, i_continuacion = 0, j_padding = 0;
    for (i = 0; i < header_origen->alto; i++)
    {
        for (j = 0; j < header_origen->ancho; j++)
            mat_destino[i][j] = mat_origen[i][j];

    }
    i_continuacion = i;
    for (i = 0; i < header_origen_2->alto; i++, i_continuacion++)
    {
        for (j = 0; j < header_origen_2->ancho; j++)
            mat_destino[i_continuacion][j] = mat_origen_2[i][j];

        for(j_padding = j; j_padding < header_destino->ancho; j_padding++)
        {
            mat_destino[i_continuacion][j_padding].r = 0;
            mat_destino[i_continuacion][j_padding].g = 255;
            mat_destino[i_continuacion][j_padding].b = 0;
        }
    }
}

void iniciarConcatenacionVer_chica (t_pixel** mat_origen, t_pixel** mat_origen_2, t_pixel** mat_destino, t_header* header_origen, t_header* header_origen_2, t_header* header_destino)
{
    unsigned int i, j, i_continuacion = 0, j_padding = 0;
    for(i = 0; i < header_origen->alto; i++)
    {
        for (j = 0; j < header_origen->ancho; j++)
            mat_destino[i][j] = mat_origen[i][j];

        for(j_padding = j; j_padding < header_destino->ancho; j_padding++)
        {
            mat_destino[i][j_padding].r = 0;
            mat_destino[i][j_padding].g = 255;
            mat_destino[i][j_padding].b = 0;
        }
    }
    i_continuacion = i;
    for (i = 0; i < header_origen_2->alto; i++, i_continuacion++)
    {
        for (j = 0; j < header_origen_2->ancho; j++)
            mat_destino[i_continuacion][j] = mat_origen_2[i][j];

    }
}

int concatenar_horizontal(FILE * pf_origen, FILE * pf_origen_2, const char * dest, t_header * header, t_header * header_2, t_pixel ** matriz, t_pixel ** matriz_2)
{
    FILE *pf_dest = abrir_archivo(dest, "wb");

    if(!pf_dest)
        return ERROR_ARCHIVO;

    t_header nuevo_header;
    copiar_header(header, &nuevo_header);

    nuevo_header.ancho = header->ancho + header_2->ancho;

    if(header->alto >= header_2->alto)
        nuevo_header.alto = header->alto;
    else
        nuevo_header.alto = header_2->alto;

    nuevo_header.padding = (4 - (nuevo_header.ancho * BYTES_X_PIXEL) % 4) % 4;
    nuevo_header.tamImagen = (nuevo_header.ancho * BYTES_X_PIXEL + nuevo_header.padding) * nuevo_header.alto;
    nuevo_header.tamArchivo = nuevo_header.tamImagen + nuevo_header.offsetDatos;

    int ret = copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    if(ret != EXITO)
    {
        fclose(pf_dest);
        return ret;
    }

    if(fseek(pf_dest, 2, SEEK_SET) != 0 ||
       fwrite(&nuevo_header.tamArchivo, sizeof(nuevo_header.tamArchivo), 1, pf_dest) != 1 ||
       fseek(pf_dest, 18, SEEK_SET) != 0 ||
       fwrite(&nuevo_header.ancho, sizeof(nuevo_header.ancho), 1, pf_dest) != 1 ||
       fwrite(&nuevo_header.alto, sizeof(nuevo_header.alto), 1, pf_dest) != 1 ||
       fseek(pf_dest, 34, SEEK_SET) != 0 ||
       fwrite(&nuevo_header.tamImagen, sizeof(nuevo_header.tamImagen), 1, pf_dest) != 1 ||
       fseek(pf_dest, header->offsetDatos, SEEK_SET) != 0)
    {
        fclose(pf_dest);
        return ERROR_ARCHIVO;
    }

    t_pixel **matriz_concatenada = crearMatriz(nuevo_header.alto, nuevo_header.ancho);

    if(!matriz_concatenada)
    {
        fclose(pf_dest);
        return ERROR_MEMORIA;
    }

    if(header->alto >= header_2->alto)
        iniciarConcatenacionHor_grande(matriz, matriz_2, matriz_concatenada, header, header_2);
    else
        iniciarConcatenacionHor_chica(matriz, matriz_2, matriz_concatenada, header, header_2);

    ret = guardarMatrizArchivo(matriz_concatenada, nuevo_header.alto, nuevo_header.ancho, nuevo_header.padding, pf_dest);

    liberarMatriz(matriz_concatenada, nuevo_header.alto);

    if(ret != EXITO)
    {
        fclose(pf_dest);
        return ret;
    }

    if(fclose(pf_dest) != 0)
        return ERROR_ARCHIVO;

    return EXITO;
}

void iniciarConcatenacionHor_grande (t_pixel** mat_origen, t_pixel** mat_origen_2, t_pixel** mat_destino, t_header* header_origen, t_header* header_origen_2)
{
    unsigned int i, j, i_padding = 0, j_continuacion = 0;
    for (i = 0; i < header_origen_2->alto; i++)
    {
        for (j = 0; j < header_origen->ancho; j++)
            mat_destino[i][j] = mat_origen[i][j];

        j_continuacion = j;
        for (j = 0; j < header_origen_2->ancho; j++, j_continuacion++)
            mat_destino[i][j_continuacion] = mat_origen_2[i][j];

    }
    for (i_padding = i; i_padding < header_origen->alto; i_padding++)
    {
        for (j = 0; j < header_origen->ancho; j++)
            mat_destino[i_padding][j]=mat_origen[i_padding][j];

        j_continuacion = j;
        for(j = 0; j < header_origen_2->ancho; j++, j_continuacion++)
        {
            mat_destino[i_padding][j_continuacion].r=0;
            mat_destino[i_padding][j_continuacion].g=250;
            mat_destino[i_padding][j_continuacion].b=0;
        }
    }
}

void iniciarConcatenacionHor_chica (t_pixel** mat_origen, t_pixel** mat_origen_2, t_pixel** mat_destino, t_header* header_origen, t_header* header_origen_2)
{
    unsigned int i, j, i_padding = 0, j_continuacion = 0;
    for (i = 0; i < header_origen->alto; i++)
    {
        for (j = 0; j < header_origen->ancho; j++)
            mat_destino[i][j] = mat_origen[i][j];

        j_continuacion = j;
        for (j = 0; j < header_origen_2->ancho; j++, j_continuacion++)
            mat_destino[i][j_continuacion] = mat_origen_2[i][j];
    }
    for (i_padding = i; i_padding < header_origen_2->alto; i_padding++)
    {
        for (j = 0; j < header_origen->ancho; j++)
        {
            mat_destino[i_padding][j].r=0;
            mat_destino[i_padding][j].g=250;
            mat_destino[i_padding][j].b=0;
        }
        j_continuacion = j;
        for(j = 0; j < header_origen_2->ancho; j++, j_continuacion++)
            mat_destino[i_padding][j_continuacion]=mat_origen_2[i_padding][j];
    }
}


