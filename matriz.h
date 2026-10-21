#ifndef MATRIZ_H_INCLUDED
#define MATRIZ_H_INCLUDED

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void* crearMatrizGenerica(int filas, int columnas, size_t tamElem);
void destruirMatrizGenerica(void* mat, int filas);


#endif // MATRIZ_H_INCLUDED
