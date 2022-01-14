#include "hw5.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>


/* int fp_check(FILE *fp) checks
 * if the file pointer is NULL and
 * is used for inner function of assert()
 */

int fp_check(FILE *fp) {
  if (fp != NULL) {
    return 1;
  }
  return 0;
} /* fp_check() */

/* int record_num_check(int value) checks
 * if the int value is within the range
 */

int record_num_check(int value) {
  if (value >= 0) {
    return 1;
  }
  return 0;
} /* record_num_check() */


/* int doctor_t_check(doctor_t record) checks
 * if the data in doctor_t is formatted right
 */

int doctor_t_check(doctor_t record) {
  int i = 0;

  while (1) {
    if (record.first_name[i] == '\0') {
      break;
    }
    else {
      if (i >= MAX_NAME_LEN) {
        return 0;
      }
      i++;
    }
  }
  while (1) {
    if (record.last_name[i] == '\0') {
      break;
    }
    else {
      if (i >= MAX_NAME_LEN) {
        return 0;
      }
      i++;
    }
  }
  if (record.id_number <= 0) {
    return 0;
  }
  if (((record.gender == FEMALE) || (record.gender == MALE) ||
       (record.gender == OTHER)) &&
      ((record.doctor_type >= CARDIOLOGIST) &&
       (record.doctor_type <= RADIOLOGIST))) {
  }
  else {
    return 0;
  }
  if (record.consultation_fee <= 0) {
    return 0;
  }
  for (int day = 0; day < N_DAYS; day++) {
    for (int hour = 0; hour < N_HOURS; hour++) {
      if ((record.calendar[day][hour] != 'A') &&
          (record.calendar[day][hour] != 'B')) {
        return 0;
      }
    }
  }
  return 1;
} /* doctor_t_check() */

/* int enum_check(enum gender_t gender_type)
 * checks if the enum value is proper
 */

int enum_check(enum gender_t gender_type) {
  if (((gender_type == FEMALE) || (gender_type == MALE) ||
     (gender_type == OTHER))) {
    return 1;
  }
  else {
    return 0;
  }
} /* enum_check() */


/* int enum_doc_check(enum doctor_type_t doctor)
 * checks if doctor_type_t is formatted prperly
 */

int enum_doc_check(enum doctor_type_t doctor) {

  if ((doctor < 0) || (doctor > 3)) {
    return 0;
  }
  else {
    return 1;
  }
} /* enum_doc_check() */


/* int id_check(int id) checks
 * if the given id is bigger than 0
 */

int id_check(int id) {
  if (id > 0) {
    return 1;
  }
  else {
    return 0;
  }
} /* id_check() */

/* int name_length_check(char *name)
 * checks if the length of name is less than
 * MAX_NAME_LEN
 */

int name_length_check(char *name) {
  if (strlen(name) > MAX_NAME_LEN) {
    return 0;
  }
  else {
    return 1;
  }
} /* name_length_check() */


/* int name_null_check(char *name) checks
 * if the name is NULL
 */

int name_null_check(char *name) {
  if (name == NULL) {
    return 0;
  }
  else {
    return 1;
  }
} /* name_null_check() */

/* doctor_t read_doctor(FILE *, int) read the doctor record
 * at record_num and return doctor_t
 */

doctor_t read_doctor(FILE *fp, int record_num) {

  assert(fp_check(fp));
  assert(record_num_check(record_num));

  int ret_fseek = 0;
  ret_fseek = fseek(fp, record_num * sizeof(doctor_t), SEEK_SET);
  if (ret_fseek == -1) {
    printf("unsuccessful fseek");
  }

  doctor_t record = BAD_DOCTOR;
  int ret_value = 0;

  ret_value = fread(&record, sizeof(record), 1, fp);


  if (ret_value < 1) {
    printf("fread() failed. ret_value: %d\n", ret_value);
    return BAD_DOCTOR;
  }
  else if (ret_value == 1) {
    if (strlen(record.first_name) > 20) {
      return BAD_DOCTOR;
    }
    else if (strlen(record.last_name) > 20) {
      return BAD_DOCTOR;
    }
    else if ((record.gender != FEMALE) &&
             (record.gender != MALE) &&
             (record.gender != OTHER)) {
      return BAD_DOCTOR;
    }
    else if ((record.doctor_type != CARDIOLOGIST) &&
             (record.doctor_type != AUDIOLOGIST) &&
             (record.doctor_type != DENTIST) &&
             (record.doctor_type != RADIOLOGIST)) {
      return BAD_DOCTOR;
    }
    else {
      return record;
    }
  }

  return record;
} /* read_doctor() */

/* write_doctor(FILE * fp, doctor_t record, int record_num)
 * will write given doctor information to the specified file
 * at the record_num position
 */

int write_doctor(FILE * fp, doctor_t record, int record_num) {

  assert(fp_check(fp));
  assert(record_num_check(record_num));
  assert(doctor_t_check(record));

  fseek(fp, 0, SEEK_END);

  int count = 0;
  count = ftell(fp) / sizeof(doctor_t);

  if (record_num >= count) {
    printf("offset is too large\n");
    return WRITE_ERR;
  }

  int ret_fseek = 0;

  ret_fseek = fseek(fp, record_num * sizeof(doctor_t), SEEK_SET);
  if (ret_fseek == -1) {
    printf("unsuccessful fseek");
  }

  /* fwrite() */
  int ret_value = 0;

  ret_value = fwrite(&record, sizeof(record), 1, fp);

  if (ret_value != 1) {
    printf("fwrite() failed. ret_value: %d\n", ret_value);
    return WRITE_ERR;
  }

  return OK;
} /* write_doctor() */

/* percent_doctor_by_gender(FILE * fp, enum gender_t gender_type)
 * calculate the percentage of doctors
 * with the given gender
 */

float percent_doctor_by_gender(FILE* fp, enum gender_t gender_type) {

  assert(fp_check(fp));
  assert(enum_check(gender_type));

  fseek(fp, 0, SEEK_END);
  float count = 0.0;
  count = ftell(fp) / sizeof(doctor_t);
  printf("count: %f\n", count);
  printf("gender: %d\n", gender_type);

  doctor_t record = BAD_DOCTOR;
  float num_doctor = 0.0;
  int i = 0;
  float percent = 0.0;

  while (1) {
    if (i < count) {
      record = read_doctor(fp, i);
      printf("record_id: %d\n", record.id_number);

      int record_gender_number = 0;
      int given_gender_number = 0;
      record_gender_number = record.gender;
      given_gender_number = gender_type;

      if (record_gender_number == given_gender_number) {
        num_doctor++;
      }
      i++;
    }
    else {
      printf("num_doctor: %f\n", num_doctor);
      percent = num_doctor / count;
      printf("percent: %f\n", percent);
      percent = percent * 100;
      return percent;
    }
  }
} /* percent_doctor_by_gender() */


/* float average_consultation_fee_by_type(FILE * fp, enum doctor_type_t doctor)
 * returns the average consulatation fee among doctors
 * whose doctor type match with the doctor_type_t
 */

float average_consultation_fee_by_type(FILE* fp, enum doctor_type_t doctor) {

  assert(fp_check(fp));
  assert(enum_doc_check(doctor));

  fseek(fp, 0, SEEK_END);
  float count = 0.0;
  count = ftell(fp) / sizeof(doctor_t);
  printf("count: %f\n", count);

  doctor_t record = BAD_DOCTOR;
  float num_doctor = 0.0;
  float fee = 0.0;
  float percent = 0.0;

  int record_type = 4;
  int given_type = 4;

  for (int i = 0; i < count; i++) {
    record = read_doctor(fp, i);
    record_type = record.doctor_type;
    given_type = doctor;

    if (record_type == given_type) {
      num_doctor++;
      fee = fee + record.consultation_fee;
    }
  }

  if (num_doctor == 0.0) {
    return NO_DOCTOR;
  }

  printf("num_doctor: %f\n", num_doctor);
  printf("fee: %f\n", fee);
  percent = fee / num_doctor;
  printf("percent: %f\n", percent);
  return percent;

} /* average_consultation_fee_by_type() */


/* find_doctor_by_id(FILE *fp, int id)
 * returns the doctor_t that matches with the second argument
 */

doctor_t find_doctor_by_id(FILE *fp, int id) {

  assert(fp_check(fp));
  assert(id_check(id));

  fseek(fp, 0, SEEK_END);
  float count = 0.0;
  count = ftell(fp) / sizeof(doctor_t);
  printf("count: %f\n", count);

  doctor_t record = BAD_DOCTOR;
  int doct_exist = 0;

  for (int i = 0; i < count; i++) {
    record = read_doctor(fp, i);
    if (record.id_number == id) {
      doct_exist++;
      return record;
    }
  }

  if (doct_exist == 0) {
    return BAD_DOCTOR;
  }
  return record;

} /* find_doctor_by_id() */


/* find_doctor_by_name(FILE* fp, char* first_n,
 * char* last_n, enum doctor_type_t type)
 * returns doctor_t that has matching first name,
 * last name and doctor type
 */

doctor_t find_doctor_by_name(FILE* fp, char* first_n,
                             char* last_n, enum doctor_type_t type) {

  assert(fp_check(fp));
  assert(name_null_check(first_n));
  assert(name_null_check(last_n));
  assert(enum_doc_check(type));
  assert(name_length_check(first_n));
  assert(name_length_check(last_n));

  fseek(fp, 0, SEEK_END);
  float count = 0.0;
  count = ftell(fp) / sizeof(doctor_t);
  printf("count: %f\n", count);

  doctor_t record = BAD_DOCTOR;
  int doc_exist = 0;
  int record_type = 4;
  int given_type = 4;

  for (int i = 0; i < count; i++) {
    record = read_doctor(fp, i);

    record_type = record.doctor_type;
    given_type = type;

    printf("id: %d\n", record.id_number);

    if ((strcmp(record.first_name, first_n) == 0) &&
        (strcmp(record.last_name, last_n) == 0) &&
        (record_type == given_type)) {
      doc_exist++;
      return record;
    }
  }

  if (doc_exist == 0) {
    return BAD_DOCTOR;
  }

  return record;
} /* find_doctor_by_name() */


/* update_doctor_consulation_fee(FILE *fp, int id, float new_fee)
 * writes in the file new_fee for the given id
 */

int update_doctor_consultation_fee(FILE* fp, int id, float new_fee) {

  assert(fp_check(fp));
  assert(id_check(id));

  if (new_fee <= 0) {
    return OK;
  }

  fseek(fp, 0, SEEK_END);
  float count = 0.0;
  count = ftell(fp) / sizeof(doctor_t);
  printf("count: %f\n", count);

  doctor_t record = BAD_DOCTOR;
  int id_exist = 0;

  for (int i = 0; i < count; i++) {
    record = read_doctor(fp, i);

    if (id == record.id_number) {
      id_exist++;
      record.consultation_fee = new_fee;
      write_doctor(fp, record, i);
    }
  }

  if (id_exist == 0) {
    return NO_DOCTOR;
  }

  return OK;
} /* update_doctor_consultation_fee() */


/* float calculate_total_consulation_fee(FILE *fp, int id) first
 * search the id and calculate total consulation fee
 * caculated by total number of hours booked during the week
 * times the consultation fee
 */

float calculate_total_consultation_fee(FILE* fp, int id) {

  assert(fp_check(fp));
  assert(id_check(id));

  fseek(fp, 0, SEEK_END);
  float count = 0.0;
  count = ftell(fp) / sizeof(doctor_t);
  printf("count: %f\n", count);

  doctor_t record = BAD_DOCTOR;
  int id_exist = 0;
  float total_h = 0.0;
  float total_fee = 0.0;
  int calendar_value = 0;
  int booked = 66;
  int booked_count = 0;

  for (int i = 0; i < count; i++) {
    record = read_doctor(fp, i);

    if (id == record.id_number) {
      id_exist++;

      for (int j = 0; j < N_DAYS; j++) {
        for (int k = 0; k < N_HOURS; k++) {
          printf("calendar: %d\n", record.calendar[j][k]);
          calendar_value = record.calendar[j][k];
          if ((calendar_value - booked) == 0) {
            total_h++;
            booked_count++;
          }
        }
      }
      total_fee = total_h * record.consultation_fee;
    }
  }

  if (id_exist == 0) {
    return NO_DOCTOR;
  }
  printf("66 count: %d\n", booked_count);

  return total_fee;
} /* calculate_total_consultation_fee() */

/* int make_appointment(FILE *fp, int id, int weekday, int time)
 * checks if the doctor with given id is available
 * in given weekday and time
 */

int make_appointment(FILE* fp, int id, int weekday, int time) {

  assert(fp_check(fp));
  assert(id_check(id));

  if ((weekday < 1) || (weekday > 5)) {
    printf("weekday is wrong!");
    return BAD_APPOINTMENT;
  }

  if ((time < 8) || (time > 16)) {
    printf("time argument is wrong!");
    return BAD_APPOINTMENT;
  }

  fseek(fp, 0, SEEK_END);
  float count = 0.0;
  count = ftell(fp) / sizeof(doctor_t);
  printf("count: %f\n", count);

  doctor_t record = BAD_DOCTOR;
  int id_exist = 0;
  printf("weekday:%d\n", weekday);
  printf("time:%d\n", time);
  int calendar_value = 0;
  int booked = 'B';
  int index_weekday = weekday - 1;
  int index_time = time - 8;

  for (int i = 0; i < count; i++) {
    record = read_doctor(fp, i);
    if (id == record.id_number) {
      id_exist++;

      calendar_value = record.calendar[index_weekday][index_time];
      printf("booked?: day:%d time:%d A/B:%d\n", weekday, time, calendar_value);

      if ((calendar_value - booked) == 0) {
        printf("Already booked!");
        return UNAVAILABLE;
      }
      else {
        record.calendar[index_weekday][index_time] = 'B';
        write_doctor(fp, record, i);
      }
    }
  }

  if (id_exist == 0) {
    return NO_DOCTOR;
  }

  return OK;
} /* make_appointment() */
