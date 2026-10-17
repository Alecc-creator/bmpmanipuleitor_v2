#include "funciones_grupo.h"
#include "comodin.h"

static unsigned char limitarColor(int valor)
{
    if(valor > 255)
        return 255;

    if(valor < 0)
        return 0;

    return (unsigned char)valor;
}

bool comodin_efecto_VHS(FILE * pf_origen, const char * dest, t_header * header, t_pixel ** matriz)
{
    FILE * pf_dest = abrir_archivo(dest, "wb");

    if(!pf_dest)
        return false;

    // Copiar la cabecera original del BMP.
    copiar_bytes(pf_origen, pf_dest, header->offsetDatos);

    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
        {
            int rojo = matriz[i][j].r;
            int verde = matriz[i][j].g;
            int azul = matriz[i][j].b;

            // Aplicar una tonalidad rojiza y azulada.
            rojo += 18;
            verde = (verde * 90) / 100;
            azul += 12;

            // Simular líneas horizontales de un video VHS.
            if(i % 3 == 0)
            {
                rojo = (rojo * 80) / 100;
                verde = (verde * 80) / 100;
                azul = (azul * 80) / 100;
            }

            // Evitar valores fuera del rango de color.
            matriz[i][j].r = limitarColor(rojo);
            matriz[i][j].g = limitarColor(verde);
            matriz[i][j].b = limitarColor(azul);
        }
    }

    // Guardar los píxeles modificados.
    guardarMatrizArchivo(matriz, header->alto, header->ancho, header->padding, pf_dest);

    fclose(pf_dest);

    return true;
}
