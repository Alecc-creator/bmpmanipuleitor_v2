#ifndef FILTROS_CON_PARAMETROS_H_INCLUDED
#define FILTROS_CON_PARAMETROS_H_INCLUDED

#include "utilidades.h"

bool aumentar_contraste (FILE * pf_origen, const char * dest, t_header * header, t_pixel **matriz, const unsigned int porcentaje);
bool reducir_contraste (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz, const unsigned int porcentaje);
bool tonalidad_azul (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz, const unsigned int porcentaje);
bool tonalidad_roja (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz, const unsigned int porcentaje);
bool tonalidad_verde (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz, const unsigned int porcentaje);
bool recortar (FILE * pf_origen, const char * dest, t_header * header, t_pixel **matrizOriginal, const unsigned int porcentaje);
bool achicar (FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz,const unsigned int porcentaje);

#endif // FILTROS_CON_PARAMETROS_H_INCLUDED
