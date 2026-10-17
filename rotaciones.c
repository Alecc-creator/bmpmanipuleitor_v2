#include "rotaciones.h"
#include "funciones_grupo.h"

bool rotar (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matOriginal, int metodo)
{
    int nuevoAncho = header->alto;
    int nuevoAlto = header->ancho;

    t_pixel **matRotada = crearMatriz(nuevoAlto, nuevoAncho);
    if(!matRotada)
        return false;

    FILE *pf_dest = abrir_archivo(dest, "wb");
    if(!pf_dest)
    {
        liberarMatriz(matRotada, nuevoAlto);
        return false;
    }

    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    // Vuelvo para intercambiar alto y ancho
    fseek(pf_dest, 18, SEEK_SET);

    fwrite(&nuevoAncho, sizeof(int), 1, pf_dest);
    fwrite(&nuevoAlto, sizeof(int), 1, pf_dest);

    // Vuelvo al offset
    fseek(pf_dest, header->offsetDatos, SEEK_SET);

    // Cargo la matriz rotada
    if(metodo == IZQUIERDA)
    {
        for(int i = 0; i < header->alto; i++)
        {
            for(int j = 0; j < header->ancho; j++)
            {
                matRotada[j][nuevoAncho - 1 - i] = matOriginal[i][j];
            }
        }
    }
    else
    {
        for(int i = 0; i < header->alto; i++)
        {
            for(int j = 0; j < header->ancho; j++)
            {
                matRotada[nuevoAlto - 1 - j][i] = matOriginal[i][j];
            }
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

    guardarMatrizArchivo(matRotada, nuevoAlto, nuevoAncho, nuevoPadding, pf_dest);

    liberarMatriz(matRotada, nuevoAlto);
    fclose(pf_dest);
    return true;
}
