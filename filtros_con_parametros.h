#ifndef FILTROS_CON_PARAMETROS_H_INCLUDED
#define FILTROS_CON_PARAMETROS_H_INCLUDED

#include "utilidades.h"

int aumentar_contraste (FILE * pf_origen, const char * dest, t_header * header, t_pixel **matriz, const unsigned int porcentaje);
int reducir_contraste (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz, const unsigned int porcentaje);
int tonalidad_azul (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz, const unsigned int porcentaje);
int tonalidad_roja (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz, const unsigned int porcentaje);
int tonalidad_verde (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz, const unsigned int porcentaje);
int recortar (FILE * pf_origen, const char * dest, t_header * header, t_pixel **matrizOriginal, const unsigned int porcentaje);
int achicar (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz,const unsigned int porcentaje);

#endif // FILTROS_CON_PARAMETROS_H_INCLUDED
