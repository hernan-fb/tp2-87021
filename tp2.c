#include "src/dibuja.h"
#include "src/lista.h"
#include "src/menu.h"
#include "src/pokemon.h"
#include "src/pokemon_privado.h"
#include "src/tp1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HOSPITALES 10
#define MAX_NOMBRE_ARCHIVO 100

const char *imagenes[] = {"img/_bulbasaur-solo_.txt", "img/_charmander1_.txt",
                          "img/_charmander-bis_.txt", "img/_pikatchu1_.txt",
                          "img/_squirtle-solo_.txt",  NULL};
const size_t cantidad_imagenes = 5;
const char *imagen_bienvenida[] = {"img/_bienvenido_a_color_.txt"};
bool flag = false;
typedef enum { BULBASAUR, CHARMANDER, SQUIRTLE, PIKACHU } pokebola_t;

typedef struct {
  int id;
  char nombreArchivo[MAX_NOMBRE_ARCHIVO];
  hospital_t *hospital;
} hospital_info_t;

hospital_info_t hospitales[MAX_HOSPITALES];
int hospitales_cargados = 0;
int hospital_activo = -1;

int obtener_numero_hospital_activo() {
  if (hospital_activo == -1) {
    printf("No hay hospital activo.\n");
    return -1;
  }
  return hospital_activo;
}

bool comando_salir() {
  // Liberar la memoria de los hospitales cargados
  for (int i = 0; i < hospitales_cargados; i++) {
    hospital_destruir(hospitales[i].hospital);
  }
  return false;
}

bool comando_cargar() {
  char nombreArchivo[MAX_NOMBRE_ARCHIVO];
  printf("Ingrese el nombre del archivo: ");
  scanf("%s", nombreArchivo);

  if (hospitales_cargados >= MAX_HOSPITALES) {
    printf("No se pueden cargar más hospitales. Límite alcanzado.\n");
    return true;
  }

  hospital_t *hospital = hospital_crear_desde_archivo(nombreArchivo);
  if (hospital == NULL) {
    printf("No se pudo cargar el hospital desde el archivo.\n");
    return true;
  }

  hospitales[hospitales_cargados].id = hospitales_cargados + 1;
  strcpy(hospitales[hospitales_cargados].nombreArchivo, nombreArchivo);
  hospitales[hospitales_cargados].hospital = hospital;
  hospitales_cargados++;

  printf("Hospital cargado exitosamente.\n");
  return true;
}

bool comando_estado() {
  printf("Hospitales cargados:\n");
  for (int i = 0; i < hospitales_cargados; i++) {
    printf("%d. %s\n", hospitales[i].id, hospitales[i].nombreArchivo);
  }

  int numero_hospital_activo = obtener_numero_hospital_activo();
  if (numero_hospital_activo != -1) {
    printf("Hospital activo: %d. %s\n", hospitales[numero_hospital_activo].id,
           hospitales[numero_hospital_activo].nombreArchivo);
  }
  return true;
}

bool comando_activar() {
  int id;
  printf("Ingrese el número de identificación del hospital a activar: ");
  scanf("%d", &id);

  if (id < 1 || id > hospitales_cargados) {
    printf("Número de hospital inválido.\n");
    return true;
  }

  hospital_activo = id - 1;
  printf("Hospital activado: %d. %s\n", hospitales[hospital_activo].id,
         hospitales[hospital_activo].nombreArchivo);
  return true;
}

bool wrapper_imprimir_pokemon(void *poke, void *contexto) {
  printf("Este es el pokemon \tid: \t\t%zu,\n\t\t\tNombre: "
         "\t%s\n\t\t\tEntrenador:\t%s\n\t\t\tSalud:\t\t%zu\n",
         pokemon_id((pokemon_t *)poke), pokemon_nombre((pokemon_t *)poke),
         pokemon_entrenador((pokemon_t *)poke),
         pokemon_salud((pokemon_t *)poke));
  return true;
}
bool wrapper_imprimir_nombre_pokemon(void *poke, void *contexto) {
  printf("Este es el pokemon \tNombre: \t%s\n",
         pokemon_nombre((pokemon_t *)poke));
  return true;
}

void listar_pokemones(hospital_t *hospital, bool wrapper(void *, void *)) {
  lista_con_cada_elemento(hospital->pokemones, wrapper, NULL);
  return;
}

bool comando_listar_nombres() {
  int numero_hospital_activo = obtener_numero_hospital_activo();
  if (numero_hospital_activo != -1) {
    hospital_t *hospital = hospitales[numero_hospital_activo].hospital;
    listar_pokemones(hospital, wrapper_imprimir_nombre_pokemon);
  }
  return true;
}

bool comando_listar_en_detalle() {
  int numero_hospital_activo = obtener_numero_hospital_activo();
  if (numero_hospital_activo != -1) {
    hospital_t *hospital = hospitales[numero_hospital_activo].hospital;
    listar_pokemones(hospital, wrapper_imprimir_pokemon);
  }
  return true;
}

bool comando_destruir() {
  int numero_hospital_activo = obtener_numero_hospital_activo();
  if (numero_hospital_activo != -1) {
    hospital_destruir(hospitales[numero_hospital_activo].hospital);
    printf("Hospital destruido.\n");
    // Mover los hospitales restantes hacia la izquierda en el arreglo
    // hospitales
    for (int i = numero_hospital_activo; i < hospitales_cargados - 1; i++) {
      hospitales[i] = hospitales[i + 1];
    }
    hospitales_cargados--;
    hospital_activo = -1;
  }
  return true;
}

bool comando_mostrar_creditos() {
  printf("\n\n\tEste es un programa de licencia libre desarrollado por el "
         "alumno Hernán Fernández Brando\n");
  printf("\tEn la cátedra \"Mendez\" de Algoritmos y programación 2,\n");
  printf("\ten el primer ciclo lectivo del año MMXXIII.\n");
  printf("\n\tCopyright 2023 © - todos los derechos están reservados.\n");
  return true;
}

bool comando_este_menu() {
  if (flag == false) {
    flag = true;
    printf("\t ups.... eso no hizo nada! ¿quién hizo el enunciado de este tp? "
           "xD\n");

    printf("\n presione una tecla para volver al menú anterior...\n");
    limpiar_buffer_entrada();

    printf("\tEra una broma!\n");
    printf("\tPero como este menu se llama ayuda, te cuento lo siguiente:\n");
  }
  printf("\tUsar este programa es muy fácil, \n");
  printf("\tsolamente tenés que elegir la opción deseada en cada menú que te "
         "aparezca\n");
  printf("\ty seguir las instrucciones que se indiquen por pantalla.");
  printf("\n\tY para elegir una opción cualquiera tenés dos métodos:\n");
  printf(
      "\t\t1. Escribiendo toda la frase que está subrayada en esa opción.\n");
  printf("\t\t2. Escribiendo la letra que está entre paréntesis al final de "
         "cada opción.\n");
  printf("\n\n\tY listo. Que te diviertas!\n");
  printf("\n");
  return true;
}

menu_t *crear_menu_principal() {
  menu_t *menu_principal = menu_crear(false, imagenes, cantidad_imagenes, NULL);
  menu_agregar_opcion(menu_principal, "salir", "Salir del programa.", 's',
                      comando_salir);
  menu_agregar_opcion(menu_principal, "ayuda", "Muestra este menú de ayuda.",
                      'h', comando_este_menu);
  menu_agregar_opcion(menu_principal, "cargar",
                      "Desde un archivo cargar un hospital.", 'c',
                      comando_cargar);
  menu_agregar_opcion(menu_principal, "mostrar",
                      "Mostrar todos los hospitales: los cargados y el activo.",
                      'e', comando_estado);
  menu_agregar_opcion(menu_principal, "activar",
                      "Activar un hospital por su número de identificación.",
                      'a', comando_activar);
  menu_agregar_opcion(menu_principal, "listar nombres",
                      "Listar nombres de los pokemones del hospital activo.",
                      'm', comando_listar_nombres);
  menu_agregar_opcion(menu_principal, "listar en detalle",
                      "Listar en detalle los pokemones del hospital activo.",
                      'l', comando_listar_en_detalle);
  menu_agregar_opcion(menu_principal, "destruir",
                      "Destruir el hospital activo.", 'd', comando_destruir);
  return menu_principal;
}

bool comando_menu_principal() {
  menu_t *menu_principal = crear_menu_principal();
  bool continuar_jugando = true;
  while (continuar_jugando) {
    menu_mostrar(menu_principal);
    continuar_jugando = menu_get_eleccion_usuario(menu_principal);
  }
  menu_destruir(menu_principal);
  return false;
}

menu_t *crear_menu_bienvenida() {
  menu_t *menu_bienvenida = menu_crear(false, imagen_bienvenida, 1, NULL);
  menu_agregar_opcion(menu_bienvenida, "salir", "Salir del programa.", 's',
                      comando_salir);
  menu_agregar_opcion(menu_bienvenida, "jugar",
                      "Empezar a jugar con los pokes.", 'j',
                      comando_menu_principal);
  menu_agregar_opcion(menu_bienvenida, "creditos",
                      "Creditos con copyright HernánF.B.", 'c',
                      comando_mostrar_creditos);
  return menu_bienvenida;
}

int main() {
  char opcion;

  menu_t *menu_bienvenida = crear_menu_bienvenida();
  bool continuar_jugando = true;
  while (continuar_jugando) {
    menu_mostrar(menu_bienvenida);
    continuar_jugando = menu_get_eleccion_usuario(menu_bienvenida);
  }
  menu_destruir(menu_bienvenida);
  return 0;
}