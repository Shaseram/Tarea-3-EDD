#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "Map.h"
#include "stack.h"


typedef struct {
char nombreTarea[32];
int prioridad;
List* adj_edges;

} Nodo;

/*
  función para comparar claves de tipo string
  retorna 1 si son iguales
*/
int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

void leerCadena(char cadena[31]){
  scanf("%30[^\n]s", cadena);
  getchar();
}

void agregarTarea(Map* grafo) {
  Nodo* nuevaTarea = (Nodo *) malloc(sizeof(Nodo));
  nuevaTarea->adj_edges = createList();
  char nombre[32];
  int prioridad;
  printf("Ingrese el nombre de la tarea\n");
  getchar();
  leerCadena(nombre);

  printf("Ingrese la priodad de la tarea\n");
  scanf("%d", &prioridad);

  strcpy(nuevaTarea->nombreTarea, strdup(nombre));
  nuevaTarea->prioridad = prioridad;

  insertMap(grafo, nuevaTarea->nombreTarea, nuevaTarea);

  /*Nodo* prueba = searchMap(grafo, nombre);
  if (prueba != NULL) {
    printf("El nombre de la tarea es %s y su prioridad es %d\n", prueba->nombreTarea, prueba->prioridad);
  }
  */
  
}


int main() {

  Map* grafo = createMap(is_equal_string);
  

  
  int opcion, titulo = 0;

  while(1) {
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
    
    scanf("%d",&opcion);
    while(opcion < 1 || opcion > 7) {
      scanf("%d",&opcion);
    }

    switch(opcion) {
      case 1:
        agregarTarea(grafo);
        break;
      case 2:
        printf("Funcion no implementada");
        break;
      case 3:
        printf("Funcion no implementada");
        break;
      case 4:
        printf("Funcion no implementada");
        break;
      case 5:
        printf("Funcion no implementada");
        break;
      case 6:
        printf("Funcion no implementada");
        break;
      case 7:
        printf("Cerrando el programa...\n");
      return 0;
    }
  }
  return 0;
}
