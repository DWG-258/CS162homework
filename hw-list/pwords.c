/*
 * Word count application with one thread per input file.
 *
 * You may modify this file in any way you like, and are expected to modify it.
 * Your solution must read each input file from a separate thread. We encourage
 * you to make as few changes as necessary.
 */

/*
 * Copyright © 2021 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>

#include "word_count.h"
#include "word_helpers.h"

#define NUM_THREADS 5
//互斥锁
word_count_list_t word_counts;
void* thread_func(void* arg)
{

  printf("thread run. sTART...\n");

  FILE* file=(FILE*)arg;
  printf("thread run. lock...\n");

  pthread_mutex_lock(&(word_counts.lock));
 count_words(&word_counts,file);
  printf("thread run. END...\n");
pthread_mutex_unlock(&(word_counts.lock));
pthread_exit(NULL);
}
/*
 * main - handle command line, spawning one thread per file.
 */
int main(int argc, char* argv[]) {
  /* Create the empty data structure. */
  // word_count_list_t word_counts;
  init_words(&word_counts);
  int thead_func;
  pthread_t tid[NUM_THREADS];
  FILE* file[NUM_THREADS];
  pthread_mutex_init(&(word_counts.lock), NULL);


  if (argc <= 1) {
    /* Process stdin in a single thread. */
    printf("1");
    count_words(&word_counts, stdin);
  } else {
    /* TODO */
    //多个文件，使用多线程

    //to debug,参数传递不正确
    for (long t = 0; t <argc-1; t++) {
      printf("main: creating thread ....%ld\n", t);
      printf("thread run....\n");
      file[t]=fopen(argv[t+1],"r");
      if (file[t] == NULL) {
        printf("Could not open file %s\n", argv[t + 1]);
        return 1;
    }
      thead_func = pthread_create(&tid[t], NULL, thread_func, (void*)file[t]);
      if (thead_func) {
        printf("ERROR; return code from pthread_create() is %d\n", thead_func);
        exit(-1);
    } else {
        printf("Thread %ld created successfully.\n", t);
    }
    }
  }
  for(int t= 0;t<argc-1;t++)
  {
    pthread_join(tid[t],NULL);
  }
  /* Output final result of all threads' work. */
  wordcount_sort(&word_counts, less_count);
  fprint_words(&word_counts, stdout);
  return 0;
}
