#include "hash.h"
#include "lista.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FACTOR_CARGA_MAXIMO 0.7F
#define CAPACIDAD_INICIAL_TABLA_HASH 3
#define CAPACIDAD_BUCKETS 3 // : b
#define CANTIDAD_DE_FUNCIONES_HASH 2 // : h
#define COEFICIENTE_REDIMENSION_POR_FACTOR_DE_CARGA 0.5
#define LARGO_LISTADO_NUMERO_PRIMOS 7
#define FUNCION_HASH_UTILIZADA 1
typedef size_t (*puntero_funcion_hash_t)(const void *, size_t);
typedef struct clave_valor clave_valor_t;
size_t hash_01(const void *, size_t);
size_t hash_02(const void *, size_t);
size_t hash_04(const void *, size_t);
size_t hash_03(const void *, size_t);
void destruir_estructura_clave_y_valor(void *tda);
void destruir_estructura_clave_pero_no_el_valor(void *tda);
bool chequear_factor_de_carga_y_redimensionar(hash_t *hash,
					      bool aumenta_el_tamanio);
hash_t *hash_guardar_clave_valor(hash_t *hash, clave_valor_t *clave_valor);
bool hash_redimensionar(hash_t *hash, bool aumenta_el_tamanio);
#define FNV_PRIME_64 1099511628211U
#define FNV_OFFSET_64 14695981039346656037U

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;
struct clave_valor {
	void *clave;
	void *valor;
};

struct hash {
	lista_t **tabla;
	size_t tamanio;
	size_t cantidad;
};
struct hash_iter {
	const hash_t *hash;
	size_t indice_lista_actual;
	lista_iterador_t *iter_posicion_actual;
};
struct lista {
	nodo_t *primero;
	nodo_t *ultimo;
	size_t tam;
};
struct lista_iterador {
	nodo_t *actual;
	lista_t *lista;
};

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = malloc(sizeof(hash_t));
	if (hash == NULL) {
		return NULL;
	}
	hash->tamanio = capacidad < CAPACIDAD_INICIAL_TABLA_HASH ?
				CAPACIDAD_INICIAL_TABLA_HASH :
				capacidad;
	hash->cantidad = 0;
	hash->tabla = calloc(hash->tamanio, sizeof(lista_t **));

	if (hash->tabla == NULL) {
		free(hash);
		return NULL;
	}
	for (size_t i = 0; i < hash->tamanio; ++i) {
		hash->tabla[i] = lista_crear();
	}

	return hash;
}

lista_iterador_t *hash_buscar(const hash_t *hash, const char *clave)
{
	clave_valor_t *actual;
	if (!hash || !clave) {
		return NULL;
	}
	puntero_funcion_hash_t array_funciones_hash[5] = { hash_01, hash_02,
							   hash_03, hash_04,
							   NULL };
	size_t posicion = array_funciones_hash[FUNCION_HASH_UTILIZADA](
		clave, hash->tamanio);
	lista_iterador_t *iter = lista_iterador_crear(hash->tabla[posicion]);
	while (lista_iterador_tiene_siguiente(iter)) {
		actual = lista_iterador_elemento_actual(iter);
		if (strcmp(clave, actual->clave) == 0) {
			return iter;
		}
		lista_iterador_avanzar(iter);
	}
	lista_iterador_destruir(iter);
	return NULL;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;
	lista_iterador_t *clave_valor_en_iter = hash_buscar(hash, clave);
	if (clave_valor_en_iter) {
		if (anterior) {
			*anterior =
				((clave_valor_t *)lista_iterador_elemento_actual(
					 clave_valor_en_iter))
					->valor;
		}
		((clave_valor_t *)lista_iterador_elemento_actual(
			 clave_valor_en_iter))
			->valor = elemento;
		lista_iterador_destruir(clave_valor_en_iter);
		return hash;
	}
	lista_iterador_destruir(clave_valor_en_iter);
	if (anterior) {
		*anterior = NULL;
	}
	clave_valor_t *clave_valor = malloc(sizeof(clave_valor_t));
	if (!clave_valor) {
		return NULL;
	}
	clave_valor->clave = malloc(sizeof(char) * (strlen(clave) + 1));
	if (clave_valor->clave == NULL) {
		free(clave_valor);
		return NULL;
	}
	strcpy(clave_valor->clave, clave);
	clave_valor->valor = elemento;
	if (!chequear_factor_de_carga_y_redimensionar(hash, true)) {
		return NULL;
	}

	return hash_guardar_clave_valor(hash, clave_valor);
}

hash_t *hash_guardar_clave_valor(hash_t *hash, clave_valor_t *clave_valor)
{
	puntero_funcion_hash_t array_funciones_hash[5] = { hash_01, hash_02,
							   hash_03, hash_04,
							   NULL };
	size_t resultado_hash = array_funciones_hash[FUNCION_HASH_UTILIZADA](
		clave_valor->clave, hash->tamanio);
	if (!hash->tabla[resultado_hash]) {
		hash->tabla[resultado_hash] = lista_crear();
	}
	hash->cantidad++;
	if (lista_insertar_en_posicion(
		    hash->tabla[resultado_hash], clave_valor,
		    lista_tamanio(hash->tabla[resultado_hash]))) {
		return hash;
	}
	return NULL;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash)
		return NULL;
	void *valor = NULL;
	lista_iterador_t *iter = hash_buscar(hash, clave);
	if (iter) {
		valor = ((clave_valor_t *)lista_iterador_elemento_actual(iter))
				->valor;
		destruir_estructura_clave_pero_no_el_valor(
			lista_iterador_borrar(iter));
		hash->cantidad--;
		if (!chequear_factor_de_carga_y_redimensionar(hash, false)) {
			return NULL;
		}
	}
	lista_iterador_destruir(iter);
	return valor;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	lista_iterador_t *dato_en_iter = hash_buscar(hash, clave);
	if (dato_en_iter) {
		clave_valor_t *actual =
			lista_iterador_elemento_actual(dato_en_iter);
		lista_iterador_destruir(dato_en_iter);
		return actual->valor;
	}
	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash)
		return false;

	lista_iterador_t *dato_en_iter = hash_buscar(hash, clave);
	if (dato_en_iter) {
		lista_iterador_destruir(dato_en_iter);
		return true;
	}
	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (hash)
		return hash->cantidad;
	return 0;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;
	lista_t *bucket;
	for (size_t i = 0; i < hash->tamanio; i++) {
		bucket = hash->tabla[i];
		lista_iterador_t *iter = lista_iterador_crear(bucket);
		while (hash->cantidad > 0 &&
		       lista_iterador_tiene_siguiente(iter)) {
			clave_valor_t *clave_valor =
				lista_iterador_borrar(iter);
			if (destructor)
				destructor(clave_valor->valor);
			if (clave_valor)
				destruir_estructura_clave_pero_no_el_valor(
					clave_valor);
			hash->cantidad--;
		}
		lista_iterador_destruir(iter);
		lista_destruir(bucket);
	}
	free(hash->tabla);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t n = 0;
	if (!hash || !f)
		return n;
	lista_t *bucket;
	bool resultado_de_funcion = true;
	for (size_t i = 0; i < hash->tamanio && resultado_de_funcion; i++) {
		bucket = hash->tabla[i];
		lista_iterador_t *iter = lista_iterador_crear(bucket);
		while (lista_iterador_tiene_siguiente(iter)) {
			clave_valor_t *clave_valor =
				lista_iterador_elemento_actual(iter);
			n++;
			resultado_de_funcion = f((char *)clave_valor->clave,
						 clave_valor->valor, aux);
			if (!resultado_de_funcion) {
				break;
			}
			lista_iterador_avanzar(iter);
		}
		lista_iterador_destruir(iter);
	}
	return n;
}

size_t hash_01(const void *clave, size_t largo_tabla_hash)
{
	// código FNV Hashing. Ref: http://ctips.pbworks.com/w/page/7277591/FNV%20Hash
	size_t n = strlen((char *)clave);
	uint_fast64_t hash = FNV_OFFSET_64;
	for (size_t i = 0; i < n; i++) {
		hash = hash ^
		       (((unsigned char *)clave)
				[i]); // xor next byte into the bottom of the hash
		hash = hash *
		       FNV_PRIME_64; // Multiply by prime number found to work well
	}
	return (size_t)(hash % largo_tabla_hash);
}

size_t hash_02(const void *clave, size_t largo_tabla_hash)
{
	// Jenkins Hashing
	// Ref: https://en.wikipedia.org/wiki/Jenkins_hash_function
	size_t i = 0, n = strlen((char *)clave);
	uint32_t hash = 0;
	while (i != n) {
		hash += ((unsigned char *)clave)[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return (size_t)(hash % largo_tabla_hash);
}

size_t hash_04(const void *clave, size_t largo_tabla_hash)
{
	// murmur hash, ref:
	// http://bitsquid.blogspot.com/2011/08/code-snippet-murmur-hash-inverse-pre.html
	const uint_fast64_t m = 0xc6a4a7935bd1e995ULL;
	const int r = 47;

	uint_fast64_t h = FNV_PRIME_64 ^ (largo_tabla_hash * m);

	const uint_fast64_t *data = (const uint_fast64_t *)clave;
	const uint_fast64_t *end =
		data +
		(strlen(clave) / 8); // originariamente decía data + (largo/8);
	// size_t i = 0;
	while (data != end) // originariamente decía (data != end)
	{
#ifdef PLATFORM_BIG_ENDIAN
		uint_fast64_t k = *data++;
		char *p = (char *)&k;
		char c;
		c = p[0];
		p[0] = p[7];
		p[7] = c;
		c = p[1];
		p[1] = p[6];
		p[6] = c;
		c = p[2];
		p[2] = p[5];
		p[5] = c;
		c = p[3];
		p[3] = p[4];
		p[4] = c;
#else
		uint_fast64_t k = *data++; // originariamente decía *data++;
			//	i++;
#endif

		k *= m;
		k ^= k >> r;
		k *= m;

		h ^= k;
		h *= m;
	}

	const unsigned char *data2 = (const unsigned char *)data;

	switch (strlen(clave) & 7) {
	case 7:
		h ^= (uint_fast64_t)data2[6] << 48;
		break;
	case 6:
		h ^= (uint_fast64_t)data2[5] << 40;
		break;
	case 5:
		h ^= (uint_fast64_t)data2[4] << 32;
		break;
	case 4:
		h ^= (uint_fast64_t)data2[3] << 24;
		break;
	case 3:
		h ^= (uint_fast64_t)data2[2] << 16;
		break;
	case 2:
		h ^= (uint_fast64_t)data2[1] << 8;
		break;
	case 1:
		h ^= (uint_fast64_t)data2[0];
		h *= m;
		break;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return (size_t)(h % largo_tabla_hash);
}

size_t hash_03(const void *clave, size_t largo_tabla_hash)
{
	unsigned long hash = 5381;
	int c = *(const char *)clave;
	const char *clave_aux = clave;
	while (c) {
		hash = ((hash << 5) + hash) +
		       (unsigned long)c; /* hash * 33 + c */
		c = *(const char *)clave_aux++;
	}
	return hash % largo_tabla_hash;
}

void destruir_estructura_clave_y_valor(void *tda)
{
	clave_valor_t *estructura = tda;
	free(estructura->clave);
	free(estructura->valor);
	free(estructura);
}
void destruir_estructura_clave_pero_no_el_valor(void *tda)
{
	clave_valor_t *estructura = tda;
	free(estructura->clave);
	free(estructura);
}
size_t busquedabinaria(size_t *array, size_t inicio, size_t fin,
		       size_t elemento_buscado, bool aumenta)
{
	if (inicio > fin)
		return aumenta ? fin : inicio;
	size_t medio = (inicio + fin) / 2;

	if (array[medio] == elemento_buscado)
		return medio;
	if (array[medio] < elemento_buscado)
		return busquedabinaria(array, (size_t)medio + 1, fin,
				       elemento_buscado, aumenta);
	else
		return busquedabinaria(array, inicio, (size_t)medio - 1,
				       elemento_buscado, aumenta);
}
size_t calcula_nuevo_tamanio(hash_t *hash, bool aumenta_el_tamanio)
{
	size_t nuevo_tamanio;
	size_t lista_numeros_primos[LARGO_LISTADO_NUMERO_PRIMOS] = {
		3, 37, 197, 1973, 19709, 197003, 999983
	};

	size_t posicion_aux = busquedabinaria(lista_numeros_primos, 0,
					      LARGO_LISTADO_NUMERO_PRIMOS - 1,
					      hash->tamanio,
					      aumenta_el_tamanio);
	if (aumenta_el_tamanio) {
		if (posicion_aux == LARGO_LISTADO_NUMERO_PRIMOS - 1) {
			nuevo_tamanio = hash->tamanio * 10;
		} else {
			nuevo_tamanio = lista_numeros_primos[posicion_aux + 1];
		}
	} else {
		if (posicion_aux == 0) {
			return lista_numeros_primos[0];
		} else if (posicion_aux <= LARGO_LISTADO_NUMERO_PRIMOS - 1) {
			nuevo_tamanio = lista_numeros_primos[posicion_aux - 1];
		} else {
			nuevo_tamanio = hash->tamanio / 10;
			if (nuevo_tamanio <=
			    lista_numeros_primos[LARGO_LISTADO_NUMERO_PRIMOS -
						 1]) {
				nuevo_tamanio = lista_numeros_primos
					[LARGO_LISTADO_NUMERO_PRIMOS - 1];
			}
		}
	}
	return nuevo_tamanio;
}
bool chequear_factor_de_carga_y_redimensionar(hash_t *hash,
					      bool aumenta_el_tamanio)
{
	int aux = (int)hash_cantidad(hash);
	float factor_de_carga = (float)aux / (float)hash->tamanio;

	if (aumenta_el_tamanio && factor_de_carga > FACTOR_CARGA_MAXIMO) {
		return hash_redimensionar(hash, true);
	}
	if (!aumenta_el_tamanio) {
		size_t nuevo_tamanio = calcula_nuevo_tamanio(hash, false);
		if (nuevo_tamanio == hash->tamanio) {
			return true;
		}
		float nuevo_factor_de_carga =
			(float)aux / (float)(nuevo_tamanio * CAPACIDAD_BUCKETS);
		if (nuevo_factor_de_carga <
		    FACTOR_CARGA_MAXIMO *
			    COEFICIENTE_REDIMENSION_POR_FACTOR_DE_CARGA) {
			// return hash_redimensionar(hash, false);
		}
	}
	return true;
}

bool hash_redimensionar(hash_t *hash, bool aumenta_el_tamanio)
{
	size_t nuevo_tamanio;
	nuevo_tamanio = calcula_nuevo_tamanio(hash, aumenta_el_tamanio);
	lista_t **nueva_tabla = calloc(nuevo_tamanio, sizeof(lista_t *));
	if (!nueva_tabla) {
		return false;
	}
	//--- copio la tabla actual en un hash aux porque hash_guardar conserva el
	// mismo hash---//
	hash_t *hash_aux = malloc(sizeof(hash_t));
	hash_aux->tabla = hash->tabla;
	hash_aux->cantidad = hash_cantidad(hash);
	hash_aux->tamanio = hash->tamanio;
	//---					---------
	//---//
	hash->tabla = nueva_tabla;
	hash->cantidad = 0;
	hash->tamanio = nuevo_tamanio;
	//---					---------
	//---//
	size_t cantidad_hash_viejo = hash_cantidad(hash_aux);
	bool guardados_ok = true;

	lista_t *bucket_aux;
	for (size_t i = 0; i < hash_aux->tamanio; i++) {
		bucket_aux = hash_aux->tabla[i];
		lista_iterador_t *iter_aux = lista_iterador_crear(bucket_aux);
		while (lista_iterador_tiene_siguiente(iter_aux)) {
			clave_valor_t *clave_valor =
				lista_iterador_borrar(iter_aux);
			hash_aux->cantidad--;
			guardados_ok &= (hash_guardar_clave_valor(
						 hash, clave_valor) != NULL);
			if (!guardados_ok) {
				return false;
			}
		}
		lista_iterador_destruir(iter_aux);
	}

	bool verificacion_ok = hash_cantidad(hash) == cantidad_hash_viejo;
	verificacion_ok &= hash_cantidad(hash_aux) == 0;
	hash_destruir(hash_aux);
	if (verificacion_ok) {
		return true;
	} else {
		return false;
	}
}