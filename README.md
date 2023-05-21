# Tarea3EDD

- Crear tarea y agregarla (tarea) ✔
- Establece precedencia entre tareas (tarea) ✔
- Muestra tareas por realizar (tarea) ✔
- Marca tareas como completadas (tarea) ✔
- Deshace la ultima accion realizada (tarea) ✔
- Cargar tareas desde un archivo de texto (nombre_archivo) ✔ ???????????

===== 1 - COMO COMPILAR Y EJECUTAR LA TAREA =====

Lo primero que hay que hacer, es en el repositorio de GitHub, apretar el botón que dice **< > Code** y copiar el link que se encuenta abajo de donde dice HTTPS. Nosotros usamos Replit asi que lo que hicimos despúes fue crear un nuevo replit, y seleccionar la opción **Import from Github**, y pegar el link previamente copiado en la sección GitHub URL, luego seleccionamos el botón azul **Import from Github** y el código se importará automáticamente, también, solamente con apretar **Run** el código ya se compila solo y funciona correctamente.

===== 2 - LO QUE FUNCIONA Y LO QUE NO =====
  
Todo funciona correctamente, como se puede ver en un commit anterior la funcion **importarArchivo** no funcionaba correctamente pero eso fue solucionado. A pesar de la complejidad de la tarea, todo termino resultando bien.

===== 3 - HERRAMIENTAS UTILIZADAS =====

Las herramientas usadas en esencia fueron:

- Replit [https://replit.com/] : Se logró el trabajo en equipo mediante replit, donde pudimos analizar y cambiar grupalmente el código.
- GitHub [https://github.com] : Creamos el repositorio y lo vinculamos a replit, asi teniendo exitosamente un lugar para ver todos los commits y cambios realizados en todos los aspectos de la tarea, como el "main.c","heap_answer.c", "heap.h", "list_answer.c", "list.h", "map.c/h" y "stack.h", el archivo de tareas ".csv" y el README.


===== 4 - COEVALUACIÓN DEL TRABAJO EN EQUIPO =====

Aspectos a mejorar: 

Grupo = Clippers-2
  Participación activa y equitativa = 20
  Resolución de conflictos = 20
  Colaboración = 20
  Responsabilidad = 20
  Comunicación = 20
  Pje Total = 100
----------------------------------------
  
===== 5 - COEVALUACIÓN DEL APORTE INDIVIDUAL =====

Aspectos positivos: Avance con antelación de la tarea y participativo.

A mejorar: 

Estudiante: Vicente Arratia
  Participación = 
  Responsabilidad = 
  Comunicación = 
  Calidad del Trabajo = 
  Trabajo en Equipo = 
  Pje Total = 
------------------------------

Aspectos positivos: Ninguno.

A mejorar: 

Estudiante: Vicente Mediano
  Participación = 
  Responsabilidad = 
  Comunicación = 
  Calidad del Trabajo = 
  Trabajo en Equipo = 
  Pje Total = 
------------------------------


===== 6 - ANALISIS DEL CÓDIGO =====
Usaremos la siguiente estructura

````c
typedef struct {
  char nombreTarea[32];
  int prioridad;
  bool visitado;
  bool explorado;
  List *adj_edges;

} Nodo;

typedef enum Accion {
  AGREGAR_TAREA,
  ELIMINAR_TAREA,
  PRECEDENCIA
} Accion;

typedef struct Pila {
  char nombre[32];
  Accion accion;
  Nodo* auxNodo;
} Pila;
````
Es necesario entender la funcion **main** para entender el programa, para ello se crea un grafo para todas las tareas, ademas de crear el stack para la funcion **deshacerAccion**, con esto cada Tarea quedara almacenada con su struct correspondiente.
Luego de eso, presentamos el menú de bienvenida, para realizar cada opción que desee el usuario, usamos variables como 'opcion' y 'titulo' para saber las opciones a elegir del usuario, además de poseer sus restricciones correspondientes para no generar errores, implementamos cada función con un ciclo switch asignando así a cada caso su función correspondiente, y la ultima opcion termina el programa.


En la primera función **agregarTarea** que recibe un mapa y la pila, se inicializa la tarea, agregandole memoria, y añadiendo en una lista los nodos adjacentes, ademas de iniciar un char para el nombre de la tarea y un numero entero que corresponde a la prioridad. Se le pide al usuario que agregue la tarea que desea, y buscamos en el mapa que no hayan otras tareas con el mismo nombre. Se insertan en el mapa de nodos del grafos la nuevaTarea con la clave. Y finalmente se crea la pila para agregar que se realizo una accion, en este caso que una tarea fue agregada.


En la funcion  **precedenciaTareas** que recibe un mapa y la pila, se le pide al usuario que ingrese cual sera la tarea precedente (la llamaremos tareaY) de otra tarea (la cual llamaremos tareaX), se busca en el programa la tareaY y se manda un mensaje de error si no llega a existir. Se pide al usuario que ingrese el nombre de la tareaX,se busca en el programa de la misma forma que antes y se manda un mensaje de error si no llega a existir. Si el usuario llega a poner dos veces el nombre de la misma tarea, se imprime un mensaje de error. Finalmente se le agrega la precedencia a la tarea, y se añade la accion realizada en la pila.


La funcion  **reestablecerBooleanos** no es parte de la Tarea de por si y fue hecha para arreglar una situacion en particular que ocurria, su funcionalidad quedo detallada en el archivo **main.c**.
 
La funcion  **nodosCompletados** tambien sale detallada en **main.c**.

En la funcion  **mostrarTareas** se crea el monticulo de mínimos para ir guardando en la lista las tareas de forma ordenada según su precedencia y prioridad, ademas se crea la lista de tareas que se encargará de almacenar las tareas de forma ordenada y para que la tarea no colapse despues de usar esta funcion muchas veces, se llama a la funcion **reestablecerBooleanos**. Se asigna a todas las tareas que no tengan precedencias el booleano de explorado como true. Finalmente el programa muestra las tareas, ordenadas por prioridad y precedencia.

En la funcion **tareaCompletada** que recibe un mapa y una pila, se ingresa el nombre de la tarea completada, se verifica que exista esta en el programa y se imprime un mensaje de error si no llega a existir.
Se manda un mensaje de advertencia en caso de que la tarea tenga precedentes con otras tareas, y si el usuario se arrepiente se cancela la funcion. En caso de que este seguro o la tarea no tenga precedentes con otras, se llama a la funcion **tareaElim**, la cual elimina la tarea y ademas guarda la ultima accion realizada (en este caso eliminar la tarea) en la pila.

La funcion **deshacerAccion** la cual recibe un mapa y una pila, se recorre la pila y se revisa cual fue la ultima accion (la cual era una variable que se guardaba al final de cada funcion) y dependiendo del caso, se borra una tarea, una precedencia o se vuelve a crear una tarea si se llego a borrar.

La funcion void **importarArchivo** permite importar datos de un archivo ".csv" para guardarlos en el mapa. Para eso, solicita el nombre del archivo que se desea importar y lo abre en modo lectura, si no se puede abrir, la funcion mostrara un mensaje de error y terminara la ejecucion. A continuacion, la funcion creara una tarea reservando memoria e inicializando valores para luego ir copiando los que estan en el ".csv". Finalmente se termina la ejecucion de la funcion y se cierra el archivo.