/*
 EXECICIO 2.7 Ponteiro 1
 
*/

#include <stdio.h>

void swap(int *p1, int *p2);

int main()
{
    int x = 10;
    int y = 20;
    
    swap(&x, &y);
    
    printf("x = %d, y = %d\n", x, y);
}


void swap(int *p1, int *p2){
    int *temp;
    printf("p1 = %d, valor = %d\n", p1, *p1);
    printf("p2 = %d, valor = %d\n", p2, *p2);
    printf("temp = %d, valor = %d\n", temp, &temp);
    *temp = *p1; //valor de temp <- valor de x.
    *p1 = *p2; //valor de x <- valor de y.
    *p2 = *temp; //valor de y <- valor de temp.
    
}