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
  
Al momento de escribir esto **20/05/2023, 13:49 PM**, todo el codigo funciona perfectamente a excepcion de la funcion **importarArchivo**, que por alguna razon al importar el archivo **tareas.csv** las precedencias salen con valor 0, no mostrando todo

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
  Pje Total = 0
----------------------------------------
  
===== 5 - COEVALUACIÓN DEL APORTE INDIVIDUAL =====


Aspectos positivos: 

A mejorar: 

Estudiante: Vicente Arratia
  Participación = 
  Responsabilidad = 
  Comunicación = 
  Calidad del Trabajo = 
  Trabajo en Equipo = 
  Pje Total = 
------------------------------

Aspectos positivos: 

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
