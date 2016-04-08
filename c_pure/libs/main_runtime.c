#include <stdio.h>
#include <dlfcn.h>

int main()
{
    void *lhandler = dlopen("./libmylib.so", RTLD_LAZY);
    if (!lhandler)
    {
        fprintf(stderr,"dlopen error :\n%s\n", dlerror());
        return 0;
    }

    float (*sum)(float,float) = dlsym(lhandler,"sum");
    float (*sub)(float,float) = dlsym(lhandler,"sub");
    float (*mult)(float,float) = dlsym(lhandler,"mult");

    printf("Runtime linking example\nInsert A and B floats\n");
    float a, b;
    scanf("%f %f", &a, &b);
    float fsum = (*sum)(a,b);
    float fsub = (*sub)(a,b);
    float fmult = (*mult)(a,b);
    printf("sum\t%f\nsub\t%f\nmult\t%f\n", fsum, fsub, fmult);
    return 0;
}
