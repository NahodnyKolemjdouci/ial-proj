/**
 * gnereator.c
 *
 * Authors:
 * xkolbj00
 * xsalus00
 * xturek05
 *
 * Zadání č.4 Nejkratší cesta v grafu
 * Posledni zmena 5.12.2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

void print_help() {
    printf("Usage: ./generator -size N -orient true/false -max X\n");
}

/* 
 * Generovani cisel - 0 az max
 */
int get_random(int max) {
    return rand() % max;
}

/*
 * Generovani grafu
 */
int create_graphs(int n, bool orient, int max) {
    FILE *file;

    char filename[20];
    int j, k;
    int matrix[n][n];

    sprintf(filename, "graph%d.txt", n);
    file = fopen(filename, "wb");
    if (file == NULL) {
        return -1;
    }

    //generate table
    for (j = 0; j <n; j++) {
        for (k = 0; k < n; k++) {
            if (k == j) {
                matrix[j][k] = 0;
            } else {
                //generate random 0-max
                int x = get_random(max);
                //insert into matrix
                matrix[j][k] = x;
                if (orient) {
                    //if it's oriented insert 0 on oposit position
                    matrix[k][j] = 0;
                } else {
                    //if not oriented
                    matrix[k][j] = x;
                }
            }
        }
    }
    //save to file
    for (j = 0; j < n; j++) {
        for (k = 0; k < n; k++) {
            if (k + 1 != n) {
                //not end of the line
                fprintf(file, "%d,", matrix[j][k]);
            } else {
                if (j + 1 != n) {
                    //not last line
                    fprintf(file, "%d\n", matrix[j][k]);
                } else {
                    //last line
                    fprintf(file, "%d", matrix[j][k]);
                }
            }
        }
    }
    printf("Graph %d created\n", n);
    fclose(file);
    return n;
}

int main(int argc, char *argv[]) {

    int n;
    bool orient;
    int max;

    if (argc == 1) {
        print_help();
        fprintf(stderr, "Invalid argument\n");
        return -1;
    } else if (argc == 7) {
        if (strcmp(argv[1], "-size") == 0) {
            n = strtod(argv[2], '\0');
            if (n <= 0) {
                print_help();
                fprintf(stderr, "Invalid argument: N <= 0\n");
                return -1;
            }
        } else {
            print_help();
            fprintf(stderr, "Invalid argument\n");
            return -1;
        }
        if (strcmp(argv[3], "-orient") == 0) {
            if (strcmp(argv[4], "true") == 0) {
                orient = true;
            } else if (strcmp(argv[4], "false") == 0) {
                orient = false;
            } else {
                print_help();
                fprintf(stderr, "Invalid argument: invalid value -orient\n");
                return -1;
            }
        } else {
            print_help();
            fprintf(stderr, "Invalid argument\n");
            return -1;
        }
        if (strcmp(argv[5], "-max") == 0) {
            max = strtod(argv[6], '\0');
            max++;
            if (n <= 0) {
                print_help();
                fprintf(stderr, "Invalid argument: max <= 0\n");
                return -1;
            }
        } else {
            print_help();
            fprintf(stderr, "Invalid argument\n");
            return -1;
        }
    } else {
        print_help();
        fprintf(stderr, "Invalid argument\n");
        return -1;
    }

    create_graphs(n, orient, max);


    return 0;
}