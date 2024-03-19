#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define A 1000000
#define num_threads 50

struct buckets {
    int number;
    int* element;
};

pthread_mutex_t lock;

struct buckets crtbuckets[num_threads];
int SORTPERM[A];
int SORT[A];

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

/*
void crtbucketsortingElement(int sortarry[], int A) {
    int m, j, k;
    for (m = 0; m < 10; m++) {
        crtbuckets[m].number = 0;
        crtbuckets[m].element = (int*)malloc(sizeof(int) * A);
    }
    int temp;
    int index;
    for (m = 0; m < A; m++) {
        temp = sortarry[m];
        index = temp/10;
        crtbuckets[index].element[crtbuckets[index].number++] = temp;
    }
    for (k = 0, m = 0; m < 10; m++) {
        qsort(crtbuckets[m].element, crtbuckets[m].number, sizeof(int), &comparevalues);
        for (j = 0; j < crtbuckets[m].number; j++) {
            sortarry[k + j] = crtbuckets[m].element[j];
        }
        k = k + crtbuckets[m].number;
        free(crtbuckets[m].element);
    }
}
*/

void *sort(void *ptr) {
    int id = (int) ptr;
    int k,j;
    
    /*printf("BUCKET %d: ", id);
    for(j=0; j<crtbuckets[id].number; j++) {
        printf("%d ", crtbuckets[id].element[j]);
    }
    printf("\n");*/

    int pos = 0;
    qsort(crtbuckets[id].element, crtbuckets[id].number, sizeof(int), &comparevalues);
    for (k=0; k<id; k++){
        pos = pos+crtbuckets[k].number;
    }
    for(j=0; j<crtbuckets[id].number; j++) {
        SORT[pos + j] = crtbuckets[id].element[j];
    }
    free(crtbuckets[id].element);
    return  id;
}

void *fill(void *ptr) {
    int id = (int) ptr;
    int temp, index;
    int interval = A/num_threads;
    int start = id*(interval);
    int end = start + interval;
    
    for (int m = start; m < end; m++) {
        temp = SORT[m];
        index = (temp/num_threads)/(A/num_threads);
        pthread_mutex_lock(&lock);
        crtbuckets[index].element[crtbuckets[index].number++] = temp; //populating the buckets
        pthread_mutex_unlock(&lock);
    }
    return id;
}

int main(int argc, char **argv) {
    double AVG = 0;
    double total_time[10];
    SORTPERM[0]=A-1;
    SORT[0]=A-1;
    for(int i=1; i<A; i++){
        SORTPERM[i] = (A-i);
        SORT[i] = (A-i);
    }

    /*printf("Before bucket Sorting array element: \n");
    for (int j = 0; j<A; j++) {
        printf("%d ", SORT[j]);
    }*/

    for (int k=0; k<10; k++) {
        clock_t start_time = clock();

        for (int m = 0; m < num_threads; m++) {
            crtbuckets[m].number = 0;
            crtbuckets[m].element = (int*)malloc(sizeof(int) * A); //create buckets
        }
        pthread_t thread_handles[num_threads];

        int temp;
        int index;
        for (int m = 0; m < A; m++) {
            temp = SORT[m];
            index = (temp*num_threads)/A;
            crtbuckets[index].element[crtbuckets[index].number++] = temp;
        }

        /*for(int i=0; i<num_threads; i++) {
            pthread_create(&thread_handles[i], NULL, *fill, (void *) i);
        }
        for(int i=0; i<num_threads; i++) {
            pthread_join(thread_handles[i], NULL);
        }*/

        for(int i=0; i<num_threads; i++) {
            pthread_create(&thread_handles[i], NULL, *sort, (void *) i);
        }
        for(int i=0; i<num_threads; i++) {
            pthread_join(thread_handles[i], NULL);
        }


        clock_t end_time = clock();
        total_time[k] = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Execution time of trial [%d]: %f seconds\n", k , total_time[k]);
        AVG += total_time[k];
        if (k!=9){
            for(int i=0; i<A; i++) {
                SORT[i] = SORTPERM[i];
            }
        }
    }

    /*printf("\nAfter bucket Sorting array element: \n");
    for (int k = 0; k<A; k++)
        printf("%d ", SORT[k]);*/
    
    printf("\nThe average execution time between 10 trials is: %f ms", AVG/10);
    return 0;
}