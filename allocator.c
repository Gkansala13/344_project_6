// Grayson Kansala
// CS344
// My Allocator

// Refractored old code after class on Wed.
// Added Coalescing feature to myfree() using approach option 1

#include "allocator.h"
struct block *head = NULL;

// Split_Space(current_node, requested_size):
void split_space(struct block *cur, int padded_size){
  int block_padded_size = PADDED_SIZE(sizeof(struct block)); 
  int cur_size= cur->size;
  int remaining_free_space = cur_size - padded_size - block_padded_size;
  int room_to_split = remaining_free_space>=16;
  // Split if it can hold the three things, space requested from user, 
  // A new struct block, and the remaining space referred to by the new struct block (padded)
  if (room_to_split){
    struct block *new = PTR_OFFSET(cur, padded_size + block_padded_size);           
    new ->size = cur->size - padded_size - block_padded_size;
    cur->size = padded_size;
    new->in_use = 0;
    new->next = cur->next;                              // Wire it into the linked list
    cur->next = new;
  }
}

void myfree(void *p) {    
  int block_padded_size = PADDED_SIZE(sizeof(struct block));
  struct block *b = PTR_OFFSET(p, -block_padded_size);    
  b -> in_use = 0;
                                                         //Start of Coalescing
  struct block *cur = head;                              //start cur at head
  while(cur->next!=NULL){                                //while cur->next isn't NULL:
    if((!cur->in_use)&&(!cur->next->in_use)){            //if cur is not in_use and next node not in use:
      cur->size += cur->next->size + block_padded_size;  //add the next node's region's size to cur's (PLUS padded size)
      cur->next=cur->next->next;                         //make cur's next pointer skip the next node
      }else{                                             //else
      cur = cur->next;                                   //move cur to next node
    }
  }
}

void *myalloc(int bytes){
  int actual_size = PADDED_SIZE(bytes);
  
  if (head == NULL){
    head = sbrk(1024);
    head->next = NULL;
    head->size = 1024 - PADDED_SIZE(sizeof(struct block));
    head->in_use = 0;
  }

  int padded_size = PADDED_SIZE(bytes);
  int padded_block_size = PADDED_SIZE(sizeof(struct block));
  struct block *cur = head;

  while(cur != NULL){
    if ((!cur->in_use)&&(cur->size>=padded_size)){
      split_space(cur, padded_size);
      cur->in_use = 1;
      return PTR_OFFSET(cur, padded_block_size);
    }
    cur = cur->next;
  }
  return NULL;
}

void print_data(void)
{
  struct block *b = head;
  if (b == NULL) {
    printf("[empty]\n");
    return;
  }
  while (b != NULL) {
    // Uncomment the following line if you want to see the pointer values
    //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
    printf("[%d,%s]", b->size, b->in_use? "used": "free");
    if (b->next != NULL) {
      printf(" -> ");
    }
    b = b->next;
  }
  printf("\n");
}

int main(void) {

  // void *p;  
  // p = myalloc(10); print_data();
  // myfree(p); print_data();


  // void *p, *q;
  // p = myalloc(10); print_data();
  // q = myalloc(20); print_data();
  // myfree(p); print_data();
  // myfree(q); print_data();


  // void *p, *q;
  // p = myalloc(10); print_data();
  // q = myalloc(20); print_data();
  // myfree(q); print_data();
  // myfree(p); print_data();


  void *p, *q, *r, *s;
  p = myalloc(10); print_data();
  q = myalloc(20); print_data();
  r = myalloc(30); print_data();
  s = myalloc(40); print_data();
  myfree(q); print_data();
  myfree(p); print_data();
  myfree(s); print_data();
  myfree(r); print_data();
}