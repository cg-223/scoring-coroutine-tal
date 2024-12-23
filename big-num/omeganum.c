#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>



int maxArrow = 1000;
int capPageSize = 4096; //power of 2

struct dblArray {
    double *first;
    size_t size;
    size_t capacity;
};

struct Big {
    struct dblArray *array;
    int sign;
};

void initDblArray(struct dblArray *array, size_t pages) {
    if (pages == NULL) {
        pages = 1;
    };
    array->first = (double *)malloc(pages * capPageSize * sizeof(double)); //capPageSize * pages doubles
    if (array->first == NULL) {
        perror("oom");
        exit(EXIT_FAILURE);
    };
    array->size = 0;
    array->capacity = pages * capPageSize;
};

void initBig(struct Big *big) {
    big->sign = 1;
    big->array->first = NULL;
    big->array->size = 0;
    big->array->capacity = 0;
};



bool ext = true;


char omegaNumError[] = "[OmegaNumError] ";
char invalidArgument[] = "[OmegaNumError] Invalid Argument: ";

int main() {
    return 0;
};