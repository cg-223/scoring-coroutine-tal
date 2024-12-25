#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define SNULL (size_t)NULL
#define allocBig() (struct Big *)xmalloc(sizeof(struct Big))
//TODO (from the readme of OmegaNum)
/*
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
    isfinite
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
long long int maxSafeInt = 9e9; //arbitrary
long long int minSafeInt = -9e9;
//we use llongs
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

//unused as of now
//xmalloc uses its own thing
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

//initialize array, allocate initial space for array. set an initial value. 
void initDblArray(struct dblArray *array, double initial, size_t pages) {

    if (pages == SNULL) {
        pages = defaultPages;
    };


    array->first = (double *)xmalloc(pages * capPageSize * sizeof(double)); //capPageSize * pages doubles

    memset(array->first, 0, pages * capPageSize * sizeof(double));

    array->first[0] = initial;
    array->size = 1;
    array->capacity = pages * capPageSize;
};

void reallocArray(struct dblArray *array) {
    double* newptr = realloc(array->first, array->capacity + (capPageSize * sizeof(double)));

    if (newptr == NULL) {
        perror("oom");
        exit(EXIT_FAILURE);
    }
};

void initBig(struct Big *big, double initial) {
    big->sign = 1;
    big->nan = 0;
    struct dblArray *mydbl = (struct dblArray *)xmalloc(sizeof(struct dblArray));
    big->array = mydbl;
    initDblArray(mydbl, initial, SNULL);
};

void copyBig(struct Big* toCopy, struct Big* thisBig) {
    thisBig->sign = toCopy->sign;
    thisBig->nan = toCopy->nan;
    
    struct dblArray *mydbl = (struct dblArray *)xmalloc(sizeof(struct dblArray));
    thisBig->array = mydbl;




    mydbl->first = (double *)xmalloc(toCopy->array->capacity * sizeof(double)); //capPageSize * pages doubles
    memcpy(mydbl->first, toCopy->array->first, toCopy->array->capacity * sizeof(double));

    mydbl->size = 1;
    mydbl->capacity = toCopy->array->capacity;
};

void negBig(struct Big* toNeg, struct Big* thisBig) {
    copyBig(toNeg, thisBig);
    thisBig->sign = -(thisBig->sign);
};

void absBig(struct Big* toAbs, struct Big* absTo) {
    copyBig(toAbs, absTo);
    absTo->sign = 1;
}

bool isBigNan(struct Big* isThisNeg) {
    return isThisNeg->nan != 0;
};

bool isBigInt(struct Big* isThisInt) {
    if (isThisInt->array->first[0] > maxSafeInt || isThisInt->array->first[0] < minSafeInt) {
        return true; //why do we do this? its default omeganum behavior
    }
    
    if (isThisInt->array->first[1] != 0) {
        return true;
    }
    if (floor(isThisInt->array->first[0]) == isThisInt->array->first[0]) {
        return true;
    } else if (ceil(isThisInt->array->first[0]) == isThisInt->array->first[0]) {
        return true;
    }
    return false;
};

void bigMul(struct Big* mulFromThis, struct Big* mulToThis) {
    //TODO: handle negatives, handle nans, handle infinities, handle zeroes, handle ones
    //handle very large numbers

    //this uses add, pow, and log10, we need to make those first
}

void bigAdd(struct Big* addFromThis, struct Big* addToThis) {
    //TODO: handle negatives, handle nans, handle infinities, handle zeroes, handle ones
    //handle very large numbers
}

struct Big* bigMin(struct Big* firstMin, struct Big* secondMin) {

}

struct Big* bigMax(struct Big* firstMax, struct Big* secondMax) {
    
}

//-1 = second is larger
//0 = they are equal
//1 = first is larger
//2 = nan

int compareBig(struct Big* firstComp, struct Big* secondComp) {
    if (isBigNan(firstComp) || isBigNan(secondComp)) {
        return 2;
    }
    //TODO: handle infinities
    if (firstComp->sign != secondComp->sign) {
        return firstComp->sign;
    }

    int firstSign = firstComp->sign;
    int toRetPreSignage = 0;
    if (firstComp->array->size != secondComp->array->size) {
        toRetPreSignage = (firstComp->array->size > secondComp->array->size)*2-1;
    } else {
        for (int i = firstComp->array->size - 1; i >= 0; i--) {
            if (firstComp->array->first[i] != secondComp->array->first[i]) {
                toRetPreSignage = (firstComp->array->first[i] < secondComp->array->first[i])*2-1;
                break;
            }
        }
    }
    return toRetPreSignage * firstSign;
}

double toDouble(struct Big *big) {
    double toReturn = 0;
    if (big->sign == -1) {
        struct Big* toGiveRet = allocBig();
        negBig(big, toGiveRet);
        double toRetDoub = toGiveRet->array->first[0] * -1;
        freeBig(toGiveRet);
        return toRetDoub;
    };
    if (isBigNan(big)) {
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

int freeBig(struct Big *big) {
    free(big->array->first);
    free(big->array);
    free(big);
    return 0;
};


int main() {
    clock_t start, end;
    double diff;
    start = clock();
    for (int i = 1; i <= 3000000; i++) {
        struct Big *testBig = allocBig();
        initBig(testBig, i);
        if ((double)i != toDouble(testBig)) {
            printf("Mismatched bigs! %3.3f, %3.3f\n", (double)i, toDouble(testBig));
        };
        freeBig(testBig);
        if (i % 1000000 == 0) {
            printf("Finished %1.0f initiations & frees of bigs.\n", toDouble(testBig));
        };
    };
    end = clock();
    diff = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Finished in %3.6f\n", diff);

    struct Big *initial = allocBig();
    struct Big *negTo = allocBig();
    initBig(initial, 5);
    printf("Before copy to negative: %3.3f\n", toDouble(initial));
    negBig(initial, negTo);
    printf("After copy to negative: %3.3f\n", toDouble(negTo));
    struct Big *absThis = allocBig();
    absBig(negTo, absThis);
    printf("After abs: %3.3f\n", toDouble(absThis));
    freeBig(initial);
    freeBig(negTo);
    struct Big *thisIsInteger = allocBig();
    struct Big *thisIsNotInteger = allocBig();
    initBig(thisIsInteger, (double)(30000));
    initBig(thisIsNotInteger, ((double)(9e9 - 100)) * 0.95435253425234623454325324532453242);
    printf("Int check should be true: %d\n", (int)isBigInt(thisIsInteger));
    printf("Int check should be false: %d\n", (int)isBigInt(thisIsNotInteger));
    struct Big *forComparison1 = allocBig();
    struct Big *forComparison2 = allocBig();
    initBig(forComparison1, 10);
    initBig(forComparison2, 20);
    int result = compareBig(forComparison1, forComparison2);
    printf("Result should be 1: %d\n", result);
    result = compareBig(forComparison2, forComparison1);
    printf("Result should be -1: %d\n", result);
    forComparison2->array->first[0] = 10;
    result = compareBig(forComparison1, forComparison2);
    printf("Result should be 0: %d\n", result);
    result = compareBig(forComparison1, forComparison1);
    printf("Result should be 0: %d\n", result);
    freeBig(forComparison1);
    freeBig(forComparison2);
    return 0;
};