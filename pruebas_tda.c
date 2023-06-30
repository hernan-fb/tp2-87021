#include "src/lista.h"
#include "src/menu.h"
#include "src/tp1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HOSPITALES 10
#define MAX_NOMBRE_ARCHIVO 100

const char *imagen_bienvenida[] = {"img/_bienvenido_a_color_.txt"};

bool comando_mostrar_creditos() {
  printf("\n\n\tEste es un programa de licencia libre desarrollado por el "
         "alumno Hernán Fernández Brando\n");
  printf("\tEn la cátedra \"Mendez\" de Algoritmos y programación 2,\n");
  printf("\ten el primer ciclo lectivo del año MMXXIII.\n");
  printf("\n\tCopyright 2023 © - todos los derechos están reservados.\n");
  return true;
}

bool comando_salir() { return false; }

bool comando_menu_principal() {
  printf("Este es el menu principal.... enjoy!\n");
  return false;
}

menu_t *pruebas_c_crear_menu_bienvenida_con_tres_opciones() {
  menu_t *menu_bienvenida = menu_crear(true, imagen_bienvenida, 1, NULL);
  menu_agregar_opcion(menu_bienvenida, "salir", "Salir del programa.", 's',
                      comando_salir);
  menu_agregar_opcion(menu_bienvenida, "play", "Empezar a jugar con los pokes.",
                      'j', comando_menu_principal);
  menu_agregar_opcion(menu_bienvenida, "creditos",
                      "Creditos con copyright HernánF.B.", 'c',
                      comando_mostrar_creditos);
  return menu_bienvenida;
}
menu_t *pruebas_b_crear_menu_bienvenida_con_tres_opciones() {
  menu_t *menu_bienvenida = menu_crear(true, imagen_bienvenida, 1, NULL);
  menu_agregar_opcion(menu_bienvenida, "salir", "Salir del programa.", 's',
                      comando_salir);
  menu_agregar_opcion(menu_bienvenida, "jugar",
                      "Empezar a jugar con los pokes.", 'j', NULL);
  menu_agregar_opcion(menu_bienvenida, "creditos",
                      "Creditos con copyright HernánF.B.", 'c', NULL);
  return menu_bienvenida;
}

void prueba_a_crear_y_destruir_menu_sin_opciones_sin_imagen() {
  menu_t *menu_bienvenida = menu_crear(true, NULL, 0, NULL);
  menu_mostrar(menu_bienvenida);
  menu_get_eleccion_usuario(menu_bienvenida);
  menu_destruir(menu_bienvenida);
}

void prueba_b_crear_y_destruir_menu_con_opciones_sin_accion() {
  menu_t *menu_bienvenida = pruebas_b_crear_menu_bienvenida_con_tres_opciones();
  menu_mostrar(menu_bienvenida);
  menu_get_eleccion_usuario(menu_bienvenida);
  menu_destruir(menu_bienvenida);
}

void prueba_c_crear_y_destruir_menu_con_opciones_invalidas() {
  menu_t *menu_bienvenida = pruebas_c_crear_menu_bienvenida_con_tres_opciones();
  bool validacion =
      menu_cuantas_opciones_fueron_agregadas(menu_bienvenida) == 2;
  printf("validacion: %s\n", validacion ? "true" : "false");
  menu_destruir(menu_bienvenida);
}

int main() {
  prueba_a_crear_y_destruir_menu_sin_opciones_sin_imagen();
  prueba_b_crear_y_destruir_menu_con_opciones_sin_accion();
  prueba_c_crear_y_destruir_menu_con_opciones_invalidas();
  return 0;
}