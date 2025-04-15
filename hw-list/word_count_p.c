/*
 * Implementation of the word_count interface using Pintos lists and pthreads.
 *
 * You may modify this file, and are expected to modify it.
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

#ifndef PINTOS_LIST
#error "PINTOS_LIST must be #define'd when compiling word_count_lp.c"
#endif

#ifndef PTHREADS
#error "PTHREADS must be #define'd when compiling word_count_lp.c"
#endif

#include "word_count.h"

void init_words(word_count_list_t* wclist) { /* TODO */

 if(&(wclist->lst)==NULL)
 {
  printf("EMPTY LIST");

 }
 list_init(&(wclist->lst));

}

size_t len_words(word_count_list_t* wclist) {
  /* TODO */
  if(wclist==NULL)
  {
    printf("空链表");
     return 0;
  }

  return list_size(&(wclist->lst));
}

word_count_t* find_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  if(&(wclist->lst)==NULL)
  {
    printf("空链表");
    return NULL;
  }
  struct list_elem* node=list_begin(&(wclist->lst));
  while(node!=list_end(&(wclist->lst)))
  {
    if(strcmp(list_entry(node,word_count_t,elem)->word,word)==0)
    {
        return list_entry(node,word_count_t,elem);
    }
    node=list_next(node);
  }
  return NULL;
}

word_count_t* add_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  if(wclist==NULL)
  {
    printf("空链表");
    return NULL;
  }
  word_count_t *findnode=find_word(wclist,word);

  if(findnode!=NULL)
  {
    findnode->count++;
  }
  else
  {
    word_count_t* newNode = (word_count_t*)malloc(sizeof(word_count_t));
    if(newNode==NULL)
    {
      printf("create fail");
      return NULL;
    }
    newNode->count=1;
    newNode->word = strdup(word);

    list_push_back(&wclist->lst,&(newNode->elem));
    return newNode;
   }
  
  
  return findnode;
}

void fprint_words(word_count_list_t* wclist, FILE* outfile) {
  /* TODO */
  /* Please follow this format: fprintf(<file>, "%i\t%s\n", <count>, <word>); */
  struct list_elem * elem;
 
  for(elem=list_begin(&(wclist->lst));elem!=list_end(&(wclist->lst));elem=list_next(elem))
  {
    word_count_t *node= list_entry(elem,word_count_t,elem);
    fprintf(outfile,"%i\t%s\n",node->count,node->word);
  }

}

static bool less_list(const struct list_elem* ewc1, const struct list_elem* ewc2, void* aux) {
  /* TODO */
  word_count_t *node1=list_entry(ewc1,word_count_t,elem);
  word_count_t *node2=list_entry(ewc2,word_count_t,elem);
  
  bool (*less_count)(word_count_t* ,word_count_t*)=aux;
  return less_count(node1,node2);
}
void wordcount_sort(word_count_list_t* wclist,
                    bool less(const word_count_t*, const word_count_t*)) {
  /* TODO */
  list_sort(&(wclist->lst), less_list, less);
}
