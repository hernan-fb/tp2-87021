#include "dibuja.h"
#include <math.h>

bool dibuja_imagen(const char *ruta_imagen_ascii) {
    FILE *archivo;
    char linea[256];
    archivo = fopen(ruta_imagen_ascii, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return false;
    }

    while (fgets(linea, sizeof(linea), archivo)) {
        printf("%s", linea);
    }

    fclose(archivo);

    return true;
}