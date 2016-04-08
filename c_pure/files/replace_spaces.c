

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	char* filename;
	char needly;
	if (argc > 2)
	{
		filename = argv[1];
		needly = *argv[2];
	}
	else { printf("Too few arguments: %d\n", argc); return 0; }
	printf("Filename: %s\nChar: %c\n", filename, needly);

	FILE* f = fopen(filename, "r");
	FILE* out = fopen("replace_spaces.out","w");
	if (!f)   {   printf("Input file error"); return 0; }
	if (!out) { printf("Output file error"); return 0; }

	int max = 0;
	fscanf(f,"%d\n",&max);
	max +=2;
	char* buffer = (char*)malloc(sizeof(char)*max);
	char c = '\0';
	while(c != EOF)
	{
		if (c==needly) c = ' ';
		fputc(c, out);
		c = fgetc(f);
	}

	fclose(f);
	fclose(out);
	free(buffer);
	return 0;
}

