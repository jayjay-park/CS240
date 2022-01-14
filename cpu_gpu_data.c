#include "hw12.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>


/*
 * void create_cpu_data(void **new_node, const char *model,
 * const char *manufacturer, int speed, int cores) create new node
 * for cpu_t then set the first argument's pointer to point to the
 * cpu_t
 */

void create_cpu_data(void **new_node, const char *model,
                     const char *manufacturer, int speed, int cores) {

  assert(new_node != NULL);
  assert(*new_node == NULL);
  assert(model != NULL);
  assert(manufacturer != NULL);

  cpu_t *cpu_node = malloc(sizeof(cpu_t));
  assert(cpu_node != NULL);

  cpu_node->model = malloc(strlen(model) + 1);
  assert(cpu_node->model != NULL);
  strcpy(cpu_node->model, model);

  cpu_node->manufacturer = malloc(strlen(manufacturer) + 1);
  assert(cpu_node->manufacturer != NULL);
  strcpy(cpu_node->manufacturer, manufacturer);

  cpu_node->speed = speed;
  cpu_node->cores = cores;

  *new_node = cpu_node;
} /* create_cpu_data() */

/*
 * delete_cpu_data(void **given_node) will deallocate the memory
 * for the relevant fields and then deallocate the node itself.
 */

void delete_cpu_data(void **given_cpu) {

  assert(given_cpu != NULL);
  assert(*given_cpu != NULL);
  cpu_t *cpu_data = (cpu_t *)(*given_cpu);
  assert((cpu_data)->model != NULL);
  assert((cpu_data)->manufacturer != NULL);

  if (cpu_data->model != NULL) {
    free(cpu_data->model);
    cpu_data->model = NULL;
  }
  if (cpu_data->manufacturer != NULL) {
    free(cpu_data->manufacturer);
    cpu_data->manufacturer = NULL;
  }
  free(cpu_data);
  cpu_data = NULL;

  given_cpu = NULL;
} /* delete_cpu_data() */


/*
 * int compare_cpu_data(void *first_cpu, void *second_cpu) will
 * compare the overall speed of two cpu node. Comparing value is
 * speed*cores of each cpu_t. Return value will be 1 if first cpu
 * has bigger value than the second cpu. If equal 0, and if second cpu
 * has bigger value than the first cpu, return value will be -1.
 */

int compare_cpu_data(void *first_cpu, void *second_cpu) {

  assert(first_cpu != NULL);
  assert(second_cpu != NULL);

  cpu_t *first_cpu_ptr = (cpu_t *) first_cpu;
  cpu_t *second_cpu_ptr = (cpu_t *) second_cpu;

  int first_cpu_value = first_cpu_ptr->speed * first_cpu_ptr->cores;
  int second_cpu_value = second_cpu_ptr->speed * second_cpu_ptr->cores;

  if (first_cpu_value > second_cpu_value) {
    return 1;
  }
  if (first_cpu_value < second_cpu_value) {
    return -1;
  }
  else {
    return 0;
  }

} /* compare_cpu_data() */


/*
 * void create_memory_data(void **new_node, const char *model,
 * const cahr *manufacturer, int size, int speed, int ddr_gen)
 * will create new node for memory_t then set the first argument's
 * pointer to point to the new memory_t
 */

void create_memory_data(void **new_node, const char*model,
                        const char *manufacturer, int size,
                        int speed, int ddr_gen) {

  assert(new_node != NULL);
  assert(*new_node == NULL);
  assert(model != NULL);
  assert(manufacturer != NULL);

  memory_t *memory_node = malloc(sizeof(memory_t));
  assert(memory_node != NULL);

  memory_node->model = malloc(strlen(model) + 1);
  assert(memory_node->model != NULL);
  strcpy(memory_node->model, model);

  memory_node->manufacturer = malloc(strlen(manufacturer) + 1);
  assert(memory_node->manufacturer != NULL);
  strcpy(memory_node->manufacturer, manufacturer);

  memory_node->speed = speed;
  memory_node->size = size;
  memory_node->ddr_gen = ddr_gen;

  *new_node = memory_node;

} /* create_memory_data() */


/*
 * void delete_memory_data(void **given_memory) will deallocate the memory
 * for the relevant fields and then deallocate the node itself
 */

void delete_memory_data(void **given_memory) {

  assert(given_memory != NULL);
  assert(*given_memory != NULL);

  memory_t *memory_data = (memory_t *)(*given_memory);
  assert(memory_data->model != NULL);
  assert(memory_data->manufacturer != NULL);

  if (memory_data->model != NULL) {
    free(memory_data->model);
    memory_data->model = NULL;
  }
  if (memory_data->manufacturer != NULL) {
    free(memory_data->manufacturer);
    memory_data->manufacturer = NULL;
  }
  free(memory_data);
  memory_data = NULL;

  given_memory = NULL;


} /* delete_memory_data() */


/*
 * int compare_memory_data(void *first_memory, void *second_memory)
 * will compare the model and manufacturer of memory node.
 * If the model is identical, check if manufacturer is identical.
 * Return 1 if first_memory's manufacturer is bigger than the
 * second_memory's manufacturer, and return -1 if it is the other case,
 * return 0 if both memory's manufacturer is equal
 */

int compare_memory_data(void *first_memory, void *second_memory) {

  assert(first_memory != NULL);
  assert(second_memory != NULL);

  memory_t *first_memory_ptr = (memory_t *) first_memory;
  memory_t *second_memory_ptr = (memory_t *) second_memory;

  int compare_model = strcmp(first_memory_ptr->model,
                             second_memory_ptr->model);
  if (compare_model == 0) {
    int compare_manufacturer = strcmp(first_memory_ptr->manufacturer,
                                      second_memory_ptr->manufacturer);
    if (compare_manufacturer > 0) {
      return 1;
    }
    else if (compare_manufacturer < 0) {
      return -1;
    }
    else {
      return 0;
    }
  }
  else if (compare_model > 0) {
    return 1;
  }

  return -1;


} /* compare_memory_data() */


/*
 * void create_node(struct node **new_node, void *data, void (*print)(void*),
 * void (*delete)(void **), int (*compare)(void*, void*)) will dynamically
 * allocate new struct node and initialize the pointers of the node
 * using rest of the arguments.
 */

void create_node(struct node **new_node, void *data, void (*print)(void*),
                 void (*delete)(void **), int (*compare)(void*, void*)) {

  assert(new_node != NULL);
  assert(*new_node == NULL);
  assert(data != NULL);
  assert(print != NULL);
  assert(delete != NULL);
  assert(compare != NULL);


  *new_node = malloc(sizeof(struct node));
  assert(*new_node != NULL);

  (*new_node)->left = NULL;
  (*new_node)->right = NULL;
  (*new_node)->data = data;
  (*new_node)->print = print;
  (*new_node)->delete = delete;
  (*new_node)->compare = compare;

} /* create_node() */


/*
 * void delete_node(struct node** given_node) deallocate the data field
 * by calling the delete function
 */

void delete_node(struct node **given_node) {

  assert(given_node != NULL);
  assert(*given_node != NULL);
  assert((*given_node)->left == NULL);
  assert((*given_node)->right == NULL);
  assert((*given_node)->delete != NULL);

  void **delete_data = &((*given_node)->data);

  if ((*delete_data) != NULL) {
    ((*given_node)->delete)(delete_data);
  }
  free(*given_node);
  *given_node = NULL;
  given_node = NULL;

} /* delete_node() */


/*
 * void insert_node(struct node **root, struct node *new_element)
 * will insert the new_element to the tree using compare function.
 */

void insert_node(struct node **root, struct node *new_element) {

  assert(root != NULL);
  assert(new_element != NULL);

  if (*root == NULL) {
    *root = new_element;
    return;
  }

  int comparison = ((*root)->compare)((*root)->data, new_element->data);

  if (comparison == -1) {
    if ((*root)->right == NULL) {
      (*root)->right = new_element;
      return;
    }
    else {
      return insert_node(&(*root)->right, new_element);
    }
  }
  else {
    if ((*root)->left == NULL) {
      (*root)->left = new_element;
      return;
    }
    else {
      return insert_node(&(*root)->left, new_element);
    }
  }

} /* insert_node() */


/*
 * struct node **find_nodes(struct node *root, void *data,
 * int *num_of_duplicate) will create pointer array using malloc
 * to fill in pointer array with the addresses of the matching nodes.
 * Returns the array of pointers.
 */

struct node **find_nodes(struct node *root, void *data,
                         int *num_of_duplicates) {

  assert(root != NULL);
  assert(data != NULL);
  assert(num_of_duplicates != NULL);

  struct node *original_root = root;
  printf("started\n");


  while ((root != NULL)) {

    int data_comparison = (root->compare)(root->data, data);
    printf("data_comparison:%d\n", data_comparison);

    if (data_comparison > 0) {
      if (root->left != NULL) {
        root = root->left;
      }
      else {
        break;
      }
    }
    else if (data_comparison < 0) {
      if (root->right != NULL) {
        root = root->right;
      }
      else {
        break;
      }
    }
    else {
      (*num_of_duplicates)++;
      printf("num_of_dups:%d\n", *num_of_duplicates);
      if (root->left != NULL) {
        root = root->left;
      }
      else {
        break;
      }
    }
  }
  printf("outof first while loop\n");

  if ((*num_of_duplicates) == 0) {
    return NULL;
  }

  struct node **array_of_dups = malloc((*num_of_duplicates) *
                                        sizeof(struct node*));
  assert(array_of_dups != NULL);
  int array_index = 0;


  while (original_root != NULL) {
    int data_comparison = (original_root->compare)(original_root->data, data);
    if (data_comparison > 0) {
      if (original_root->left != NULL) {
        original_root = original_root->left;
      }
      else {
        break;
      }
    }
    else if (data_comparison < 0) {
      if (original_root->right != NULL) {
        original_root = original_root->right;
      }
      else {
        break;
      }
    }
    else if (data_comparison == 0) {
      array_of_dups[array_index++] = original_root;
      printf("index:%d\n", array_index);
      if (original_root->left != NULL) {
        original_root = original_root->left;
      }
      else {
        break;
      }
    }
  }
  return array_of_dups;
} /* find_nodes() */


/*
 * void remove_node(struct node **root, struct node *given_node) will
 * make comparisons to find the given_node and remove the given_node
 * from the tree
 */

void remove_node(struct node **root, struct node *given_node) {

  assert(root != NULL);
  assert(given_node != NULL);

  if (*root == NULL) {
    return;
  }

  if (((*root)->compare)((*root)->data, given_node->data) > 0) {
    if ((*root)->left != NULL) {
      remove_node(&((*root)->left), given_node);
    }
  }
  if (((*root)->compare)((*root)->data, given_node->data) < 0) {
    if ((*root)->right != NULL) {
      remove_node(&((*root)->right), given_node);
    }
  }
  if (((*root)->compare)((*root)->data, given_node->data) == 0) {

    if (given_node == *root) {
      printf("%p\n", given_node);
      printf("%p\n", *root);

    }

  }
} /* remove_node() */


/*
 * void delete_tree(struct node **root) will delete the whole tree
 * and set the root pointer to NULL
 */

void delete_tree(struct node **root) {

  assert(root != NULL);

  if (*root == NULL) {
    root = NULL;
    return;
  }

  if ((*root)->left != NULL) {
    delete_tree(&((*root)->left));
    printf("left\n");
  }
  if ((*root)->right != NULL) {
    delete_tree((&(*root)->right));
    printf("right\n");
  }
  printf("delete\n");
  delete_node(root);

} /* delete_tree() */
