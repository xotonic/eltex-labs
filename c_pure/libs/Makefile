static:
	gcc -c math_lib.c -o math_lib.o
	ar rc libmymath.a math_lib.o
	ranlib libmymath.a
	gcc -L. main_static.c -o main_static -lmymath
dynamic:
	gcc -c math_lib.c -o math_lib.o
	gcc -shared -o math_lib.o libmylib.so
	gcc main_static.c -L. -l:libmylib.so  -o main_dynamic_linked
runtime:
	gcc -c math_lib.c -o math_lib.o
	gcc -shared -o math_lib.o libmylib.so
	gcc main_runtime.c -L. -ldl -o main_runtime
