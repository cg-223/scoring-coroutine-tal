#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct dblArray {
    double* first;
    size_t size;
    size_t capacity;
};

struct Big {
    struct dblArray* array;
    int sign;
    int nan;
};
int maxArrow = 1000;
int capPageSize = 32; //power of 2
size_t defaultPages = 1;
long long int maxSafeInt = 9e9; //arbitrary
long long int minSafeInt = -9e9;
typedef struct Big* Big;
#define SNULL (size_t)NULL
#define allocBig() (Big)xmalloc(sizeof(struct Big))

//TODO (from the readme of OmegaNum)
/*
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

//we use llongs

//unused as of now
//xmalloc uses its own thing
void checkNull(void* thing) {
    if (thing == NULL) {
        perror("oom");
        exit(EXIT_FAILURE);
    };
}

void* xmalloc(size_t size) {
    void* ourptr = malloc(size);
    if (ourptr == NULL) {
        perror("oom");
        exit(EXIT_FAILURE);
    };
    return ourptr;
}

Big ZEROBIG;
Big NANBIG;
Big MAXBIGINT;

//initialize array, allocate initial space for array. set an initial value. 
void initDblArray(struct dblArray* array, double initial, size_t pages) {

    if (pages == SNULL) {
        pages = defaultPages;
    };


    array->first = (double*)xmalloc(pages * capPageSize * sizeof(double)); //capPageSize * pages doubles

    memset(array->first, 0, pages * capPageSize * sizeof(double));

    array->first[0] = initial;
    array->size = 1;
    array->capacity = pages * capPageSize;
};

void reallocArray(struct dblArray* array) {
    double* newptr = realloc(array->first, array->capacity + (capPageSize * sizeof(double)));

    if (newptr == NULL) {
        perror("oom");
        exit(EXIT_FAILURE);
    }
};

void initBig(Big big, double initial) {
    big->sign = 1;
    big->nan = 0;
    struct dblArray* mydbl = (struct dblArray*)xmalloc(sizeof(struct dblArray));
    big->array = mydbl;
    initDblArray(mydbl, initial, SNULL);
};

void copyBig(Big toCopy, Big thisBig) {
    thisBig->sign = toCopy->sign;
    thisBig->nan = toCopy->nan;

    struct dblArray* mydbl = (struct dblArray*)xmalloc(sizeof(struct dblArray));
    thisBig->array = mydbl;




    mydbl->first = (double*)xmalloc(toCopy->array->capacity * sizeof(double)); //capPageSize * pages doubles
    memcpy(mydbl->first, toCopy->array->first, toCopy->array->capacity * sizeof(double));

    mydbl->size = 1;
    mydbl->capacity = toCopy->array->capacity;
};

void negBig(Big toNeg, Big thisBig) {
    copyBig(toNeg, thisBig);
    thisBig->sign = -(thisBig->sign);
};

void absBig(Big toAbs, Big absTo) {
    copyBig(toAbs, absTo);
    absTo->sign = 1;
}

Big maxBig(Big firstMax, Big secondMax) {
    int result = compareBig(firstMax, secondMax);
    if (result == -1) {
        return secondMax;
    }
    return firstMax;
}

Big minBig(Big firstMin, Big secondMin) {
    int result = compareBig(firstMin, secondMin);
    if (result == 1) {
        return secondMin;
    }
    return firstMin;
}



int isBigNan(Big isThisNeg) {
    return isThisNeg->nan != 0;
};

int isBigInt(Big isThisInt) {
    if (isThisInt->array->first[0] > maxSafeInt || isThisInt->array->first[0] < minSafeInt) {
        return 1; //why do we do this? its default omeganum behavior
    }

    if (isThisInt->array->first[1] != 0) {
        return 1;
    }
    if (floor(isThisInt->array->first[0]) == isThisInt->array->first[0]) {
        return 1;
    }
    else if (ceil(isThisInt->array->first[0]) == isThisInt->array->first[0]) {
        return 1;
    }
    return 0;
};

void bigMul(Big mulFromThis, Big mulToThis) {
    //TODO: handle negatives, handle nans, handle infinities, handle zeroes, handle ones
    //handle very large numbers

    //this uses add, pow, and log10, we need to make those first
}

Big bigAdd(Big add1, Big add2) {
    //TODO: handle negatives, handle nans, handle infinities, handle zeroes, handle ones
    //handle very large numbers
    //cg you better make sure this function doesnt memleak
    Big clone1 = allocBig();
    Big clone2 = allocBig();
    copyBig(add1, clone1);
    copyBig(add2, clone2);
    if (clone1->sign == -1) {
        //todo: handle this weird chaining bs
        //x:neg():add(other:neg()):neg()
    }
    if (clone2->sign == -1) {
        //todo: handle this weird chaining less bs
        //return x:sub(other:neg());
    }
    if (eqBig(clone1, ZEROBIG)) {
        return clone2;
    }
    if (eqBig(clone2, ZEROBIG)) {
        return clone1;
    }
    //todo: handle nans and infinities

    Big lesser = minBig(clone1, clone2);
    Big bigger = maxBig(clone1, clone2);
    Big final = allocBig(); //copy to this
    int didBig = 0;
    if (lesser->array->first[1] == 2 && !gtBig(lesser, MAXBIGINT)) {
        lesser->array->first[1] = 1;
        lesser->array->first[0] = pow(10, (lesser->array->first[0]));
    }
    if (bigger->array->first[1] == 2 && !gtBig(bigger, MAXBIGINT)) {
        bigger->array->first[1] = 1;
        bigger->array->first[0] = pow(10, (bigger->array->first[0]));
    }
    if (gtBig(bigger, MAXBIGINT) || bigDivAndGt(bigger, lesser, MAXBIGINT)) {
        didBig = 1;
        copyBig(final, bigger);
    } else if ((bigger->array->first[1] == 0)) {
        didBig = 1;
        initBig(final, lesser->array->first[0] + bigger->array->first[1]);
    } else if (bigger->array->first[1] == 1) {
        if (lesser->array->first != 0) {
            double a = lesser->array->first[0];
        } else {
            double a = log10(lesser->array->first[0]);
        }
        initBig(final, 0);
        final->array->first[1] = 1;
        final->array->first[0] = log10(pow(10, bigger->array->first[0])+1);
    }

    if (!didBig) {
        printf("bigAdd failed to find a solution. Returning 0...\n");
        initBig(final, 0);
    }
    return final;
}

//unsafe: creates a new big
Big bigDiv(Big big1, Big big2) {
    Big final = allocBig();
    initBig(final, big1->array->first[0]/big2->array->first[0]);
    return final;
}

//unsafe: creates a new big
Big bigPow(Big big1, Big big2) {

}

//safe, creates a new big but cleans up after itself
int bigDivAndGt(Big div1, Big div2, Big greaterThan) {
    Big afterDiv = bigDiv(div1, div2);
    int result = gtBig(afterDiv, greaterThan);
    freeBig(afterDiv);
    return result;
}



//-1 = second is larger
//0 = they are equal
//1 = first is larger
//2 = nan


//unsafe: we make a new big here
//must free the return from this, and the argument
Big log10Big(Big toLog10) {
    Big newBig = allocBig();
    copyBig(toLog10, newBig);
    //todo: handle zeroes and negatives
    //todo: handle unsafe numbers
    //todo: handle infinities
    //todo: handle highly large numbers
    newBig->array->first[1] = (double)(min(newBig->array->first[1] - 1, 0));
    return newBig;
}

//safe
int compareBig(Big firstComp, Big secondComp) {
    if (isBigNan(firstComp) || isBigNan(secondComp)) {
        //return 2;
        perror("nan");
        exit(2);
    }
    //TODO: handle infinities
    if (firstComp->sign != secondComp->sign) {
        return firstComp->sign;
    }

    int firstSign = firstComp->sign;
    int toRetPreSignage = 0;
    if (firstComp->array->size != secondComp->array->size) {
        toRetPreSignage = (firstComp->array->size > secondComp->array->size) * 2 - 1;
    }
    else {
        for (int i = firstComp->array->size - 1; i >= 0; i--) {
            if (firstComp->array->first[i] != secondComp->array->first[i]) {
                toRetPreSignage = (firstComp->array->first[i] < secondComp->array->first[i]) * 2 - 1;
                break;
            }
        }
    }
    return toRetPreSignage * firstSign;
}

//safe
int gtBig(Big firstgt, Big secondgt) {
    return compareBig(firstgt, secondgt) == 1;
}

//safe
int gteBig(Big firstgt, Big secondgt) {
    return compareBig(firstgt, secondgt) > -1;
}

//safe
int ltBig(Big firstgt, Big secondgt) {
    return compareBig(firstgt, secondgt) == -1;
}

//safe
int lteBig(Big firstgt, Big secondgt) {
    return compareBig(firstgt, secondgt) < 1;
}

//safe
int eqBig(Big firstgt, Big secondgt) {
    return compareBig(firstgt, secondgt) == 0;
}

//safe
int neqBig(Big firstgt, Big secondgt) {
    return compareBig(firstgt, secondgt) != 0;
}

//safe: creates a new big, but frees it afterwards
double toDouble(Big big) {
    double toReturn = 0;
    if (big->sign == -1) {
        Big toGiveRet = allocBig();
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


int bigLen(Big toLen) {
    int soFar = 0;
    int i = -1;
    while (toLen->array->first[i++] > 0) {
        soFar++;
    }
    return soFar;
}

//safe
int freeBig(Big big) {
    free(big->array->first);
    free(big->array);
    free(big);
    return 0;
};

//for lua
Big createBig(double fromThisNum) {
    Big toRet = allocBig();
    initBig(toRet, fromThisNum);
    return toRet;
}

int main() {


    clock_t start, end;
    double diff;
    start = clock();
    for (int i = 1; i <= 3000000; i++) {
        Big testBig = allocBig();
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

    Big initial = allocBig();
    Big negTo = allocBig();
    initBig(initial, 5);
    printf("Before copy to negative: %3.3f\n", toDouble(initial));
    negBig(initial, negTo);
    printf("After copy to negative: %3.3f\n", toDouble(negTo));
    Big absThis = allocBig();
    absBig(negTo, absThis);
    printf("After abs: %3.3f\n", toDouble(absThis));
    freeBig(initial);
    freeBig(negTo);
    Big thisIsInteger = allocBig();
    Big thisIsNotInteger = allocBig();
    initBig(thisIsInteger, (double)(30000));
    initBig(thisIsNotInteger, ((double)(9e9 - 100)) * 0.95435253425234623454325324532453242);
    printf("Int check should be true: %d\n", (int)isBigInt(thisIsInteger));
    printf("Int check should be false: %d\n", (int)isBigInt(thisIsNotInteger));
    Big forComparison1 = allocBig();
    Big forComparison2 = allocBig();
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

    Big forAdd1 = allocBig();
    Big forAdd2 = allocBig();
    initBig(forAdd1, 10000);
    initBig(forAdd2, 20000);
    Big afterAdd = bigAdd(forAdd1, forAdd2);

    return 0;
};