#include "hw3.h"

#include <stdio.h>
#include <string.h>


/* Global variables */
int g_cars_count = 0;
char g_vin[MAX_RECORDS][VIN_LEN] = {0};
char g_basic_info[MAX_RECORDS][NUM_BASIC_COLS][MAX_INFO_LEN] = {0};
float g_rating[MAX_RECORDS][NUM_RATING_COLS] = {0};
int g_price[MAX_RECORDS] = {0};



/* int read_tables(char *in_file) will read in the file and fill
 * in four tables(g_vin, g_basic_info, g_rating, g_price arrays)
 * Returns the total number of cars(records) read
 */

int read_tables(char *in_file) {

  /* local variables */
  FILE *fp = NULL;
  int ret = 0;

  /* fopen in_file + file read error */
  fp = fopen(in_file, "r");
  if (fp == NULL) {
    return FILE_READ_ERROR;
  }

  char vin[VIN_LEN] = "\0";
  char make[MAX_INFO_LEN] = "\0";
  char model[MAX_INFO_LEN] = "\0";
  char color[MAX_INFO_LEN] = "\0";
  char dealer_name[MAX_INFO_LEN] = "\0";
  float performance = 0.0;
  float comfort = 0.0;
  float reliability = 0.0;
  int price = 0;
  // sharing index across four tables (in place of MAX_RECORDS)
  int i = 0;

  /* fscanf in_file + no data point, record error, out of bounds error*/
  while ( 1 ) {

    if (g_cars_count > MAX_RECORDS) {
      fclose(fp);
      fp = NULL;
      return OUT_OF_BOUNDS;
    }

    ret = fscanf(fp, "%[^;];%[^;];%[^;];%[^;];%[^;];%f;%f;%f;%d\n", vin, make,
                 model, color, dealer_name, &performance,
                 &comfort, &reliability, &price);

    printf("vin: %s\n", vin);
    printf("g_cars_count: %d\n", g_cars_count);
    printf("ret: %d\n", ret);

    //1. if fscanf reached the EOF
    if ((ret == -1) || (ret == 0)) {
      // if there is any record
      if ((g_cars_count != 0) && (g_cars_count < MAX_RECORDS)) {
        fclose(fp);
        fp = NULL;
        return g_cars_count;
      }
      else if (g_cars_count > MAX_RECORDS) {
        fclose(fp);
        fp = NULL;
        return OUT_OF_BOUNDS;
      }
      else {
        // if there is no record recorded
        fclose(fp);
        fp = NULL;
        return NO_DATA_POINTS;
      }
    }
    //2. if fscanf did not reach the EOF yet
    else {
      if (ret < 9) {
        fclose(fp);
        fp = NULL;
        return RECORD_ERROR;
      }
      else {
        /* check out of bound*/
        if (g_cars_count >= MAX_RECORDS) {
          fclose(fp);
          fp = NULL;
          return OUT_OF_BOUNDS;
        }
        /* check vip format */
        for (int j = 0; j < VIN_LEN; j++) {
          if ((('a' < vin[j]) && (vin[j] < 'z')) || (vin[j] == 'I') ||
              (vin[j] == 'O') || (vin[j] == 'Q')) {
            fclose(fp);
            fp = NULL;
            return RECORD_ERROR;
          }
        }
        /* check buffer overflow */
        if (strlen(vin) > VIN_LEN) {
          fclose(fp);
          fp = NULL;
          return RECORD_ERROR;
        }
        else if (strlen(make) > MAX_INFO_LEN) {
          fclose(fp);
          fp = NULL;
          return RECORD_ERROR;
        }
        else if (strlen(model) > MAX_INFO_LEN) {
          fclose(fp);
          fp = NULL;
          return RECORD_ERROR;
        }
        else if (strlen(color) > MAX_INFO_LEN) {
          fclose(fp);
          fp = NULL;
          return RECORD_ERROR;
        }
        else if (strlen(dealer_name) > MAX_INFO_LEN) {
          fclose(fp);
          fp = NULL;
          return RECORD_ERROR;
        }

        g_cars_count++;

        // fill in the four tables
        strcpy(g_vin[i], vin);
        strcpy(g_basic_info[i][0], make);
        strcpy(g_basic_info[i][1], model);
        strcpy(g_basic_info[i][2], color);
        strcpy(g_basic_info[i][3], dealer_name);
        g_rating[i][0] = performance;
        g_rating[i][1] = comfort;
        g_rating[i][2] = reliability;
        g_price[i] = price;

        i++;
      }
    }
  }
} /* read_tables() */


/* int count_cars_by_make(char *given_name) takes make as an argument
 * and outputs total number of cars for the given make
 */

int count_cars_by_make(char *given_name) {

  /* return NO_DATA_POINTS if g_cars_count = 0 */
  if (g_cars_count == 0) {
    return NO_DATA_POINTS;
  }

  int total_num_given_make = 0;

  /* return NO_SUCH_DATA if there is no such a car for the given make */
  for (int i = 0; i < g_cars_count; i++) {
    if (strcmp(given_name, g_basic_info[i][0]) == 0) {
      total_num_given_make++;
    }
  }

  if (total_num_given_make == 0) {
    return NO_SUCH_DATA;
  }

  return total_num_given_make;
} /* count_cars_by_make() */


/* int find_cheapest_car(void)
 * return the index of the car with the lowest price
 */

int find_cheapest_car(void) {

  /* return NO_DATA_POINTS if g_cars_count = 0 */
  if (g_cars_count == 0) {
    return NO_DATA_POINTS;
  }

  /* in case of tie, return the first
   * appearing cheapest car in the input order */
  int min_price = 0;
  int min_index = 0;

  min_price = g_price[0];
  printf("min: %d\n", min_price);

  for (int i = 1; i < g_cars_count; i++) {
    if (g_price[i] < min_price) {
      min_price = g_price[i];
      min_index = i;
      printf("min: %d\n", min_index);
    }
  }

  return min_index;
} /* find_cheapest_car() */


/* int find_cheapest_car_by_color(char *given_color) return
 * the index of the car with the lowest price with the given color
 */

int find_cheapest_car_by_color(char * given_color) {

  /* return NO_DATA_POINTS if g_cars_count = 0 */
  if (g_cars_count == 0) {
    return NO_DATA_POINTS;
  }

  int num_given_color = 0;
  int given_c[MAX_RECORDS][2] = {0};

  for (int i = 0; i < g_cars_count; i++) {

    if (strcmp(g_basic_info[i][2], given_color) == 0) {
      num_given_color++;

      printf("price: %d\n", g_price[i]);
      printf("index: %d\n", i);
      given_c[i][0] = i;
      given_c[i][1] = g_price[i];
    }
  }

  /* check for NO_SUCH_DATA */
  if (num_given_color == 0) {
    return NO_SUCH_DATA;
  }

  int temp = 0;
  int temp2 = 0;

  for (int i = 0; i < MAX_RECORDS; i++) {
    printf("bs_index: %d, bs_price: %d\n", given_c[i][0], given_c[i][1]);
  }
  /* find the lowest price among given_c[] */
  // 1. sort in ascending order
  for (int i = 0; i < MAX_RECORDS; i++) {
    for (int j = 1; j < MAX_RECORDS - 1; j++) {
      if (given_c[j][1] > given_c[j + 1][1]) {
        temp = given_c[j + 1][0];
        temp2 = given_c[j + 1][1];
        given_c[j + 1][0] = given_c[j][0];
        given_c[j + 1][1] = given_c[j][1];
        given_c[j][0] = temp;
        given_c[j][1] = temp2;
      }
    }
  }
  for (int i = 0; i < MAX_RECORDS; i++) {
    printf("as_index: %d, as_price: %d\n", given_c[i][0], given_c[i][1]);
  }

  // 2. loop until it find something that is not zero
  int i = 0;
  while (1) {
    if (given_c[i][0] == 0) {
      i++;
    }
    else if (given_c[i][0] != 0) {
      return given_c[i][0];
    }
  }

} /* find_cheapest_car_by_color() */


/* int count_cars_by_dealer(char *given_dealer, int min_price, int max_price)
 * return the number of cars which fall into [min_price, max_price]
 * for the given dealer name
 */

int count_cars_by_dealer(char *given_dealer, int min_price, int max_price) {

  if ((min_price < 0) || (max_price < 0) || (max_price < min_price)) {
    return INVALID_PRICE;
  }

  /* return NO_DATA_POINTS if g_cars_count is 0 */
  if (g_cars_count == 0) {
    return NO_DATA_POINTS;
  }

  int num_cars_by_dealer = 0;

  for (int i = 0; i < g_cars_count; i++) {

    if (strcmp(g_basic_info[i][3], given_dealer) == 0) {
      if ((min_price < g_price[i]) && (g_price[i] < max_price)) {
        num_cars_by_dealer++;
      }
      printf("price: %d\n", g_price[i]);
    }
  }

  if (num_cars_by_dealer == 0) {
    return NO_SUCH_DATA;
  }
  return num_cars_by_dealer;
} /* count_cars_by_dealer() */


/* float calculate_avg_rating(char *given_vin) outputs
 * the average rating for the given vin
 */

float calculate_avg_rating(char *given_vin) {

  /* if g_cars_count is 0, return NO_DATA_POINTS */
  if (g_cars_count == 0) {
    return NO_DATA_POINTS;
  }

  /* return INVALID_VIN if given_vin is invalid */
  for (int j = 0; j < VIN_LEN; j++) {
    if ((('a' < given_vin[j]) && (given_vin[j] < 'z')) ||
        (given_vin[j] == 'I') || (given_vin[j] == 'O') ||
        (given_vin[j] == 'Q')) {
      return INVALID_VIN;
    }
  }

  int num_vin = 0;
  float average = 0.0;

  /* average rating = average of ratings in performance,
   * comfort and reliability for each car */
  for (int i = 0; i < g_cars_count; i++) {
    if (strcmp(g_vin[i], given_vin) == 0) {
      num_vin++;
      printf("vin: %s\n", g_vin[i]);

      average = (g_rating[i][0] + g_rating[i][1] + g_rating[i][2]) / 3;
    }
  }

  if (num_vin == 0) {
    return NO_SUCH_DATA;
  }

  return average;
} /* calculate_avg_rating() */


/* int show_best_rating_for_each_car(char* out_file)
 * write the best one among all the rating aspects for each car to out_file.
 * If two or more aspects have the same highest rating,
 * display "More than one"
 */

int show_best_rating_for_each_car(char* out_file) {

  FILE *fileptr = NULL;

  /* file open + error */
  fileptr = fopen(out_file, "w");
  if (fileptr == NULL) {
    return FILE_WRITE_ERROR;
  }

  if (g_cars_count == 0) {
    fclose(fileptr);
    fileptr = NULL;
    return NO_DATA_POINTS;
  }

  float max_rating = 0.0;
  float max_r[MAX_RECORDS] = {0.0};

  /* find maximum rating */
  // for each car
  for (int i = 0; i < g_cars_count; i++) {
    // find the max rating
    max_rating = g_rating[i][0];
    for (int j = 1; j < NUM_RATING_COLS; j++) {

      if (g_rating[i][j] > max_rating) {
        max_rating = g_rating[i][j];
        max_r[i] = j;
      }
      else if (g_rating[i][j] == max_rating) {
        max_r[i] = 3;
      }
    }
  }

  char *performance = "Performance";
  char *comfort = "Comfort";
  char *reliability = "Reliability";
  char *more_than_one = "More than one";

  /* print to out_file */
  for (int i = 0; i < g_cars_count; i++) {
    if (max_r[i] == 3) {
      fprintf(fileptr, "%s: %s\n", g_vin[i], more_than_one);
      printf("%s: %s\n", g_vin[i], more_than_one);
    }
    else if (max_r[i] == 0) {
      fprintf(fileptr, "%s: %s\n", g_vin[i], performance);
      printf("%s: %s\n", g_vin[i], performance);
    }
    else if (max_r[i] == 1) {
      fprintf(fileptr, "%s: %s\n", g_vin[i], comfort);
      printf("%s: %s\n", g_vin[i], comfort);
    }
    else if (max_r[i] == 2) {
      fprintf(fileptr, "%s: %s\n", g_vin[i], reliability);
      printf("%s: %s\n", g_vin[i], reliability);
    }
  }

  /* close file */
  fclose(fileptr);
  fileptr = NULL;

  /* return OK if no errors occur and output to the specified file */
  return OK;
} /* show_best_rating_for_each_car() */


/* int write_tables() two integers representing
 * the start and end cols from Table 2
 */

int write_tables(char *out_file, int start_col, int end_col) {

  FILE *fileptr = NULL;

  /* file open + error */
  fileptr = fopen(out_file, "w");
  if (fileptr == NULL) {
    //fclose(fileptr);
    //fileptr = NULL;
    return FILE_WRITE_ERROR;
  }

  if (g_cars_count == 0) {
    fclose(fileptr);
    fileptr = NULL;
    return NO_DATA_POINTS;
  }

  if ((start_col < 0) || (end_col < 0) || (start_col > end_col)) {
    fclose(fileptr);
    fileptr = NULL;
    return INVALID_COLUMN;
  }

  /* write file */
  for (int i = 0; i < g_cars_count; i++) {
    for (int j = start_col; j < end_col; j++) {
      fprintf(fileptr, "%s,", g_basic_info[i][j]);
      printf("%s,", g_basic_info[i][j]);
    }
    fprintf(fileptr, "%s\n", g_basic_info[i][end_col]);
    printf("%s\n", g_basic_info[i][end_col]);
  }
  /* close file */
  fclose(fileptr);
  fileptr = NULL;

  /* return OK if no errors occur */
  return OK;
} /* write_tables() */

