#include "hw8.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>



/* song_t *add_song_to_head(song_t* head, char *title, char *artist)
 * create song_t that holds title and artist and
 * append it to the head of the list and returns
 * a pointer to the head of the new list
 */

song_t *add_song_to_head(song_t* head, char *title, char *artist) {

  assert(title != NULL);
  assert(artist != NULL);

  printf("head: %p, size: %ld\n", head, sizeof(head));

  if (head == NULL) {
    printf("Head is null!\n");

    head = malloc(sizeof(song_t));
    assert(head != NULL);

    head->next_song = NULL;

    head->title = malloc(strlen(title) + 1);
    assert(head->title != NULL);

    head->artist = malloc(strlen(artist) + 1);
    assert(head->artist != NULL);

    strncpy(head->title, title, strlen(title) + 1);
    strncpy(head->artist, artist, strlen(artist) + 1);
  }
  else {

    printf("head_title: %s, head_artist: %s\n", head->title, head->artist);
    printf("head_size: %ld, head_title_size: %ld\n",
           sizeof(head), sizeof(head->title));
    song_t *element = malloc(sizeof(song_t));

    printf("size_song_t: %ld\n", sizeof(song_t));
    printf("size_element: %ld\n", sizeof(element));
    assert(element != NULL);

    element->title = malloc(strlen(title) + 1);
    printf("size_element_title: %ld\n", sizeof(element->title));
    assert(element->title != NULL);

    element->artist = malloc(strlen(artist) + 1);
    assert(element->artist != NULL);

    element->next_song = NULL;

    strncpy(element->title, title, strlen(title) + 1);
    strncpy(element->artist, artist, strlen(artist) + 1);
    element->next_song = head;

    printf("element_title: %s, element_artist: %s\n",
           element->title, element->artist);

    head = element;
    printf("address of head:%p, address of element:%p\n", head, element);
    printf("sizeof_head:%ld, sizeof_element:%ld\n",
           sizeof(head), sizeof(element));

  }

  return head;
} /* add_song_to_head() */


/*song_t *remove_song_from_head(song_t *head)
 * will remove the head of the list and move the head
 * accordingly
 */

song_t *remove_song_from_head(song_t *head) {

  if (head == NULL) {
    return NULL;
  }
  else if (head->next_song == NULL) {
    printf("remove_head_title: %s\n", head->title);
    free(head->title);
    head->title = NULL;
    free(head->artist);
    head->artist = NULL;
    free(head);
    head = NULL;
    return NULL;
  }
  else {
    printf("remove_head_title1: %s\n", head->title);
    song_t *temp = head;
    head = head->next_song;

    free(temp->title);
    temp->title = NULL;
    free(temp->artist);
    temp->artist = NULL;
    free(temp);
    temp = NULL;
    printf("new_head_title: %s\n", head->title);

    return head;
  }
} /* remove_song_from_head() */


/* song_t *add_song_to_tail(song_t *head, char *title, char *artist)
 * will add song to the tail and return pointer to the
 * head of the new list
 */

song_t *add_song_to_tail(song_t *head, char *title, char *artist) {

  assert(title != NULL);
  assert(artist != NULL);

  if (head == NULL) {
    head = malloc(sizeof(song_t));
    assert(head != NULL);

    head->next_song = NULL;

    head->title = malloc(strlen(title) + 1);
    assert(head->title != NULL);

    head->artist = malloc(strlen(artist) + 1);
    assert(head->artist != NULL);

    strncpy(head->title, title, strlen(title) + 1);
    strncpy(head->artist, artist, strlen(artist) + 1);
  }
  else {
    song_t *element = malloc(sizeof(song_t));
    assert(element != NULL);

    element->title = malloc(strlen(title) + 1);
    assert(element->title != NULL);

    element->artist = malloc(strlen(artist) + 1);
    assert(element->artist != NULL);

    element->next_song = NULL;
    strncpy(element->title, title, strlen(title) + 1);
    strncpy(element->artist, artist, strlen(artist) + 1);

    song_t *tail = head;
    assert(tail != NULL);


    printf("Going to the end of the list\n");
    while (tail->next_song != NULL) {
      tail = tail->next_song;
    }
    printf("adding element to the tail\n");
    tail->next_song = element;

    printf("OK\n");
  }

  return head;

} /* add_song_to_tail() */


/* song_t *remove_song_from_tail(song_t *head)
 * removes the tail of the given song_t and
 * returns the new head of the list
 */

song_t *remove_song_from_tail(song_t *head) {

  if (head == NULL) {
    return NULL;
  }
  if (head->next_song == NULL) {
    free(head->title);
    head->title = NULL;
    free(head->artist);
    head->artist = NULL;
    free(head);
    head = NULL;
    return NULL;
  }

  song_t *temp = head;
  song_t *temp_prev = temp;

  while ((temp->next_song != NULL)) {
    temp_prev = temp;
    temp = temp->next_song;
  }

  temp_prev->next_song = NULL;
  free(temp->title);
  temp->title = NULL;
  free(temp->artist);
  temp->artist = NULL;
  free(temp);

  temp = NULL;

  return head;
} /* remove_song_from_tail() */


/* int count_songs(song_t *head)
 * traverses the list and return the number of songs
 * stored inside the list
 */

int count_songs(song_t *head) {

  if (head == NULL) {
    return 0;
  }
  int count_song = 1;

  while ((head != NULL) && (head->next_song != NULL)) {
    count_song++;
    head = head->next_song;
  }
  return count_song;
} /* count_songs() */


/* song_t *search_by_index(song_t *head, int index)
 * searches through the list pointed by head of song_t
 * of given index and returns the corresponding song_t
 */

song_t *search_by_index(song_t *head, int index) {

  if (index > count_songs(head)) {
    return NULL;
  }
  assert(index > 0);

  int loop_count = 1;
  while (head->next_song != NULL) {
    if (loop_count == index) {
      break;
    }
    head = head->next_song;
    loop_count++;
  }
  return head;
} /* search_by_index() */


/* song_t *search_by_title_and_artist(song_t *head,
 * char *given_title, char *given_artist) searches song_t
 * that matches with given_title, given_artist from the given list
 * and returns the song_t if found. If not, return NULL
 */

song_t *search_by_title_and_artist(song_t *head, char *given_title,
                                   char *given_artist) {

  assert(given_title != NULL);
  assert(given_artist != NULL);

  int song_found = 0;

  if (head->next_song == NULL) {
    if ((strncmp(head->title, given_title, strlen(given_title) + 1) == 0) &&
        (strncmp(head->artist, given_artist, strlen(given_artist) + 1) == 0)) {
      return head;
    }
    else {
      return NULL;
    }
  }

  while (head->next_song != NULL) {
    if ((strncmp(head->title, given_title, strlen(given_title) + 1) == 0) &&
        (strncmp(head->artist, given_artist, strlen(given_artist) + 1) == 0)) {
      song_found++;
      break;
    }
    head = head->next_song;
  }

  if (song_found == 0) {
    return NULL;
  }

  return head;
} /* search_by_title_and_artist() */


/* song_t *modify_song_by_index(song_t *head,
 * int index, char *new_title, char *new_artist) will change
 * corresponding song_t of given index in the list with
 * new title and new artist name and return song_t
 */

song_t *modify_song_by_index(song_t *head, int given_index,
                             char *new_title, char *new_artist) {

  assert(given_index > 0);
  assert(new_title != NULL);
  assert(new_artist != NULL);

  if (head == NULL) {
    return NULL;
  }

  song_t *cnt_s = head;
  int count_song = 0;
  while (cnt_s != NULL) {
    count_song++;
    cnt_s = cnt_s->next_song;
  }

  if (given_index > count_song) {
    return NULL;
  }

  printf("sizeof_list: %d\n", count_song);
  printf("index: %d\n", given_index);
  int list_index = 1;

  song_t *temp = head;

  while (temp->next_song != NULL) {
    if (list_index == given_index) {
      break;
    }
    temp = temp->next_song;
    list_index++;
  }

  free(temp->title);
  temp->title = NULL;
  free(temp->artist);
  temp->artist = NULL;

  temp->title = malloc(strlen(new_title) + 1);
  assert(temp->title != NULL);
  temp->artist = malloc(strlen(new_artist) + 1);
  assert(temp->artist != NULL);

  strncpy(temp->title, new_title, strlen(new_title) + 1);
  strncpy(temp->artist, new_artist, strlen(new_artist) + 1);

  return temp;
} /* modify_song_by_index() */


/* song_t *move_to_tail(song_t *head, int n)
 * will move the first n song_ts of the list to the tail
 * and will return a pointer to the head of the final list
 */

song_t *move_to_tail(song_t *head, int n) {

  assert(n >= 0);
  assert(n <= count_songs(head));


  if (head == NULL) {
    return NULL;
  }
  if (head->next_song == NULL) {
    return head;
  }
  if (n == 0) {
    return head;
  }
  if (n == count_songs(head)) {
    return head;
  }

  song_t *temp = head;
  song_t *head_after_remove = head;
  song_t *head_after_add = head;

  for (int i = 0; i < n; i++) {

    printf("Starting %d index move_to_tail\n", i);

    temp = head;
    song_t *element = malloc(sizeof(song_t));
    assert(element != NULL);
    element->title = malloc(strlen(temp->title) + 1);
    assert(element->title != NULL);
    element->artist = malloc(strlen(temp->artist) + 1);
    assert(element->artist != NULL);
    element->next_song = NULL;

    strncpy(element->title, temp->title, strlen(temp->title) + 1);
    strncpy(element->artist, temp->artist, strlen(temp->artist) + 1);

    printf("title: %s\n", element->title);
    printf("artist: %s\n", element->artist);

    head_after_remove = remove_song_from_head(head);
    printf("new_head_after_remove: %s\n", head_after_remove->title);
    printf("new_head_after_remove: %s\n", head_after_remove->artist);


    head_after_add = add_song_to_tail(head_after_remove,
                                      element->title, element->artist);
    head = head_after_add;

    free(element->title);
    free(element->artist);
    free(element);
    element->title = NULL;
    element->artist = NULL;
    element = NULL;
    printf("Successful %d index move_to_tail\n", i);
  }

  return head_after_add;
} /* move_to_tail() */


/* song_t *interleave_songs(song_t *song_one, song_t *song_two)
 * decides which list is larger or smaller and interleave based on the
 * larger one and returns head of the new list.
 */

song_t *interleave_songs(song_t *song_one, song_t *song_two) {

  assert(song_one != NULL);
  assert(song_two != NULL);
  printf("song_one: %d", count_songs(song_one));
  printf("song_two: %d\n", count_songs(song_two));

  if (count_songs(song_one) == count_songs(song_two)) {
    song_t *head_new_list = song_one;

    song_t *temp = song_one;
    song_t *temp_two = song_two;

    while (song_two->next_song != NULL) {
      temp = song_one->next_song;
      song_one->next_song = song_two;

      temp_two = song_two->next_song;
      song_two->next_song = temp;

      song_one = temp;
      song_two = temp_two;

    }
    song_one->next_song = song_two;
    return head_new_list;
  }
  if (count_songs(song_one) > count_songs(song_two)) {
    song_t *head_new_list = song_one;

    song_t *temp = song_one;
    song_t *temp_two = song_two;

    while (song_two->next_song != NULL) {
      temp = song_one->next_song;
      song_one->next_song = song_two;

      temp_two = song_two->next_song;
      song_two->next_song = temp;

      song_one = temp;
      song_two = temp_two;
    }
    temp = song_one->next_song;
    song_one->next_song = song_two;
    song_one = temp;
    song_two->next_song = temp;

    return head_new_list;
  }
  else {
    song_t *head_new_list = song_two;

    song_t *temp = song_one;
    song_t *temp_two = song_two;

    while (song_one->next_song != NULL) {
      temp_two = song_two->next_song;
      song_two->next_song = song_one;

      temp = song_one->next_song;
      song_one->next_song = temp_two;

      song_one = temp;
      song_two = temp_two;
    }
    temp_two = song_two->next_song;
    song_two->next_song = song_one;
    song_two = temp_two;
    song_one->next_song = temp_two;

    return head_new_list;
  }
  return NULL;
} /* interleave_songs() */


/* void free_song_list(song_t *head)
 * traverses the list and free all of the song_t
 */

void free_song_list(song_t *head) {

  song_t *temp = head;
  while (temp->next_song != NULL) {
    temp = head->next_song;
    free(head->artist);
    head->artist = NULL;
    free(head->title);
    head->title = NULL;
    free(head);
    head = NULL;
    head = temp;
  }

  free(temp->artist);
  temp->artist = NULL;
  free(temp->title);
  temp->title = NULL;
  free(temp);
  temp = NULL;

} /* free_song_list() */
