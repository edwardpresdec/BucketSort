#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define SIZE 1000000
#define bucks 10

struct buckets {
int number;
int* element;
};

int comparevalues(const void* fvalue, const void* svalue) {
    int p = *((int*)fvalue), q = *((int*)svalue);
    if (p == q) {
        return 0;
    } else if (p < q) {
        return -1;
    } else {
        return 1;
    }
}

void crtbucketsortingElement(int sortarry[], int arylength) {
    struct buckets crtbuckets[bucks];
    int m, j, k;
    for (m = 0; m < bucks; m++) {
        crtbuckets[m].number = 0;
        crtbuckets[m].element = (int*)malloc(sizeof(int) * arylength);
    }
    int temp;
    int index;
    for (m = 0; m < arylength; m++) {
        temp = sortarry[m];
        index = (temp*bucks)/arylength;
        crtbuckets[index].element[crtbuckets[index].number++] = temp;
    }
    /*for(k=0, m=0; m<bucks; m++){
        printf("BUCKET %d: ", k);
        for(j=0; j<crtbuckets[m].number; j++) {
            printf("%d ", crtbuckets[m].element[j]);
        }
        printf("\n");
        k = k + 1;
    }*/
    k = 0;
    #pragma omp parallel for num_threads (2)
    for (m = 0; m < bucks; m++) {
        qsort(crtbuckets[m].element, crtbuckets[m].number, sizeof(int), &comparevalues);
        for (j = 0; j < crtbuckets[m].number; j++) {
            sortarry[k + j] = crtbuckets[m].element[j];
        }
        k = k + crtbuckets[m].number;
        free(crtbuckets[m].element);
    }
    
}

int main(char *arg[]) {

    int sortarry_perm[SIZE];
    int sortarry[SIZE];
    sortarry_perm[0]=SIZE-1;
    sortarry[0]=SIZE-1;
    for(int i=1; i<SIZE; i++){
        sortarry_perm[i] = (SIZE-i);
        sortarry[i] = (SIZE-i);
    }

    int j,k;
    double AVG = 0;
    double total_time[10];

    /*printf("Before bucket Sorting array element: \n");
    for (j = 0; j<SIZE; j++) {
        printf("%d ", sortarry[j]);
    }*/

    for (int k=0; k<10; k++) {
        clock_t start_time = clock();
        crtbucketsortingElement(sortarry, SIZE);
        clock_t end_time = clock();
        total_time[k] = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Execution time of trial [%d]: %f seconds\n", k , total_time[k]);
        AVG += total_time[k];
        if (k!=9){
            for(int i=0; i<SIZE; i++) {
                sortarry[i] = sortarry_perm[i];
            }
        }
    }

    /*printf("\nAfter bucket Sorting array element: \n");
    for (k = 0; k<SIZE; k++)
        printf("%d ", sortarry[k]);*/
    
    printf("\nThe average execution time between 10 trials with %d processors is: %f s", bucks, AVG/10);
    return 0;
}