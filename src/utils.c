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