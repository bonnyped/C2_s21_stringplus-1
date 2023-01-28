#include "s21_sprintf_parser.h"

#include "s21_string.h"

int get_format_type(const char* format_string) {
  int result = NOT_VALID_RESULT;
  char correct_formats[] = "ducsfeoxpgiEXG";
  char* result_of_search = s21_strpbrk(format_string, correct_formats);
  if (result_of_search != NULL) {
    for (int i = 0; i < BASE_TYPES; i++) {
      if (*result_of_search == correct_formats[i]) {
        result = i;
      }
    }
  }
  if (result_of_search != NULL && *result_of_search == 'i') {
    result = TYPE_INT;
  }
  if (result_of_search != NULL && *result_of_search == 'E') {
    result = TYPE_SCIENTIFIC_DOUBLE_CAPITAL;
  }
  if (result_of_search != NULL && *result_of_search == 'X') {
    result = TYPE_HEXADECIMAL_CAPITAL;
  }
  if (result_of_search != NULL && *result_of_search == 'G') {
    result = TYPE_SHORTEST_DOUBLE_CAPITAL;
  }
  return result;
}

pattern init_pattern(int data_type) {
  pattern result;
  result.precision = 1;
  result.length = 'n';
  result.width = 0;
  result.pattern_flags.minus_sgn = 0;
  result.pattern_flags.plus_sgn = 0;
  result.pattern_flags.space_char = 0;
  result.pattern_flags.reshetka = 0;
  result.pattern_flags.is_capital = 0;
  result.padding_char = ' ';
  result.pattern_is_ok = 1;
  result.type = data_type;
  switch (data_type) {
    case TYPE_STRING:
      result.precision = -1;
      break;
    case TYPE_FLOAT:
      result.precision = 6;
      break;
    case TYPE_SCIENTIFIC_DOUBLE:
      result.precision = 6;
      break;
    case TYPE_SCIENTIFIC_DOUBLE_CAPITAL:
      result.precision = 6;
      break;
    case TYPE_SHORTEST_DOUBLE:
      result.precision = 6;
      break;
    case TYPE_SHORTEST_DOUBLE_CAPITAL:
      result.precision = 6;
      break;
  }
  return result;
}

void execute_pattern(int data_type, va_list* args, char* dst,
                     pattern executable_pattern) {
  void* pointer_arg;
  if (data_type == TYPE_HEXADECIMAL_CAPITAL) {
    executable_pattern.pattern_flags.is_capital = 1;
    data_type = TYPE_HEXADECIMAL;
  }
  if (data_type == TYPE_SCIENTIFIC_DOUBLE_CAPITAL) {
    executable_pattern.pattern_flags.is_capital = 1;
    data_type = TYPE_SCIENTIFIC_DOUBLE;
  }
  if (data_type == TYPE_SHORTEST_DOUBLE_CAPITAL) {
    executable_pattern.pattern_flags.is_capital = 1;
    data_type = TYPE_SHORTEST_DOUBLE;
  }
  if (data_type == TYPE_FLOAT || data_type == TYPE_SCIENTIFIC_DOUBLE ||
      data_type == TYPE_SHORTEST_DOUBLE) {
    execute_float_pattern(data_type, args, dst, executable_pattern);
  } else if (data_type == TYPE_OCTAL || data_type == TYPE_HEXADECIMAL) {
    execute_hex_oct_pattern(data_type, args, dst, executable_pattern);
  } else if (data_type == TYPE_CHAR || data_type == TYPE_STRING) {
    execute_chars_pattern(data_type, args, dst, executable_pattern);
  } else {
    switch (data_type) {
      case TYPE_POINTER:
        pointer_arg = va_arg(*args, void*);
        print_hexadecimal(
            (unsigned long int)pointer_arg, executable_pattern.precision,
            executable_pattern.width,
            executable_pattern.pattern_flags.minus_sgn,
            executable_pattern.padding_char,
            executable_pattern.pattern_flags.plus_sgn,
            executable_pattern.pattern_flags.space_char, 1, 0, 1, dst);
        break;
      case TYPE_INT:
        execute_int_pattern(args, dst, executable_pattern);
        break;
      case TYPE_UNSIGNED_INT:
        execute_unsigned_int_pattern(args, dst, executable_pattern);
        break;
    }
  }
}

void execute_chars_pattern(int data_type, va_list* args, char* dst,
                           pattern executable_pattern) {
  char* str_arg;
  char char_arg = '\0';
  switch (data_type) {
    case TYPE_STRING:
      str_arg = va_arg(*args, char*);
      print_string(str_arg, dst, executable_pattern.precision,
                   executable_pattern.width,
                   executable_pattern.pattern_flags.minus_sgn,
                   executable_pattern.padding_char);
      break;
    case TYPE_CHAR:
      char_arg = va_arg(*args, int);
      print_char(char_arg, dst, executable_pattern.width,
                 executable_pattern.pattern_flags.minus_sgn, ' ');
      break;
  }
}

void execute_hex_oct_pattern(int data_type, va_list* args, char* dst,
                             pattern executable_pattern) {
  unsigned long int long_uint_arg;
  if (executable_pattern.length == 'n') {
    long_uint_arg = (unsigned long int)va_arg(*args, unsigned int);
  } else if (executable_pattern.length == 'l') {
    long_uint_arg = va_arg(*args, unsigned long int);
  } else if (executable_pattern.length == 'h') {
    long_uint_arg = (unsigned long int)va_arg(*args, unsigned int);
  }
  switch (data_type) {
    case TYPE_OCTAL:
      print_octal(long_uint_arg, executable_pattern.precision,
                  executable_pattern.width,
                  executable_pattern.pattern_flags.minus_sgn,
                  executable_pattern.padding_char,
                  executable_pattern.pattern_flags.reshetka, dst);
      break;
    case TYPE_HEXADECIMAL:
      print_hexadecimal(long_uint_arg, executable_pattern.precision,
                        executable_pattern.width,
                        executable_pattern.pattern_flags.minus_sgn,
                        executable_pattern.padding_char,
                        executable_pattern.pattern_flags.plus_sgn,
                        executable_pattern.pattern_flags.space_char,
                        executable_pattern.pattern_flags.reshetka,
                        executable_pattern.pattern_flags.is_capital, 0, dst);
      break;
  }
}

void execute_unsigned_int_pattern(va_list* args, char* dst,
                                  pattern executable_pattern) {
  unsigned long int long_uint_arg;
  if (executable_pattern.length == 'n') {
    long_uint_arg = (unsigned long int)va_arg(*args, unsigned int);
  } else if (executable_pattern.length == 'l') {
    long_uint_arg = va_arg(*args, unsigned long int);
  } else if (executable_pattern.length == 'h') {
    long_uint_arg = (unsigned long int)va_arg(*args, unsigned int);
  }
  print_int(
      &long_uint_arg, executable_pattern.precision, executable_pattern.width,
      executable_pattern.pattern_flags.minus_sgn,
      executable_pattern.padding_char, 0, 0, 0, dst, TYPE_SHORT_UNSIGNED_INT);
}

void execute_int_pattern(va_list* args, char* dst, pattern executable_pattern) {
  long int long_int_arg;
  if (executable_pattern.length == 'n') {
    long_int_arg = (long int)va_arg(*args, int);
  } else if (executable_pattern.length == 'l') {
    long_int_arg = va_arg(*args, long int);
  } else if (executable_pattern.length == 'h') {
    long_int_arg = (long int)va_arg(*args, int);
  }
  print_int(&long_int_arg, executable_pattern.precision,
            executable_pattern.width,
            executable_pattern.pattern_flags.minus_sgn,
            executable_pattern.padding_char,
            executable_pattern.pattern_flags.plus_sgn, 1,
            executable_pattern.pattern_flags.space_char, dst, TYPE_LONG_INT);
}

void execute_float_pattern(int data_type, va_list* args, char* dst,
                           pattern executable_pattern) {
  long double ldouble_arg;
  if (executable_pattern.length == 'L') {
    ldouble_arg = va_arg(*args, long double);
  } else {
    ldouble_arg = (long double)va_arg(*args, double);
  }
  switch (data_type) {
    case TYPE_FLOAT:
      print_double(ldouble_arg, executable_pattern.precision,
                   executable_pattern.width,
                   executable_pattern.pattern_flags.minus_sgn,
                   executable_pattern.padding_char,
                   executable_pattern.pattern_flags.plus_sgn,
                   executable_pattern.pattern_flags.space_char,
                   executable_pattern.pattern_flags.reshetka, dst);
      break;
    case TYPE_SCIENTIFIC_DOUBLE:
      print_double_scientific(ldouble_arg, executable_pattern.precision,
                              executable_pattern.width,
                              executable_pattern.pattern_flags.minus_sgn,
                              executable_pattern.padding_char,
                              executable_pattern.pattern_flags.plus_sgn,
                              executable_pattern.pattern_flags.space_char,
                              executable_pattern.pattern_flags.is_capital,
                              executable_pattern.pattern_flags.reshetka, dst);
      break;
    case TYPE_SHORTEST_DOUBLE:
      print_double_shortest(ldouble_arg, executable_pattern.precision,
                            executable_pattern.width,
                            executable_pattern.pattern_flags.minus_sgn,
                            executable_pattern.padding_char,
                            executable_pattern.pattern_flags.plus_sgn,
                            executable_pattern.pattern_flags.space_char,
                            executable_pattern.pattern_flags.is_capital,
                            executable_pattern.pattern_flags.reshetka, dst);
      break;
  }
}

pattern read_pattern(const char** format_string, pattern result) {
  int state = FLAG_STATE;
  const char* curr_ptr = *format_string;
  char correct_formats[] = "ducsfeoxpgiEXG";
  char* result_of_search = s21_strpbrk(curr_ptr, correct_formats);
  while (curr_ptr != result_of_search && result.pattern_is_ok) {
    int set_result = 0;
    if (state == FLAG_STATE) {
      set_result = flag_set(*curr_ptr, &result);
      if (!set_result) {
        state++;
      }
    }
    if (state <= WIDTH_STATE && !set_result) {
      set_result = num_param_set(&curr_ptr, &(result.width));
      state = PRECISION_STATE;
    }
    if (state <= PRECISION_STATE && !set_result && *curr_ptr == '.') {
      int shift = 1;
      if ((result_of_search - curr_ptr > 1)) {
        curr_ptr++;
        shift--;
        set_result = num_param_set(&curr_ptr, &(result.precision));
      }
      if (!set_result) {
        set_result =
            check_zero_precision(*(curr_ptr + shift), &(result.precision));
        curr_ptr = curr_ptr - (1 - shift);
      }
      state = LENGTH_STATE;
    }
    if (state <= LENGTH_STATE) {
      if (!set_result) {
        set_result = length_set(*curr_ptr, &result);
        state++;
      }
    }
    if (set_result) {
      curr_ptr++;
    } else {
      result.pattern_is_ok = 0;
    }
  }
  *format_string = result_of_search;
  return result;
}

int flag_set(char symbol, pattern* pattern_to_set) {
  int result = 0;
  if (symbol == '+') {
    pattern_to_set->pattern_flags.plus_sgn = 1;
    result = 1;
  } else if (symbol == '-') {
    pattern_to_set->pattern_flags.minus_sgn = 1;
    result = 1;
  } else if (symbol == ' ') {
    pattern_to_set->pattern_flags.space_char = 1;
    result = 1;
  } else if (symbol == '#') {
    pattern_to_set->pattern_flags.reshetka = 1;
    result = 1;
  } else if (symbol == '0') {
    pattern_to_set->padding_char = '0';
    result = 1;
  }
  return result;
}

int check_zero_precision(const char symb, int* precision) {
  char correct_next_symb[] = "ducsfeoxpgiEXGhlL";
  int i = 0;
  int found = 0;
  while (correct_next_symb[i] && !found) {
    if (symb == correct_next_symb[i]) {
      found = 1;
      *precision = 0;
    }
    i++;
  }
  return found;
}

int num_param_set(const char** str, int* param) {
  const char* curr_ptr = *str;
  int width = 0;
  int result = 0;
  while (*curr_ptr >= '0' && *curr_ptr <= '9') {
    width = width * 10 + *curr_ptr - '0';
    curr_ptr++;
  }
  if (curr_ptr != *str) {
    result = 1;
    *param = width;
    *str = curr_ptr - 1;
  } else if (*curr_ptr == '*') {
    result = 1;
    *param = READ_REQUIRED;
  }
  return result;
}

int length_set(char symbol, pattern* pattern_to_set) {
  int result = 0;
  if (symbol == 'l') {
    pattern_to_set->length = 'l';
    result = 1;
  }
  if (symbol == 'h') {
    pattern_to_set->length = 'h';
    result = 1;
  }
  if (symbol == 'L') {
    pattern_to_set->length = 'L';
    result = 1;
  }
  return result;
}
