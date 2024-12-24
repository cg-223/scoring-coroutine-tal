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
    if (ourptr == NULL) {
        perror("oom");
        exit(EXIT_FAILURE);
    };
    return ourptr;
};

void initDblArray(struct dblArray *array, double initial, size_t pages) {

    if (pages == NULL) {
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
    struct dblArray *mydbl = (struct dblArray *)xmalloc(sizeof(struct dblArray));
    big->array = mydbl;
    initDblArray(mydbl, initial, NULL);
}

double toDouble(struct Big *big) {
    double toReturn = 0;
    if (big->sign == -1) {
        //return -1*(self:neg():to_number())
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

bool ext = true;


char omegaNumError[] = "[OmegaNumError] ";
char invalidArgument[] = "[OmegaNumError] Invalid Argument: ";

int main() {
    struct Big *ourBig = (struct Big *)xmalloc(sizeof(struct Big));

    initBig(ourBig, 3);
    printf("Final double: %1.0f. ", toDouble(ourBig));
    return 0;
};