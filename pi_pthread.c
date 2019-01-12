#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
int num_threads;
int N;
double area[100];
void* cal_pi(void* threadid) {
    int* tid = (int*)threadid;
    //printf("Hello, thread #%d!\n", *tid);
	int i;
	for(i = *tid ; i < N; i=num_threads+i) { 
		double x = (double)i / N;
        double y = sqrt(1 - x * x);
		area[*tid] += y / (double)N;
    } 
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    assert(argc == 3);
    num_threads = atoi(argv[1]);
	N = atoi(argv[2]);
    pthread_t threads[num_threads];
    int rc;
    int ID[num_threads];
    int t;
	double pi=0;
    for (t = 0; t < num_threads; t++) {
        ID[t] = t;
        printf("In main: creating thread %d\n", t);
        rc = pthread_create(&threads[t], NULL, cal_pi, (void*)&ID[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
	for(t = 0; t < num_threads; t++){
		pthread_join(threads[t],NULL); 
	}
	
	for(t = 0; t < num_threads; t++){
		pi += area[t]; 
	}
	printf("%.16lf\n", pi * 4);
    pthread_exit(NULL);
}
