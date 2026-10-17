#ifndef CONCATENACIONES_H_INCLUDED
#define CONCATENACIONES_H_INCLUDED

#include "utilidades.h"

bool concatenar_vertical(FILE * pf_origen, FILE * pf_origen_2, const char * dest, t_header * header, t_header * header_2, t_pixel ** matriz, t_pixel ** matriz_2);
void iniciarConcatenacionVer_grande (t_pixel** mat_origen, t_pixel** mat_origen_2, t_pixel** mat_destino, t_header* header_origen, t_header* header_origen_2, t_header* header_destino);
void iniciarConcatenacionVer_chica (t_pixel** mat_origen, t_pixel** mat_origen_2, t_pixel** mat_destino, t_header* header_origen, t_header* header_origen_2, t_header* header_destino);
bool concatenar_horizontal(FILE * pf_origen, FILE * pf_origen_2, const char * dest, t_header * header, t_header * header_2, t_pixel ** matriz, t_pixel ** matriz_2);
void iniciarConcatenacionHor_grande (t_pixel** mat_origen, t_pixel** mat_origen_2, t_pixel** mat_destino, t_header* header_origen, t_header* header_origen_2);
void iniciarConcatenacionHor_chica (t_pixel** mat_origen, t_pixel** mat_origen_2, t_pixel** mat_destino, t_header* header_origen, t_header* header_origen_2);


#endif // CONCATENACIONES_H_INCLUDED
