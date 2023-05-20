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

void leerCadena(char cadena[31]) {
  scanf("%30[^\n]s", cadena);
  getchar();
}

void agregarTarea(Map *grafo, Stack *stack) {
  Nodo *nuevaTarea = (Nodo *)malloc(sizeof(Nodo));
  nuevaTarea->adj_edges = createList();
  char tarea[32];
  int prioridad;
  printf("Ingrese el nombre de la tarea\n");
  getchar();
  
  leerCadena(tarea);

  while(searchMap(grafo, tarea) != NULL) {
    printf("El nombre ya existe dentro del programa!\n");
    leerCadena(tarea);
  }
    
  
  printf("Ingrese la prioridad de la tarea\n");
  scanf("%d", &prioridad);

  strcpy(nuevaTarea->nombreTarea, strdup(tarea));
  nuevaTarea->prioridad = prioridad;

  insertMap(grafo, nuevaTarea->nombreTarea, nuevaTarea);

  Pila* registro = (Pila *) malloc(sizeof(Pila));
  registro->accion = AGREGAR_TAREA;
  strcpy(registro->nombre, tarea);

  stack_push(stack, registro);
  
}

void precedenciaTareas(Map *grafo, Stack *stack) {
  char tarea1[32];
  char tarea2[32];
  getchar();
  printf("Ingrese el nombre de la tarea 1, la cual será precedente de la tarea 2\n");
  leerCadena(tarea1);

  if (searchMap(grafo, tarea1) == NULL) {
    printf("La tarea '%s' no existe dentro del programa\n", tarea1);
    return;
  }
  printf("Ingrese el nombre de la tarea 2, la cual tendra de precedente a la tarea 1\n");
  leerCadena(tarea2);

  if (searchMap(grafo, tarea2) == NULL) {
    printf("La tarea '%s' no existe dentro del programa\n", tarea2);
    return;
  }

 
  Nodo *aux1 = searchMap(grafo, tarea1);
  Nodo *aux2 = searchMap(grafo, tarea2);

  pushBack(aux2->adj_edges, aux1);
  
  Pila* registro = (Pila *) malloc(sizeof(Pila));
  registro->accion = PRECEDENCIA;
  strcpy(registro->nombre , tarea1);
  registro->auxNodo = aux2;
  
  stack_push(stack, registro);
  
}

void reestablecerBooleanos(Map *grafo) {
 Nodo *tarea = firstMap(grafo);
    
  while(tarea != NULL) {
    tarea->explorado = false;
    Nodo *nodoAdj = firstList(tarea->adj_edges);
      
    while(nodoAdj != NULL) {
      nodoAdj->visitado = false; 
      nodoAdj = nextList(tarea->adj_edges);
    }
          
    tarea = nextMap(grafo);
  }
}

void nodosCompletados(Map *grafo, char *nombreNodo) {
  Nodo *tarea = firstMap(grafo);
  
  while(tarea != NULL) {
  
    Nodo *nodoAdj = firstList(tarea->adj_edges);

    while(nodoAdj != NULL) {
      if(strcmp(nodoAdj->nombreTarea, nombreNodo) == 0) {
        nodoAdj->visitado = true;
      }
      nodoAdj = nextList(tarea->adj_edges);
    }
          
    tarea = nextMap(grafo);
  }
}

void mostrarTareas(Map *grafo) {
  Heap *monticulo = createHeap();
  List *listaTareas = createList();

  reestablecerBooleanos(grafo);

  Nodo *aux = firstMap(grafo);
  while(aux != NULL) {
    if (firstList(aux->adj_edges) == NULL) {
      heap_push(monticulo, aux->nombreTarea, aux->prioridad);
      aux->explorado = true;
    }
    aux = nextMap(grafo);
  }

  while(heap_top(monticulo) != NULL) {
    Nodo *actualTarea = heap_top(monticulo);
    heap_pop(monticulo);

    pushBack(listaTareas, actualTarea);
    nodosCompletados(grafo, actualTarea->nombreTarea);

    Nodo *aux = firstMap(grafo);
    while(aux != NULL) {
      if (aux->explorado != true) {
        bool tienePrecedencia = false;

        Nodo *aux2 = firstList(aux->adj_edges);
        while(aux2 != NULL) {
          if (aux2->visitado != true) {
            tienePrecedencia = true;
            break;
          }
          aux2 = nextList(aux->adj_edges);
        }

        if (tienePrecedencia != true && aux->explorado != true) {
          heap_push(monticulo, aux, aux->prioridad);
          aux->explorado = true;
        }
      }
      aux = nextMap(grafo);
    }
  }

  printf("Tareas por hacer, ordenadas por prioridad y precedencia:\n");
  Nodo *auxTareas = firstList(listaTareas);
  while (auxTareas != NULL) {
    printf("Tarea: %s, Prioridad: %d", auxTareas->nombreTarea, auxTareas->prioridad);

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

void tareaCompletada(Map *grafo, Stack *stack) {
  char tarea[32];

  getchar();
  printf("Ingrese el nombre de la tarea completada: ");
  leerCadena(tarea);

  Nodo *nodoTarea = searchMap(grafo, tarea);
  if (nodoTarea == NULL) {
    printf("La tarea '%s' no existe dentro del programa.\n", tarea);
    return;
  }

  if (firstList(nodoTarea->adj_edges) != NULL) {
    printf("¡Advertencia! La tarea '%s' tiene relaciones de precedencia con otras tareas.\n",tarea);
    printf("¿Estás seguro que deseas eliminar la tarea? (s/n): ");
    char respuesta;
    scanf(" %c", &respuesta);
    if (respuesta != 's' && respuesta != 'S') {
      printf("Operación cancelada. La tarea no ha sido eliminada.\n");
      return;
    }
  }

  Pila* registro = (Pila *) malloc(sizeof(Pila));
  registro->accion = ELIMINAR_TAREA;
  
  strcpy(registro->nombre, tarea);
  registro->auxNodo = nodoTarea;
  stack_push(stack, registro);

  eraseMap(grafo, tarea);
  printf("La tarea '%s' ha sido marcada como completada y eliminada de la lista de tareas por hacer.\n",tarea);

  
}

void deshacerAccion(Map *grafo, Stack *stack) {
  if(stack_top(stack) == NULL) {
    printf("No se ha registrado ninguna acción reciente!\n");
    return;
  }

  Pila *aux = stack_pop(stack);
  
  switch(aux->accion) {
    case AGREGAR_TAREA: 
      if (searchMap(grafo, aux->nombre) != NULL) {
        eraseMap(grafo, aux->nombre);
        break;
      }
    case ELIMINAR_TAREA:
      if (searchMap(grafo, aux->nombre) == NULL) {
        insertMap(grafo, aux->nombre, aux->auxNodo);
        break;
      }
    case PRECEDENCIA:
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

void importarArchivo(Map* grafo) {
  FILE* archImportar;
  char nombreArchivo[100];
  printf("Ingrese el nombre del archivo del cual desea importar las tareas en formato .csv\n");
  scanf("%s", nombreArchivo);

  archImportar = fopen(nombreArchivo, "r");
  if (!archImportar) {
    printf("Error: no se pudo abrir el archivo\n");
    return;
  }

  char linea[1024];
  fgets(linea, 1024, archImportar); // Saltar la primera línea

  while(fgets(linea, sizeof(linea), archImportar) != NULL) {
    Nodo* nuevaTarea = (Nodo*)malloc(sizeof(Nodo));
    nuevaTarea->adj_edges = createList();
    
    for(int i = 0; i < 3 ; i++) {
      char* aux = get_csv_field(linea,i);
      switch (i){
        case 0:
          strcpy(nuevaTarea->nombreTarea,aux);
          break;
        case 1:
          nuevaTarea->prioridad = atoi(aux);
          break;
        case 2:
        if (strlen(aux) > 1) {
          char * precedentes = strtok (aux, " ");
          while (precedentes != NULL) {
            pushFront(nuevaTarea->adj_edges, precedentes);
            precedentes = strtok(NULL, " ");
          }
        }
        break;
        default:
        break;
      }
    }
    insertMap(grafo, strdup(nuevaTarea->nombreTarea), nuevaTarea);
  }
  fclose(archImportar);
}

void mab(Map* mapa) {

  Nodo * aux = firstMap(mapa);
  while (aux != NULL) {
    printf("NODO: %s -> prioridad %d\n\n", aux->nombreTarea, aux->prioridad);

    Nodo* nodoLista = firstList(aux->adj_edges);
    int cont = 1;
    while(nodoLista != NULL) {
      printf("nodo lista (c): %s -> prioridad %d\n", nodoLista->nombreTarea, nodoLista->prioridad);
      nodoLista = nextList(aux->adj_edges);
      cont++;
    }

    aux = nextMap(mapa);
    printf("\n\n");
  }
}

int main() {
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
    while (opcion < 1 || opcion > 8) {
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
    case 8:
      mab(grafo);
      break;
    }
  }
  return 0;
}

