#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>


/*
 * Napoveda
 */
void print_help(){
	printf("Pouziti: ./generator -size N -orient true/false -max X\n");
}

/* 
 * Generovani cisel - 0 az max
 */
int get_random (int max) {
	return rand() % max;
}

/*
 * Generovani grafu
 */

int create_graphs(int n, bool orient, int max) {
	FILE *file;
	int i;
	for (i = 0; i < n; i++) {
		
		char filename[11];
		int j, k;
		int matrix[i][i];
		
		sprintf(filename, "graph%d.txt", i);
		file = fopen(filename, "wb");
		if (file == NULL) {
			return -1;
		}
		
		//generovani tabulky
		for (j = 0; j < i; j++) {
			for (k = 0; k < i; k++) {
				if (k == j) {
					matrix[j][k] = 0; 
				} else {
					//vygeneruj random cislo od 0 po max
					int x = get_random(max);
					//uloz ho na spravnou pozici
					matrix[j][k] = x;
					if (orient) {
						//pokud je orientovany, tak na opacnou pozici vlozim 0
						matrix[k][j] = 0;
					} else {
						//pokud neni orientovany, tak graf je symetricky podle diagonaly
						matrix[k][j] = x;
					}
				}
			}
		}
		//ulozeni do souboru
		for (j = 0; j < i; j++) {
			for (k = 0; k < i; k++) {
				if (k + 1 != i) {
					//neni konec radku
					fprintf(file, "%d,", matrix[j][k]);
				} else {
					if (j + 1 != i){
						//neni posledni radek
						fprintf(file, "%d\n", matrix[j][k]);
					} else {
						//posledni radek
						fprintf(file, "%d", matrix[j][k]);
					}
				}
			}
		}
		printf("Graf %d vytvoren\n", i);
		fclose(file);
	}
	return n;
}

int main(int argc, char *argv[]) {
	
	int n;
	bool orient;
	int max;
	
	if (argc == 1) {
		print_help();
		fprintf(stderr, "Chyba argumentu:");
		return -1;
	} else if (argc == 7) {
		if (strcmp(argv[1], "-size") == 0) {
			n =  strtod(argv[2], '\0');
			if (n <= 0) {
				print_help();
				fprintf(stderr, "Chyba argumentu: N je mensi nebo roven 0\n");
				return -1;
			}
		} else {
			print_help();
			fprintf(stderr, "Chyba argumentu");
			return -1;
		}
		if (strcmp(argv[3], "-orient") == 0) {
			if (strcmp(argv[4], "true") == 0) {
				orient = true;
			} else if (strcmp(argv[4], "false") == 0) {
				orient = false;
			} else {
				print_help();
				fprintf(stderr, "Chyba argumentu: Neplatna hodnota argumentu -orient");
				return -1;
			}
		} else {
			print_help();
			fprintf(stderr, "Chyba argumentu");
			return -1;
		}
		if (strcmp(argv[5], "-max") == 0) {
			max =  strtod(argv[6], '\0');
			max++;
			if (n <= 0) {
				print_help();
				fprintf(stderr, "Chyba argumentu: max je mensi nebo roven 0\n");
				return -1;
			}
		} else {
			print_help();
			fprintf(stderr, "Chyba argumentu");
			return -1;
		} 
	} else {
		print_help();
		fprintf(stderr, "Chyba argumentu");
		return -1;
	}
	
	create_graphs(n, orient, max);
	
	
	return 0;
}