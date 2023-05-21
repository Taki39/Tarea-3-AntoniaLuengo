#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include "printFormato.h"


void printMenu() {
    printf("\n");
    printf("╭──────────────────────────────────────────────────────────╮\n");
    printf("│                     MENU PRINCIPAL                       │\n");
    printf("├──────────────────────────────────────────────────────────┤\n");
    printf("│ 1. Agregar Tarea                                         │\n");
    printf("│ 2. Establecer Precedencia entre Tareas                   │\n");
    printf("│ 3. Mostrar tareas por hacer                              │\n");
    printf("│ 4. Marcar tarea como completada                          │\n");
    printf("│ 0. Salir                                                 │\n");
    printf("╰──────────────────────────────────────────────────────────╯\n");
    printf("\nIngrese el número de la opción deseada: ");
}

void printMensajeGeneroJugadorCorrectamente() {
    printf("\n╔══════════════════════════════════════╗\n");
    printf(  "║  ¡Se agregó la tarea correctamente!  ║\n");
    printf(  "╚══════════════════════════════════════╝\n\n");
}

void printMensajeJugadorRepetido(){
  printf("Este nombre de usuario ya existe\n");
}

//Al parecer, cuando una cadena que corresponde a la clave del inventario contiene un carácter con acento, la función "strlen" detecta el carácter con acento como si fuera dos caracteres, debido a que es un carácter ancho cuya representación en bits es mayor que la de un solo carácter. Esto causaba problemas al imprimir los marcos en el menú, ya que necesitábamos saber el tamaño exacto de la cadena pero nos daba problemas al detectar caracteres demás debido a los acentos en algunos caracteres. Para solucionar esto, manejamos la situación de la siguiente manera:

int printInventario(char *palabra){
  
    setlocale(LC_ALL, ""); 
    mbstate_t state = {0};
    const char *s = palabra;
    size_t len = strlen(palabra);
    wchar_t wc;
    int tiene_acento = 0;
    
    while (s < palabra + len) {
        size_t n = mbrtowc(&wc, s, len - (s - palabra), &state);
        if (n == (size_t) -2 || n == (size_t) -1) {
            break;
        }
        s += n;

        if (iswalpha(wc) && wc > 127) {
            tiene_acento++;
            
        }
    }
    len-=tiene_acento;
    return len;
}

//esta funcion solo imprime los espacios necesarios
void printEspacios(int cantidad){

  while(cantidad!=0){
    printf(" ");
    cantidad--;
  }
}