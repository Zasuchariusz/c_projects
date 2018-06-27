#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct dancing_list{
  char c;
  bool covered;
  struct dancing_list *right;
  struct dancing_list *left;
  struct dancing_list *up;
  struct dancing_list *down;
  struct list_header *column;
  struct list_header *downC;
  struct list_header *upC;
} Tdancing_list;

typedef struct list_header{
  bool empty;
  struct list_header *right;
  struct list_header *left;
  struct dancing_list *down;
} Tlist_header;

typedef struct list_pointer_header{
  Tlist_header *header;
  struct list_pointer_header *next;
}Tl_p_header;

typedef struct element_list{
  Tdancing_list *e;
  struct element_list *next;
}Te_list;

typedef struct solution_list{
  Te_list *e_l;
  struct solution_list *next;
} Tsolution;

Tl_p_header *new_lph(Tlist_header *header){
  Tl_p_header *new;
  new = (Tl_p_header*)malloc(sizeof(Tl_p_header));
  new->header = header;
  new->next = NULL;
}

void add_to_lph(Tl_p_header *start, Tlist_header *header){
  Tl_p_header *list;
  list = start;
  while(list->next != NULL) list = list->next;
  list->next = new_lph(header);
}


//Returns sentinel, sentinel has NULL as next
Tsolution *new_solution(){
  Tsolution *new;
  new = (Tsolution*)malloc(sizeof(Tsolution));
  //new->e_l = NULL;
  //new->next = NULL;
  return new;
}

void solution_push(Tsolution **s, Tdancing_list *e){
  Tsolution *new;
  Te_list *e_list, *e_list_temp;
  Tdancing_list *element_temp;
  if((*s)->e_l != NULL) printf("(przed wrzutem) zaczynam sie od %c\n", (*s)->e_l->e->c);
  new = (Tsolution*)malloc(sizeof(Tsolution));
  new->e_l = (Te_list*)malloc(sizeof(Te_list));
  e_list = new->e_l;
  e_list->e = e;
  e_list->next = NULL;
  element_temp = e;
  do{
    do{
      element_temp = element_temp->right;
    }while(element_temp->column->right == element_temp->column && element_temp != e);
    if(element_temp != e){
      e_list->next = (Te_list*)malloc(sizeof(Te_list));
      e_list = e_list->next;
      e_list->e = element_temp;
      e_list->next = NULL;
    }
  }while(element_temp != e);
  new->next = *s;
  *s = new;
  if(*s == (*s)->next) printf ("TURBO WTF\n");
}

Te_list* solution_pop(Tsolution **s){ //does not free e_l
  printf("POP\n");
  Te_list *temp;
  Tsolution *start;
  start = *s;
  temp = (*s)->e_l;
  printf("wywalam %c\n", temp->e->c);
  *s = (*s)->next;
  if((*s)->e_l == NULL) printf("WYNULLOWANE\n");
  else printf("Po popowaniu %c\n", (*s)->e_l->e->c);
  //free(start);
  //printf("ZWALNIAM\n");
  return temp;
}

/*
void cover (Tlist_header *header){
  Tdancing_list *col, *row;

  header->right->left = header->left;
  header->left->right = header->right;
  if(header->size > 0){
    col = header->down;
    for(int i = 0; i < header->size; i++){
      row = col->right;
      while(row != col){
        row->down->up = row->up;
        row->up->down = row->down;
        row->column->size--;
        row = row->right;
      }
      col = col->down;
    }
  }
}*/

bool emptyColumn(Tlist_header *column){
  if(column->down == NULL) return true;
  else return column->down->c == '_';
}

void cover (Tlist_header *header){
  Tdancing_list *col, *row;
  header->right->left = header->left;
  header->left->right = header->right;
  if(!emptyColumn(header) && !header->empty){
    col = header->down;
    putchar('X');
    putchar(col->c);
    putchar('\n');
    //col->covered = true; //temp
    //printf("%c litera\n", col->c);
    do{
      row = col->right;
      //putchar(row->c);
      while(row != col){
        //putchar(row->c);
        if(row->down->down = row->down){
          row->column->empty = true;
        } else {
          if(row = row->column->down){
            row->column->down = row->down;
          }
          row->down->up = row->up;
          row->up->down = row->down;
          //row->covered = true; //temp
          row = row->right;
        }
      }
      col = col->down;
      //col->covered = true;
    }
    while(col != header->down);
  }
}

/*void uncover(Tlist_header *header){
    Tdancing_list *col, *row;

    if(header->size > 0){
      col = header->down->up;
      for(int i = 0; i < header->size; i++){
        row = col->left;
        while(row != col){
          row->column->size++;
          row->up->down = row;
          row->down->up = row;
          row = row -> left;
        }
        col = col->up;
      }
    }
    header->left->right = header;
    header->right->left = header;
}*/

void uncover(Tlist_header *header){
    Tdancing_list *col, *row;

    if(!emptyColumn(header)){
      col = header->down->up;
      //col->covered = false; //temp
      do{
        row = col->left;
        while(row != col){
          if(col = col->column->down){
            row->column->down = row;
          }
          row->up->down = row;
          row->down->up = row;
          row->column->empty = false;
          //row->covered = false; //temp
          row = row -> left;
        }
        col = col->up;
        //col->covered = false; //temp
      }
      while(col != header->down->up);
    }
    header->left->right = header;
    header->right->left = header;
}

Tdancing_list *new_dancing(char c){
  Tdancing_list *new;
  new = (Tdancing_list*)malloc(sizeof(Tdancing_list));
  new->c = c;
  new->covered = false; //temp
  return new;
}

Tlist_header *new_header(){
  Tlist_header *new;
  new = (Tlist_header*)malloc(sizeof(Tlist_header));
  new->down = NULL;
  new->empty = false;
  return new;
}

void link_right(Tdancing_list *left, Tdancing_list *right){
  left->right = right;
  right->left = left;
}

void link_down(Tdancing_list *up, Tdancing_list *down){
  up->down = down;
  down->up = up;
  down->column = up->column;
}

void link_right_header(Tlist_header *left, Tlist_header *right){
  left->right = right;
  right->left = left;
}

Tdancing_list* load_filter(){
  Tdancing_list *start, *filter;
  char c;
  start = (Tdancing_list*)malloc(sizeof(Tdancing_list));
  start->c = 0;
  filter = start;
  c = getchar();
  while(c != '\n' && c != EOF){
    filter->c = c;
    filter->right = new_dancing(0);
    filter = filter->right;
    c = getchar();
  }
  return start;
}

Tlist_header* load_first(Tl_p_header **lph){
  Tlist_header *root, *header;
  Tdancing_list *row_start, *row;
  char c;
  root = new_header();
  c = getchar();
  if(c != '\n' && c != EOF){
    header = new_header();
    link_right_header(root, header);
    row_start = new_dancing(c);
    header->down = row_start;
    row_start->column = header;
    *lph = new_lph(header);
    row = row_start;
    c = getchar();
    while(c != '\n' && c != EOF){
      link_right_header(header, new_header());
      header = header->right;
      link_right(row, new_dancing(c));
      row = row->right;
      header->down = row;
      row->column = header;
      add_to_lph(*lph, header);
      c = getchar();
    }
    link_right_header(header, root);
    link_right(row, row_start);
  }
  if(c == EOF) ungetc(c, stdin);
  return root;
}

void load_rest(Tlist_header *root){
  Tdancing_list *up_start, *up, *down_start, *down;
  char c;
  c = getchar();
  down_start = root->right->down;
  while(c != '\n' && c != EOF){
    up_start = down_start;
    up = up_start;

    down_start = new_dancing(c);
    down = down_start;
    link_down(up_start, down_start);
    c = getchar();
    while(c != '\n' && c != EOF){
      up = up->right;
      link_right(down, new_dancing(c));
      down = down->right;
      link_down(up, down);
      c = getchar();
    }
    link_right(down, down_start);
    if(c != EOF) c = getchar();
  }

  link_down(down_start, down_start->column->down);
  //down_start->column->up = down_start;
  down = down_start->right;
  while(down != down_start){
    link_down(down, down->column->down);
    //down->column->up = down;
    down = down->right;
  }
}

void detach(Tdancing_list *e){
  if(e == e->column->down){
    e->column->down = e->down;
  }
  if(e->down == e){
    e->column->down = NULL;
  }
  e->down->up = e->up;
  e->up->down = e->down;
  e->right->left = e->left;
  e->left->right = e->right;
  free(e);
}

//may leave empty columns pointing on '_'
void clean_list(Tlist_header *root){
  Tdancing_list *start, *col, *next_col, *row, *next_row;

  start = root->right->down;
  col = start->down;
  while(col != start){
    next_col = col->down;
    row = col->right;
    while(row != col){
      next_row = row->right;
      if (row->c == '_') detach(row);
      row = next_row;
    }
    if (row->c == '_') detach(row);
    col = next_col;
  }
  row = col->right;

  while(row != col){
    next_row = row->right;
    if (row->c == '_') detach(row);
    row = next_row;
  }
  if (row->c == '_'){
    row->column->down = row->down;
    detach(row);
  }
}

/*
Tlist_header *copy_list(Tlist_header *root){
  Tlist_header *new_root, *header, *new_header;
  Tdancing_list *row_start, *row;
  new_root = new_header();
  header = root->right;
  link_right_header(new_root, new_header());
  new_header = root->right;
  new_header->size = header->size;
  header = header->right;
  while(header != root){
    link_right_header(new_header, new_header());
    new_header = new_header->right;
    new_header->size = header->size;
    header = header->right;
  }
}*/


bool search_and_print(Tsolution *s, Tlist_header *col){
  Te_list *e_list;
  e_list = s->e_l;
  //printf("char w e_list %c\n", e_list->e->c);
  do{
    if(e_list->e->column == col){
      printf("XxX");
      putchar(e_list->e->c);
      printf("XxX");
      return true;
    }
    e_list = e_list->next;
  }
  while(e_list != NULL);
  return false;
}

void printSolution(Tsolution *s, Tdancing_list *filter_list, Tl_p_header *lph){
  Tl_p_header *list_p_h;
  Tdancing_list *filter;
  Tsolution *sol;
  list_p_h = lph;
  filter = filter_list;
  if(s == s->next) printf("WTF\n");
  while(list_p_h != NULL){
    printf("\nw princie\n");
    if (filter->c == '+'){
      sol = s;
      while(sol->next != NULL && !search_and_print(sol, list_p_h->header)) sol = sol->next;
    }
    list_p_h = list_p_h->next;
    filter = filter->right;
  }
  printf("\n");
}

/*
void printList(Tlist_header *root){
  Tdancing_list *start, *row, *col;
  start = root->right->down;
  col = start;
  do{
    row = col;
    do{

    }
  }
}*/

void printList(Tlist_header *root){
  Tdancing_list *start, *row, *col;
  start = root->right->down;
  while(start->covered) start = start->down;
  col = start;
  do{
    row = col;
    do{
      if(!row->covered){
        putchar(row->c);
      }
      row = row->right;
    } while (row != col);
    printf("\n");
    col = col->down;
  } while (col != start && col->down != col);
}


void algorithm_x(Tlist_header *root, Tl_p_header **lph, Tsolution **solution, Tdancing_list *filter){
  Tlist_header *column;
  Tdancing_list *col_start, *col, *row;
  printf("znowu rek\n");
  if(root->right == root){
    printSolution(*solution, filter, *lph);
    printf("to sie drukuje\n");
    if((*solution)->e_l != NULL) printf("zaczynam sie od %c\n", (*solution)->e_l->e->c);
  } else {
    column = root->right;
    if(!emptyColumn(column)){
      cover(column);
      printf("zakrylem\n");
      col_start = column->down;
      col = col_start;
      do{
        printf("pushuje %c\n", col->c);
        solution_push(solution, col);
        printf("pushniete %c\n", col->c);
        if((*solution)->e_l != NULL) printf("zaczynam sie od %c\n", (*solution)->e_l->e->c);
        row = col->right;
        while(row != col){
          printf("char w kol %c\n", row->column->down->c);
          cover(row->column);
          printf("zakrylem boczne\n");
          row = row->right;
        }
        printf("rekurencjuje po %c\n", col->c);
        algorithm_x(root, lph, solution, filter);
        solution_pop(solution);
        if((*solution)->e_l != NULL) printf("zaczynam sie od %c\n", (*solution)->e_l->e->c);
        printf("po rekurencji po %c\n", col->c);
        row = col->left;
        while(row != col){
          uncover(row->column);
          printf("odkrylem\n");
          row = row->left;
        }
        col = col->down;
        printf("kolumna w dol \n");
      } while (col != col_start);
      uncover(column);
      printf("odkrylem\n");
    } else {
      printf("ELSE TODO!\n");
      //TODO?
    }
  }
}

int main(){
  Tdancing_list *filter;
  Tlist_header *list;
  Tl_p_header **lph;
  Tsolution **solution;
  solution = malloc(sizeof(*solution));
  *solution = new_solution();
  filter = load_filter();
  list = load_first(lph);
  load_rest(list);
  clean_list(list);
  algorithm_x(list, lph, solution, filter);
}

/*
Tlist_header *lista, *covered, *covered2;
Tl_p_header *lph;
lista = load_first(&lph);
load_rest(lista);
clean_list(lista);
covered = lista->right->right;
covered2 = covered->right;
cover(covered);
cover(covered2);*/
/* while(lph != NULL){
  putchar(lph->header->down->c);
  lph = lph->next;
}
printf("\n"); */
//printf("ha\n");
/*uncover(covered2);
uncover(covered);
printList(lista);
return 0;*/
