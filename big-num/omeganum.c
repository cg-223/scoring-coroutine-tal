#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>



int maxArrow = 1000;
int capPageSize = 32; //power of 2
int defaultPages = 1; 

struct dblArray {
    double *first;
    size_t size;
    size_t capacity;
};

struct Big {
    struct dblArray *array;
    int sign;
};

void checkNull(void* thing) {
    if (thing == NULL) {
        perror("oom");
        exit(EXIT_FAILURE);
    };
};

void* xmalloc(size_t size) {
    void* ourptr = malloc(size);
    checkNull(ourptr);
    return ourptr;
};

void initDblArray(struct dblArray *array, double initial, size_t pages) {

    if (pages == NULL) {
        pages = defaultPages;
    };


    array->first = (double *)xmalloc(pages * capPageSize * sizeof(double)); //capPageSize * pages doubles

    memset(array->first, NULL, pages * capPageSize * sizeof(double));


    array->first[0] = initial;
    array->size = 0;
    array->capacity = pages * capPageSize;
};

void initBig(struct Big *big, double initial) {
    big->sign = 1;
    struct dblArray *mydbl;
    big->array = mydbl;
    initDblArray(mydbl, initial, NULL);
};



bool ext = true;


char omegaNumError[] = "[OmegaNumError] ";
char invalidArgument[] = "[OmegaNumError] Invalid Argument: ";

int main() {
    return 0;
};