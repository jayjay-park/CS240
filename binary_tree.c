/* Add any includes here */
#include "hw11.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>

/* Define any global variables here */
int g_values[MAX_NODE_SIZE] = {0};

/* Define your functions here */

/*
 * int prefix(tree_node_t *root, int count_tree) is a helper function
 * that will traverse the tree in prefix order and returns the number of nodes
 * in the tree
 */

int prefix(tree_node_t *root, int count_tree) {

  if (root == NULL) {
    return count_tree;
  }

  g_values[count_tree++] = root->value;
  printf("value:%d\n", root->value);
  printf("count_tree:%d\n", count_tree);

  count_tree = prefix(root->left_child_ptr, count_tree);
  count_tree = prefix(root->right_child_ptr, count_tree);

  return count_tree;
} /* prefix() */

/*
 * int forward(tree_node_t *root, int count_tree) is a helper function
 * that will traverse the tree in forward order and
 * returns the number of nodes in the tree
 */

int forward(tree_node_t * root, int count_tree) {
  if (root == NULL) {
    return count_tree;
  }

  count_tree = forward(root->left_child_ptr, count_tree);
  g_values[count_tree++] = root->value;
  count_tree = forward(root->right_child_ptr, count_tree);

  return count_tree;
} /* forward() */


/*
 * int reverse(tree_node_t *root, int count_tree) is a helper function
 * that will traverse the tree in reverse order and
 * returns the number of nodes in the tree
 */

int reverse(tree_node_t * root, int count_tree) {

  if (root == NULL) {
    return count_tree;
  }

  count_tree = reverse(root->right_child_ptr, count_tree);
  g_values[count_tree++] = root->value;
  count_tree = reverse(root->left_child_ptr, count_tree);

  return count_tree;
} /* reverse() */


/*
 * int traverse_order_check(int traversal_order) is a helper function
 * that checks if the argument is either 1,2,3 or 4
 * it returns 1 when it is true
 */

int traverse_order_check(int traversal_order) {
  if (((traversal_order != 1) && (traversal_order != 2)) &&
      ((traversal_order != 3) && (traversal_order != 4))) {
    return 0;
  }
  return 1;
} /* traverse_order_check() */

/*
 * tree_node_t *create_node(int given_value) will dynamically allocate
 * memory space for the new tree_node_t and return the pointer
 * to the new tree_node_t
 */

tree_node_t *create_node(int given_value) {

  assert(given_value > 0);

  tree_node_t *new_node = malloc(sizeof(tree_node_t));
  assert(new_node != NULL);

  new_node->value = given_value;
  new_node->left_child_ptr = NULL;
  new_node->right_child_ptr = NULL;

  return new_node;

} /* create_node() */


/*
 * void insert_node(tree_node_t **root, tree_node_t *new_element)
 * inserts the node in the appropriate area recursively
 */

void insert_node(tree_node_t **root, tree_node_t *new_element) {

  assert(root != NULL);
  assert(new_element != NULL);
  assert(new_element->left_child_ptr == NULL);
  assert(new_element->right_child_ptr == NULL);

  // if root tree is NULL
  if (*root == NULL) {
    *root = new_element;
    return;
  }

  if (new_element->value <= (*root)->value) {
    if ((*root)->left_child_ptr == NULL) {
      (*root)->left_child_ptr = new_element;
      return;
    }
    else {
      //*root = (*root)->left_child_ptr;
      insert_node(&((*root)->left_child_ptr), new_element);
    }
  }
  else {
    if ((*root)->right_child_ptr == NULL) {
      (*root)->right_child_ptr = new_element;
      return;
    }
    else {
      //*root = (*root)->right_child_ptr;
      insert_node(&(*root)->right_child_ptr, new_element);
    }
  }

} /* insert_node() */


/*
 * tree_node_t *search_node(tree_node_t *root, int given_value)
 * searches for tee_node_t that has matching value
 * and returns the pointer to the node found
 */

tree_node_t *search_node(tree_node_t *root, int given_value) {

  assert(given_value > 0);

  if (root == NULL) {
    return NULL;
  }

  if (root->value == given_value) {
    return root;
  }

  if (given_value < root->value) {
    return search_node(root->left_child_ptr, given_value);
  }
  else {
    return search_node(root->right_child_ptr, given_value);
  }


} /* search_node() */


/* void delete_tree(tree_node_t **root) deletes the enitre tree
 * recursively. The root  should be set to NULL.
 */

void delete_tree(tree_node_t **root) {

  assert(root != NULL);

  if (*root == NULL) {
    return;
  }

  delete_tree(&((*root)->left_child_ptr));
  delete_tree(&((*root)->right_child_ptr));

  free(*root);
  *root = NULL;

} /* delete_tree() */


/*
 * int get_max_depth(tree_node_t *tree) traverses the tree recursively
 * and returns the number of the maximum  depth of tree
 */

int get_max_depth(tree_node_t *tree) {

  if (tree == NULL) {
    return 0;
  }

  int left_sub_tree = get_max_depth(tree->left_child_ptr);
  int right_sub_tree = get_max_depth(tree->right_child_ptr);

  if (left_sub_tree > right_sub_tree) {
    return left_sub_tree + 1;
  }
  else {
    return right_sub_tree + 1;
  }

} /* get_max_depth() */


/*
 * int find_values_by_order(tree_node_t *root, int traversal_order)
 * traverses the tree in four different way that was given by the argument
 * (prefix, postfix, forward, reverse) and returns
 * the total number of nodes in the tree
 */

int find_values_by_order(tree_node_t *root, int traversal_order) {

  assert(traverse_order_check(traversal_order));

  if (root == NULL) {
    return 0;
  }

  if (traversal_order == 1) {
    int count_tree = 0;
    int total_number_of_tree = prefix(root, count_tree);

    return total_number_of_tree;
  }
  else if (traversal_order == 2) {
    int count_tree = 0;
    int total_number_of_tree = postfix(root, count_tree);

    return total_number_of_tree;
  }
  else if (traversal_order == 3) {
    int count_tree = 0;
    int total_number_of_tree = forward(root, count_tree);

    return total_number_of_tree;
  }
  else {
    int count_tree = 0;
    int total_number_of_tree = reverse(root, count_tree);
    return  total_number_of_tree;
  }


} /* find_values_by_order() */

