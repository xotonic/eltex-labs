#include "mylib.h"
#include <stdio.h>
int main()
{
    printf("Static lib example\nInsert A and B floats\n");
    float a, b;
    scanf("%f %f", &a, &b);
    float fsum = sum(a,b);
    float fsub = sub(a,b);
    float fmult = mult(a,b);
    printf("sum\t%f\nsub\t%f\nmult\t%f\n", fsum, fsub, fmult);
}
