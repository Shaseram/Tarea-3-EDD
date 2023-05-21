#include "Map.h"
#include "heap.h"
#include "list.h"
#include "stack.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  List* listaAux;
} Pila;

const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }


    return NULL;
}


/*
  función para comparar claves de tipo string
  retorna 1 si son iguales
*/
int is_equal_string(void *key1, void *key2) {
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

// Función que se utilizará para leer las cadenas en las funciones que lo requieran.
void leerCadena(char cadena[31]) {
  scanf("%30[^\n]s", cadena);
  getchar();
}

void agregarTarea(Map *grafo, Stack *stack) {
  // Se inicializan variables de la nueva tarea, incluyendo la asignacion de memoria.
  Nodo *nuevaTarea = (Nodo *)malloc(sizeof(Nodo));
  nuevaTarea->adj_edges = createList();
  char tarea[32];
  int prioridad;
  printf("Ingrese el nombre de la tarea: \n");
  getchar();
  
  leerCadena(tarea);

  // Validamos que se ingrese un nombre válido, que no exista dentro del programa.
  while(searchMap(grafo, tarea) != NULL) {
    printf("El nombre ya existe dentro del programa! Ingrese uno válido: \n");
    leerCadena(tarea);
  }
    
  
  printf("Ingrese la prioridad de la tarea: \n");
  scanf("%d", &prioridad);

  /* Se hace la copia del char ingresado arriba a la variable nombreTarea de la nueva tarea al igual que se le asigna la prioridad ingresada por el usuario. */
  strcpy(nuevaTarea->nombreTarea, strdup(tarea));
  nuevaTarea->prioridad = prioridad;
  // Insertamos en el mapa de los nodos del grafo la nuevaTarea con la clave que es su nombre.
  insertMap(grafo, nuevaTarea->nombreTarea, nuevaTarea);

  /* Se crea la varible registro de tipo Pila, donde se iran guardando los registros (últimas acciones), en este caso es la acción de Agregar Tarea.*/
  Pila* registro = (Pila *) malloc(sizeof(Pila));
  registro->accion = AGREGAR_TAREA;
  strcpy(registro->nombre, tarea);

  stack_push(stack, registro);
  
}

void precedenciaTareas(Map *grafo, Stack *stack) {
  char tarea1[32];
  char tarea2[32];
  getchar();
  printf("Ingrese el nombre de la tarea 1, la cual será precedente de la tarea 2: \n");
  leerCadena(tarea1);

  // En caso de que la tarea1 no exista dentro del programa se finaliza la función indicando que la tarea no existe, volviendo al menú.
  if (searchMap(grafo, tarea1) == NULL) {
    printf("La tarea '%s' no existe dentro del programa\n", tarea1);
    return;
  }
  printf("Ingrese el nombre de la tarea 2, la cual tendra de precedente a la tarea 1: \n");
  
  leerCadena(tarea2);

  // Si el usuario ingresa la misma tarea en la tarea1 y tarea2 se indicara que no se puede establecer una precedencia entre la misma tarea, volviendo al menú.
  if (strcmp(tarea1,tarea2) == 0) {
    printf("No puedes establecer una precedencia entre la misma tarea!\n");
    return;
  }

   // En caso de que la tarea2 no exista dentro del programa se finaliza la función indicando que la tarea no existe, volviendo al menú.
  if (searchMap(grafo, tarea2) == NULL) {
    printf("La tarea '%s' no existe dentro del programa\n", tarea2);
    return;
  }

  // Creamos dos auxiliares que contendrán el nodo, ambos se buscan en el mapa de nodos del grafo y se les asigna el dato de tipo Nodo.
  Nodo *aux1 = searchMap(grafo, tarea1);
  Nodo *aux2 = searchMap(grafo, tarea2);

  // Agregando así en la lista de precedencias del aux2 la tarea que haya sido ingresada en aux1.
  pushBack(aux2->adj_edges, aux1);

  /* Se crea la varible registro de tipo Pila, donde se iran guardando los registros (últimas acciones), en este caso es la acción de Precedencias*/
  Pila* registro = (Pila *) malloc(sizeof(Pila));
  registro->accion = PRECEDENCIA;
  strcpy(registro->nombre , tarea1);
  registro->auxNodo = aux2;
  
  stack_push(stack, registro);
  
}

/* Función encargada de reestablecer los Booleanos que son utilizados en la función mostrarTareas, recorriendo los nodos del grafo junto a sus listas de adyacencia. Asignando así "explorado" y "visitado" como falso.*/
void reestablecerBooleanos(Map *grafo) {
 Nodo *auxTarea = firstMap(grafo);
    
  while(auxTarea != NULL) {
    auxTarea->explorado = false;
    Nodo *auxAdj = firstList(auxTarea->adj_edges);
      
    while(auxAdj != NULL) {
      auxAdj->visitado = false; 
      auxAdj = nextList(auxTarea->adj_edges);
    } 
    auxTarea = nextMap(grafo);
  }
}

// Esta función se encarga de marcar los nodos adyacentes que compartan el nombre del char nombreNodo como visitados. Se recorre el mapa y en la lista de adyacencia del nodo se compara el nombreNodo con el nombre del nodo que está en la lista, en caso de ser iguales se marcará el booleano visitado como true.
void nodosCompletados(Map *grafo, char *nombreNodo) {
  Nodo *auxTarea = firstMap(grafo);
  
  while(auxTarea != NULL) {
    Nodo *auxAdj = firstList(auxTarea->adj_edges);
    while(auxAdj != NULL) {
      if(strcmp(auxAdj->nombreTarea, nombreNodo) == 0) {
        auxAdj->visitado = true;
      }
      auxAdj = nextList(auxTarea->adj_edges);
    }     
    auxTarea = nextMap(grafo);
  }
}

void mostrarTareas(Map *grafo) {
  // Se crea el monticulo de mínimos para ir guardando en la lista las tareas de forma ordenada según su precedencia y prioridad.
  Heap *monticulo = createHeap();
  // Se crea la lista de tareas que se encargará de almacenar las tareas de forma ordenada.
  List *listaTareas = createList();

  // Llamamos a la función reestablecer booleanos cada vez que se ingrese a esta función.
  reestablecerBooleanos(grafo);

  // Asignamos primeramente a todas las tareas que no tengan precedencias el booleano de explorado como true. De igual forma se agregan los datos que no tienen precedencias al monticulo.
  Nodo *aux = firstMap(grafo);
  while(aux != NULL) {
    if (firstList(aux->adj_edges) == NULL) {
      heap_push(monticulo, aux->nombreTarea, aux->prioridad);
      aux->explorado = true;
    }
    aux = nextMap(grafo);
  }

  
  while(heap_top(monticulo) != NULL) {
    // Asignamos a actualTarea la raíz del monticulo (el dato con menor prioridad).
    Nodo *actualTarea = heap_top(monticulo);
    // Se elimina la raíz del monticulo.
    heap_pop(monticulo);

    // Agregamos a la lista la actual tarea, ya que esta es la con menor prioridad.
    pushBack(listaTareas, actualTarea);
    // Llamamos a la función nodosCompletados luego de haber agregado la tarea a la lista.
    nodosCompletados(grafo, actualTarea->nombreTarea);

    // Se busca el nodo no explorado en el mapa de nodos del grafo.
    Nodo *aux = firstMap(grafo);
    while(aux != NULL) {
      if (aux->explorado != true) {
        // Esta variable se utiliza para verificar si el nodo aux tiene precedencia       sobre otros.
        bool tienePrecedencia = false;

        Nodo *aux2 = firstList(aux->adj_edges);
        while(aux2 != NULL) {
          // Se verifica si el booleano "visitado" de aux2 no es verdadero. Lo que determina si el nodo adyacente ha sido visitado previamente.
          if (aux2->visitado != true) {
            tienePrecedencia = true;
            break;
          }
          aux2 = nextList(aux->adj_edges);
        }
        // Se verifica al haber salido del bucle de la lista de adyacencia si es que tienePrecedencia no es verdadero al igual que el explorado de aux. Lo que significa que el nodo aux no tiene nodos adyacentes no visitados y aún no ha sido explorado.
        if (tienePrecedencia != true && aux->explorado != true) {
          // Agregando así al monticulo el nodo aux.
          heap_push(monticulo, aux, aux->prioridad);
          aux->explorado = true;
        }
      }
      aux = nextMap(grafo);
    }
  }

  // Se muestran las tareas de la lista de tareas, la cuál almacenó de forma ordenada en prioridad y precendencia las tareas.
  printf("Tareas por hacer, ordenadas por prioridad y precedencia:\n");
  Nodo *auxTareas = firstList(listaTareas);
  while (auxTareas != NULL) {
    printf("Tarea: %s, (Prioridad: %d)", auxTareas->nombreTarea, auxTareas->prioridad);

    Nodo *auxAdj = firstList(auxTareas->adj_edges);
    if (auxAdj != NULL) {
      printf(" - Precedente:");
      while (auxAdj != NULL) {
        printf(" %s", auxAdj->nombreTarea);
        auxAdj = nextList(auxTareas->adj_edges);
      }
    }
    printf("\n");
    auxTareas = nextList(listaTareas);
  }
  free(monticulo);
}

// Función llamada en tareaCompletada, la que cumple la función de eliminar la tarea de todo el programa.
void tareaElim(Map *grafo, Stack *stack, char *nombreTarea, Nodo *nodoTarea) {
  // Creamos la listaAuxiliar que guardará el nodo de la tarea en caso de que exista en la lista de adyacencia de otra tarea.
  List* listaAuxiliar = createList();
  // Eliminamos dentro del grafo aquella tarea que coincida con la clave (nombreTarea).
  eraseMap(grafo, nombreTarea);  

  // Recorremos el mapa de los nodos del grafo, buscando si el nodo de la lista adyacente de auxTarea coincide con el nombreTarea recibido por la función.
  Nodo* auxTarea = firstMap(grafo);
  while(auxTarea != NULL) {
    Nodo* auxAdj = firstList(auxTarea->adj_edges);
    while(auxAdj != NULL) {
      // En caso de que exista el nodo de nombreTarea en la lista de adyacencia de auxTarea, se guardará en la listaAuxiliar y se eliminará completando la función que es eliminar la tarea indicada por el usuario (lo que implica eliminarla de todo el programa).
      if (strcmp(auxAdj->nombreTarea, nombreTarea) == 0) {
        pushBack(listaAuxiliar,auxTarea);
        popCurrent(auxTarea->adj_edges);
      }
      auxAdj = nextList(auxTarea->adj_edges);
    }
   auxTarea = nextMap(grafo); 
  }

  /* Se crea la varible registro de tipo Pila, donde se iran guardando los registros (últimas acciones), en este caso es la acción de Eliminar Tarea. En este caso se hace uso de una lista auxiliar.*/
  Pila* registro = (Pila *) malloc(sizeof(Pila));
  registro->accion = ELIMINAR_TAREA;

  
  strcpy(registro->nombre, nodoTarea->nombreTarea);
  registro->auxNodo = nodoTarea;
  registro->listaAux = listaAuxiliar;
  stack_push(stack, registro);

  printf("\nLa tarea '%s' ha sido marcada como completada y eliminada de la lista de tareas por hacer.\n", nombreTarea);

}

void tareaCompletada(Map *grafo, Stack *stack) {
  char tarea[32];

  getchar();
  printf("Ingrese el nombre de la tarea completada: ");
  leerCadena(tarea);

  // En caso de que nombre ingresado por el usuario no exista dentro del programa se indicara que se ingrese uno válido
  Nodo *nodoTarea = searchMap(grafo, tarea);
  while(searchMap(grafo, tarea) == NULL) {
    printf("El nombre no existe dentro del programa! Ingrese uno válido: ");
    leerCadena(tarea);
  }

  // En caso de que la lista de adyacencia del nodo ingresado por el usuario tenga relaciones de procedencia con otras tareas se preguntará al usuario si está seguro de que quiere eliminar esta tarea.
  if (firstList(nodoTarea->adj_edges) != NULL) {
    printf("\n¡Advertencia! La tarea '%s' tiene relaciones de precedencia con otras tareas.\n\n",tarea);
    printf("¿Estás seguro que deseas eliminar la tarea? (1-s/2-n): ");
    int respuesta;
    scanf("%d", &respuesta);
    if (respuesta == 2) {
      printf("Operación cancelada. La tarea no ha sido eliminada.\n");
      return;
    }
    // Si la respuesta es si (1) se procederá con la eliminación de la tarea llamando a la función tareaElim.
    if(respuesta == 1) {
      tareaElim(grafo, stack, tarea,  nodoTarea);
    }
    // Si no es el caso, simplemente se llama a la función sin preguntar al usuario.
  } else tareaElim(grafo, stack, tarea,  nodoTarea);
}

void deshacerAccion(Map *grafo, Stack *stack) {
  // Se recibe stack del main, la cuál pudo tanto como haber sido llenada de acciones o simeplemente no tener ninguna. Por eso se pregunta si el top de la pila es NULL, ya que encaso de que sea NULL significa que la pila está vacía, por ende no hay ninguna acción reciente.
  if(stack_top(stack) == NULL) {
    printf("No se ha registrado ninguna acción reciente!\n");
    return;
  }

  // Creamos un auxiliar de tipo Pila que almacena lo que retorna stack_pop.
  Pila *aux = stack_pop(stack);

  // Según que acción tenga aux se usará un switch, el cuál determinará que operaciones realizar.
  switch(aux->accion) {
    case AGREGAR_TAREA: 
      // Se elimina del mapa la tarea con el nombre de aux en caso de que la acción haya sido agregar tarea.
      if (searchMap(grafo, aux->nombre) != NULL) {
        eraseMap(grafo, aux->nombre);
        break;
      }
    case ELIMINAR_TAREA:
      // Se agrega al mapa nuevamente aquella tarea que tenga el nombre de aux en caso de que la acción haya sido eliminar.
      if (searchMap(grafo, aux->nombre) == NULL) {
        insertMap(grafo, aux->nombre, aux->auxNodo);
      }
      // Al tener que agregar la tarea nuevamente tendremos que asignarla a aquellas tareas que la tenían como precedente, por eso se usa la lista auxiliar que tiene el aux de tipo Pila. Con esto nos aseguramos que al deshacer acción todo vuelva a como se estuvo realmente al momento de haber elimnado una tarea.
      Nodo* auxLista = firstList(aux->listaAux);
      while(auxLista != NULL) {
        Nodo* auxMapa = searchMap(grafo, auxLista->nombreTarea);
        pushBack(auxMapa->adj_edges, aux);
        auxLista = nextList(aux->listaAux);
      }
      break;
    case PRECEDENCIA:
      // Se elimina la precedencia en caso de que se haya agregado una precedencia entre dos tareas.
      if (searchMap(grafo, aux->nombre) != NULL && searchMap(grafo, aux->auxNodo->nombreTarea) != NULL) {
        Nodo* aux2 = firstList(aux->auxNodo->adj_edges);
        while (aux2 != NULL) {
          if (strcmp(aux2->nombreTarea, aux->nombre) == 0) {
            popCurrent(aux->auxNodo->adj_edges);
            break;
          }
          nextList(aux->auxNodo->adj_edges);
        }
      }
  }
}

void importarArchivo(Map *grafo) {
  //Se crea una variable de tipo FILE para el archivo que se importará.
  FILE* archImportar;
  char nombreArchivo[100];
  //Se pregunta el nombre del archivo que se quiere importar.
  printf("Ingrese el nombre del archivo del cual desea importar tareas en formato .csv: \n");
  scanf("%s", nombreArchivo);

  //Se abre el archivo en modo de lectura.
  archImportar = fopen(nombreArchivo, "r");
  printf("Se han importado los datos del archivo %s\n",nombreArchivo);
  if (!archImportar) {
    printf("Error: No se pudo abrir el archivo\n");
    return;
  }
  
  char delimit[]=" \t\r\n\v\f"; 
  char linea[1024];
  
  //Se consigue la linea para hacer un salto y no tomar en cuenta la primera linea.
  fgets(linea,1024,archImportar);
  
  while (fgets(linea,1024,archImportar) != NULL){
    //Mientras la linea contenga información se irá iterando y almacenando las tareas.
    Nodo* nuevaTarea = NULL;
    nuevaTarea = (Nodo *)malloc(sizeof(Nodo));
    nuevaTarea->adj_edges = createList();
    char* precedente = "";

    //Según que retorne la función get_csv_field se irán haciendo las asignaciones de los datos.
    for(int i = 0 ; i < 3 ; i++) {
      char* aux = get_csv_field(linea, i);
      switch(i) {
        case 0:
          strcpy(nuevaTarea->nombreTarea, aux);
          break;

        case 1:
          nuevaTarea->prioridad = atoi(aux);
          break;

        case 2:
          precedente = strtok(aux, delimit);
          while(precedente != NULL) {
            Nodo *aux2 = searchMap(grafo, precedente);
            if (aux2 != NULL) pushBack(nuevaTarea->adj_edges, aux2);
            precedente = strtok(NULL, delimit);
          }
          break;
        
        default:
        break;
      }
    }
    insertMap(grafo, nuevaTarea->nombreTarea, nuevaTarea);
  }
  fclose(archImportar);
}

int main() {
  // Se inicializa el grafo (mapa que contendrá sus nodos) y la pila que almacenará las acciones.
  Map *grafo = createMap(is_equal_string);
  Stack *stack = stack_create();

  int opcion, titulo = 0;

  while (1) {
    if (titulo == 0) {
      printf("=============================================\n");
      printf(" === BIENVENIDO AL ORGANIZADOR DE TAREAS === \n");
      titulo++;
    }

    printf("=============================================\n");
    printf("Escoja una opción: \n\n");
    printf("1. Agregar Tarea\n");
    printf("2. Establecer precedencia entre tareas\n");
    printf("3. Mostrar tareas por hacer\n");
    printf("4. Marcar tarea como completada\n");
    printf("5. Deshacer última acción\n");
    printf("6. Cargar datos de tareas desde un archivo de texto\n");
    printf("7. Salir del programa\n");

    scanf("%d", &opcion);
    while (opcion < 1 || opcion > 7) {
      printf("Ingrese una opción válida!\n");
      scanf("%d", &opcion);
    }

    switch (opcion) {
    case 1:
      agregarTarea(grafo, stack);
      break;
    case 2:
      precedenciaTareas(grafo, stack);
      break;
    case 3:
      mostrarTareas(grafo);
      break;
    case 4:
      tareaCompletada(grafo, stack);
      break;
    case 5:
      deshacerAccion(grafo, stack);
      break;
    case 6:
      importarArchivo(grafo);
      break;
    case 7:
      printf("Cerrando el programa...\n");
      return 0;
    }
  }
  return 0;
}