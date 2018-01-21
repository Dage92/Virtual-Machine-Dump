#include <stdio.h>
#include <pthread.h>

typedef struct{
	pthread_cond_t * cond;
	pthread_mutex_t * mut;
	char name [12];
	int id;
} targ;

void * func (void * arg){
	targ * a = (targ *) arg;
	
	if (a->id != 0){
		pthread_mutex_lock (a->mut + a->id - 1);
		pthread_cond_wait (a->cond + a->id - 1, a->mut + a->id - 1);
		pthread_mutex_unlock (a->mut + a->id - 1);
	}
	printf ("%s spreaking!\n", a->name);
	
	if (a->id != 4){
		pthread_mutex_lock (a->mut + a->id);
		pthread_cond_signal (a->cond + a->id);
		pthread_mutex_unlock (a->mut + a->id);
	}
	
	return NULL;
}

int main (int argc, char ** argv){
	pthread_cond_t c [4] = {PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER};
	pthread_mutex_t m [4] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};
	targ arg [5]= {{c, m, "Angela", -1}, {c, m, "Brandon", -1}, {c, m, "Charlize", -1}, {c, m, "Desmond", -1}, {c, m, "Erika", -1}};
	pthread_t tid[5];
	int i;
	
	for (i=0; i<=4; i++){
		arg[i].id=(i*3+1)%5;
		pthread_create (tid+1, NULL, &func, arg+1);
	}
	
	for (i=0; i<=4; ++i){
		pthread_join (tid [i],NULL);
	}	
	return 0;
}
