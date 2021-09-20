/*
 * ключевое слово restrict дает понять компилятору, что область памяти, на которую ссылается указатель с кодификатором
 * restrict, должна принадлжать ему и только ему (этому указателю) и ни с чем не перекрываться.
   */


#include <stdio.h>
#include <pthread.h>
#include <assert.h>
struct ctx{
    int id;
    int begin;
    int end;
    int v[10];
    int ans;
};

void * part_sum(void * arg){
    int ans = 0;
    struct ctx a = * (struct ctx *) arg;
    ans = 0;
    for (int i = 0; i < a.end - a.begin; i++){
        ans += a.v[i];
    }
    ((struct ctx *)arg)->ans = ans;
}
int vector_sum(int *v, int size, int nthreads){
    int result = 0;
    struct ctx ctxs[nthreads];
    ctxs[0].end = size % nthreads + size / nthreads - 1;
    ctxs[0].begin = 0;
    ctxs[0].id = 1;
    int beg = ctxs[0].end + 1;
    for (int i = 1; i < nthreads; i++){
        ctxs[i].id = i;
        ctxs[i].begin = beg;
        ctxs[i].end = beg + size / nthreads - 1;
        beg = ctxs[i].end + 1;
        for (int j = 0; j < ctxs[i].end - ctxs[i].begin; j++){
            ctxs[i].v[j] = v[ctxs[i].begin + j];
        }
    }

    pthread_t threads[nthreads];
    for (int i = 0; i < nthreads; i++){
        int retval = pthread_create(&threads[i], NULL, part_sum, ctxs + i);
        assert(retval == 0);
    }

    for (int i = 0; i < nthreads; i++){
        struct ctx* answer;
        pthread_join(threads[i], (void *)(answer));
        result += answer->ans;
    }

    return result;
}


int main(){
    int nthreads;
    int size;

    scanf("%d%d", &nthreads, &size);
    int v[size];

    for (int i = 0; i < size; i++){
        scanf("%d", &v[i]);
    }

    printf("%d", vector_sum(v, size, nthreads));

/*
    for (int i = size - 1; i >=0; i-- ){
        printf("%d ", v[i]);
    }
*/

    return 0;
}
