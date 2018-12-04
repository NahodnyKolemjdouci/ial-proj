#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define IN 999999 //my "infinite number", used at begening of dijsktra alg.

// pro zajisteni ze se nedostanu do zapornych cisel
size_t safe_usub(size_t x, size_t y) {
    return x > y ? x - y : y - x;
}

//presklada prvky pole: poledni = prvni atd.
char *str_reverse(const char *const str) {
    if (!str) { return NULL; }

    size_t len = strlen(str);//strnlen(str, MAX_CHARS);
    char *revesre = malloc(sizeof(char) * len);

    size_t i;
    for (i = 0; i < len; i++) {
        revesre[i] = str[safe_usub(i + 1, len)];
    }

    revesre[i] = 0;

    return revesre;
}

int dijsktra(int **cost, int source, int target, int size) {
    int dist[size], selected[size], i, start, m, min, d, j;
    for (i = 0; i < size; i++) {
        dist[i] = IN;
        selected[i] = 0;
    }
    start = source;
    selected[start] = 1;
    dist[start] = 0;
    j = 0;
    while (j < size - 1) {
        min = IN;
        m = 0;
        for (i = 0; i < size; i++) {
            if (cost[i][start] != 0) {
                d = dist[start] + cost[i][start];
                if (d < dist[i] && selected[i] == 0) {
                    dist[i] = d;
                }
                if (min > dist[i] && selected[i] == 0) {
                    min = dist[i];
                    m = i;
                }
            }
        }
        start = m;
        j++;
        selected[start] = 1;
    }
    if (dist[target] == IN) {

        fprintf(stderr, "Cesta neexistuje\n");

        exit(-1);
        //return (-1);
    }
    return dist[target];
}

void print_help() {
    printf("Použití: \n ./proj + [graf.txt] +[-b(Bellman Ford)|-d(Dijkstra)]\n");
}

int *allocate_mem(int ***arr, int n, int m) {
    *arr = (int **) malloc(n * sizeof(int *));
    int *arr_data = malloc(n * m * sizeof(int));
    for (int i = 0; i < n; i++) {
        (*arr)[i] = arr_data + i * m;
    }
    return arr_data; //free point
}

void deallocate_mem(int ***arr) {
    free(*arr);
    *arr = NULL;
}

int count_lines(char *filename) {
    int lines = 0;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Chyba otevirani souboru\n");
        exit(-2);
    }
    char ch, last;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n' && last != '\n') {
            lines++;
        }
        last = ch;
    }
    if (lines > 0) {
        lines++;
    }
    return lines;

}

void print_with_indent(int indent, int number) {
    printf("%*s%i", indent, "", number);
}

void printout_matrix(int **matrix, int rows, int columns) {
    int i, j;

    printf("  ");
    for (i = 0; i < columns; i++) {
        printf("\tn%i ", i);//+ 65

    }
    printf("\n");

    for (i = 0; i < rows; i++) {
        printf("n%i ", i);//+ 65
        for (j = 0; j < columns; j++) {
            printf("\t%d ", matrix[i][j]);
        }
        printf("\n");
        j = 0;
    }
}

int **get_matrix(char *filename, int size) {


    char buffer[1024];
    char *record, *line;
    int i = 0, j = 0;
    int **matrix;
    char *bin = NULL;

    allocate_mem(&matrix, size, size);

    if (matrix == NULL) {
        fprintf(stderr, "Chyba pameti\n");
        exit(-3);
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Chyba otevirani souboru\n");
        exit(-2);
    }

    while ((line = fgets(buffer, sizeof(buffer), file)) != NULL) {
        if (line[0] == '\n') {
            break;
        }
        record = strtok(line, " ,;");
        while (record != NULL) {
            matrix[i][j] = (int) strtol(record, &bin, 10);
            record = strtok(NULL, " ,;");
            j++;
        }
        i++;
        j = 0;
    }
    return matrix;
}

<<<<<<< HEAD
int main(int argc, char *argv[]){
    //printf("%s::%s::%s\n", argv[1], argv[2],argv[3]);

	if (argc == 1){
		print_help();
	}else if (argc >= 2) {
        int** matrix;
		int size = count_lines(argv[1]);

		if (argc == 2) {
            if (size > 0) {
                printf("\n Matrix: %s \n",argv[1]);
                printf("Pocet uzlu grafu: %d\n", size);
                matrix = get_matrix(argv[1], size);
                printout_matrix(matrix, size, size);
                deallocate_mem(&matrix);
            } else {
                fprintf(stderr, "Graf je prazdny\n");
                return(-1);
            }
		} else if (argc == 3 && (strcmp(argv[2], "dijkstra")) == 0) {
            if (size > 0) {
                matrix = get_matrix(argv[1], size);
                printf("Matrix: %s \n",argv[1]);
                printf("Pocet uzlu grafu: %d\n", size);
                printout_matrix(matrix, size, size);
                printf("Prozatimni reseni\n\n");

                int cost[N][N],i,j,weight,co; //,ch
                int max_weight;
                int source, target,x,y;
                printf("Dijsktra is here \n\n");
                printf("Enter the weight of the path between nodes:\n");
                scanf("%d",&max_weight);
                for(i=1;i< N;i++)
                    for(j=1;j< N;j++)
                        cost[i][j] = IN;
                for(x=1;x< N;x++)
                {
                    for(y=x+1;y< N;y++)
                    {
                        weight = (rand() % max_weight); //generovani ceny hran
                        printf("weight%i\n", weight);
                        cost [x][y] = cost[y][x] = weight;
=======
int BellmanFord(int **cost, int size, int target, int source) {
    int *distance = malloc(size * sizeof *distance);
    int i, j;
    for (i = 0; i < size; i++)
        distance[i] = IN;

    distance[target] = 0;
    for (int v = 0; v < size - 1; v++) {
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                if (cost[i][j] != 0) {
                    int new_distance = distance[i] + cost[i][j];
                    if (new_distance < distance[j]) {
                        distance[j] = new_distance;
>>>>>>> master
                    }
                }
            }
        }
    }

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (cost[i][j] != 0) {
                if (distance[j] > distance[i] + cost[i][j]) {
                    fprintf(stderr, "Negativni cyklus\n");
                    free(distance);
                    exit(-1);
                }
            }
        }
    }

    if (distance[source] == IN) {

        fprintf(stderr, "Cesta neexistuje\n");
        exit(-1);
        //return (-1);
    }
    return distance[source];


}

int main(int argc, char *argv[]) {
    int co, source2, target2, size;
    //char source, target;
    clock_t begin, end;
    double time_spent;
    if (argc == 1) {
        print_help();
    } else if (argc == 3 && strcmp(argv[2], "-d") == 0) {
        size = count_lines(argv[1]);
        if (size > 0) {
            int **matrix = get_matrix(argv[1], size);
            printf("Matrix: %s \n", argv[1]);
            printf("Pocet uzlu grafu: %d\n", size);
            printout_matrix(matrix, size, size);

            printf("\nZadejte pocatecni misto:");
            scanf("%i", &target2);
            //printf("\ntarget2: %i\n",target2);

            /*
            if (!isdigit(target)) {
                target2 = target - 65;
            } else {
                target2 = target - '0';
            }
            printf("\ntarget2: %i\n",target2);
            */

            printf("\nZadejte cil:");
            scanf("%i", &source2);
            //printf("\nsrc2: %i\n",source2);

            /*
            if (!isdigit(source)) {
                source2 = source - 65;
            } else {
                source2 = source - '0';
            }
            */

            /*
            if (source2 > (size - 1) || source2 < 0|| target2 < 0 || target2 > (size - 1)) {
                fprintf(stderr, "Spatny cil nebo start\n");
                return (-1);
            }
             */

            begin = clock();

            co = dijsktra(matrix, source2, target2, size);

            end = clock();

            printf("\nNejkratsi cesta: %d\n", co);

            time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("\nTime: %f\n", time_spent);

            deallocate_mem(&matrix);

        } else {
            fprintf(stderr, "Graf je prazdny\n");
            return (-1);
        }

    } else if (argc == 3 && strcmp(argv[2], "-b") == 0) {
        size = count_lines(argv[1]);
        if (size > 0) {
            int **matrix = get_matrix(argv[1], size);
            printf("Matrix: %s \n", argv[1]);
            printf("Pocet uzlu grafu: %d\n", size);
            printout_matrix(matrix, size, size);

            printf("\nZadejte pocatecni misto:");
            scanf("%i", &target2);

            /*
            if (!isdigit(target)) {
                target2 = target - 65;
            } else {
                target2 = target - '0';
            }
            */

            printf("\nZadejte cil:");
            scanf("%i", &source2);

            /*
            if (!isdigit(source)) {
                source2 = source - 65;
            } else {
                source2 = source - '0';
            }
             */
            /*
            if (source2 > (size - 1) || source2 < 0|| target2 < 0 || target2 > (size - 1)) {
                fprintf(stderr, "Spatny cil nebo start\n");
                return (-1);
            }
             */

            begin = clock();
            co = BellmanFord(matrix, size, target2, source2);
            end = clock();

            printf("\nNejkratsi cesta: %d\n", co);

            time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("\nTime: %f\n", time_spent);

            deallocate_mem(&matrix);
        } else {
            fprintf(stderr, "Graf je prazdny\n");
            return (-1);
        }
    } else {
        fprintf(stderr, "Zadny soubor\n");
        print_help();
        return -1;
    }

    return 0;
}