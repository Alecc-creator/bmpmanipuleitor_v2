#include "funciones_grupo.h"

bool verbose = false;

int procesar_imagen (int argc, char* argv[])
{
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
        {
            printf("%s ", argv[i]);
        }
        printf("\n");
    }

    char* bmpEncontrados[MAX_BMP] = {NULL, NULL};
    int cantBMP = encontrarImagenes(argv, bmpEncontrados);

    if(!cantBMP)
        return ERROR_ARGUMENTOS;

    char nombre_salida[TAM_MAX_NOMBRE];
    limpiarNombreArchivo(nombre_salida);
    unsigned int porcentaje=0;

    t_header header;
    const char * imagen = bmpEncontrados[0];
    FILE * archEntrada = abrir_archivo(imagen, "rb");
    if(!archEntrada)
        return ERROR_ARCHIVO;
    cargar_header(archEntrada, &header);

    t_pixel **matriz = crearMatriz(header.alto, header.ancho);
    if(!matriz)
    {
        fclose(archEntrada);
        return ERROR_MEMORIA;
    }
    cargarMatriz(archEntrada, matriz, &header);

    t_header header2;
    const char * imagen2 = NULL;
    FILE * archEntrada2 = NULL;
    t_pixel **matriz2 = NULL;
    if(cantBMP == 2)
    {
        imagen2 = bmpEncontrados[1];
        archEntrada2 = abrir_archivo(imagen2, "rb");
        if(!archEntrada2)
        {
            fclose(archEntrada);
            liberarMatriz(matriz, header.alto);
            return ERROR_ARCHIVO;
        }
        cargar_header(archEntrada2, &header2);
        if(!validar_bmp(&header2))
        {
            printf("Archivo %s invalido para concatenar.\n", imagen2);
            fclose(archEntrada);
            liberarMatriz(matriz, header.alto);
            fclose(archEntrada2);
            return ERROR_ARCHIVO;
        }

        matriz2 = crearMatriz(header2.alto, header2.ancho);
        if(!matriz2)
        {
            fclose(archEntrada);
            liberarMatriz(matriz, header.alto);
            fclose(archEntrada2);
            return ERROR_MEMORIA;
        }
        cargarMatriz(archEntrada2, matriz2, &header2);
    }

    t_pixel ** matrizCopia = crearMatriz(header.alto, header.ancho);
    if(!matrizCopia)
    {
        liberarMatriz(matriz, header.alto);
        fclose(archEntrada);

        if(cantBMP == 2)
        {
            liberarMatriz(matriz2, header2.alto);
            fclose(archEntrada2);
        }

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
                if(!comando_validar(&header))
                    printf("ARCHIVO INVALIDO - No se puede procesar\n");
                flagFunciones[0] = 1;
            }

            else if(strncmp(opcion, "--info", strlen("--info")) == 0 && flagFunciones[1] != 1)
            {
                printf("Archivo: %s\n", imagen);
                mostrar_info(&header);
                flagFunciones[1] = 1;
            }

            else
            {
                porcentaje=buscarPorcentaje(opcion); // por cada nueva pasada se busca porcentaje
                generarNombreArchivo(prefijo, imagen, opcion, nombre_salida, porcentaje);

                if(bmpValidado == false)
                {
                    if(!validar_bmp(&header))
                    {
                        liberarMatriz(matriz, header.alto);
                        liberarMatriz(matrizCopia, header.alto);
                        fclose(archEntrada);

                        if(cantBMP == 2)
                        {
                            liberarMatriz(matriz2, header2.alto);
                            fclose(archEntrada2);
                        }
                        return BMP_INVALIDO;
                    }

                    bmpValidado = true;
                }

                if(strcmp(opcion, "--negativo") == 0 && flagFunciones[2] != 1)
                {
                    copiarMatriz(matriz, matrizCopia, header.alto, header.ancho);
                    resultado = negativo(archEntrada, nombre_salida, &header, matrizCopia);
                    flagFunciones[2] = 1;
                }

                else if(strcmp(opcion, "--escala-de-grises") == 0 && flagFunciones[3] != 1)
                {
                    copiarMatriz(matriz, matrizCopia, header.alto, header.ancho);
                    resultado = escala_de_grises(archEntrada, nombre_salida, &header, matrizCopia);
                    flagFunciones[3] = 1;
                }

                else if(strcmp(opcion, "--espejar-horizontal") == 0 && flagFunciones[4] != 1)
                {
                    resultado = espejar_horizontal(archEntrada, nombre_salida, &header, matriz, matrizCopia);
                    flagFunciones[4] = 1;
                }

                else if(strcmp(opcion, "--espejar-vertical") == 0 && flagFunciones[5] != 1)
                {
                    resultado = espejar_vertical(archEntrada, nombre_salida, &header, matriz, matrizCopia);
                    flagFunciones[5] = 1;
                }

                else if(strcmp(opcion, "--rotar-derecha") == 0 && flagFunciones[6] != 1)
                {
                    resultado = rotar(archEntrada, nombre_salida, &header, matriz, DERECHA);
                    flagFunciones[6] = 1;
                }

                else if(strcmp(opcion, "--rotar-izquierda") == 0 && flagFunciones[7] != 1)
                {
                    resultado = rotar(archEntrada, nombre_salida, &header, matriz, IZQUIERDA);
                    flagFunciones[7] = 1;
                }

                else if(strncmp(opcion, "--aumentar-contraste", strlen("--aumentar-contraste")) == 0 && flagFunciones[8] != 1 && porcentaje !=-1 )
                {
                    copiarMatriz(matriz, matrizCopia, header.alto, header.ancho);
                    resultado = aumentar_contraste(archEntrada, nombre_salida, &header, matrizCopia, porcentaje);
                    flagFunciones[8] = 1;
                }

                else if(strncmp(opcion, "--reducir-contraste", strlen("--reducir-contraste")) == 0 && flagFunciones[9] != 1 && porcentaje !=-1 )
                {
                    copiarMatriz(matriz, matrizCopia, header.alto, header.ancho);
                    resultado = reducir_contraste(archEntrada, nombre_salida, &header, matrizCopia, porcentaje);
                    flagFunciones[9] = 1;
                }

                else if(strncmp(opcion, "--tonalidad-azul", strlen("--tonalidad-azul")) == 0 && flagFunciones[10] != 1 && porcentaje !=-1 )
                {
                    copiarMatriz(matriz, matrizCopia, header.alto, header.ancho);
                    resultado = tonalidad_azul(archEntrada, nombre_salida, &header, matrizCopia, porcentaje);
                    flagFunciones[10] = 1;
                }

                else if(strncmp(opcion, "--tonalidad-verde", strlen("--tonalidad-verde")) == 0 && flagFunciones[11] != 1 && porcentaje !=-1 )
                {
                    copiarMatriz(matriz, matrizCopia, header.alto, header.ancho);
                    resultado = tonalidad_verde(archEntrada, nombre_salida, &header, matrizCopia, porcentaje);
                    flagFunciones[11] = 1;
                }
                else if(strncmp(opcion, "--tonalidad-roja", strlen("--tonalidad-roja")) == 0 && flagFunciones[12] != 1 && porcentaje!=-1 )
                {
                    copiarMatriz(matriz, matrizCopia, header.alto, header.ancho);
                    resultado = tonalidad_roja(archEntrada, nombre_salida, &header, matrizCopia, porcentaje);
                    flagFunciones[12] = 1;
                }

                else if(strncmp(opcion, "--recortar", strlen("--recortar")) == 0 && flagFunciones[13] != 1 && porcentaje!=-1 )
                {
                    resultado = recortar(archEntrada, nombre_salida, &header, matriz, porcentaje);
                    flagFunciones[13] = 1;
                }

                else if(strncmp(opcion, "--achicar", strlen("--achicar")) == 0 && flagFunciones[14] != 1 && porcentaje!=-1 )
                {
                    resultado = achicar(archEntrada,nombre_salida,&header,matriz,porcentaje);
                    flagFunciones[14] = 1;
                }

                else if(strncmp(opcion, "--concatenar-vertical", strlen("--concatenar-vertical")) == 0 && flagFunciones[15] != 1 && cantBMP == 2)
                {
                    resultado = concatenar_vertical(archEntrada, archEntrada2, nombre_salida, &header, &header2, matriz, matriz2);
                    flagFunciones[15] = 1;
                }

                else if(strncmp(opcion, "--concatenar-horizontal", strlen("--concatenar-horizontal")) == 0 && flagFunciones[16] != 1 && cantBMP == 2)
                {
                    resultado = concatenar_horizontal(archEntrada, archEntrada2, nombre_salida, &header, &header2, matriz, matriz2);
                    flagFunciones[16] = 1;
                }

                else if(strcmp(opcion, "--comodin") == 0 && flagFunciones[17] != 1)
                {
                    copiarMatriz(matriz, matrizCopia, header.alto, header.ancho);
                    resultado = comodin_efecto_VHS(archEntrada, nombre_salida, &header, matrizCopia);
                    flagFunciones[17] = 1;
                }

                else
                {
                    filtroValido = false;
                    if(!esParametroUtilidad(opcion))
                    {
                        printf("Filtro invalido: %s\n", opcion);
                    }
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

    liberarMatriz(matriz, header.alto);
    liberarMatriz(matrizCopia, header.alto);
    if(cantBMP == 2)
    {
        liberarMatriz(matriz2, header2.alto);
        fclose(archEntrada2);
    }
    fclose(archEntrada);

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

bool validar_bmp (t_header * header)
{
    bool valido = true;

    if(verbose)
        printf("[INFO] Validando header bmp...\n");

    if(header->firma[0] != 'B' || header->firma[1] != 'M')
    {
        printf("Firma invalida.\n");
        valido = false;
    }

    if(header->ancho < 1 || header->alto < 1)
    {
        printf("Dimensiones invalidas. Minimo: 1x1). Actual: %ux%u.\n", header->ancho, header->alto);
        valido = false;
    }

    if(header->bits != 24)
    {
        printf("Profundidad de bits invalida. Requerida: 24 bits. Actual: %hu bits.\n", header->bits);
        valido = false;
    }

    if(header->compresion != 0)
    {
        printf("Imagen comprimida.\n");
        valido = false;
    }

    if(verbose)
    {
        if(valido)
            printf("[INFO] Archivo valido - Dimensiones: %ux%u, Tamaño: %'u bytes\n", header->ancho, header->alto, header->tamArchivo);

        else
            printf("[INFO] Archivo invalido\n");
    }

    return valido;
}

t_pixel** crearMatriz (int filas, int col)
{
    if(verbose)
        printf("[INFO] Reservando memoria para matriz %dx%d...\n", filas, col);

    t_pixel **matriz = malloc(filas * sizeof(t_pixel*));
    if(!matriz)
    {
        printf("Error al asignar memoria\n");
        return NULL;
    }

    for(int i = 0; i < filas; i++)
    {
        matriz[i] = malloc(col * sizeof(t_pixel));

        if(!matriz[i])
        {
            for(int j = 0; j < i; j++)
            {
                free(matriz[j]);
            }
            free(matriz);
            printf("Error al asignar memoria\n");
            return NULL;
        }
    }

    if(verbose)
        printf("[INFO] Memoria reservada exitosamente (%d pixeles)\n", filas * col * BYTES_X_PIXEL);

    return matriz;
}

void liberarMatriz (t_pixel** mat, int filas)
{
    if(verbose)
        printf("[INFO] Liberando memoria...\n");

    for(int i = 0; i < filas; i++)
    {
        free(mat[i]);
    }
    free(mat);
}

void guardarMatrizArchivo (t_pixel ** matriz, int filas, int col, int padding, FILE * pf)
{
    unsigned char pad[3] = {0, 0, 0};

    for(int i = 0; i < filas; i++)
    {
        for(int j = 0; j < col; j++)
        {
            fwrite(&matriz[i][j], sizeof(t_pixel), 1, pf);
        }
        fwrite(pad, sizeof(unsigned char), padding, pf);
    }
}

void copiar_bytes (FILE * origen, FILE * dest, int offsetDatos)
{
    char byte;
    long posInicial = ftell(origen);

    for(int i = 0; i < offsetDatos; i++)
    {
        fread(&byte, sizeof(char), 1, origen);
        fwrite(&byte, sizeof(char), 1, dest);
    }

    fseek(origen, posInicial, SEEK_SET);
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

void limpiarNombreArchivo(char* cad_nombre)
{
    *cad_nombre = '\0';
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

void cargarMatriz (FILE * pf, t_pixel ** mat, t_header * header)
{
    long posInicial = ftell(pf);
    fseek(pf, header->offsetDatos, SEEK_SET);

    for(int i = 0; i < header->alto; i++)
    {
        for(int j = 0; j < header->ancho; j++)
        {
            fread(&mat[i][j], sizeof(t_pixel), 1, pf);
        }
        fseek(pf, header->padding, SEEK_CUR);
    }

    if(verbose)
    {
        printf("[INFO] Leyendo datos de imagen...\n");
        printf("[INFO] Datos cargados correctamente\n");
    }

    fseek(pf, posInicial, SEEK_SET);
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
    {
        memcpy(matCopia[i], matOriginal[i], col * sizeof(t_pixel));
    }
}
