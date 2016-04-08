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
	FILE* out = fopen("anti_grep_string.out","w");
	if (!f) { printf("Input file error"); return 0; }
	if (!out) { printf("Output file error"); return 0; }

	int max = 0;
	fscanf(f,"%d\n",&max);
	max +=2;
	char* buffer = (char*)malloc(sizeof(char)*max);

	while(1)
	{
		int n = fgets(buffer, max ,f );
		if (n == 0) break;
		int finded = 0;

		for (int i = 0; i < max; i++)
		{
			if (buffer[i] == needly){ finded = 1; break;}
		}

		if (finded==0) 
		fprintf(out, "%s", buffer);
	}

	fclose(f);
	fclose(out);
	free(buffer);
	return 0;
}

