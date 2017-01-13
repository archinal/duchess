#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_PLOTS 256

void printArrayAsPlots(double arr[], int n);

int main(int argc, char* argv[]) {
    int scanned = 1;
    int numScanned = 0;
    double arr[MAX_PLOTS];
    double x;
    int cont = TRUE;

    while (cont) {
        scanned = scanf("%lf", &x);
        arr[numScanned] = x;
        numScanned++;
        if (scanned == 0 || numScanned == MAX_PLOTS) {
            cont = FALSE;
        }
    }

    printArrayAsPlots(arr, numScanned);

    return EXIT_SUCCESS;
}

void printArrayAsPlots(double arr[], int n) {
    printf("{");
    int i;
    for (i = 0; i < n; i++) {
        printf("(%d,%lf) ", i, arr[i]);
    }
    printf("};");
}
