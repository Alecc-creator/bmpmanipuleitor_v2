#ifndef ROTACIONES_H_INCLUDED
#define ROTACIONES_H_INCLUDED

#include "utilidades.h"

#define DERECHA 0
#define IZQUIERDA 1

bool rotar (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matOriginal, int metodo);


#endif // ROTACIONES_H_INCLUDED
