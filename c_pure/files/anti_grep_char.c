
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
	FILE* out = fopen("anti_grep_char.out","w");
	if (!f) { printf("Input file error"); return 0; }
	if (!out) { printf("Output file error"); return 0; }

	int max = 0;
	fscanf(f,"%d\n",&max);
	max +=2;
	char* buffer = (char*)malloc(sizeof(char)*max);

	while(1)
	{
		int finded = 0;
		int eof = 0;
		for(int i = 0; i < max; i++)
		{
			char c = getc(f);
			if (c == EOF) eof = 1;
			if (c == needly) { finded = 1; }
			else if (c != '\n') {
				buffer[i] = c;
			}
			else {
				break;
			}
		}
		
		if (finded == 0 && eof == 0)
		fprintf(out, "%s\n", buffer);
		if (eof) break;
	}

	fclose(f);
	fclose(out);
	free(buffer);
	return 0;
}

