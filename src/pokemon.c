#include "pokemon.h"
#include <string.h>
#include <stdio.h>
#include "pokemon_privado.h"

pokemon_t *pokemon_crear_desde_string(const char *string)
{
	// * <ID>,<NOMBRE>,<SALUD>,<NOMBRE ENTRENADOR>
	if (string == NULL || strcmp(string, "") == 0) {
		return NULL;
	}
	pokemon_t *nuevo_poke = malloc(sizeof(pokemon_t));
	if (nuevo_poke == NULL) {
		return NULL;
	}
	int parametros_leidos = sscanf(string, "%zu,%[^,],%zu,%s",
				       &nuevo_poke->id, nuevo_poke->nombre,
				       &nuevo_poke->salud,
				       nuevo_poke->nombre_entrenador);
	if (parametros_leidos != 4) {
		free(nuevo_poke);
		return NULL;
	}

	return nuevo_poke;
}

pokemon_t *pokemon_copiar(pokemon_t *poke_origen)
{
	pokemon_t *poke_destino;
	if (poke_origen == NULL)
		return NULL;
	poke_destino = malloc(sizeof(pokemon_t));
	if (poke_destino == NULL) {
		return NULL;
	}
	poke_destino->id = poke_origen->id;
	poke_destino->salud = poke_origen->salud;
	strcpy(poke_destino->nombre_entrenador, poke_origen->nombre_entrenador);
	strcpy(poke_destino->nombre, poke_origen->nombre);
	return poke_destino;
}

bool pokemon_son_iguales(pokemon_t *pokemon1, pokemon_t *pokemon2)
{
	if (pokemon1 == NULL || pokemon2 == NULL)
		return false;

	if (pokemon1->id != pokemon2->id) {
		return false;
	}

	if (pokemon1->salud != pokemon2->salud) {
		return false;
	}

	if (strcmp(pokemon1->nombre_entrenador, pokemon2->nombre_entrenador) !=
	    0) {
		return false;
	}

	if (strcmp(pokemon1->nombre, pokemon2->nombre) != 0) {
		return false;
	}

	return true;
}

char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;
	return pokemon->nombre;
}

char *pokemon_entrenador(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;
	return pokemon->nombre_entrenador;
}

size_t pokemon_salud(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return 0;
	return pokemon->salud;
}

size_t pokemon_id(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return 0;
	return pokemon->id;
}

void pokemon_destruir(pokemon_t *pkm)
{
	free(pkm);
}
