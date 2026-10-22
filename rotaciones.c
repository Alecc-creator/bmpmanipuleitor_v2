#include "rotaciones.h"
#include "funciones_grupo.h"

int rotar (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matOriginal, int metodo)
{
    if(pf_origen == NULL || dest == NULL || header == NULL || matOriginal == NULL)
        return ERROR_ARGUMENTOS;

    int nuevoAncho = header->alto;
    int nuevoAlto = header->ancho;

    t_pixel** matRotada = crearMatriz(nuevoAlto, nuevoAncho);
    if(matRotada == NULL)
        return ERROR_MEMORIA;

    FILE* pf_dest = abrir_archivo(dest, "wb");
    if(pf_dest == NULL)
    {
        liberarMatriz(matRotada, nuevoAlto);
        return ERROR_ARCHIVO;
    }

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    fseek(pf_dest, 18, SEEK_SET);

    fwrite(&nuevoAncho, sizeof(int), 1, pf_dest);
    fwrite(&nuevoAlto, sizeof(int), 1, pf_dest);

    fseek(pf_dest, header->offsetDatos, SEEK_SET);

    if(metodo == IZQUIERDA)
    {
        for(int i = 0; i < header->alto; i++)
        {
            for(int j = 0; j < header->ancho; j++)
                matRotada[j][nuevoAncho - 1 - i] = matOriginal[i][j];
        }
    }
    else
    {
        for(int i = 0; i < header->alto; i++)
        {
            for(int j = 0; j < header->ancho; j++)
                matRotada[nuevoAlto - 1 - j][i] = matOriginal[i][j];
        }
    }

    int nuevoPadding = (4 - (nuevoAncho * 3) % 4) % 4;
    int nuevoTamImagen = (nuevoAncho * BYTES_X_PIXEL + nuevoPadding) * nuevoAlto;
    int nuevoTamArchivo = nuevoTamImagen + header->offsetDatos;

    fseek(pf_dest, 2, SEEK_SET);
    fwrite(&nuevoTamArchivo, sizeof(int), 1, pf_dest);

    fseek(pf_dest, 34, SEEK_SET);
    fwrite(&nuevoTamImagen, sizeof(int), 1, pf_dest);

    fseek(pf_dest, header->offsetDatos, SEEK_SET);

    int ret = guardarMatrizArchivo(matRotada, nuevoAlto, nuevoAncho, nuevoPadding, pf_dest);

    liberarMatriz(matRotada, nuevoAlto);

    if(fclose(pf_dest) != 0 && ret == EXITO)
        ret = ERROR_ARCHIVO;

    if(ret != EXITO)
        return ret;

    return EXITO;
}
