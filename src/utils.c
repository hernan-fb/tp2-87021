#include "utils.h"
#define ANSI_UNDERLINE "\033[4m"
#define ANSI_RESET "\033[0m"

void convertirCapital(char *string) {
  size_t i = 0;

  if (string == NULL)
    return;
  // Convertir primera letra a mayúscula, independientemente de que la palabra
  // esté subrayada
  size_t var = strlen(ANSI_UNDERLINE);
  if (strncmp(string, ANSI_UNDERLINE, var) != 0) {
    var = 0;
  }
  string[var] = (char)toupper((unsigned char)string[var]);
  // Convertir las otras letras a minúsculas.
  for (i = var + 1; string[i]; i++) {
    string[i] = (char)tolower((unsigned char)string[i]);
  }
  // Convertir las primeras letras de cada oración nueva a mayúsculas.
  char *inicioPalabra = string;
  do {
    inicioPalabra = strstr(inicioPalabra, ". ");
    if (inicioPalabra != NULL) {
      inicioPalabra += 2;
      if (*inicioPalabra != '\0') {
        *inicioPalabra = (char)toupper((unsigned char)*inicioPalabra);
      }
    }
  } while (inicioPalabra != NULL);
}

void convertirMinusculas(char *string) {
  int i = 0;
  if (string == NULL)
    return;
  while (string[i]) {
    string[i] = (char)tolower((unsigned char)string[i]);
    i++;
  }
}

void reemplazarPalabra(char **texto, const char *palabraBuscada,
                       const char *palabraReemplazo) {
  char *posicion = strstr(*texto, palabraBuscada);
  size_t longitudPalabraBuscada = strlen(palabraBuscada);
  size_t longitudPalabraReemplazo = strlen(palabraReemplazo);

  if (longitudPalabraBuscada < longitudPalabraReemplazo) {
    size_t nuevaLongitud =
        strlen(*texto) + longitudPalabraReemplazo - longitudPalabraBuscada;
    char *nuevoTexto = realloc(*texto, nuevaLongitud + 1);
    if (nuevoTexto == NULL) {
      printf("Error: No se pudo asignar memoria.\n");
      return;
    }
    *texto = nuevoTexto;
    posicion = strstr(*texto, palabraBuscada);
  }

  memmove(posicion + longitudPalabraReemplazo,
          posicion + longitudPalabraBuscada,
          strlen(posicion + longitudPalabraBuscada) + 1);
  memcpy(posicion, palabraReemplazo, longitudPalabraReemplazo);
}

void subrayarPalabra(char **texto, const char *palabra_a_Subrayar) {
  char *palabra_subrayada =
      malloc(sizeof(char) * (strlen(palabra_a_Subrayar) + 20));
  if (palabra_subrayada == NULL) {
    return;
  }
  sprintf(palabra_subrayada, "%s%s%s", ANSI_UNDERLINE, palabra_a_Subrayar,
          ANSI_RESET);

  reemplazarPalabra(texto, palabra_a_Subrayar, palabra_subrayada);

  free(palabra_subrayada);
  return;
}

/*
void** crear_vector(size_t elemento1, size_t elemento2, char elemento3) {
    void* vector[3];

    size_t* ptr_elemento1 = malloc(sizeof(size_t));
    if (ptr_elemento1 == NULL) {
        return NULL;
    }
    *ptr_elemento1 = elemento1;
    vector[0] = ptr_elemento1;

    size_t* ptr_elemento2 = malloc(sizeof(size_t));
    if (ptr_elemento2 == NULL) {
        free(ptr_elemento1);
        return NULL;
    }
    *ptr_elemento2 = elemento2;
    vector[1] = ptr_elemento2;

    char* ptr_elemento3 = malloc(sizeof(char));
    if (ptr_elemento3 == NULL) {
        free(ptr_elemento1);
        free(ptr_elemento2);
        return NULL;
    }
    *ptr_elemento3 = elemento3;
    vector[2] = ptr_elemento3;

    return &vector;
}

void liberar_vector(void* vector) {
    size_t* ptr_elemento1 = ((void**)vector)[0];
    size_t* ptr_elemento2 = ((void**)vector)[1];
    char* ptr_elemento3 = ((void**)vector)[2];

    free(ptr_elemento1);
    free(ptr_elemento2);
    free(ptr_elemento3);
}*/