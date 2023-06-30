#ifndef __UTILS_H__
#define __UTILS_H__

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convertirCapital(char *string);
void convertirMinusculas(char *string);
void reemplazarPalabra(char **texto, const char *palabraBuscada,
                       const char *palabraReemplazo);
void subrayarPalabra(char **texto, const char *palabra_a_Subrayar);

#endif /* __UTILS_H__ */