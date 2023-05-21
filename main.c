#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include "heap.h"
#include "printFormato.h"

int MAXCAR = 30;

typedef struct
{
  char nombre[30];
  int prioridad;
  int numPrecedentes;
  char** precedentes;
  bool completada;
}Tarea;

typedef struct nodo{
   void* data;
   int priority;
}heapElem;

typedef struct Heap{
  heapElem* heapArray;
  int size;
  int capac;
} Heap;

/////////////////////////////////////////////////////////

void intercambiar(heapElem* a, heapElem* b)
{
  heapElem temp = *a;
  *a = *b;
  *b = temp;
}

//mueve el ultimo elemento hacia arriba
void siftUp(heapElem* heapArray, int index)
{
  int parent = (index - 1) / 2;
  while (index > 0 && heapArray[index].priority < heapArray[parent].priority) 
  {
    intercambiar(&heapArray[index], &heapArray[parent]);
    index = parent;
    parent = (index - 1) / 2;
  }
}

void ordenarHeapAscendente(Heap* listaTareas)
{
  for (int i = 1; i < listaTareas->size; i++)
    {
      siftUp(listaTareas->heapArray, i);
    }
}

//inicializa la estructura tipo tarea con los datos entregados 
Tarea* crearTarea(char *nombre, int prioridad, char** precedentes, int numPrecedentes)
{
  Tarea* nuevaTarea = malloc(sizeof(Tarea));
  strncpy(nuevaTarea->nombre, nombre, sizeof(nuevaTarea->nombre));
  nuevaTarea->prioridad = prioridad;
  nuevaTarea->numPrecedentes = numPrecedentes;
  nuevaTarea->completada = false;

  if (numPrecedentes > 0)
  {
    nuevaTarea->precedentes = malloc(numPrecedentes * sizeof(char*));
    for (int i = 0; i < numPrecedentes; i++) 
    {
      nuevaTarea->precedentes[i] = malloc((strlen(precedentes[i]) + 1) * sizeof(char));
      strcpy(nuevaTarea->precedentes[i], precedentes[i]);
    }
  }

  else
  {
    nuevaTarea->precedentes = NULL;
  }

  return nuevaTarea;
  
}

// 1 //
void AgregarTarea(Heap* grafo, Tarea* nuevaTarea)
{
  if(grafo->size >= grafo->capac)
  {
    grafo->capac *= 2;
    grafo->heapArray = realloc(grafo->heapArray, grafo->capac * sizeof(heapElem));
    if(grafo->heapArray == NULL)
    {
      //no se pudo asignar memoria
      exit(EXIT_FAILURE);
    }
  }
  grafo->heapArray[grafo->size].data = nuevaTarea;
  grafo->heapArray[grafo->size].priority = nuevaTarea->prioridad;
  grafo->size++;

  ordenarHeapAscendente(grafo);
  printf("La tarea se agregó correctamente!");
}

// 2 //
void addPrecedencia(Heap* grafo, char* tarea1, char* tarea2)
{
  Tarea* t1 = NULL;
  Tarea* t2 = NULL;

  for (int i = 0; i < grafo->size; i++)
  {
    Tarea* tarea = grafo->heapArray[i].data;
    if (strcmp(tarea->nombre, tarea1) == 0)
    {
      t1 = tarea;
    }
        
    else if (strcmp(tarea->nombre, tarea2) == 0)
    {
      t2 = tarea;
    }

    if (t1 && t2)
    {
      if (t1 == t2)
      {
        printf("La tarea1 y la tarea2 son iguales.\n");
        return;
      }

      for(int j = 0; j < t2->numPrecedentes; j++)
      {
        if (strcmp(t2->precedentes[j], t1->nombre) == 0)
        {
          printf("La tarea 2 ya tiene la tarea 1 como precedente.\n");
          return;
        }
      }
      t2->precedentes = realloc(t2->precedentes, (t2->numPrecedentes + 1) * sizeof(char*));
      t2->precedentes[t2->numPrecedentes] = malloc((strlen(t1->nombre) + 1) * sizeof(char));
      strcpy(t2->precedentes[t2->numPrecedentes], t1->nombre);
      t2->numPrecedentes++;

      printf("Se estableció la precedencia: %s -> %s\n", t1->nombre, t2->nombre);
      return;
    } 
  }
  
  if (t1 == NULL || t2 == NULL)
  {
    printf("Una o ambas tareas no existen en el grafo.\n");
    return;
  }
}

// 3 //
void printPendientes(Heap* grafo)
{
  Tarea** pendientes = malloc(grafo->size * sizeof(Tarea*));
  int numPendientes = 0;
  
  for (int i = 0; i < grafo->size; i++)
  {
    Tarea* tarea = grafo->heapArray[i].data;
    if (!tarea->completada)
    {
      bool tienePreComp = false;

      for (int j = 0; j < tarea->numPrecedentes; j++)
      {
        for (int k = 0; k < grafo->size; k++)
        {
          Tarea* precedente = grafo->heapArray[k].data;
          if (strcmp(precedente->nombre, tarea->precedentes[j]) == 0 && precedente->completada)
          {
            tienePreComp = true;
            break;
          }
        }
        if (tienePreComp)
          break;
      }

      if(!tienePreComp)
      {
        pendientes[numPendientes] = tarea;
        numPendientes++;
      }
    }
  }

  /*
  for (int i = 1; i < numPendientes; i++)
  {
    int j = i;
    while (j > 0 && pendientes[j]->prioridad < pendientes[j - 1]->prioridad)
    {
      Tarea* temp = pendientes[j];
      pendientes[j] = pendientes[j - 1];
      pendientes[j - 1] = temp;
      j--;
    }
  }
  */
  
  printf("╭──────────────────────────────────────────────────────────╮\n");
  printf("│                    Tareas pendientes:                    │\n");
  printf("╰──────────────────────────────────────────────────────────╯\n");
  for (int i = 0; i < numPendientes; i++)
  {
    printf("│  %d. %s (Prioridad: %d)", i + 1, pendientes[i]->nombre, pendientes[i]->prioridad);
    
    if (pendientes[i]->numPrecedentes > 0)
    {
      printf(" - Precedente(s): ");
      for (int j = 0; j < pendientes[i]->numPrecedentes; j++)
      {
        printf("%s", pendientes[i]->precedentes[j]);
        
        if (j < pendientes[i]->numPrecedentes - 1)
        {
          printf(", ");
        }
      }
      
    }
    else{
      printf(" - Sin precedentes                   │");
    }
    printf("\n");
  }
  printf    ("╰──────────────────────────────────────────────────────────╯\n");
  free(pendientes);
  return;
}




// 4 //
void marcarCompletada(Heap* grafo, char* nombre)
{
  int i;
  bool tareaEncontrada = false;
  
  for (i = 0; i < grafo->size; i++)
    {
      Tarea* tarea = (Tarea*)grafo->heapArray[i].data;
      if (strcmp(tarea->nombre, nombre) == 0)
      {
        tareaEncontrada = true;
        break;
      }
    }

  if (tareaEncontrada)
  {
    Tarea* tarea = (Tarea*)grafo->heapArray[i].data;

    if(tarea->numPrecedentes > 0)
    {
      printf("¿Estás seguro que deseas eliminar la tarea '%s'? (S/N): ", tarea->nombre);
      char respuesta[10];
      scanf("%s", respuesta);

      if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0)
      {
        tarea->completada = true;
        
        for(int j = 0; j< grafo->size - 1; j++)
        {
          Tarea* tareaActual = (Tarea*)grafo->heapArray[j].data;

          for(int k = 0; k < tareaActual->numPrecedentes; k++)
          {
            if(strcmp(tareaActual->precedentes[k], tarea->nombre) == 0)
            {
              free(tareaActual->precedentes[k]);

              tareaActual->numPrecedentes--;

              for(int m = k; m < tareaActual->numPrecedentes - 1; m++)
              {
                tareaActual->precedentes[m] = tareaActual->precedentes[m+1];
              }
              tareaActual->numPrecedentes--;
              tareaActual->precedentes = realloc(tareaActual->precedentes, tareaActual->numPrecedentes * sizeof(char*));
              k--;
            }
          }
        }
        
        for(; i < grafo->size - 1; i++)
        {
          grafo->heapArray[i] = grafo->heapArray[i+1];
        }
        grafo->size--;
        
        printf("La tarea '%s' ha sido marcada como completada y eliminada de la lista.\n", tarea->nombre);
      }
      else
      {
        printf("La tarea no ha sido eliminada.\n");
      }
    }
    else
    {
      tarea->completada = true;
      for (; i < grafo->size - 1; i++)
        {
          grafo->heapArray[i] = grafo->heapArray[i + 1];
        }
      grafo->size--;

      printf("La tarea '%s' ha sido marcada como completada y eliminada de la lista.\n", tarea->nombre);
    }
  }

  else
  {
    printf("La tarea '%s' no existe en la lista.\n", nombre);
  }
  return;
}


int main(void) {

  Heap* grafo = createHeap();
  
  char continuar = 's';
  char opcion;
  int valido;
  char input[250];
  char nombre[MAXCAR];
  int prioridad; 
  int numPrecedentes;
  char** precedentes = NULL;
  char tarea1[MAXCAR];
  char tarea2[MAXCAR];
  Tarea tarea;
  
  while (continuar == 's' || continuar == 'S') 
  {
    do {
        valido = 1;
        printMenu();
        scanf(" %c", &opcion);
        getchar();

        switch (opcion) {
          case '1':
            printf("Ingrese el nombre de la Tarea: ");
            scanf("%s", nombre);
            printf("Ingrese el numero de prioridad: ");
            scanf("%d", &prioridad);
            printf("Ingrese el numero de precedentes: ");
            scanf("%d", &numPrecedentes);
            if (numPrecedentes > 0)
            {
              precedentes = malloc(numPrecedentes * sizeof(char*));
              for (int i = 0; i < numPrecedentes; i++)
                {
                  precedentes[i] = malloc(MAXCAR * sizeof(char));
                  printf("Ingrese el nombre de la tarea precedente %d: ", i+1);
                  scanf("%s", precedentes[i]);
                }
            }
            Tarea *nuevaTarea = crearTarea(nombre, prioridad, precedentes, numPrecedentes);
            AgregarTarea(grafo, nuevaTarea);
            break;
          case '2':
            printf("Ingrese el nombre de la tarea precedente: ");
            scanf("%s", tarea1);
            printf("Ingrese el nombre de la tarea a la que se agregará una precedencia: ");
            scanf("%s", tarea2);
            addPrecedencia(grafo, tarea1, tarea2);
            break;
          case '3':
            printPendientes(grafo);
            break;
          case '4' : 
            printf("Ingrese el nombre de la tarea completada: ");
            scanf("%s", nombre);
            marcarCompletada(grafo, nombre);
            break; 
          case '0':
            printf("¡Hasta luego!\n");
            exit(0);
          
          default:
            printf("Opción inválida. Por favor, ingrese un número válido.\n");
            valido = 0;
        }
    } while (!valido);
  }
return 0;
}