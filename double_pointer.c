#include "hw10.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>



/*
 * void create_transaction(char *account_name, char *hash_id,
 * float account_amount,float time_entry,
 * transaction_t ** ptr) will crate new entry_t
 * and transaction_t and move ptr to the newly created transaction_t
 */

void create_transaction(char *account_name, char *hash_id, float account_amount,
                        float time_entry, transaction_t **ptr) {

  assert(account_name != NULL);
  assert(hash_id != NULL);
  assert(account_amount >= 0.0);
  assert((time_entry >= 0.0) && (time_entry < 24.0));
  assert(ptr != NULL);
  assert(*ptr == NULL);

  entry_t *new_entry = malloc(sizeof(entry_t));
  assert(new_entry != NULL);
  new_entry->account_name = malloc(strlen(account_name) + 1);
  assert(new_entry->account_name != NULL);
  new_entry->hash_id = malloc(strlen(hash_id) + 1);
  assert(new_entry->hash_id != NULL);
  printf("Successfully created entry_t!\n");

  strcpy(new_entry->account_name, account_name);
  strcpy(new_entry->hash_id, hash_id);
  new_entry->amount = account_amount;
  new_entry->time = time_entry;
  printf("Successfully stored data to new entry_t!\n");

  transaction_t *new_transaction = malloc(sizeof(transaction_t));
  assert(new_transaction != NULL);
  new_transaction->data = new_entry;
  new_transaction->next_transaction = NULL;
  new_transaction->prev_transaction = NULL;
  printf("Successfully created transaction_t!\n");

  *ptr = new_transaction;
} /* create_transaction() */


/*
 * void delete_transaction(transaction_t **given_transaction will
 * deallocate the given transaction
 */

void delete_transaction(transaction_t ** given_transaction) {

  assert(given_transaction != NULL);
  assert(*given_transaction != NULL);
  assert((*given_transaction)->next_transaction == NULL);
  assert((*given_transaction)->prev_transaction == NULL);
  assert((*given_transaction)->data != NULL);

  free((*given_transaction)->data->account_name);
  (*given_transaction)->data->account_name = NULL;

  free((*given_transaction)->data->hash_id);
  (*given_transaction)->data->hash_id = NULL;

  free((*given_transaction)->data);
  (*given_transaction)->data = NULL;

  free(*given_transaction);
  *given_transaction = NULL;
} /* delete_transaction() */


/*
 * void insert_transaction(transaction_t **list,
 * transaction_t *given_transaction) will insert
 * given_t to list according to the order of hash_id on the list.
 */

void insert_transaction(transaction_t **list,
                        transaction_t *given_transaction) {

  assert(list != NULL);
  assert(*list != NULL);
  assert(given_transaction != NULL);

  printf("given_t: %s\n", given_transaction->data->hash_id);
  while ((*list)->prev_transaction != NULL) {
    *list = (*list)->prev_transaction;
  }
  printf("Successfully moved pointer to head!\n");
  printf("head: %s\n", (*list)->data->hash_id);

  int hash_id_order = 0;
  if ((*list)->prev_transaction == NULL) {
    hash_id_order = strcmp((*list)->data->hash_id,
                           given_transaction->data->hash_id);
    if (hash_id_order > 0) {
      (*list)->prev_transaction = given_transaction;
      given_transaction->next_transaction = *list;
      given_transaction->prev_transaction = NULL;
      *list = given_transaction;
      return;
    }
  }

  transaction_t *temp_traverse = *list;
  int compare_current = 0;
  int compare_next = 0;

  while (temp_traverse != NULL) {
    compare_current = strcmp(temp_traverse->data->hash_id,
                             given_transaction->data->hash_id);
    if (compare_current < 0) {
      if (temp_traverse->next_transaction != NULL) {
        compare_next = strcmp(given_transaction->data->hash_id,
                              temp_traverse->next_transaction->data->hash_id);
        if (compare_next < 0) {
          if (temp_traverse->next_transaction != NULL) {
            temp_traverse->next_transaction->
                         prev_transaction = given_transaction;
          }
          given_transaction->prev_transaction = temp_traverse;
          given_transaction->next_transaction = temp_traverse->next_transaction;
          temp_traverse->next_transaction = given_transaction;
          printf("Successfully inserted the transaction!\n");
          return;
        }
      }
      else {
        temp_traverse->next_transaction = given_transaction;
        given_transaction->prev_transaction = temp_traverse;
        return;
      }
    }
    temp_traverse = temp_traverse->next_transaction;
  }

} /* insert_transaction() */


/*
 * int delete_bank_transaction_history(transaction_t **list)
 * will deallocate the entire bank history pointed by list.
 */

int delete_bank_transaction_history(transaction_t **list) {

  assert(list != NULL);

  if (*list == NULL) {
    return 0;
  }
  while ((*list)->prev_transaction != NULL) {
    *list = (*list)->prev_transaction;
  }
  printf("Successfully moved pointer to head\n");

  transaction_t *temp_delete = *list;
  printf("temp_delete:%p\n", temp_delete);
  printf("list:%p\n", list);
  int count_transaction = 0;

  while ((*list) != NULL) {
    temp_delete = *list;
    *list = (*list)->next_transaction;

    (temp_delete)->next_transaction = NULL;
    (temp_delete)->prev_transaction = NULL;
    delete_transaction(&temp_delete);

    count_transaction++;
  }
  printf("Successfully deleted the transaction history!\n");

  list = NULL;
  printf("count:%d\n", count_transaction);
  return count_transaction;
} /* delete_bank_transaction_history() */


/*
 * int find_transaction(transaction_t **list, char *given_hash_id)
 * will identify the first transaction of the specified hash id.
 * If the transaction is found, this function will return
 * the placement number of the first found transaction.
 * Also, pointer pointed by list will point to the matching transaction.
 */

int find_transaction(transaction_t **list, char *given_hash_id) {

  assert(list != NULL);
  assert(*list != NULL);
  assert(given_hash_id != NULL);

  while ((*list)->prev_transaction != NULL) {
    *list = (*list)->prev_transaction;
  }

  transaction_t *temp_traverse = *list;
  int count_transaction = 0;

  while (temp_traverse != NULL) {
    count_transaction++;
    if (strncmp(temp_traverse->data->hash_id, given_hash_id,
                strlen(given_hash_id) + 1) == 0) {
      *list = temp_traverse;
      return count_transaction;
    }
    temp_traverse = temp_traverse->next_transaction;
  }

  *list = NULL;
  return TRANSACTION_NOT_FOUND;
} /* find_transaction() */


/*
 * int get_over_amount_list(transaction_t *list, float given_amount,
 * transaction_t **head_new_list) will build a new list of transaction_t
 * that has amount larger than a specifed amount without
 * duplicating the actual data. The pointer the third argument points to
 * should be set to head of the new list. The function returns the number
 * of transactions found.
 */

int get_over_amount_list(transaction_t *list, float given_amount,
                         transaction_t **new_list) {
  assert(list != NULL);
  assert(new_list != NULL);
  assert(given_amount >= 0.0);
  assert(*new_list == NULL);

  if (list->prev_transaction != NULL) {
    while (list->prev_transaction != NULL) {
      list = list->prev_transaction;
    }
  }
  printf("Successfully moved pointer to head!\n");

  transaction_t *head = NULL;
  int count_transaction = 0;

  while (list != NULL) {

    printf("OK\n");
    if (list->data->amount > given_amount) {

      transaction_t *new_node = malloc(sizeof(transaction_t));
      assert(new_node != NULL);

      new_node->prev_transaction = NULL;
      new_node->next_transaction = NULL;
      new_node->data = list->data;
      printf("Successfully created the new_node!\n");

      if (*new_list != NULL) {
        if ((*new_list)->next_transaction != NULL) {
          (*new_list)->next_transaction->prev_transaction = (new_node);
        }
        (*new_list)->next_transaction = new_node;
        (new_node)->prev_transaction = *new_list;
        (new_node)->next_transaction = NULL;
        (*new_list) = (*new_list)->next_transaction;
      }
      else {
        *new_list = new_node;
        head = *new_list;
      }
      count_transaction++;
    }

    list = list->next_transaction;
  }

  if (*new_list != NULL) {
    *new_list = head;
  }

  return count_transaction;
} /* get_over_amount_list() */


/*
 * int delete_list(transaction_t **list) should deallocate the entire list
 * of transaction_ts without deallocating the actual payloads. Returns the
 * number of transactions deallocated aswell as set the pointer pointed to
 * by the argument to NULL.
 */

int delete_list(transaction_t **list) {

  assert(list != NULL);
  assert(*list != NULL);

  while ((*list)->prev_transaction != NULL) {
    *list = (*list)->prev_transaction;
  }

  int count_transaction = 0;
  transaction_t *temp_delete = *list;
  while (temp_delete != NULL) {
    *list = (*list)->next_transaction;

    free(temp_delete);
    temp_delete = NULL;

    temp_delete = *list;
    count_transaction++;
  }

  *list = NULL;
  return count_transaction++;
} /* delete_list() */
