#include "hw9.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>



/*
 *  transaction_t *create_transaction(char *account_n, char *hash_id,
 *  float amount, float time) will create entry_t with above data then,
 *  initialize next_transaction and prev_transaction to NULL.
 *  Returns pointer to created transaction_t struct
 */

transaction_t *create_transaction(char *account_n, char *hash_id,
                                  float amount, float time) {
  assert(account_n != NULL);
  assert(hash_id != NULL);
  assert(amount >= 0);
  assert((time >= 0) && (time < 24));

  entry_t *new_entry = malloc(sizeof(entry_t));
  assert(new_entry != NULL);

  new_entry->account_name = malloc(strlen(account_n) + 1);
  assert(new_entry->account_name != NULL);

  new_entry->hash_id = malloc(strlen(hash_id) + 1);
  assert(new_entry->hash_id != NULL);

  strncpy(new_entry->account_name, account_n, strlen(account_n) + 1);
  strncpy(new_entry->hash_id, hash_id, strlen(hash_id) + 1);
  new_entry->amount = amount;
  new_entry->time = time;


  transaction_t *new_transaction = malloc(sizeof(transaction_t));
  assert(new_transaction != NULL);
  new_transaction->data = new_entry;
  new_transaction->next_transaction = NULL;
  new_transaction->prev_transaction = NULL;

  return new_transaction;

} /* create_transaction() */

/*
 * void delete_transaction(transaction_t *given_trasaction)
 * deallocates a single transaction_t and all of its associated data
 */

void delete_transaction(transaction_t *given_transaction) {

  assert(given_transaction != NULL);
  assert(given_transaction->next_transaction == NULL);
  assert(given_transaction->prev_transaction == NULL);
  assert(given_transaction->data != NULL);

  printf("Start delete_transaction\n");
  if (given_transaction->data->account_name != NULL) {
    free(given_transaction->data->account_name);
    given_transaction->data->account_name = NULL;
  }
  if (given_transaction->data->hash_id != NULL) {
    free(given_transaction->data->hash_id);
    given_transaction->data->hash_id = NULL;
  }

  free(given_transaction->data);
  given_transaction->data = NULL;

  free(given_transaction);
  given_transaction = NULL;
  printf("Finish delete_transaction\n");

} /* delete_transaction() */


/*
 * void insert_transaction(transaction_t *list, transaction_t *new_element)
 * inserts element into list which is sorted by hash_id
 */

void insert_transaction(transaction_t *list, transaction_t *new_element) {

  assert(list != NULL);
  assert(new_element != NULL);

  int hash_id_order = 0;

  while (list->prev_transaction != NULL) {
    list = list->prev_transaction;
  }
  printf("Successfully moved pointer to head!\n");

  if (list->prev_transaction == NULL) {
    hash_id_order = strcmp(list->data->hash_id, new_element->data->hash_id);
    if (hash_id_order > 0) {
      list->prev_transaction = new_element;
      new_element->next_transaction = list;
      new_element->prev_transaction = NULL;
      return;
    }
  }

  transaction_t *temp_traverse = list;
  int compare_current = 0;
  int compare_next = 0;

  while (temp_traverse != NULL) {
    compare_current = strcmp(temp_traverse->data->hash_id,
                             new_element->data->hash_id);
    if (compare_current < 0) {
      if (temp_traverse->next_transaction != NULL) {
        compare_next = strcmp(new_element->data->hash_id,
                              temp_traverse->next_transaction->data->hash_id);
        if (compare_next < 0) {
          if (temp_traverse->next_transaction != NULL) {
            temp_traverse->next_transaction->prev_transaction = new_element;
          }
          new_element->prev_transaction = temp_traverse;
          new_element->next_transaction = temp_traverse->next_transaction;
          temp_traverse->next_transaction = new_element;
          printf("Successfully inserted the transaction!\n");
          return;
        }
      }
      else {
        temp_traverse->next_transaction = new_element;
        new_element->prev_transaction = temp_traverse;
        return;
      }
    }
    temp_traverse = temp_traverse->next_transaction;
  }

} /* insert_transaction() */


/*
 * void delete_bank_transaction_history(transaction_t *list)
 * deallocates an entire bank transaction history
 */

void delete_bank_transaction_history(transaction_t *list) {

  assert(list != NULL);

  if (list->prev_transaction != NULL) {
    while (list->prev_transaction != NULL) {
      list = list->prev_transaction;
    }
  }
  printf("Successfully move pointer to head\n");

  transaction_t *temp_traverse = list;

  while (list->next_transaction != NULL) {
    printf("%p\n", list);
    temp_traverse = list;
    list = temp_traverse->next_transaction;

    temp_traverse->prev_transaction = NULL;
    temp_traverse->next_transaction = NULL;

    printf("OK\n");

    delete_transaction(temp_traverse);
    printf("Returning from delete_transaction()\n");
  }

  printf("Delete the last node\n");
  temp_traverse = list;
  temp_traverse->prev_transaction = NULL;
  temp_traverse->next_transaction = NULL;
  delete_transaction(temp_traverse);

} /* delete_bank_transaction_history() */


/*
 * transaction_t *find_transaction(transaction_t *list, char *given_h_id)
 * will return transaction_t if transaction_t with the matching given_h_id
 * exists in the list*/

transaction_t *find_transaction(transaction_t *list, char *given_h_id) {

  assert(list != NULL);
  assert(given_h_id != NULL);

  transaction_t *temp_traverse = list;

  if (list->prev_transaction != NULL) {
    while (temp_traverse->prev_transaction != NULL) {
      temp_traverse = temp_traverse->prev_transaction;
    }
  }
  printf("Successfully moved pointer to head!\n");

  int compare_hash_id = 1;

  while (temp_traverse != NULL) {
    compare_hash_id = strcmp(temp_traverse->data->hash_id, given_h_id);
    if (compare_hash_id == 0) {
      return temp_traverse;
    }
    temp_traverse = temp_traverse->next_transaction;
  }

  return NULL;

} /* find_transaction() */


/*
 * transaction_t *remove_transaction(transaction_t *transaction_r)
 * removes the transaction pointed by transaction_r
 * returns the head after removing the transaction
 */

transaction_t *remove_transaction(transaction_t *t_remove) {

  if (t_remove == NULL) {
    return NULL;
  }

  transaction_t *head = t_remove;
  while (head->prev_transaction != NULL) {
    head = head->prev_transaction;
  }

  if (head == t_remove) {
    head = head->next_transaction;
  }

  if (t_remove->next_transaction != NULL) {
    t_remove->next_transaction->prev_transaction = t_remove->prev_transaction;
  }
  if (t_remove->prev_transaction != NULL) {
    t_remove->prev_transaction->next_transaction = t_remove->next_transaction;
  }

  t_remove->next_transaction = NULL;
  t_remove->prev_transaction = NULL;


  return head;

} /* remove_transaction() */


/*
 * transaction_t *delete_transactions_by_account_name(transaction_t *list,
 * char *given_account_n) will traverse the list
 * and find the matching transaction that has given_account_n
 * and remove all transactions from the list that were made by the given
 * account name
 */

transaction_t *delete_transactions_by_account_name(transaction_t *list,
                                                   char *given_account_n) {
  assert(given_account_n != NULL);
  if (list == NULL) {
    return NULL;
  }

  if (list->prev_transaction != NULL) {
    while (list->prev_transaction != NULL) {
      list = list->prev_transaction;
    }
  }
  printf("Successcully moved pointer to head!\n");

  transaction_t *temp_traverse = list;
  int compare_account_n = 1;
  transaction_t *head = list;

  while (list != NULL) {
    compare_account_n = strcmp(list->data->account_name, given_account_n);

    if (compare_account_n == 0) {
      temp_traverse = list;
      list = list->next_transaction;

      if (temp_traverse == head) {
        head = head->next_transaction;
      }
      if (temp_traverse->prev_transaction != NULL) {
        temp_traverse->prev_transaction->next_transaction = temp_traverse->
                                                              next_transaction;
      }
      if (temp_traverse->next_transaction != NULL) {
        temp_traverse->next_transaction->prev_transaction = temp_traverse->
                                                              prev_transaction;
      }
      temp_traverse->next_transaction = NULL;
      temp_traverse->prev_transaction = NULL;
      delete_transaction(temp_traverse);
    }
    else {
      list = list->next_transaction;
    }
  }

  return head;
} /* delete_transactions_by_account_name() */

