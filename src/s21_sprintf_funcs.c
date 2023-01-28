#include "s21_sprintf_funcs.h"

#include <math.h>
#include <stdlib.h>

#include "s21_string.h"

void print_char(char symb, char* dst, int width, int right_padding,
                char pading_symbol) {
  char source[2] = {0};
  source[0] = symb;
  source[1] = 0;
  if (width > 1 && !right_padding) {
    add_padding(width - 1, pading_symbol, dst);
  }
  s21_strcat(dst, source);
  if (width > 1 && right_padding) {
    add_padding(width - 1, pading_symbol, dst);
  }
}

void add_padding(int num, char pading_symbol, char* dst) {
  for (int i = 0; i < num; i++) {
    print_char(pading_symbol, dst, 1, 0, pading_symbol);
  }
}

void print_string(char* string, char* dst, int precision, int width,
                  int right_padding, char pading_symbol) {
  pading_symbol = ' ';
  s21_size_t str_len = 0;
  if (string != s21_NULL) {
    if (precision == -1) {
      str_len = s21_strlen(string);
    } else {
      str_len = s21_strlen(string) < (long long unsigned int)precision
                    ? s21_strlen(string)
                    : (long long unsigned int)precision;
    }
    if ((int)str_len < width && !right_padding) {
      add_padding(width - str_len, pading_symbol, dst);
    }
    if (precision == -1) {
      s21_strcat(dst, string);
    } else {
      s21_strncat(dst, string, precision);
    }
    if ((int)str_len < width && right_padding) {
      add_padding(width - str_len, pading_symbol, dst);
    }
  } else {
    if (precision == -1) {
      precision = 6;
    }
    print_string("(null)", dst, precision >= 6 ? precision : 0, width,
                 right_padding, pading_symbol);
  }
}

void print_int(void* number, int precision, int width, int right_padding,
               char pading_symbol, int plus_sgn, int minus_sgn,
               int space_symbol, char* dst, int type) {
  s21_size_t array_len = number_length(number, type);
  char* array = s21_NULL;
  int printing_len = precision > (int)array_len ? precision : (int)array_len;
  if (precision != 0 || (precision == 0 && !is_zero(number, type))) {
    array = malloc(sizeof(char) * (array_len+2));
  } else {
    printing_len = (plus_sgn || space_symbol);
    if (pading_symbol == '0') {
      print_sign(plus_sgn, 0, space_symbol, 0, dst);
    }
    if (pading_symbol == ' ') {
      print_sign(plus_sgn, 0, space_symbol, 0, dst);
    }
  }
  int number_sgn = get_int_sign(number, type);
  printing_len += (plus_sgn || space_symbol || (minus_sgn && number_sgn == -1));
  if (pading_symbol == '0') {
    print_sign(plus_sgn, minus_sgn, space_symbol, number_sgn, dst);
  }
  if (width > printing_len && !right_padding) {
    add_padding(width - (printing_len), pading_symbol, dst);
  }
  if (array != s21_NULL) {
    int_to_chars(number, array_len, array, type);
    if (pading_symbol == ' ') {
      print_sign(plus_sgn, minus_sgn, space_symbol, number_sgn, dst);
    }
    if (precision > 0 && precision > (int)array_len) {
      int difference = precision - (int)array_len;
      for (; difference > 0; difference--) {
        print_char('0', dst, 1, 0, pading_symbol);
      }
    }
    for (int i = 0; i < (int)array_len; i++) {
      print_char(array[i], dst, 1, 0, pading_symbol);
    }
    free(array);
  }
  if (width > printing_len && right_padding) {
    add_padding(width - (printing_len), ' ', dst);
  }
}

void print_octal(long unsigned int number, int precision, int width,
                 int right_padding, char pading_symbol, int prefix, char* dst) {
  int num_width = precision > width ? precision : width;
  char* tmp = malloc(sizeof(char) * (40 + num_width));
  if (tmp != s21_NULL) {
    tmp[0] = 0;
    if (prefix && number != 0) {
      print_char('0', tmp, 0, 0, pading_symbol);
    }
    print_hexa_oct_num_part(number, 8, 0, 0, pading_symbol, tmp);
    int printed_len = s21_strlen(tmp);
    if (precision > 0 && precision > printed_len) {
      int difference = precision - printed_len;
      for (; difference > 0; difference--) {
        print_char('0', tmp, 1, 0, pading_symbol);
      }
      printed_len = s21_strlen(tmp);
    }
    if (precision == 0 && is_zero(&number, TYPE_LONG_UNSIGNED_INT)) {
      printed_len = 0;
    }
    if (width > printed_len && !right_padding) {
      add_padding(width - (printed_len), pading_symbol, dst);
    }
    if (precision != 0 || number != 0) {
      reverse_string(tmp + prefix);
      print_string(tmp, dst, -1, 0, 0, pading_symbol);
    }
    if (width > printed_len && right_padding) {
      add_padding(width - (printed_len), pading_symbol, dst);
    }
    free(tmp);
  }
}

void reverse_string(char* str) {
  int left = 0;
  int right = s21_strlen(str) - 1;
  while (left < right) {
    char tmp = str[left];
    str[left] = str[right];
    str[right] = tmp;
    left++;
    right--;
  }
}

void print_hexa_oct_num_part(long unsigned int number, int base,
                             int in_upper_case, int pointer, char pading_symbol,
                             char* dst) {
  if (number != 0) {
    for (long unsigned int num = number; num > 0; num = num / base) {
      int digit = num % base;
      print_char(hex_char_from_num(digit, in_upper_case), dst, 0, 0,
                 pading_symbol);
    }
  } else if (!pointer) {
    print_char('0', dst, 0, 0, pading_symbol);
  }
}

void print_hexadecimal(long unsigned int number, int precision, int width,
                       int right_padding, char pading_symbol, int plus,
                       int space, int prefix, int in_upper_case, int pointer,
                       char* dst) {
  int num_width = precision > width ? precision : width;
  char* tmp = malloc(sizeof(char) * (50 + num_width));
  if (tmp != s21_NULL) {
    tmp[0] = 0;
    if (pointer && number == 0) {
      plus = 0;
      space = 0;
      precision = 0;
    }
    if (prefix && number != 0) {
      print_prefix('X', 'x', in_upper_case, 1, tmp);
    }
    print_hexa_oct_num_part(number, 16, in_upper_case, pointer, pading_symbol,
                            tmp);
    int printed_len = s21_strlen(tmp);
    if (pointer && number == 0) {
      print_string("(nil)", tmp, -1, 0, 0, pading_symbol);
      printed_len = s21_strlen("(nil)");
    }
    if (precision > 0 &&
        precision > (printed_len - prefix * 2 * (number == 0 ? 0 : 1))) {
      int difference =
          precision - (printed_len - prefix * 2 * (number == 0 ? 0 : 1));
      for (; difference > 0; difference--) {
        print_char('0', tmp, 1, 0, pading_symbol);
      }
      printed_len = s21_strlen(tmp);
    }
    if (precision == 0 && is_zero(&number, TYPE_LONG_UNSIGNED_INT) &&
        !pointer) {
      printed_len = 0;
    }
    if (width > printed_len && !right_padding) {
      if (pading_symbol == '0') {
        add_padding(width - (printed_len), pading_symbol, tmp);
      } else {
        add_padding(width - (printed_len), pading_symbol, dst);
      }
    }
    if (precision != 0 || number != 0 || pointer) {
      if (number != 0) {
        reverse_string(tmp + prefix * (2 + ((space || plus) && pointer)));
      }
      print_string(tmp, dst, -1, 0, 0, pading_symbol);
    }
    if (width > printed_len && right_padding) {
      add_padding(width - (printed_len), ' ', dst);
    }
    free(tmp);
  }
}

void print_prefix(char symbol_up, char symbol, int in_upper_case,
                  int print_zero, char* dst) {
  if (print_zero) {
    print_char('0', dst, 0, 0, ' ');
  }
  if (in_upper_case) {
    print_char(symbol_up, dst, 0, 0, ' ');
  } else {
    print_char(symbol, dst, 0, 0, ' ');
  }
}

char hex_char_from_num(int num, int capital) {
  char result = 0;
  if (num < 10) {
    result = '0' + num;
  } else {
    if (!capital) {
      result = 'a' + (num - 10);
    } else {
      result = 'A' + (num - 10);
    }
  }
  return result;
}

void int_to_chars(void* input, int number_length, char* array, int type) {
  int i = number_length - 1;
  if (type == TYPE_INT || type == TYPE_SHORT_INT || type == TYPE_LONG_INT) {
    long int number = *(long int*)input;
    do {
      int digit = number % 10;
      array[i--] = '0' + abs(digit);
      number /= 10;
    } while (number != 0);
  } else if (type == TYPE_UNSIGNED_INT || type == TYPE_SHORT_UNSIGNED_INT ||
             type == TYPE_LONG_UNSIGNED_INT) {
    unsigned long int number = *(unsigned long int*)input;
    do {
      int digit = number % 10;
      array[i--] = '0' + digit;
      number /= 10;
    } while (number != 0);
  }
}

s21_size_t number_length(void* input, int type) {
  s21_size_t result = 0;
  if (type == TYPE_INT || type == TYPE_LONG_INT || type == TYPE_SHORT_INT) {
    long int number = *(long int*)input;
    if (number == 0) {
      result = 1;
    }
    for (; number != 0; number /= 10, result++) {
    }
  } else if (type == TYPE_UNSIGNED_INT || type == TYPE_LONG_UNSIGNED_INT ||
             type == TYPE_SHORT_UNSIGNED_INT) {
    unsigned long int number = *(unsigned long int*)input;
    if (number == 0) {
      result = 1;
    }
    for (; number != 0; number /= 10, result++) {
    }
  }
  return result;
}

int get_int_sign(void* number, int type) {
  int result = 1;
  if (type == TYPE_INT || type == TYPE_LONG_INT || type == TYPE_SHORT_INT) {
    int zero = 0;
    if (*(long int*)number < zero) {
      result = -1;
    }
  }
  return result;
}

void print_sign(int plus_sgn, int minus_sgn, int space_symbol, int sgn,
                char* dst) {
  if ((sgn == -1 && minus_sgn)) {
    print_char('-', dst, 1, 0, ' ');
  } else if (plus_sgn) {
    print_char('+', dst, 1, 0, ' ');
  } else if (space_symbol) {
    print_char(' ', dst, 1, 0, ' ');
  }
}

int is_zero(void* number, int type) {
  int result = 0;
  if (type == TYPE_INT || type == TYPE_SHORT_INT || type == TYPE_LONG_INT) {
    long int zero = 0;
    if (*(long int*)number == zero) {
      result = 1;
    }
  }
  if (type == TYPE_UNSIGNED_INT || type == TYPE_SHORT_UNSIGNED_INT ||
      type == TYPE_LONG_UNSIGNED_INT) {
    unsigned long int zero = 0;
    if (*(unsigned long int*)number == zero) {
      result = 1;
    }
  }
  return result;
}

void print_double(long double num, int precision, int width, int right_padding,
                  char pading_symbol, int plus_sgn, int space_symbol,
                  int point_forced, char* dst) {
  if (!(double)check_special_float_nums(num, width, right_padding,
                                        pading_symbol, plus_sgn, space_symbol,
                                        dst)) {
    int number_sgn = 0;
    int whole_len = 0;
    if (num < 0) {
      number_sgn = -1;
      num = -num;
    }
    long int power = 0;
    num = round_double(num, &power, precision);
    long double whole_part = 0;
    long double fract_part = modfl(num, &whole_part);
    if (whole_part > 0) {
      whole_len = log10l(whole_part) + 1;
      if (whole_len < 0) {
      whole_len = 20;}
    } else {
      whole_len = 1;
    }
    char* tmp = malloc(sizeof(char) * (precision + 30 + whole_len));
    int printed_len = precision + whole_len + (precision > 0 || point_forced) +
                      (plus_sgn || space_symbol || number_sgn == -1);
    if (pading_symbol == '0') {
      print_sign(plus_sgn, 1, space_symbol, number_sgn, dst);
    }
    if (width > printed_len && !right_padding) {
      add_padding(width - printed_len, pading_symbol, dst);
    }
    if (tmp != s21_NULL) {
      tmp[0] = 0;
      if (pading_symbol == ' ') {
        print_sign(plus_sgn, 1, space_symbol, number_sgn, tmp);
      }
      if (whole_len > 1 || whole_part != 0) {
        print_whole_float(precision == 0 ? roundl(num) : whole_part, tmp);
      } else {
        print_char('0', tmp, 1, 0, pading_symbol);
      }
      if (precision > 0 || point_forced) {
        print_char('.', tmp, 1, 0, pading_symbol);
      }
      int next_digigt = fmodl(fract_part * powl(10, precision + 1), 10);
      if (next_digigt >= 5) {
        fract_part = fract_part + 0.5 * powl(10, -precision);
      }
      print_fractional_float(fract_part, precision, 0, tmp);
      print_string(tmp, dst, -1, 0, 0, pading_symbol);
      free(tmp);
    }
    if (width > printed_len && right_padding) {
      add_padding(width - printed_len, ' ', dst);
    }
  }
}

int check_special_float_nums(double num, int width, int right_padding,
                             char pading_symbol, int plus_sgn, int space_symbol,
                             char* dst) {
  int result = 0;
  int sign = signbit(num);
  int printed_len = 3 + (plus_sgn || space_symbol || sign != 0);
  if (isnan(num) || isinf(num)) {
    if (width > printed_len && !right_padding) {
      add_padding(width - printed_len, pading_symbol, dst);
    }
  }
  if (isnan(num) != 0) {
    print_inf_nan_sgn(sign, plus_sgn, space_symbol, dst);
    print_string("nan", dst, -1, 0, 0, ' ');
    result = 1;
  } else if (isinf(num)) {
    print_inf_nan_sgn(sign, plus_sgn, space_symbol, dst);
    print_string("inf", dst, -1, 0, 0, ' ');
    result = 1;
  }
  if (result) {
    if (width > printed_len && right_padding) {
      add_padding(width - printed_len, pading_symbol, dst);
    }
  }
  return result;
}

void print_inf_nan_sgn(int sgn, int plus_sgn, int space_symbol, char* dst) {
  if (sgn == 0) {
    if (plus_sgn) {
      print_char('+', dst, 0, 0, ' ');
    } else if (space_symbol) {
      print_char(' ', dst, 0, 0, ' ');
    }
  } else {
    print_char('-', dst, 0, 0, ' ');
  }
}

void print_fractional_float(long double fractional, int precision, int plus_sgn,
                            char* dst) {
  int i = 0;
  for (; i < precision; i++) {
    fractional = fractional * 10;
    long double whole;
    fractional = modfl(fractional, &whole);
    long int digit = (long int)(whole);
    print_int(&digit, 1, 0, 0, ' ', plus_sgn, 0, 0, dst, TYPE_INT);
  }
}

void print_whole_float(long double num, char* dst) {
  long int digit;
  for (int i = log10l(num); i >= 0; i--) {
    long double weight = powl(10.0, i);
    if (weight > 0 && !isinf(weight)) {
      digit = floorl(num / weight);
      num -= (digit * weight);
      print_int(&digit, 1, 0, 0, ' ', 0, 0, 0, dst, TYPE_INT);
    }
  }
}

long double round_double(long double num, long int* power, int precision) {
  int next_digit = fmodl(num * powl(10, precision + 1), 10);
  if (next_digit >= 5) {
    num = num + 0.5 * powl(10, -precision);
    *power = get_power(num);
  }
  return num;
}

void print_double_scientific(long double num, int precision, int width,
                             int right_padding, char pading_symbol,
                             int plus_sgn, int space_symbol, int capital,
                             int point_forced, char* dst) {
  if (!(double)check_special_float_nums(num, width, right_padding,
                                        pading_symbol, plus_sgn, space_symbol,
                                        dst)) {
    int number_sgn = 0;
    char* start_of_num = &dst[s21_strlen(dst)];
    if (num < 0) {
      number_sgn = -1;
      num = -num;
    }

    long int power = get_power(num);
    round_double(num, &power, precision - power);
    int power_len = (int)number_length(&power, TYPE_LONG_INT);
    if (power_len < 2) {
      power_len = 2;
    }
    int num_len = 3 + power_len + (precision > 0 || point_forced) + precision +
                  (plus_sgn || space_symbol || number_sgn == -1);
    if (width > num_len && !right_padding) {
      add_padding(width - num_len, pading_symbol, dst);
    }
    print_sign(plus_sgn, 1, space_symbol, number_sgn, dst);
    print_double(num * powl(10, -power), precision, 0, 0, pading_symbol, 0, 0,
                 point_forced, dst);
    if (!right_padding && (plus_sgn || space_symbol || number_sgn == -1)) {
      correct_padding(start_of_num);
    }
    print_prefix('E', 'e', capital, 0, dst);
    print_int(&power, 2, 0, 0, pading_symbol, 1, 1, 0, dst, TYPE_INT);
    if (width > num_len && right_padding) {
      add_padding(width - num_len, pading_symbol, dst);
    }
  }
}

long int get_power(long double num) {
  long int result;
  if (num == 0.f) {
    result = 0;
  } else {
    result = (long int)log10l(num);
    if (result <= 0) {
      result--;
    }
  }
  return result;
}

void print_double_shortest(long double num, int precision, int width,
                           int right_padding, char pading_symbol, int plus_sgn,
                           int space_symbol, int capital, int point_forced,
                           char* dst) {
  if (!(double)check_special_float_nums(num, width, right_padding,
                                        pading_symbol, plus_sgn, space_symbol,
                                        dst)) {
    char* start_of_num = &dst[s21_strlen(dst)];
    long double original_num = num;
    if (num < 0) {
      num = -num;
    }
    long int power = get_power(num);
    char* tmp = malloc(sizeof(char) * (width + 300));
    tmp[0] = 0;
    if (precision == 0) {
      precision = 1;
    }
    round_double(original_num, &power, precision);
    print_shortest_part(power, precision, original_num, right_padding,
                        pading_symbol, plus_sgn, space_symbol, capital,
                        point_forced, tmp);
    int num_len = (int)s21_strlen(tmp);
    int padding_added = 0;
    if (width > num_len && !right_padding) {
      padding_added = 1;
      add_padding(width - num_len, pading_symbol, dst);
    }
    print_string(tmp, dst, -1, 0, right_padding, pading_symbol);
    if (!right_padding && (plus_sgn || space_symbol || original_num < 0) &&
        padding_added) {
      correct_padding(start_of_num);
    }
    if (width > num_len && right_padding) {
      add_padding(width - num_len, ' ', dst);
    }
    free(tmp);
  }
}

void print_shortest_part(long int power, int precision, long double num,
                         int right_padding, char pading_symbol, int plus_sgn,
                         int space_symbol, int capital, int point_forced,
                         char* dst) {
  if (power < precision && power >= -4) {
    print_double(num, precision - 1 - power, 0, right_padding, pading_symbol,
                 plus_sgn, space_symbol, point_forced, dst);
    if (!point_forced && precision - 1 - power > 0) {
      delete_zeros(dst);
    }
  } else {
    print_double(num * powl(10, -power), precision - 1, 0, 0, pading_symbol,
                 plus_sgn, space_symbol, point_forced, dst);
    if (!point_forced) {
      delete_zeros(dst);
    }
    print_prefix('E', 'e', capital, 0, dst);
    print_int(&power, 2, 0, 0, pading_symbol, 1, 1, 0, dst, TYPE_INT);
  }
}

void delete_zeros(char* str) {
  int i = s21_strlen(str);

  while (str[--i] == '0') {
    str[i] = 0;
    // i--;
  }
  if (str[i] == '.') {
    str[i] = 0;
  }
}

void correct_padding(char* str) {
  char* search_result = s21_strpbrk(str, " +-");
  char tmp = *str;
  *str = *search_result;
  *search_result = tmp;
}
