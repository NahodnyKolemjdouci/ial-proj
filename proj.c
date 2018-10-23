#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>


void print_help(){
	printf("Použití: \n ./proj + [graf.txt]\n");
}

int* allocate_mem(int*** arr, int n, int m) {
  *arr = (int**)malloc(n * sizeof(int*));
  int *arr_data = malloc( n * m * sizeof(int));
  for(int i=0; i<n; i++) {
     (*arr)[i] = arr_data + i * m ;
  }
  return arr_data; //free point
} 

void deallocate_mem(int*** arr){
    free(*arr);
    *arr=NULL;
}

int count_lines(char *filename) {
	int lines = 0;
	FILE *file = fopen(filename,"r");
	if (file == NULL) {
		fprintf(stderr, "Chyba otevirani souboru\n");
		exit(-2);
	}
	char ch;
	while(!feof(file)) {
		ch = fgetc(file);
		if(ch == '\n') {
			lines++;
		}
	}
	if (lines > 0) {
		lines++;
	}
	return lines;

}

void printout_matrix(int **matrix, int rows, int columns) {
	int i, j;
	printf("  ");
	for (i = 0; i < columns; i++) {
		printf("%c ", i + 65);
	}
	printf("\n");
	
	for (i = 0; i < rows ; i++) {	
		printf("%c ", i + 65);
		for (j = 0; j < columns; j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
		j = 0;
	}
}

int **get_matrix(char *filename, int size) {
	
	
	char buffer[1024] ;
    char *record, *line;
    int i=0,j=0;
	int **matrix;

    allocate_mem(&matrix, size, size);

	if (matrix == NULL) {
		fprintf(stderr, "Chyba pameti\n");
		exit(-3);
	}

	FILE *file = fopen(filename,"r");
	if (file == NULL) {
		fprintf(stderr, "Chyba otevirani souboru\n");
		exit(-2);
	}

	while((line = fgets(buffer, sizeof(buffer), file)) != NULL) {
		record = strtok(line, " ,;");
		while(record != NULL) {
			matrix[i][j] = atoi(record);
			record = strtok(NULL, " ,;");
			j++;
		}
		i++;
		j = 0;
	}

	return matrix;
}

int main(int argc, char *argv[]){
	if (argc == 1){
		print_help();
	}else if (argc == 2) {
		int size = count_lines(argv[1]);
		printf("Pocet uzlu grafu: %d\n", size);
		if (size > 0) {
			int** matrix = get_matrix(argv[1], size);
			printout_matrix(matrix, size, size);
            deallocate_mem(&matrix);
		} else {
			fprintf(stderr, "Graf je prazdny\n");
			return(-1);
		}

	} else {
		fprintf(stderr, "Zadny soubor\n");
		print_help();
		return -1;
	}
	return 0;
}