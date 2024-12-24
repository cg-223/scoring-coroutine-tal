#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define SNULL (size_t)0
#define allocBig() (struct Big *)xmalloc(sizeof(struct Big))
//TODO (from the readme of OmegaNum)
/*
    abs
    neg
    cmp
    gt
    gte
    lt
    lte
    eq
    neq
    cmp_tolerance
    gt_tolerance
    gte_tolerance
    lt_tolerance
    lte_tolerance
    eq_tolerance
    neq_tolerance
    min
    max
    ispos
    isneg
    isnan
    isfinite
    isint
    floor
    ceiling
    round
    add
    sub
    mul
    div
    rec
    mod
    gamma
    fact
    pow
    exp
    sqrt
    cbrt
    root
    log10
    logBase
    log
    ln
    lambertw
    tetr
    iteratedexp
    iteratedlog
    layeradd
    layeradd10
    ssrt
    linear_sroot
    slog
    pent
    arrow
    chain
    hyper
    affordGeometricSeries
    affordArithmeticSeries
    sumGeometricSeries
    sumArithmeticSeries
    choose
    toString
    toValue
    toStringWithDecimalPlaces
    toExponential
    toFixed
    toPrecision
    toJSON
    toHyperE
*/

int maxArrow = 1000;
int capPageSize = 32; //power of 2
size_t defaultPages = 1; 

struct dblArray {
    double *first;
    size_t size;
    size_t capacity;
};

struct Big {
    struct dblArray *array;
    int sign;
    int nan;
};

void checkNull(void* thing) {
    if (thing == NULL) {
        perror("oom");
        exit(EXIT_FAILURE);
    };
};

void* xmalloc(size_t size) {
    void* ourptr = malloc(size);
    if (ourptr == NULL) {
        perror("oom");
        exit(EXIT_FAILURE);
    };
    return ourptr;
};

void initDblArray(struct dblArray *array, double initial, size_t pages) {

    if (pages == SNULL) {
        pages = defaultPages;
    };


    array->first = (double *)xmalloc(pages * capPageSize * sizeof(double)); //capPageSize * pages doubles

    memset(array->first, 0, pages * capPageSize * sizeof(double));


    array->first[0] = initial;
    array->size = 0;
    array->capacity = pages * capPageSize;
};

void initBig(struct Big *big, double initial) {
    big->sign = 1;
    big->nan = 0;
    struct dblArray *mydbl = (struct dblArray *)xmalloc(sizeof(struct dblArray));
    big->array = mydbl;
    initDblArray(mydbl, initial, SNULL);
}

double toDouble(struct Big *big) {
    double toReturn = 0;
    if (big->sign == -1) {
        //return -1*(self:neg():to_number())
    };
    if (big->nan != 0) {
        return NAN;
    };

    //if ((#self.array>=2) and ((self.array[2]>=2) or (self.array[2]==1) and (self.array[1]>308))) then
    //    return R.POSITIVE_INFINITY;
    //end
    //if (#self.array >= 3) and ((self.array[1] >= 3) or (self.array[2] >= 1) or (self.array[3] >= 1)) then
        //return R.POSITIVE_INFINITY;
    //end
    //if (#self.array >= 4) and ((self.array[1] > 1) or (self.array[2] >= 1) or (self.array[3] >= 1)) then
    //    for i = 4, #self.array do
    //        if self.array[i] > 0 then
    //            return R.POSITIVE_INFINITY;
    //        end
    //    end
    //end
    //if (type(self.array[1]) == "table") then
    //    self.array[1] = self.array[1]:to_number()
    //end
    //if (self.array[2]==1) then
    //    return math.pow(10,self.array[1]);
    //end
    return big->array->first[0];
};

void freeBig(struct Big *big) {
    free(big->array->first);
    free(big->array);
    free(big);
};


int main() {
    struct Big *ourBig = allocBig();

    initBig(ourBig, 3);
    printf("Final double: %1.0f.\n", toDouble(ourBig));

    clock_t start, end;
    double diff;
    start = clock();

    for (int i = 1; i <= 30000000; i++) {
        struct Big *testBig = allocBig();
        initBig(testBig, i);
        freeBig(testBig);
        if (i % 100000 == 0) {
            printf("%10.6f\n", toDouble(testBig));
        };
    };

    end = clock();
    diff = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Finished in %3.6f", diff);
    return 0;
};