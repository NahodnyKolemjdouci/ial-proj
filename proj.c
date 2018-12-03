#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>

#define IN 999 //my "infinite number", used at begening of dijsktra alg.

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
    int dist[size], prev[size],
    selected[size],
    i, start,
    m,
            min,
    d,
            j;
    char path[size];
    for (i = 0; i < size; i++) {
        dist[i] = IN;
        selected[i] = 0;
        prev[i] = -1;
    }
    start = source;
    selected[start] = 1;
    dist[start] = 0;
    // while (selected[target] == 0) {
    j = 0;
   /* while (j<size-1) {
        min = IN;
        m = 0;
        for (i = 0; i < size; i++) {
            d = dist[start] + cost[start][i];
            if (d < dist[i] && selected[i] == 0) {
                dist[i] = d;
                prev[i] = start;
            }
            if (min > dist[i] && selected[i] == 0) {
                min = dist[i];
                m = i;
            }
        }
        start = m;
        j++;
        selected[start] = 1;
    }*/
    while (j<size-1) {
        min = IN;
        m = 0;
        for (i = 0; i < size; i++) {
            if(cost[i][start] != 0 ) {
                d = dist[start] + cost[i][start];
                if (d < dist[i] && selected[i] == 0) {
                    dist[i] = d;
                    prev[i] = start;
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
    int tmp = target;
    j = 0;
    while (target != -1) {
        path[j++] = target + 65;
        target = prev[target];
    }
    path[j] = '\0';
    str_reverse(path);
    if (dist[tmp] == IN) {

        fprintf(stderr, "Cesta neexistuje\n");
        return (-1);
    }
    printf("%s", path);
    return dist[tmp];
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

void printout_matrix(int **matrix, int rows, int columns) {
    int i, j;
    printf("  ");
    for (i = 0; i < columns; i++) {
        printf("%c ", i + 65);
    }
    printf("\n");

    for (i = 0; i < rows; i++) {
        printf("%c ", i + 65);
        for (j = 0; j < columns; j++) {
            printf("%d ", matrix[i][j]);
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
                    puts("Negativni");
                    free(distance);

                    return (-1);
                }
            }
        }
    }

    if (distance[source] == IN) {

        fprintf(stderr, "Cesta neexistuje\n");
        return (-1);
    }
    return distance[source];


}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_help();
    } else if (argc == 3 && strcmp(argv[2], "-d") == 0) {
        int size = count_lines(argv[1]);
        int co;
        int source2, target2;
        char source, target;
        if (size > 0) {
            int **matrix = get_matrix(argv[1], size);
            printf("Matrix: %s \n", argv[1]);
            printf("Pocet uzlu grafu: %d\n", size);
            printout_matrix(matrix, size, size);


            // nastavim vsechny 0 na IN(999)
            /*for (int i = 0; i < size; i++) {
                for (int j = 0; j < i; j++) {
                    if (matrix[i][j] == 0) {
                        matrix[i][j] = matrix[j][i] = IN;
                    }
                }
            }*/

            printf("\nZadejte pocatecni misto:");
            scanf("%s", &target);
            if (!isdigit(target)) {
                target2 = target - 65;
            } else {
                target2 = target - '0';
            }

            printf("\nZadejte cil:");
            scanf("%s", &source);
            if (!isdigit(source)) {
                source2 = source - 65;
            } else {
                source2 = source - '0';
            }

            if (source2 > (size - 1) || target2 > (size - 1)) {
                fprintf(stderr, "Spatny cil nebo start\n");
                return (-1);
            }
            co = dijsktra(matrix, source2, target2, size);

            if (co >= 0) {
                printf("\nNejkratsi cesta: %d\n", co);
            }
            deallocate_mem(&matrix);

        } else {
            fprintf(stderr, "Graf je prazdny\n");
            return (-1);
        }

    } else if (argc == 3 && strcmp(argv[2], "-b") == 0) {
        int size = count_lines(argv[1]);
        int co;
        int source2, target2;
        char source, target;
        int **matrix = get_matrix(argv[1], size);
        printf("Matrix: %s \n", argv[1]);
        printf("Pocet uzlu grafu: %d\n", size);
        printout_matrix(matrix, size, size);

        printf("\nZadejte pocatecni misto:");
        scanf("%s", &target);
        if (!isdigit(target)) {
            target2 = target - 65;
        } else {
            target2 = target - '0';
        }

        printf("\nZadejte cil:");
        scanf("%s", &source);
        if (!isdigit(source)) {
            source2 = source - 65;
        } else {
            source2 = source - '0';
        }

        if (source2 > (size - 1) || target2 > (size - 1)) {
            fprintf(stderr, "Spatny cil nebo start\n");
            return (-1);
        }

        co = BellmanFord(matrix, size, target2, source2);
        if (co >= 0) {
            printf("\nNejkratsi cesta: %d\n", co);
        } else {
            fprintf(stderr, "Cesta neexistuje\n");
            return (-1);
        }
        deallocate_mem(&matrix);
    } else {
        fprintf(stderr, "Zadny soubor\n");
        print_help();
        return -1;
    }

    return 0;
}