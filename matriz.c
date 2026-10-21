#include "matriz.h"

void* crearMatrizGenerica(int filas, int columnas, size_t tamElem)
{
    if(filas <= 0 || columnas <= 0 || tamElem == 0)
        return NULL;

    void** mat = malloc(filas* sizeof(void*));
    if(!mat)
        return NULL;

    void** ult = mat + filas -1;

    for(void** i = mat; i <= ult; i++)
    {
        *i = malloc(columnas * tamElem);

        if(!*i)
        {
            for(void** j = mat; j < i; j++)
                free(*j);

            free(mat);

            return NULL;
        }
    }

    return mat;
}

void destruirMatrizGenerica(void* mat, int filas)
{
    if(!mat || filas <= 0)
        return;

    void** ult = mat + filas - 1;

    for(void** i = mat; i <= ult ; i++)
        free(*i);

    free(mat);
}
