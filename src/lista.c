#include "lista.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
/* ******************************************************************
 *                    PRIMITIVAS DE NODO
 * *****************************************************************/

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

nodo_t *nodo_crear(nodo_t *sigue, void *nuevo_dato)
{
	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo) {
		return NULL;
	}
	nuevo_nodo->siguiente = sigue;
	nuevo_nodo->elemento = nuevo_dato;
	return nuevo_nodo;
}

bool nodo_destruir(nodo_t *nodo, void (*destruir_tda)(void *))
{
	if (destruir_tda != NULL && nodo != NULL) {
		(*destruir_tda)(nodo->elemento);
		nodo->elemento = NULL;
	}
	free(nodo);
	return true;
}
/* ******************************************************************
 *                    PRIMITIVAS DE LISTA
 * *****************************************************************/
struct lista {
	nodo_t *primero;
	nodo_t *ultimo;
	size_t tam;
};

struct lista_iterador {
	nodo_t *actual;
	nodo_t *previo;
	lista_t *lista;
};

lista_t *lista_crear()
{
	lista_t *listita = calloc(1, sizeof(lista_t));
	if (!listita) {
		return NULL;
	}
	listita->primero = NULL;
	listita->ultimo = NULL;
	listita->tam = 0;
	return listita;
}

nodo_t *lista_llegar_a_posicion(lista_t *lista, size_t posicion)
{
	if (lista_vacia(lista) == true) {
		return NULL;
	}
	if (posicion >= lista_tamanio(lista)) {
		return lista->ultimo;
	}
	nodo_t *nodo_aux = lista->primero;
	for (size_t i = 0; i++ < posicion; nodo_aux = nodo_aux->siguiente)
		;
	return nodo_aux;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista) {
		return NULL;
	}
	nodo_t *nuevo_nodo = nodo_crear(NULL, elemento);
	if (!nuevo_nodo) {
		// ver decisiones de diseño. En el enunciado se pide devolver NULL en caso
		// de error. Yo devolvería la lista entera y validaría. Sino se insertó el
		// elemento libero y listo
		return NULL;
	}
	if (lista->tam == 0) {
		lista->primero = nuevo_nodo;
	} else {
		lista->ultimo->siguiente = nuevo_nodo;
	}
	lista->ultimo = nuevo_nodo;
	lista->tam++;
	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista) {
		return NULL;
	}
	nodo_t *nodo_anterior;
	nodo_t *nuevo_nodo;
	if (posicion == 0 || lista_vacia(lista)) {
		nodo_anterior = lista->primero;
		nuevo_nodo = nodo_crear(nodo_anterior, elemento);
	} else {
		nodo_anterior = lista_llegar_a_posicion(lista, posicion - 1);
		nuevo_nodo = nodo_crear(nodo_anterior->siguiente, elemento);
	}
	if (!nuevo_nodo) {
		// ver decisiones de diseño.
		return NULL;
	}
	if (lista->tam == 0) { // la lista estaba vacía
		lista->primero = nuevo_nodo;
		lista->ultimo = nuevo_nodo;
	} else if (posicion == 0) {
		lista->primero = nuevo_nodo;
	} else {
		if (nodo_anterior == lista->ultimo) {
			lista->ultimo = nuevo_nodo;
		}
		nodo_anterior->siguiente = nuevo_nodo;
	}
	lista->tam++;
	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (lista_vacia(lista)) {
		return NULL;
	}
	nodo_t *aux = lista->ultimo;
	if (lista_tamanio(lista) == 1) {
		lista->primero = NULL;
	} else if (lista_tamanio(lista) == 2) {
		lista->primero->siguiente = NULL;
		lista->ultimo = lista->primero;
	} else {
		nodo_t *ante_ultimo = lista_llegar_a_posicion(
			lista, lista_tamanio(lista) - 2);
		lista->ultimo = ante_ultimo;
		lista->ultimo->siguiente = NULL;
	}
	void *dato = aux->elemento;
	nodo_destruir(aux, NULL);
	lista->tam--;
	return dato;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (lista_vacia(lista)) {
		return NULL;
	}
	nodo_t *nodito = NULL;
	if (lista_tamanio(lista) == 1) {
		nodito = lista->primero;
		lista->primero = NULL;
		lista->ultimo = NULL;
	} else if (posicion == 0) {
		nodito = lista->primero;
		lista->primero = lista->primero->siguiente;
	} else {
		if (posicion > lista_tamanio(lista) - 1) {
			posicion = lista_tamanio(lista) - 1;
		}
		nodo_t *anterior = lista_llegar_a_posicion(lista, posicion - 1);
		nodito = anterior->siguiente;
		if (nodito == lista->ultimo) {
			lista->ultimo = anterior;
		}
		anterior->siguiente = nodito->siguiente;
	}
	lista->tam--;
	if (!nodito) {
		printf("hubo un error\n");
		return NULL;
	}
	void *dato = nodito->elemento;
	nodo_destruir(nodito, NULL);
	return dato;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (lista_vacia(lista)) {
		return NULL;
	}
	nodo_t *nodito = NULL;
	if (posicion == 0) {
		nodito = lista->primero;
	} else if (posicion >= lista_tamanio(lista)) {
		return NULL;
	} else {
		nodito = lista_llegar_a_posicion(lista, posicion);
	}
	return nodito->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (lista_vacia(lista) || !comparador) {
		return NULL;
	}
	nodo_t *nodito = NULL;
	for (nodito = lista->primero;
	     nodito != NULL && comparador(nodito->elemento, contexto) != 0;
	     nodito = nodito->siguiente)
		;
	if (nodito == NULL) {
		return NULL;
	} else {
		return nodito->elemento;
	}
}

void *lista_primero(lista_t *lista)
{
	if (lista_vacia(lista)) {
		return NULL;
	}
	return lista->primero->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (lista_vacia(lista)) {
		return NULL;
	}
	return lista->ultimo->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista) {
		return true;
	}
	return (lista->tam == 0);
}

size_t lista_tamanio(lista_t *lista)
{
	if (lista_vacia(lista)) {
		return 0;
	}
	return lista->tam;
}

void lista_destruir(lista_t *lista)
{
	while (!lista_vacia(lista)) {
		lista_quitar_de_posicion(lista, 0);
	}
	free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*destruir_dato)(void *))
{
	if (destruir_dato) {
		while (!lista_vacia(lista)) {
			destruir_dato(lista_quitar_de_posicion(lista, 0));
		}
	} else {
		while (!lista_vacia(lista)) {
			lista_quitar_de_posicion(lista, 0);
		}
	}
	free(lista);
}

/* ******************************************************************
 *                 PRIMITIVAS DE ITERADOR EXTERNO
 * *****************************************************************/

// Crea una iterador para una lista. Si la lista no está vacía, caso contrario
// devuelve NULL. Pre: la lista no está vacía. Post: devuelve un nuevo iterador
// vacío.
lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista) {
		return NULL;
	}
	lista_iterador_t *iter_externo = malloc(sizeof(lista_iterador_t));
	if (!iter_externo) {
		return NULL;
	}
	if (lista_vacia(lista)) {
		iter_externo->actual = NULL;
	} else {
		iter_externo->actual = lista->primero;
	}
	iter_externo->lista = lista;
	iter_externo->previo = NULL;
	return iter_externo;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador || lista_vacia(iterador->lista)) {
		return false;
	}
	return (iterador->actual != NULL);
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || lista_vacia(iterador->lista)) {
		return false;
	}
	if (!iterador->actual) { // la posición es posterior al último elemento.
		// Devuelve NULL
		return NULL;
	}
	iterador->previo = iterador->actual;
	iterador->actual = iterador->actual->siguiente;
	if (!lista_iterador_tiene_siguiente(iterador)) {
		return false;
	}
	return true;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || lista_vacia(iterador->lista) || !iterador->actual) {
		return NULL;
	}
	return iterador->actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*visitar)(void *, void *),
			       void *contexto)
{
	if (lista_vacia(lista) || !visitar) {
		return 0;
	}
	size_t contador = 0;
	for (nodo_t *iter = lista->primero; iter; iter = iter->siguiente) {
		contador++;
		if (!visitar(iter->elemento,
			     contexto)) { // no voy a acceder a iter->elemento si iter == NULL !
			break;
		}
	}
	return contador;
}

bool lista_iterador_insertar(lista_iterador_t *iter, void *dato) {
	if (!iter) {
		return false;
	}
	nodo_t* nodito = nodo_crear(iter->actual,dato);
	if(!nodito) {
		return false;
	}
	if (iter->previo != NULL) { //equivalente a decir: iter->actual != iter->lista->primero
		iter->previo->siguiente = nodito;
	}
	if (!iter->actual) { 		// equivalente a lista_insertar_ultimo
		iter->lista->ultimo = nodito;
	}
	if (iter->lista->tam == 0 || iter->actual == iter->lista->primero ) { // equivalente a lista_insertar_primero
		iter->lista->primero = nodito;
	}
	iter->actual = nodito;
	(iter->lista->tam)++;
	return true;
}

void *lista_iterador_borrar(lista_iterador_t *iter)
{
	if (!iter || !iter->actual) {
		return NULL;
	}
	if (!iter->previo) { // actualizo primer nodo
		iter->lista->primero = iter->actual->siguiente;
	} else {
		iter->previo->siguiente = iter->actual->siguiente;
	}
	if (!iter->actual->siguiente) { // actualizo ultimo nodo
		iter->lista->ultimo = iter->previo;
	}
	void *dato = iter->actual->elemento;
	nodo_t *aux = iter->actual;
	iter->actual = iter->actual->siguiente;
	free(aux);
	(iter->lista->tam)--;
	return dato;
}
