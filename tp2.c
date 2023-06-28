#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/tp1.h"
#include "src/lista.h"
#include "src/dibuja.h"

#define MAX_HOSPITALES 10
#define MAX_NOMBRE_ARCHIVO 100

typedef struct {
    int id;
    char nombreArchivo[MAX_NOMBRE_ARCHIVO];
    hospital_t* hospital;
} hospital_info_t;

hospital_info_t hospitales[MAX_HOSPITALES];
int hospitales_cargados = 0;
int hospital_activo = -1;

void mostrar_menu_ayuda() {
	dibuja_imagen_poke(PIKACHU);
    printf("Comandos disponibles:\n");
    printf("  S (salir/exit): Sale del programa.\n");
    printf("  H (ayuda/help): Muestra este menú de ayuda.\n");
    printf("  C (cargar): Carga un hospital desde un archivo.\n");
    printf("  E (estado): Muestra los hospitales cargados y el activo.\n");
    printf("  A (activar): Activa un hospital por su número de identificación.\n");
    printf("  M (mostrar): Muestra los nombres de todos los pokemones en el hospital activo.\n");
    printf("  L (listar): Muestra un listado detallado de todos los pokemones en el hospital activo.\n");
    printf("  D (destruir): Destruye el hospital activo.\n");
}

int obtener_numero_hospital_activo() {
    if (hospital_activo == -1) {
        printf("No hay hospital activo.\n");
        return -1;
    }
    return hospital_activo;
}

void comando_salir() {
    // Liberar la memoria de los hospitales cargados
    for (int i = 0; i < hospitales_cargados; i++) {
        destruir_hospital(hospitales[i].hospital);
    }
    exit(0);
}

void comando_cargar() {
    char nombreArchivo[MAX_NOMBRE_ARCHIVO];
    printf("Ingrese el nombre del archivo: ");
    scanf("%s", nombreArchivo);

    if (hospitales_cargados >= MAX_HOSPITALES) {
        printf("No se pueden cargar más hospitales. Límite alcanzado.\n");
        return;
    }

    hospital_t* hospital = hospital_crear_desde_archivo(nombreArchivo);
    if (hospital == NULL) {
        printf("No se pudo cargar el hospital desde el archivo.\n");
        return;
    }

    hospitales[hospitales_cargados].id = hospitales_cargados + 1;
    strcpy(hospitales[hospitales_cargados].nombreArchivo, nombreArchivo);
    hospitales[hospitales_cargados].hospital = hospital;
    hospitales_cargados++;

    printf("Hospital cargado exitosamente.\n");
}

void comando_estado() {
    printf("Hospitales cargados:\n");
    for (int i = 0; i < hospitales_cargados; i++) {
        printf("%d. %s\n", hospitales[i].id, hospitales[i].nombreArchivo);
    }

    int numero_hospital_activo = obtener_numero_hospital_activo();
    if (numero_hospital_activo != -1) {
        printf("Hospital activo: %d. %s\n", hospitales[numero_hospital_activo].id, hospitales[numero_hospital_activo].nombreArchivo);
    }
}

void comando_activar() {
    int id;
    printf("Ingrese el número de identificación del hospital a activar: ");
    scanf("%d", &id);

    if (id < 1 || id > hospitales_cargados) {
        printf("Número de hospital inválido.\n");
        return;
    }

    hospital_activo = id - 1;
    printf("Hospital activado: %d. %s\n", hospitales[hospital_activo].id, hospitales[hospital_activo].nombreArchivo);
}

bool wrapper_imprimir_pokemon(pokemon_t *poke, void* contexto) {
	printf("Este es el pokemon id: %zu,\n\t\tNombre: \t%s\n\t\tEntrenador: %s\n\t\tSalud: \t%zu\n", 
	pokemon_id(poke), 
	pokemon_nombre(poke),
	pokemon_entrenador(poke),
	pokemon_salud(poke));
	return true;
}

void comando_mostrar() {
    int numero_hospital_activo = obtener_numero_hospital_activo();
    if (numero_hospital_activo != -1) {
        hospital_t* hospital = hospitales[numero_hospital_activo].hospital;
		listar_pokemones(hospital);
    }
	return;
}

void listar_pokemones(hospital_t* hospital) {
    lista_con_cada_elemento(hospital->pokemones, wrapper_imprimir_pokemon, NULL);
	return;
}

void comando_listar() {
    int numero_hospital_activo = obtener_numero_hospital_activo();
    if (numero_hospital_activo != -1) {
        hospital_t* hospital = hospitales[numero_hospital_activo].hospital;
        listar_pokemones(hospital);
    }
}

void comando_destruir() {
    int numero_hospital_activo = obtener_numero_hospital_activo();
    if (numero_hospital_activo != -1) {
        destruir_hospital(hospitales[numero_hospital_activo].hospital);
        printf("Hospital destruido.\n");
        // Mover los hospitales restantes hacia la izquierda en el arreglo hospitales
        for (int i = numero_hospital_activo; i < hospitales_cargados - 1; i++) {
            hospitales[i] = hospitales[i + 1];
        }
        hospitales_cargados--;
        hospital_activo = -1;
    }
}

int main() {
    char opcion;
    mostrar_menu_ayuda();

    while (1) {
        printf("\nIngrese una opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
            case 'S':
                comando_salir();
                break;
            case 'H':
                mostrar_menu_ayuda();
                break;
            case 'C':
                comando_cargar();
                break;
            case 'E':
                comando_estado();
                break;
            case 'A':
                comando_activar();
                break;
            case 'M':
                comando_mostrar();
                break;
            case 'L':
                comando_listar();
                break;
            case 'D':
                comando_destruir();
                break;
            default:
                printf("Opción inválida. Ingrese 'H' para ver los comandos disponibles.\n");
                break;
        }
    }

    return 0;
}