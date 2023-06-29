<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

## TP2

Ya estamos cerca del final de la materia. Implementaste el TP1 y varios TDAs. Ahora el objetivo es aplicar todo lo aprendido para mejorar tu TP1.


## Desarrollo

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

####Lista enlazada:
* Ventajas: Permite la inserción y eliminación eficiente de elementos en cualquier posición de la lista. No requiere un tamaño inicial fijo y puede crecer dinámicamente según sea necesario. Es útil si se requieren operaciones de inserción y eliminación frecuentes en distintas posiciones de la lista.
* Desventajas: La búsqueda de un elemento en una lista enlazada puede tener una complejidad lineal en el peor caso, ya que se debe recorrer la lista secuencialmente. Esto puede ser un inconveniente si se necesita acceder a elementos específicos de manera eficiente.

####Cola de prioridad (heap):
* Desventajas: No permite el acceso directo a elementos en posiciones arbitrarias como lo hace una lista enlazada. Si se necesitan realizar operaciones de inserción y eliminación en posiciones específicas de la lista, una cola de prioridad no sería la opción más adecuada.

####Tabla hash:
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
