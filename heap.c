#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include "heap.h"

typedef struct nodo{
   void* data;
   int priority;
}heapElem;

typedef struct Heap{
  heapElem* heapArray;
  int size;
  int capac;
} Heap;


void* heap_top(Heap* pq)
{
  if(pq->size == 0)
  {
    return NULL;
  }
  return pq->heapArray[0].data;
  
}



void heap_push(Heap* pq, void* data, int priority)
{
  //verificar si el arreglo esta o no lleno
  if (pq->size == pq->capac)
  {
    int newCapac = pq->capac * 2 + 1;
    pq->heapArray = realloc(pq->heapArray, newCapac * sizeof(heapElem));
    pq->capac = newCapac;
  }

  //insertar
  int i = pq->size;
  pq->heapArray[i].data = data;
  pq->heapArray[i].priority = priority;
  pq->size++;

  while(i>0 && pq->heapArray[i].priority > pq->heapArray[(i-1)/2].priority)
    {
      heapElem temp = pq->heapArray[i];
      pq->heapArray[i] = pq->heapArray[(i-1)/2];
      pq->heapArray[(i-1)/2] = temp;
      i = (i-1)/2;
    }

}


void heap_pop(Heap* pq)
{
  //si el monticulo esta vacio
  if(pq->size == 0)
  {
    return;
  }
  //mover ultimo elem a raiz
  pq->heapArray[0] = pq->heapArray[pq->size-1];
  pq->size--;

  //ajuste
  int i = 0;
  while(1)
    {
      int hijoIzq = 2 * i + 1;
      int hijoDer = 2 * i + 2;
      int hijoMayor = i;
      
      //buscar hijo mayor
      if(hijoIzq < pq->size && pq->heapArray[hijoIzq].priority > pq->heapArray[hijoMayor].priority)
      {
        hijoMayor = hijoIzq;
      }

      if(hijoDer < pq->size && pq->heapArray[hijoDer].priority > pq->heapArray[hijoMayor].priority)
      {
        hijoMayor = hijoDer;
      }  

      if(hijoMayor == i)
      {
        break;
      }

      //intercambiar
      heapElem temp = pq->heapArray[i];
      pq->heapArray[i] = pq->heapArray[hijoMayor];
      pq->heapArray[hijoMayor] = temp;

      i = hijoMayor;
    }
}

Heap* createHeap()
{
  Heap* heap = (Heap*)malloc(sizeof(Heap));
  heap->heapArray = (heapElem*)malloc(3*sizeof(heapElem));
  heap->size = 0;
  heap->capac = 3;

   return heap;
}
