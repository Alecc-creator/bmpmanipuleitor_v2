#include "funciones_grupo.h"

bool verbose = false;

int procesar_imagen (int argc, char* argv[])
{
    int ret;
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "--help") == 0)
        {
            mostrar_comandos();
            return EXITO;
        }
        else if(strcmp(argv[i], "--verbose") == 0)
            verbose = true;
    }

    if(verbose)
    {
        printf("[INFO] Iniciando bmpmanipuleitor...\n");
        printf("[INFO] Argumentos detectados: ");
        for(int i = 1; i < argc; i++)
            printf("%s ", argv[i]);

        printf("\n");
    }

    char* bmpEncontrados[MAX_BMP] = {NULL, NULL};
    int cantBMP = encontrarImagenes(argv, bmpEncontrados);

    if(!cantBMP)
        return ERROR_ARGUMENTOS;

    char nombre_salida[TAM_MAX_NOMBRE];
    limpiarNombreArchivo(nombre_salida);
    int porcentaje = 0;

    const char* imagen = bmpEncontrados[0];
    t_ImagenBMP imagenBMP;

    ret = cargarImagen(imagen, &imagenBMP);
    if(ret != EXITO)
        return ret;

    t_ImagenBMP imagenBMP2 = {NULL, {0}, NULL};
    const char* imagen2 = NULL;

    if(cantBMP == 2)
    {
        imagen2 = bmpEncontrados[1];

        ret = cargarImagen(imagen2, &imagenBMP2);
        if(ret != EXITO)
        {
            liberarImagen(&imagenBMP);
            return ret;
        }
    }

    t_pixel ** matrizCopia = crearMatriz(imagenBMP.header.alto, imagenBMP.header.ancho);
    if(!matrizCopia)
    {
        liberarImagen(&imagenBMP);

        if(cantBMP == 2)
            liberarImagen(&imagenBMP2);

        return ERROR_MEMORIA;
    }

    int flagFunciones[18]= {0};
    int contador = 0;
    bool bmpValidado = false;

    for(int i = 1; i < argc; i++)       // argv[0] es el nombre del programa
    {
        char prefijo[] = "RECURSIVIDAD";
        const char * opcion = argv[i];
        bool filtroValido = true;
        bool resultado = false;

        if((strcmp(opcion, imagen) != 0) && (cantBMP < 2 || strcmp(opcion, imagen2) != 0))
        {
            if(strncmp(opcion, "--validar", strlen("--validar")) == 0 && flagFunciones[0] != 1)
            {
                printf("Validando %s...", imagen);
                if(!comando_validar(&imagenBMP.header))
                    printf("ARCHIVO INVALIDO - No se puede procesar\n");
                flagFunciones[0] = 1;
            }

            else if(strncmp(opcion, "--info", strlen("--info")) == 0 && flagFunciones[1] != 1)
            {
                printf("Archivo: %s\n", imagen);
                mostrar_info(&imagenBMP.header);
                flagFunciones[1] = 1;
            }

            else
            {
                porcentaje = buscarPorcentaje(opcion); // por cada nueva pasada se busca porcentaje
                generarNombreArchivo(prefijo, imagen, opcion, nombre_salida, porcentaje);

                if(bmpValidado == false)
                {
                    if(!validar_bmp(&imagenBMP.header))
                    {
                        liberarImagen(&imagenBMP);
                        liberarMatriz(matrizCopia, imagenBMP.header.alto);


                        if(cantBMP == 2)
                            liberarImagen(&imagenBMP2);

                        return ERROR_BMP_INVALIDO;
                    }

                    bmpValidado = true;
                }

                if(strcmp(opcion, "--negativo") == 0 && flagFunciones[2] != 1)
                {
                    copiarMatriz(imagenBMP.matriz, matrizCopia, imagenBMP.header.alto, imagenBMP.header.ancho);
                    resultado = negativo(imagenBMP.archivo, nombre_salida, &imagenBMP.header, matrizCopia);
                    flagFunciones[2] = 1;
                }

                else if(strcmp(opcion, "--escala-de-grises") == 0 && flagFunciones[3] != 1)
                {
                    copiarMatriz(imagenBMP.matriz, matrizCopia, imagenBMP.header.alto, imagenBMP.header.ancho);
                    resultado = escala_de_grises(imagenBMP.archivo, nombre_salida, &imagenBMP.header, matrizCopia);
                    flagFunciones[3] = 1;
                }

                else if(strcmp(opcion, "--espejar-horizontal") == 0 && flagFunciones[4] != 1)
                {
                    resultado = espejar_horizontal(imagenBMP.archivo, nombre_salida, &imagenBMP.header, imagenBMP.matriz, matrizCopia);
                    flagFunciones[4] = 1;
                }

                else if(strcmp(opcion, "--espejar-vertical") == 0 && flagFunciones[5] != 1)
                {
                    resultado = espejar_vertical(imagenBMP.archivo, nombre_salida, &imagenBMP.header, imagenBMP.matriz, matrizCopia);
                    flagFunciones[5] = 1;
                }

                else if(strcmp(opcion, "--rotar-derecha") == 0 && flagFunciones[6] != 1)
                {
                    resultado = rotar(imagenBMP.archivo, nombre_salida, &imagenBMP.header, imagenBMP.matriz, DERECHA);
                    flagFunciones[6] = 1;
                }

                else if(strcmp(opcion, "--rotar-izquierda") == 0 && flagFunciones[7] != 1)
                {
                    resultado = rotar(imagenBMP.archivo, nombre_salida, &imagenBMP.header, imagenBMP.matriz, IZQUIERDA);
                    flagFunciones[7] = 1;
                }

                else if(strncmp(opcion, "--aumentar-contraste", strlen("--aumentar-contraste")) == 0 && flagFunciones[8] != 1 && porcentaje !=-1 )
                {
                    copiarMatriz(imagenBMP.matriz, matrizCopia, imagenBMP.header.alto, imagenBMP.header.ancho);
                    resultado = aumentar_contraste(imagenBMP.archivo, nombre_salida, &imagenBMP.header, matrizCopia, porcentaje);
                    flagFunciones[8] = 1;
                }

                else if(strncmp(opcion, "--reducir-contraste", strlen("--reducir-contraste")) == 0 && flagFunciones[9] != 1 && porcentaje !=-1 )
                {
                    copiarMatriz(imagenBMP.matriz, matrizCopia, imagenBMP.header.alto, imagenBMP.header.ancho);
                    resultado = reducir_contraste(imagenBMP.archivo, nombre_salida, &imagenBMP.header, matrizCopia, porcentaje);
                    flagFunciones[9] = 1;
                }

                else if(strncmp(opcion, "--tonalidad-azul", strlen("--tonalidad-azul")) == 0 && flagFunciones[10] != 1 && porcentaje !=-1 )
                {
                    copiarMatriz(imagenBMP.matriz, matrizCopia, imagenBMP.header.alto, imagenBMP.header.ancho);
                    resultado = tonalidad_azul(imagenBMP.archivo, nombre_salida, &imagenBMP.header, matrizCopia, porcentaje);
                    flagFunciones[10] = 1;
                }

                else if(strncmp(opcion, "--tonalidad-verde", strlen("--tonalidad-verde")) == 0 && flagFunciones[11] != 1 && porcentaje !=-1 )
                {
                    copiarMatriz(imagenBMP.matriz, matrizCopia, imagenBMP.header.alto, imagenBMP.header.ancho);
                    resultado = tonalidad_verde(imagenBMP.archivo, nombre_salida, &imagenBMP.header, matrizCopia, porcentaje);
                    flagFunciones[11] = 1;
                }
                else if(strncmp(opcion, "--tonalidad-roja", strlen("--tonalidad-roja")) == 0 && flagFunciones[12] != 1 && porcentaje!=-1 )
                {
                    copiarMatriz(imagenBMP.matriz, matrizCopia, imagenBMP.header.alto, imagenBMP.header.ancho);
                    resultado = tonalidad_roja(imagenBMP.archivo, nombre_salida, &imagenBMP.header, matrizCopia, porcentaje);
                    flagFunciones[12] = 1;
                }

                else if(strncmp(opcion, "--recortar", strlen("--recortar")) == 0 && flagFunciones[13] != 1 && porcentaje!=-1 )
                {
                    resultado = recortar(imagenBMP.archivo, nombre_salida, &imagenBMP.header, imagenBMP.matriz, porcentaje);
                    flagFunciones[13] = 1;
                }

                else if(strncmp(opcion, "--achicar", strlen("--achicar")) == 0 && flagFunciones[14] != 1 && porcentaje!=-1 )
                {
                    resultado = achicar(imagenBMP.archivo,nombre_salida, &imagenBMP.header, imagenBMP.matriz, porcentaje);
                    flagFunciones[14] = 1;
                }

                else if(strncmp(opcion, "--concatenar-vertical", strlen("--concatenar-vertical")) == 0 && flagFunciones[15] != 1 && cantBMP == 2)
                {
                    resultado = concatenar_vertical(imagenBMP.archivo, imagenBMP2.archivo, nombre_salida, &imagenBMP.header, &imagenBMP2.header, imagenBMP.matriz, imagenBMP2.matriz);
                    flagFunciones[15] = 1;
                }

                else if(strncmp(opcion, "--concatenar-horizontal", strlen("--concatenar-horizontal")) == 0 && flagFunciones[16] != 1 && cantBMP == 2)
                {
                    resultado = concatenar_horizontal(imagenBMP.archivo, imagenBMP2.archivo, nombre_salida, &imagenBMP.header, &imagenBMP2.header, imagenBMP.matriz, imagenBMP2.matriz);
                    flagFunciones[16] = 1;
                }

                else if(strcmp(opcion, "--comodin") == 0 && flagFunciones[17] != 1)
                {
                    copiarMatriz(imagenBMP.matriz, matrizCopia, imagenBMP.header.alto, imagenBMP.header.ancho);
                    resultado = comodin_efecto_VHS(imagenBMP.archivo, nombre_salida, &imagenBMP.header, matrizCopia);
                    flagFunciones[17] = 1;
                }

                else
                {
                    filtroValido = false;
                    if(!esParametroUtilidad(opcion))
                        printf("Filtro invalido: %s\n", opcion);
                }

                if(verbose && filtroValido)
                {
                    const char * nombreFiltro = opcion;
                    nombreFiltro += 2;

                    if(resultado)
                    {
                        printf("[INFO] Aplicando filtro: %s\n", nombreFiltro);
                        printf("[INFO] Guardando resultado: %s\n", nombre_salida);
                        printf("[INFO] Filtro %s completado exitosamente\n", nombreFiltro);
                        contador++;
                    }

                    else
                        printf("[INFO] Error al aplicar filtro: %s\n", nombreFiltro);
                }

                limpiarNombreArchivo(nombre_salida);
            }
        }
    }

    liberarMatriz(matrizCopia, imagenBMP.header.alto);
    liberarImagen(&imagenBMP);

    if(cantBMP == 2)
        liberarImagen(&imagenBMP2);


    if(verbose)
        printf("[INFO] Proceso finalizado - %d archivos generados\n", contador);

    return EXITO;
}

int encontrarImagenes (char* argv[], char * bmpEncontrados[MAX_BMP])
{
    int cantBmp = 0;
    char** p_argv = argv + 1;

    while(*p_argv && cantBmp < MAX_BMP)
    {
        if(strstr(*p_argv, ".bmp"))
        {
            *(bmpEncontrados + cantBmp) = *p_argv;
            cantBmp++;
        }

        p_argv++;
    }

    return cantBmp;
}

int validar_bmp(t_header* header)
{
    if(header == NULL)
    {
        printf("ERROR: Cabecera invalida.\n");
        return ERROR_ARGUMENTOS;
    }

    if(verbose)
        printf("[INFO] Validando header bmp...\n");

    if(header->firma[0] != 'B' || header->firma[1] != 'M')
    {
        printf("Firma invalida.\n");
        return ERROR_BMP_INVALIDO;
    }

    if(header->ancho < 1 || header->alto < 1)
    {
        printf("Dimensiones invalidas. Minimo: 1x1. Actual: %ux%u.\n", header->ancho, header->alto);
        return ERROR_BMP_INVALIDO;
    }

    if(header->bits != 24)
    {
        printf("Profundidad de bits invalida. Requerida: 24 bits. Actual: %hu bits.\n", header->bits);
        return ERROR_BMP_INVALIDO;
    }

    if(header->compresion != 0)
    {
        printf("Imagen comprimida.\n");
        return ERROR_BMP_INVALIDO;
    }

    if(verbose)
        printf("[INFO] Archivo valido - Dimensiones: %ux%u, Tamaño: %u bytes\n", header->ancho, header->alto, header->tamArchivo);

    return EXITO;
}

t_pixel** crearMatriz (int filas, int col)
{
    if(verbose)
        printf("[INFO] Reservando memoria para matriz %dx%d...\n", filas, col);

    t_pixel** matriz = crearMatrizGenerica(filas, col, sizeof(t_pixel));

    if(matriz == NULL)
    {
        printf("Error al asignar memoria\n");
        return NULL;
    }

    if(verbose)
        printf("[INFO] Memoria reservada exitosamente (%lu bytes)\n", (unsigned long)((size_t)filas * col * sizeof(t_pixel)));

    return matriz;
}

void liberarMatriz (t_pixel** mat, int filas)
{
    if(verbose)
        printf("[INFO] Liberando memoria...\n");

    destruirMatrizGenerica(mat, filas);
}

int guardarMatrizArchivo (t_pixel ** matriz, int filas, int col, int padding, FILE * pf)
{
    if(matriz == NULL || pf == NULL || filas < 0 || col < 0 || padding < 0 || padding > 3)
        return ERROR_ARGUMENTOS;

    unsigned char pad[3] = {0, 0, 0};

    for(int i = 0; i < filas; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(fwrite(&matriz[i][j], sizeof(t_pixel), 1, pf) != 1)
                return ERROR_ARCHIVO;
        }

        if(fwrite(pad, sizeof(unsigned char), padding, pf) != padding)
            return ERROR_ARCHIVO;
    }

    return EXITO;
}

int copiar_bytes(FILE *origen, FILE *dest, int offsetDatos)
{
    if(origen == NULL || dest == NULL || offsetDatos < 0)
        return ERROR_ARGUMENTOS;

    long posInicial = ftell(origen);

    if(posInicial == -1L)
        return ERROR_ARCHIVO;

    unsigned char byte;

    for(int i = 0; i < offsetDatos; i++)
    {
        if(fread(&byte, sizeof(byte), 1, origen) != 1)
        {
            fseek(origen, posInicial, SEEK_SET);
            return ERROR_ARCHIVO;
        }

        if(fwrite(&byte, sizeof(byte), 1, dest) != 1)
        {
            fseek(origen, posInicial, SEEK_SET);
            return ERROR_ARCHIVO;
        }
    }

    if(fseek(origen, posInicial, SEEK_SET) != 0)
        return ERROR_ARCHIVO;

    return EXITO;
}

void generarNombreArchivo (const char * prefijo, const char * nombreArch, const char * opcion, char * resultado, int porcentaje)
{
    char guionB[] = "_";
    const char * nombreBase = strrchr(nombreArch, '/');
    if(nombreBase == NULL)
        nombreBase = strrchr(nombreArch, '\\');

    if(nombreBase == NULL)
        nombreBase = nombreArch;
    else
        nombreBase++;

    strcat(resultado, prefijo);
    strcat(resultado, guionB);

    opcion += 2;
    strcat(resultado, opcion);
    strcat(resultado, guionB);
    if(porcentaje != -1)
    {
        char * aux = strrchr(resultado, '=');
        *aux = '-';
    }

    strcat(resultado, nombreBase);
}

int limpiarNombreArchivo(char* cad_nombre)
{
    if(cad_nombre == NULL)
        return ERROR_ARGUMENTOS;

    *cad_nombre = '\0';

    return EXITO;
}

bool esParametroUtilidad (const char * opcion)
{
    if(strncmp(opcion, "--info", strlen("--info")) == 0)
        return true;

    if(strncmp(opcion, "--validar", strlen("--validar")) == 0)
        return true;

    if(strncmp(opcion, "--verbose", strlen("--verbose")) == 0)
        return true;

    if(strncmp(opcion, "--help", strlen("--help")) == 0)
        return true;

    return false;
}

int buscarPorcentaje (const char * parametro)
{
    char aux[4];        // 3 bytes (0-100) 1 byte (\0)
    const char * ptrCadena = strrchr(parametro, '=');
    if(!ptrCadena)
        return -1;

    ptrCadena++;        // Saltear '='

    for(const char *p = ptrCadena; *p != '\0'; p++)
    {
        if(*p < '0' || *p > '9')
        {
            printf("El porcentaje debe ser un numero (0-100)\n");
            return -1;
        }
    }

    strcpy(aux, ptrCadena);
    aux[3] = '\0';
    int porcentaje = 0;
    porcentaje = atoi(aux);

    int li = 0;

    if(strncmp(parametro, "--recortar", strlen("--recortar")) == 0 || strncmp(parametro, "--achicar", strlen("--achicar")) == 0)
        li = 1;

    if(porcentaje < li || porcentaje > 100)
    {
        printf("Valor incorrecto para el porcentaje, ingresar valor entre %d y 100.\n", li);
        return -1;
    }

    return porcentaje;
}

FILE* abrir_archivo (const char * path, const char * metodo)
{
    FILE * pf = fopen(path, metodo);
    if(!pf)
    {
        if(strcmp(metodo, "wb") == 0)
            printf("Sin permisos de escritura\n");
        else
            printf("No existe la ruta de archivo: %s\n", path);
        return NULL;
    }

    if(verbose && strcmp(metodo, "rb") == 0)
        printf("[INFO] Cargando archivo: %s\n", path);

    return pf;
}

int cargarMatriz(FILE* pf, t_pixel** mat, t_header* header)
{
    if(pf == NULL || mat == NULL || header == NULL)
        return ERROR_ARGUMENTOS;

    long posInicial = ftell(pf);

    if(posInicial == -1L)
        return ERROR_ARCHIVO;

    if(fseek(pf, header->offsetDatos, SEEK_SET) != 0)
    {
        fseek(pf, posInicial, SEEK_SET);
        return ERROR_ARCHIVO;
    }

    if(verbose)
        printf("[INFO] Leyendo datos de imagen...\n");

    for(unsigned int i = 0; i < header->alto; i++)
    {
        if(mat[i] == NULL)
        {
            fseek(pf, posInicial, SEEK_SET);
            return ERROR_ARGUMENTOS;
        }

        for(unsigned int j = 0; j < header->ancho; j++)
        {
            if(fread(&mat[i][j], sizeof(t_pixel), 1, pf) != 1)
            {
                fseek(pf, posInicial, SEEK_SET);
                return ERROR_ARCHIVO;
            }
        }

        if(fseek(pf, header->padding, SEEK_CUR) != 0)
        {
            fseek(pf, posInicial, SEEK_SET);
            return ERROR_ARCHIVO;
        }
    }

    if(fseek(pf, posInicial, SEEK_SET) != 0)
        return ERROR_ARCHIVO;

    if(verbose)
        printf("[INFO] Datos cargados correctamente\n");

    return EXITO;
}

void copiar_header(t_header* original, t_header* nuevo)
{
    nuevo->alto = original->alto;
    nuevo->ancho = original->ancho;
    nuevo->bits = original->bits;
    nuevo->compresion = original->compresion;
    strncpy(nuevo->firma,original->firma,2);
    nuevo->offsetDatos = original->offsetDatos;
    nuevo->padding = original->padding;
    nuevo->tamArchivo = original->tamArchivo;
    nuevo->tamImagen = original->tamImagen;
}

void copiarMatriz (t_pixel** matOriginal, t_pixel** matCopia, int filas, int col)
{
    for(int i = 0; i < filas; i++)
        memcpy(matCopia[i], matOriginal[i], col * sizeof(t_pixel));
}

int cargarImagen(const char* nombre, t_ImagenBMP* imagen)
{
    if(nombre == NULL || imagen == NULL)
        return ERROR_ARGUMENTOS;

    imagen->archivo = NULL;
    imagen->matriz = NULL;

    imagen->archivo = abrir_archivo(nombre, "rb");
    if(imagen->archivo == NULL)
        return ERROR_ARCHIVO;

    int ret = cargar_header(imagen->archivo, &imagen->header);
    if(ret != EXITO)
    {
        fclose(imagen->archivo);
        imagen->archivo = NULL;

        return ret;
    }

    imagen->matriz = crearMatriz(imagen->header.alto, imagen->header.ancho);
    if(imagen->matriz == NULL)
    {
        fclose(imagen->archivo);
        imagen->archivo = NULL;

        return ERROR_MEMORIA;
    }

    ret = cargarMatriz(imagen->archivo, imagen->matriz, &imagen->header);
    if(ret != EXITO)
    {
        liberarMatriz(imagen->matriz, imagen->header.alto);
        imagen->matriz = NULL;
        fclose(imagen->archivo);
        imagen->archivo = NULL;

        return ret;
    }

    return EXITO;
}

void liberarImagen(t_ImagenBMP* imagen)
{
    if(imagen == NULL)
        return;

    if(imagen->matriz != NULL)
    {
        liberarMatriz(imagen->matriz, imagen->header.alto);
        imagen->matriz = NULL;
    }

    if(imagen->archivo != NULL)
    {
        fclose(imagen->archivo);
        imagen->archivo = NULL;
    }
}

int ejecutarFiltro(const char* opcion, t_ImagenBMP* imagen, t_ImagenBMP* imagen2, const char* nombreSalida, t_pixel** matrizCopia, int porcentaje, int* flagFunciones, int cantBMP, bool* filtroValido)
{
    if(opcion == NULL || imagen == NULL || nombreSalida == NULL || matrizCopia == NULL || flagFunciones == NULL || filtroValido == NULL)
        return ERROR_ARGUMENTOS;

    *filtroValido = true;
    int resultado = EXITO;

    if(strcmp(opcion, "--validar") == 0)
    {
        if(flagFunciones[0] == 1)
        {
            *filtroValido = false;
            return EXITO;
        }

        if(!comando_validar(&imagen->header))
            resultado = ERROR_BMP_INVALIDO;

        if(resultado == EXITO)
            flagFunciones[0] = 1;
    }
    else if(strcmp(opcion, "--info") == 0)
    {
        if(flagFunciones[1] == 1)
        {
            *filtroValido = false;
            return EXITO;
        }

        mostrar_info(&imagen->header);
        flagFunciones[1] = 1;
    }
    else if(strcmp(opcion, "--negativo") == 0)
    {
        if(flagFunciones[2] == 1)
        {
            *filtroValido = false;
            return EXITO;
        }

        copiarMatriz(imagen->matriz, matrizCopia, imagen->header.alto, imagen->header.ancho);

        resultado = negativo(imagen->archivo, nombreSalida, &imagen->header, matrizCopia);
        if(resultado == EXITO)
            flagFunciones[2] = 1;
    }
    else if(strcmp(opcion, "--escala-de-grises") == 0)
    {
        if(flagFunciones[3] == 1)
        {
            *filtroValido = false;
            return EXITO;
        }

        copiarMatriz(imagen->matriz, matrizCopia, imagen->header.alto, imagen->header.ancho);

        resultado = escala_de_grises(imagen->archivo, nombreSalida, &imagen->header, matrizCopia);
        if(resultado == EXITO)
            flagFunciones[3] = 1;
    }
    else if(strcmp(opcion, "--espejar-horizontal") == 0)
    {
        if(flagFunciones[4] == 1)
        {
            *filtroValido = false;
            return EXITO;
        }

        resultado = espejar_horizontal(imagen->archivo, nombreSalida, &imagen->header, imagen->matriz, matrizCopia);
        if(resultado == EXITO)
            flagFunciones[4] = 1;
    }
    else if(strcmp(opcion, "--espejar-vertical") == 0)
    {
        if(flagFunciones[5] == 1)
        {
            *filtroValido = false;
            return EXITO;
        }

        resultado = espejar_vertical(imagen->archivo, nombreSalida, &imagen->header, imagen->matriz, matrizCopia);
        if(resultado == EXITO)
            flagFunciones[5] = 1;
    }
    else if(strcmp(opcion, "--rotar-derecha") == 0)
    {
        if(flagFunciones[6] == 1)
        {
            *filtroValido = false;
            return EXITO;
        }

        resultado = rotar(imagen->archivo, nombreSalida, &imagen->header, imagen->matriz, DERECHA);
        if(resultado == EXITO)
            flagFunciones[6] = 1;
    }
    else if(strcmp(opcion, "--rotar-izquierda") == 0)
    {
        if(flagFunciones[7] == 1)
        {
            *filtroValido = false;
            return EXITO;
        }

        resultado = rotar(imagen->archivo, nombreSalida, &imagen->header, imagen->matriz, IZQUIERDA);
        if(resultado == EXITO)
            flagFunciones[7] = 1;
    }
    else if(strncmp(opcion, "--aumentar-contraste", strlen("--aumentar-contraste")) == 0)
    {
        if(flagFunciones[8] == 1 || porcentaje < 0)
        {
            *filtroValido = false;
            return EXITO;
        }

        copiarMatriz(imagen->matriz, matrizCopia, imagen->header.alto, imagen->header.ancho);
        resultado = aumentar_contraste(imagen->archivo, nombreSalida, &imagen->header, matrizCopia, (unsigned int)porcentaje);
        if(resultado == EXITO)
            flagFunciones[8] = 1;
    }
    else if(strncmp(opcion, "--reducir-contraste", strlen("--reducir-contraste")) == 0)
    {
        if(flagFunciones[9] == 1 || porcentaje < 0)
        {
            *filtroValido = false;
            return EXITO;
        }

        copiarMatriz(imagen->matriz, matrizCopia, imagen->header.alto, imagen->header.ancho);
        resultado = reducir_contraste(imagen->archivo, nombreSalida, &imagen->header, matrizCopia, (unsigned int)porcentaje);
        if(resultado == EXITO)
            flagFunciones[9] = 1;
    }
    else if(strncmp(opcion, "--tonalidad-azul", strlen("--tonalidad-azul")) == 0)
    {
        if(flagFunciones[10] == 1 || porcentaje < 0)
        {
            *filtroValido = false;
            return EXITO;
        }

        copiarMatriz(imagen->matriz, matrizCopia, imagen->header.alto, imagen->header.ancho);
        resultado = tonalidad_azul(imagen->archivo, nombreSalida, &imagen->header, matrizCopia, (unsigned int)porcentaje);
        if(resultado == EXITO)
            flagFunciones[10] = 1;
    }
    else if(strncmp(opcion, "--tonalidad-verde", strlen("--tonalidad-verde")) == 0)
    {
        if(flagFunciones[11] == 1 || porcentaje < 0)
        {
            *filtroValido = false;
            return EXITO;
        }

        copiarMatriz(imagen->matriz, matrizCopia, imagen->header.alto, imagen->header.ancho);
        resultado = tonalidad_verde(imagen->archivo, nombreSalida, &imagen->header, matrizCopia, (unsigned int)porcentaje);
        if(resultado == EXITO)
            flagFunciones[11] = 1;
    }
    else if(strncmp(opcion, "--tonalidad-roja", strlen("--tonalidad-roja")) == 0)
    {
        if(flagFunciones[12] == 1 || porcentaje < 0)
        {
            *filtroValido = false;
            return EXITO;
        }

        copiarMatriz(imagen->matriz, matrizCopia, imagen->header.alto, imagen->header.ancho);
        resultado = tonalidad_roja(imagen->archivo, nombreSalida, &imagen->header, matrizCopia, (unsigned int)porcentaje);
        if(resultado == EXITO)
            flagFunciones[12] = 1;
    }
    else if(strncmp(opcion, "--recortar", strlen("--recortar")) == 0)
    {
        if(flagFunciones[13] == 1 || porcentaje < 0)
        {
            *filtroValido = false;
            return EXITO;
        }

        resultado = recortar(imagen->archivo, nombreSalida, &imagen->header, imagen->matriz, (unsigned int)porcentaje);
        if(resultado == EXITO)
            flagFunciones[13] = 1;
    }
    else if(strncmp(opcion, "--achicar", strlen("--achicar")) == 0)
    {
        if(flagFunciones[14] == 1 || porcentaje < 0)
        {
            *filtroValido = false;
            return EXITO;
        }

        resultado = achicar(imagen->archivo, nombreSalida, &imagen->header, imagen->matriz, (unsigned int)porcentaje);
        if(resultado == EXITO)
            flagFunciones[14] = 1;
    }
    else if(strcmp(opcion, "--comodin") == 0)
    {
        if(flagFunciones[17] == 1)
        {
            *filtroValido = false;
            return EXITO;
        }

        copiarMatriz(imagen->matriz, matrizCopia, imagen->header.alto, imagen->header.ancho);
        resultado = comodin_efecto_VHS( imagen->archivo, nombreSalida, &imagen->header, matrizCopia);
        if(resultado == EXITO)
            flagFunciones[17] = 1;
    }
    else if(strcmp(opcion, "--concatenar-vertical") == 0)
    {
        if(flagFunciones[15] == 1 || cantBMP != 2 || imagen2 == NULL)
        {
            *filtroValido = false;
            return EXITO;
        }

        resultado = concatenar_vertical(imagen->archivo, imagen2->archivo, nombreSalida, &imagen->header, &imagen2->header, imagen->matriz, imagen2->matriz);
        if(resultado == EXITO)
            flagFunciones[15] = 1;
    }
    else if(strcmp(opcion, "--concatenar-horizontal") == 0)
    {
        if(flagFunciones[16] == 1 || cantBMP != 2 || imagen2 == NULL)
        {
            *filtroValido = false;
            return EXITO;
        }

        resultado = concatenar_horizontal(imagen->archivo, imagen2->archivo, nombreSalida, &imagen->header, &imagen2->header, imagen->matriz, imagen2->matriz);
        if(resultado == EXITO)
            flagFunciones[16] = 1;
    }
    else
        *filtroValido = false;

    return resultado;
}
