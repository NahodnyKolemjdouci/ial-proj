/**
 * proj.c
 *
 * Zadání č.4 - Nejkratší cesta v grafu:
 * Vytvořte program pro hledání nejkratší cesty v grafu mezi dvěma zadanými vrcholy.
 * Úlohu řešte pro neorientované i orientované grafy a pro neohodnocené i ohodnocené grafy.
 * V případě ohodnocených grafů považujte za vzdálenost vrcholů ohodnocení hrany mezi nimi.
 * Pokud existuje více řešení, nalezněte všechna. Výsledky prezentujte vhodným způsobem.
 * Součástí projektu bude načítání grafů ze souboru a vhodné testovací grafy.
 * V dokumentaci uveďte teoretickou složitost úlohy a porovnejte ji s experimentálními výsledky.
 *
 * Authors:
 * xkolbj00
 * xsalus00
 * xturek05
 *
 * * Posledni zmena 5.12.2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define IN 999999 //my "infinite number", used at begening of dijsktra alg.

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

        fprintf(stderr, "Path does not exist\n");

        exit(-1);
    }
    return dist[target];
}

void print_help() {
    printf("Usage: \n ./proj + [graf.txt] +[-b(Bellman Ford)|-d(Dijkstra)]\n");
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
        fprintf(stderr, "Can't open file\n");
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

    char buffer[8192];
    char *record, *line;
    int i = 0, j = 0;
    int **matrix;
    char *bin = NULL;

    allocate_mem(&matrix, size, size);

    if (matrix == NULL) {
        fprintf(stderr, "Malloc error\n");
        exit(-3);
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Can't open file\n");
        exit(-2);
    }

    while ((line = fgets(buffer, sizeof(buffer), file)) != NULL) {
        if (line[0] == '\n') {
            break;
        }
        record = strtok(line, " ,;");
        while (record != NULL) {
            matrix[i][j] = (int)strtol(record, &bin, 10);
            record = strtok(NULL, " ,;");
            j++;
        }
        i++;
        j = 0;
    }
    return matrix;
}

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
                    }
                }
            }
        }
    }

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (cost[i][j] != 0) {
                if (distance[j] > distance[i] + cost[i][j]) {
                    fprintf(stderr, "Negative cycle\n");
                    free(distance);
                    exit(-1);
                }
            }
        }
    }

    if (distance[source] == IN) {

        fprintf(stderr, "Path does not exist\n");
        exit(-1);
    }
    return distance[source];


}

int main(int argc, char *argv[]) {
    int co, source2, target2, size;
    clock_t begin, end;
    double time_spent;
    if (argc == 1) {
        print_help();
    } else if (argc == 3 && strcmp(argv[2], "-d") == 0) {
        size = count_lines(argv[1]);
        if (size > 0) {
            int **matrix = get_matrix(argv[1], size);
            printf("Matrix: %s \n", argv[1]);
            printf("Node count: %d\n", size);
            printout_matrix(matrix, size, size);

            printf("\nEnter starting node:");
            scanf("%i", &target2);

            printf("\nEnter goal:");
            scanf("%i", &source2);

            if (source2 > (size - 1) || source2 < 0|| target2 < 0 || target2 > (size - 1)) {
                fprintf(stderr, "Invalid goal or start\n");
                return (-1);
            }

            begin = clock();

            co = dijsktra(matrix, source2, target2, size);

            end = clock();

            printf("\nShortest path: %d\n", co);

            time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("\nTime: %f[seconds]\n", time_spent);

            deallocate_mem(&matrix);

        } else {
            fprintf(stderr, "Graph is empty\n");
            return (-1);
        }

    } else if (argc == 3 && strcmp(argv[2], "-b") == 0) {
        size = count_lines(argv[1]);
        if (size > 0) {
            int **matrix = get_matrix(argv[1], size);
            printf("Matrix: %s \n", argv[1]);
            printf("Node count: %d\n", size);
            printout_matrix(matrix, size, size);

            printf("\nEnter starting node:");
            scanf("%i", &target2);


            printf("\nEnter goal:");
            scanf("%i", &source2);

            if (source2 > (size - 1) || source2 < 0|| target2 < 0 || target2 > (size - 1)) {
                fprintf(stderr, "Invalid goal or start\n");
                return (-1);
            }

            begin = clock();
            co = BellmanFord(matrix, size, target2, source2);
            end = clock();

            printf("\nShortest path: %d\n", co);

            time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("\nTime: %f[seconds]\n", time_spent);

            deallocate_mem(&matrix);
        } else {
            fprintf(stderr, "Graph is empty\n");
            return (-1);
        }
    } else {
        fprintf(stderr, "Can't open file\n");
        print_help();
        return -1;
    }

    return 0;
}