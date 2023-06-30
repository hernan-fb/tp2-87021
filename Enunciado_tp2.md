<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

## TP2

### Compilar Pruebas TDA Menu
Para compilar ejecute el siguiente comando:
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -g pruebas_tda.c src/tp1.c src/menu.c src/utils.c src/hash.c src/lista.c src/dibuja.c src/pokemon.c -g -o pruebas_tda

### Compilar Tp2
Para compilar ejecute el siguiente comando:
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -g tp2.c src/tp1.c src/menu.c src/utils.c src/hash.c src/lista.c src/dibuja.c src/pokemon.c -g -o tp2

### ejecutar
* pruebas de tda:
  ./pruebas_tda
* tp2
  /tp2

### Desarrollo

- Utilice el `tp1.c` ya implementado en el TP1. Explique cómo se podría reemplazar el vector dinámico de pokemones en la estructura `hospital_t` por algún otro TDA visto en la materia. ¿Que TDA conviene utilizar? ¿Que problema habría la reemplazar dicha estructura? ¿Habría algún problema con las pruebas? Justificar la respuesta. Selecciones por lo menos 2 TDA de los vistos en la materia y arme una tabla con la complejidad de cada una de las operaciones de `tp1.h` si se utilizara dicho TDA. (Por ejemplo, si usaramos una lista enlazada, la complejidad de crear el hospital es tanto por tal cosa, de recorrer el hospital es tal otra complejidad, etc).

Si deseamos reemplazar el vector dinámico de pokemones en la estructura hospital_t por algún otro TDA, podemos considerar el uso de una lista enlazada, una cola de prioridad (heap) y un hash. Todos estos TDAs pueden ser adecuados en cierta medida según las necesidades específicas.

Al reemplazar el vector dinámico de pokemones por otro TDA, pueden surgir algunos problemas, especialmente dificultad para acceder a elementos específicos: Si el programa necesita acceder a elementos en posiciones arbitrarias del hospital de manera eficiente, reemplazar el vector dinámico por una lista enlazada o una cola de prioridad puede ser problemático, ya que no permiten un acceso directo a elementos en una posición determinada sin recorrer la estructura.
Análisis de cambios en la complejidad de las operaciones:  Dependiendo del TDA utilizado, las complejidades de las operaciones pueden cambiar.

| TDA Hospital con... | Crear Hospital | Cantidad Pokemones | A Cada Pokemon | Aceptar Emergencias | Obtener Pokemon | Destruir Hospital |
|---------------------|----------------|--------------------|----------------|---------------------|-----------------|-------------------|
| Vector dinámico     | O(1)           | O(1)               | O(n)           | O(m + n)            | O(1)            | O(1)              |
| Lista enlazada      | O(1)           | O(1)               | O(n)           | O(m + n)            | O(n)            | O(n)              |
| Heap binario        | O(1)           | O(1)               | no se puede ** | O(m * log (n+m))    | no se puede *** | O(n)              |
| Tabla de hash       | O(1)           | O(1)               | no se puede ** | O(m)                | O(1)            | O(n)              |

(**) no se puede porque para aplicar esta funcion en una forma ordenada en función del estado de salud de los pokemones, requeriría que yo pueda tener ordenada la estructura de datos, o bien que tenga una estructura adicional para saber cuál es el orden de salud de los pokemones, lo cual sería a su vez contradecir el espíritu del enunciado que pide no utilizar un vector dinámico, o bien, si uso una lista enlazada para complementar el hash o el heap, ¿por qué no usar una lista enlazada y listo?
(***) no se puede implementar porque el heap que vimos en clase es una estructura de datos orientada a obtener los extremos según sea un heap mínimo o máximo. Y para estar seguro de los valores buscados se necesitaría de una estructura auxiliar. Y después iterar muchas veces hasta encontrar el elemento buscado.

#### Lista enlazada:
* Ventajas: Permite la inserción y eliminación eficiente de elementos en cualquier posición de la lista. No requiere un tamaño inicial fijo y puede crecer dinámicamente según sea necesario. Es útil si se requieren operaciones de inserción y eliminación frecuentes en distintas posiciones de la lista.
* Desventajas: La búsqueda de un elemento en una lista enlazada puede tener una complejidad lineal en el peor caso, ya que se debe recorrer la lista secuencialmente. Esto puede ser un inconveniente si se necesita acceder a elementos específicos de manera eficiente.

#### Cola de prioridad (heap):
* Desventajas: No permite el acceso directo a elementos en posiciones arbitrarias como lo hace una lista enlazada. Si se necesitan realizar operaciones de inserción y eliminación en posiciones específicas de la lista, una cola de prioridad no sería la opción más adecuada.

#### Tabla hash:
* Ventajas: Permite un acceso rápido y eficiente a los elementos utilizando una función de hash. Las operaciones de búsqueda, inserción y eliminación tienen una complejidad promedio constante (O(1)) en condiciones ideales. Es útil cuando se necesita un acceso rápido a elementos específicos según una clave.
* Desventajas: Puede requerir un mayor consumo de memoria en comparación con una lista enlazada o una cola de prioridad. La complejidad de las operaciones puede aumentar en el peor caso si ocurren colisiones frecuentes en la tabla hash, lo que puede afectar el rendimiento.


- Cree una aplicación (`tp2.c`) que haga uso de menúes para la interacción con el usuario. Se deja a cargo del alumno la presentación visual del trabajo, pero se deben incorporar las siguientes opciones:
  - S (salir/exit): Sale del programa (obviamente que libera toda la memoria).
  - H (ayuda/help): Muestra un menú de ayuda con los comandos disponibles.
  - C (cargar): Pide un nombre de archivo e intenta cargarlo creando un hospital. El hospital queda identificado con un número y el nombre del archivo.
  - E (estado): Muestra un listado con los hospitales cargados (y el activo, si alguno).
  - A (activar): Pide un número de identificación y activa el hospital. El resto de las operaciones se realizan sobre el hospital activo.
  - M (mostrar): Muestra un listado con los nombres de todos los pokemones en el hospital.
  - L (listar): Muestra un listado detallado de todos los pokemones en el hospital.
  - D (destruir): Destruye el hospital activo.

- Proponga uno o mas `TDAs` que faciliten la creación de los menúes interactivos por consola (por ejemplo, `TDA Menu` o `TDA Opcion`). Estos `TDA` pueden hacer uso de los `TDAs` implementados en la materia y no tienen permitido usar vectores dinámicos o estáticos (si se permiten strings, obvio). Defina la interfaz y explique las operaciones del `TDA`.

- Incluya pruebas para el `TDA` creado.

- Tenga en cuenta que el usuario no tiene por qué saber cómo manejar su aplicación ni saber qué está sucediendo. Muestre por pantalla la información apropiada para guiar al usuario sin ser molesto.

- Incluya en el informe todas las suposiciones que se hayan tomado (cosas que no queden claras en el enunciado por ejemplo) y diagramas de memoria explicando cómo se almacenan los datos.

El código que proporcionado consta de varios archivos, cada uno con una funcionalidad específica. A continuación, doy una explicación simple y clara de cada uno de ellos:

#### menu.c y menu.h
  * `menu_crear`: Esta función se encarga de crear un nuevo menú. Recibe como parámetros un booleano para indicar si se deben mostrar los comandos previos, un arreglo con las rutas de las imágenes, la cantidad de imágenes, y un puntero a una función de acción en caso de error dentro del menú. Esta función reserva memoria para una estructura menu_t, inicializa sus campos y crea dos tablas hash (hash_nombre_opcion y hash_letra_nombre). Retorna el puntero al menú creado.

  * `opcion_destruir`: Esta función es utilizada para destruir una opción del menú. Recibe un puntero a una opción y libera la memoria asignada a sus campos.

  * `menu_destruir`: Esta función se encarga de destruir el menú y liberar la memoria asignada. Libera la memoria de las tablas hash y de la estructura menu_t.

  * `menu_agregar_opcion`: Esta función se encarga de agregar una opción al menú. Recibe como parámetros un puntero al menú, el nombre de la opción, la descripción de la opción, un carácter para el alias de la opción y un puntero a una función de acción, acción a realizar al ser seleccionada la opción en cuestión. Se realizan varias validaciones y luego se inserta la opción en 2 tablas hash del menú utilizando el nombre y el alias como claves.

  * `imprime_opcion_de_menu`: Esta función se utiliza como función de callback en la función hash_con_cada_clave, que a su vez se utilizan en menu_mostrar (para mostrar las opciones del menu por pantalla). Imprime por pantalla la descripción y el alias de una opción del menú.

  * `menu_mostrar`: Esta función muestra el menú por pantalla. Primero limpia la pantalla si no se deben mostrar los comandos previos. Luego, si hay imágenes en el menú, muestra una imagen aleatoria. A continuación, imprime los comandos disponibles utilizando la función hash_con_cada_clave con imprime_opcion_de_menu como función de callback.

  * `menu_cuantas_opciones_fueron_agregadas`: Esta función retorna la cantidad de opciones que han sido agregadas al menú. Utiliza la función hash_cantidad para obtener el tamaño de la tabla hash hash_letra_nombre.

  * `limpiar_buffer_entrada`: Esta función se encarga de limpiar el búfer de entrada. Lee caracteres del búfer hasta encontrar un salto de línea o llegar al final del archivo.
  Esta función se agregó ya que en ocasiones el buffer de entrada quedaba con información y la pantalla se reiniciaba sola sin permitir al usuario ver los resultados de las operaciones que acababa de realizar.

  * `menu_get_eleccion_usuario`: Esta función muestra por pantalla un mensaje para que el usuario ingrese una opción. Lee la opción ingresada por el usuario y realiza las validaciones correspondientes. Luego, utiliza las tablas hash para obtener la opción seleccionada y ejecutar su función de acción.

#### dibuja.c y dibuja.h
 
  * `dibuja_imagen` abre un archivo de imagen ASCII, lee cada línea del archivo y la muestra en la consola, y luego cierra el archivo.

  * el denominado `ascii art` que se puede apreciar al utilizar este trabajo práctico, se realizó mediante `jp2a`. Esta es una herramienta de línea de comandos que convierte imágenes en ASCII utilizando los códigos de escape ANSI para formatear colores, los cuales son compatibles con printf y en la terminal de Linux. Funciona con archivos JPEG y PNG. También permite generar una salida en color y seleccionar un conjunto de caracteres para representar la imagen en ASCII.
    Para utilizar jp2a, primero debes instalarlo ejecutando el siguiente comando:
    - sudo apt install jp2a
    Una vez instalado, puedes obtener una salida en color y guardar el texto ASCII en un archivo de la siguiente manera:
    - jp2a --output=ascii.txt --colors input.png
    Este comando tomará la imagen de entrada "input.png", generará una representación en ASCII y la guardará en el archivo "ascii.txt". La opción "--colors" permite conservar los colores de la imagen original en la salida ASCII.

#### utils.c y utils.h

  * `convertirCapital`: Esta función convierte la primera letra de una cadena de caracteres en mayúscula y las demás letras en minúscula. Además, también convierte la primera letra de cada oración (separada por ". ") en mayúscula. Utiliza las funciones toupper() y tolower() para realizar las conversiones.

  * `convertirMinusculas`: Esta función convierte todas las letras de una cadena de caracteres en minúsculas. Utiliza la función tolower() para realizar la conversión.

  * `reemplazarPalabra`: Esta función busca una palabra específica (palabraBuscada) en un texto dado (texto) y la reemplaza con otra palabra (palabraReemplazo). Utiliza las funciones strstr() para encontrar la posición de la palabra buscada y memmove() y memcpy() para realizar el reemplazo.

  * `subrayarPalabra`: Esta función subraya una palabra específica (palabra_a_Subrayar) en un texto dado (texto). Crea una nueva cadena de caracteres con la palabra subrayada utilizando los códigos de escape ANSI para el subrayado. Luego, utiliza la función reemplazarPalabra() para reemplazar la palabra original con la palabra subrayada.

































"menu.h": Este archivo es un encabezado que contiene la declaración de la estructura menu_t y las funciones relacionadas con el manejo de menús, como la creación, destrucción y manipulación de opciones del menú.

"utils.h": Este archivo es otro encabezado que contiene algunas funciones de utilidad utilizadas en el programa, como la función convertirMinusculas, que convierte una cadena de caracteres a minúsculas, y la función subrayarPalabra, que subraya una palabra específica dentro de una cadena.

"dibuja.h": Este archivo es un encabezado que contiene la declaración de la función dibuja_imagen, que se encarga de mostrar una imagen en la consola.

"hash.h": Este archivo es un encabezado que contiene la declaración de la estructura hash_t y las funciones relacionadas con el manejo de tablas hash, como la creación, destrucción e inserción de elementos en el hash.

"menu.c": Este archivo contiene la implementación de las funciones declaradas en "menu.h". Aquí se definen las funciones para crear un menú, agregar opciones, mostrar el menú, obtener la elección del usuario, etc. También se implementan las funciones auxiliares para destruir opciones del menú y mostrar información en pantalla.

"utils.c": Este archivo contiene la implementación de las funciones declaradas en "utils.h". Aquí se define la función convertirMinusculas, que convierte una cadena de caracteres a minúsculas, y la función subrayarPalabra, que subraya una palabra específica dentro de una cadena.

"dibuja.c": Este archivo contiene la implementación de la función dibuja_imagen declarada en "dibuja.h". Aquí se define cómo se muestra una imagen en la consola.

"hash.c": Este archivo contiene la implementación de las funciones declaradas en "hash.h". Aquí se define la estructura y las funciones para crear, destruir y manipular tablas hash.

Estos archivos se utilizan conjuntamente para implementar un programa que permite crear y mostrar menús interactivos en la consola, con opciones que el usuario puede seleccionar. Cada opción del menú tiene un nombre, una descripción, una letra de acceso rápido y una función de acción asociada que se ejecuta cuando el usuario elige esa opción.