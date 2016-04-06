#include <stdio.h>
#include <stdlib.h>
#define STRLEN 20

struct PATH 
{
	char * name;
	float  lenght;
	int  stops;
	float  cost;
};

void sort(struct PATH ** array, int n)
{
	struct PATH* swap;
	for (int c = 0 ; c < ( n - 1 ); c++)	
	{
      		for ( int d = 0 ; d < n - c - 1; d++)
	        {
	        	struct PATH * p = array[d],
	        		    * p_next = array[d+1];
	        	
	                //if (array[d] > array[d+1])
	                if (p->cost < p_next->cost)
	                    { 
                              swap       = array[d];	                              
                              array[d]   = array[d+1];
                              array[d+1] = swap;                                      
                            }	                                                    
                }
	                                                        
	}
}

int main()
{
	int n = 0;
	printf("Insert number of structs: \n>>");
	scanf("%d", &n);
	printf("Insert lenght[float] cost[float] costs[int] name[string]:\n");
	struct PATH * pathes = (struct PATH*)malloc(sizeof(struct PATH) * n);
	struct PATH ** pathes_ptr = (struct PATH**)malloc(sizeof(struct PATH*) * n);
	for (int i = 0; i < n; i++)
	{
		float c =0.0f, l =0.0f;
		int s = 0;
		char* name = (char*)malloc(sizeof(char) * STRLEN);
		printf(">> ");
		scanf("%f %f %d %s", &l,&c,&s,name);
		struct PATH p;
		p.name = name;
		p.lenght = l;
		p.cost = c;
		p.stops = s;
		pathes[i] = p;
		pathes_ptr[i] = &pathes[i];
	}
	
	sort(pathes_ptr, n);
	printf("Source\n#\tlen\tcost\tstops\tname\n");
	for (int i =0; i < n; i++)
	{
		printf("%d.\t%f\t%f\t%d\t%s\n", 
				i,
				pathes[i].lenght,
				pathes[i].cost,
				pathes[i].stops,
				pathes[i].name);
	}
	
	printf("Sorted\n#\tlen\tcost\tstops\tname\n");
	for (int i =0; i < n; i++)
	{
		struct PATH * p = pathes_ptr[i];
		printf("%d.\t%f\t%f\t%d\t%s\n", 
				i,
				p->lenght,
				p->cost,
				p->stops,
				p->name);
	}
	
	
	return 0;
}
