#include "tp1.h"
#include "lista.h"
#include "pokemon.h"
#include "pokemon_privado.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMA_LONGITUD_LINEA_ARCHIVO 30
#define CAPACIDAD_INICIAL_HOSPITAL 10
#define ERROR -1
#define EXITO 0

bool wrapper_free(void *dato, void *contexto) {
  free(dato);
  return true;
}

void pokemones_destruir(lista_t *pokemones) {
  lista_con_cada_elemento(pokemones, wrapper_free, NULL);
  free(pokemones);
}

void hospital_destruir(hospital_t *hospital) {
  pokemones_destruir(hospital->pokemones);
  free(hospital);
}

int pokemon_inserta_ordenado_por_salud(lista_t *pokemones,
                                       pokemon_t *poke_a_insertar) {
  if (lista_vacia(pokemones)) {
    lista_insertar(pokemones, poke_a_insertar);
    return EXITO;
  }

  lista_iterador_t *iter = lista_iterador_crear(pokemones);
  if (iter == NULL) {
    return ERROR;
  }
  while (lista_iterador_tiene_siguiente(iter) &&
         ((pokemon_t *)lista_iterador_elemento_actual(iter))->salud <
             poke_a_insertar->salud) {
    lista_iterador_avanzar(iter);
  }
  lista_iterador_insertar(iter, poke_a_insertar);

  lista_iterador_destruir(iter);

  return EXITO;
}

hospital_t *hospital_crear_desde_archivo(const char *nombre_archivo) {
  if (nombre_archivo == NULL) {
    return NULL;
  }

  FILE *fp = NULL;
  fp = fopen(nombre_archivo, "r");

  if (fp == NULL) {
    return NULL;
  }
  hospital_t *nuevo_hospital = malloc(sizeof(hospital_t));
  if (nuevo_hospital == NULL) {
    fclose(fp);
    return NULL;
  }
  nuevo_hospital->pokemones = lista_crear();
  if (nuevo_hospital->pokemones == NULL) {
    fclose(fp);
    hospital_destruir(nuevo_hospital);
    return NULL;
  }
  char buff[MAXIMA_LONGITUD_LINEA_ARCHIVO];
  int resultado = 1;
  while (fscanf(fp, "%s", buff) != EOF) {
    pokemon_t *poke_aux = pokemon_crear_desde_string(buff);
    if (poke_aux == NULL) {
      hospital_destruir(nuevo_hospital);
      fclose(fp);
      return NULL;
    }
    resultado =
        pokemon_inserta_ordenado_por_salud(nuevo_hospital->pokemones, poke_aux);
    if (resultado == ERROR) {
      hospital_destruir(nuevo_hospital);
      fclose(fp);
      return NULL;
    }
  }
  fclose(fp);
  if (resultado == 1) {
    hospital_destruir(nuevo_hospital);
    return NULL;
  }
  return nuevo_hospital;
}

size_t hospital_cantidad_pokemones(hospital_t *hospital) {
  if (hospital == NULL)
    return 0;
  return lista_tamanio(hospital->pokemones);
}

size_t hospital_a_cada_pokemon(hospital_t *hospital,
                               bool (*funcion)(pokemon_t *p, void *aux),
                               void *aux) {
  if (hospital == NULL)
    return 0;

  if (funcion == NULL)
    return 0;

  lista_iterador_t *iterador = lista_iterador_crear(hospital->pokemones);
  if (iterador == NULL)
    return 0;

  size_t cantidad = 0;

  while (lista_iterador_tiene_siguiente(iterador)) {
    pokemon_t *pokemon = lista_iterador_elemento_actual(iterador);
    cantidad++;
    if (!funcion(pokemon, aux))
      break;
    lista_iterador_avanzar(iterador);
  }

  lista_iterador_destruir(iterador);

  return cantidad;
}

int hospital_aceptar_emergencias(hospital_t *hospital,
                                 pokemon_t **pokemones_ambulancia,
                                 size_t cant_pokes_ambulancia) {
  if (hospital == NULL || pokemones_ambulancia == NULL)
    return ERROR;

  if (cant_pokes_ambulancia == 0)
    return EXITO;

  for (size_t i = 0; i < cant_pokes_ambulancia; i++) {
    pokemon_inserta_ordenado_por_salud(hospital->pokemones,
                                       pokemones_ambulancia[i]);
  }

  return EXITO;
}

pokemon_t *hospital_obtener_pokemon(hospital_t *hospital, size_t prioridad) {
  if (hospital == NULL || prioridad > hospital_cantidad_pokemones(hospital) - 1)
    return NULL;

  return lista_elemento_en_posicion(hospital->pokemones, prioridad);
}