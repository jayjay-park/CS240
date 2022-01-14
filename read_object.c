#include "graphics.h"
#include "object.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


/* object *read_object(const char*filename) dynamically allocates the object
 */

object *read_object(const char *filename) {
  printf("beginning print object\n");
  if (filename == NULL) {
    return NULL;
  }

  // ----- open the file ----- //
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("error in opening file\n");
    return NULL;
  }

  // ----- read the name ----- (caused stack smashing) //
  char name_of_file[strlen(filename) + 1];
  int name = fscanf(file, "%s\n", name_of_file);
  if (name != 1) {
    printf("wrong name\n");
    fclose(file);
    file = NULL;
    return NULL;
  }
  printf("%s\n", name_of_file);

  // ----- allocate an object ----- //
  object *new_object = malloc(sizeof(object));
  assert(new_object != NULL);

  // ----- read the number of points and polygons ----- //
  int num_of_points = 0;
  int num_of_polygons = 0;

  int size = fscanf(file, "%d %d\n", &num_of_points, &num_of_polygons);
  if (size != 2) {
    printf("wrong size\n");
    fclose(file);
    file = NULL;
    return NULL;
  }
  printf("num_of_points: %d\n", num_of_points);
  printf("num_of_polygons: %d\n", num_of_polygons);
  new_object->num_polygons = num_of_polygons;
  new_object->num_points = num_of_points;

  // ----- allocate an array of points within the object ----- //
  new_object->points = malloc(sizeof(point) * num_of_points);
  assert(new_object->points != NULL);
  printf("Successful creation of points array\n");

  // ----- read each point into the array of points (caused stack smashing) ----- //
  int point_check = 0;
  int index = 0;
  int row_num = 0;
  int store_row_num = 3;
  double x = 0;
  double y = 0;
  double z = 0;

  while (index < num_of_points) {
    point_check = fscanf(file, "%d %lf %lf %lf\n", &row_num, &x, &y, &z);

    if (point_check != 4) {
      fclose(file);
      file = NULL;
      printf("error in points on row %d\n", row_num);
      return NULL;
    }
    // first round
    if (index == 0) {
      store_row_num = row_num;
    }
    new_object->points[index].arr[0] = x;
    new_object->points[index].arr[1] = y;
    new_object->points[index].arr[2] = z;

    printf("index:%d row_num: %d %lf %lf %lf\n", index, row_num,
                    new_object->points[index].arr[0],
                    new_object->points[index].arr[1],
                    new_object->points[index].arr[2]);

    index++;
  }


  if (index != num_of_points) {
    printf("less num of points\n");
    fclose(file);
    file = NULL;
    return NULL;
  }

  // allocate an array of polygons
  new_object->polygons = malloc(sizeof(polygon) * num_of_polygons);
  assert(new_object->polygons != NULL);
  printf("Successful creation of polygon array\n");
  
  /* ---- read each polygon into the array of polygons (caused stack samshing) ---- */
  char name_of_color[20] = "";
  char color[20] = "";
  int size_polygon = 0;

  for (int i = 0; i < num_of_polygons; i++) {

    printf("for polygon %d\n", i);

    // 1) read the name and throw it away
    int read = fscanf(file, "%s %s %d ", name_of_color, color, &size_polygon);
    if (read != 3) {
      printf("error in name\n");
      fclose(file);
      file = NULL;
      return NULL;
    }
    new_object->polygons[i].num_points = size_polygon;

    // 2) read the color and convert it to an R, G, B value by calling name_to_rgb()
    name_to_rgb(color, &(new_object->polygons[i].r),
                &(new_object->polygons[i].g), &(new_object->polygons[i].b));

    // 3) read the number of point -- caused stack overflows
    new_object->polygons[i].point_arr = malloc(sizeof(point *) * size_polygon);
    assert(new_object->polygons[i].point_arr != NULL);

    int point_number = 0;
    int index_p = 0;

    while (index_p < size_polygon){
      int check_number = fscanf(file, "%d", &point_number);
      if (check_number != 1) {
        printf("error in polygon points\n");
        fclose(file);
        file = NULL;
        return NULL;
      }
      printf("at index_p: %d, point_number: %d\n", index_p, point_number);
      // 4) read each of the points and make each entry of the array of pointers
      //    in the polygon structure point to a point in the object structure

      if (store_row_num == 0) {
        new_object->polygons[i].point_arr[index_p] = &(new_object->points[point_number]);
        printf("pointer array element points to points[%d]\n", point_number);

      }
      else {
        new_object->polygons[i].point_arr[index_p] = &(new_object->points[point_number - 1]);
        printf("pointer array element points to points[%d]\n", point_number - 1);
      }
      index_p++;
    }
  }

  fclose(file);
  file = NULL;
  return new_object;
} /* read_object() */


/*
 * void free_object(object *given_object) deallocate an object
 * and everything that it points to
 */
void free_object(object *given_object) {

  assert(given_object != NULL);

  int num_polygons = given_object->num_polygons;
  for (int i = 0; i < num_polygons; i++) {
    polygon *temp = &(given_object->polygons[i]);
    free(temp->point_arr);
    temp->point_arr = NULL;
  }

  // free polygon -- caused bad exits
  free(given_object->polygons);
  given_object->polygons = NULL;

  // free points
  free(given_object->points);
  given_object->points = NULL;

  // free object
  free(given_object);
  given_object = NULL;


} /* free_object() */
