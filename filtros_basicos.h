#ifndef FILTROS_BASICOS_H_INCLUDED
#define FILTROS_BASICOS_H_INCLUDED

#include "utilidades.h"

int negativo (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz);

int escala_de_grises (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz);

int espejar_horizontal (FILE * pf_origen, const char * dest, t_header * header, t_pixel **matrizOriginal, t_pixel **matriz);

int espejar_vertical (FILE * pf_origen, const char * dest, t_header * header, t_pixel **matrizOriginal, t_pixel **matriz);


#endif // FILTROS_BASICOS_H_INCLUDED
