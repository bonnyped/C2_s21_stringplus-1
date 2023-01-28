#ifndef _S21_SPRINTF_FUNCS_H_
#define _S21_SPRINTF_FUNCS_H_
#define _CRT_SECURE_NO_WARNINGS
#include <stddef.h>

#include "s21_string.h"

#define NOT_VALID_RESULT -1
#define BASE_TYPES 10
#define TYPE_INT 0
#define TYPE_UNSIGNED_INT 1
#define TYPE_CHAR 2
#define TYPE_STRING 3
#define TYPE_FLOAT 4
#define TYPE_SCIENTIFIC_DOUBLE 5
#define TYPE_OCTAL 6
#define TYPE_HEXADECIMAL 7
#define TYPE_POINTER 8
#define TYPE_SHORTEST_DOUBLE 9

#define TYPE_SCIENTIFIC_DOUBLE_CAPITAL 51
#define TYPE_HEXADECIMAL_CAPITAL 71
#define TYPE_SHORTEST_DOUBLE_CAPITAL 91

#define TYPE_LONG_INT 14
#define TYPE_SHORT_INT 10
#define TYPE_LONG_UNSIGNED_INT 11
#define TYPE_SHORT_UNSIGNED_INT 12
#define TYPE_WCHAR 13

void print_char(char symb, char* dst, int width, int right_padding,
                char pading_symbol);
void print_string(char* string, char* dst, int precision, int width,
                  int right_padding, char pading_symbol);
void add_padding(int num, char pading_symbol, char* dst);
void print_int(void* number, int precision, int width, int right_padding,
               char pading_symbol, int plus_sgn, int minus_sgn,
               int space_symbol, char* dst, int type);
void print_octal(long unsigned int number, int precision, int width,
                 int right_padding, char pading_symbol, int prefix, char* dst);
void reverse_string(char* str);
void print_hexadecimal(long unsigned int number, int precision, int width,
                       int right_padding, char pading_symbol, int plus,
                       int space, int prefix, int in_upper_case, int pointer,
                       char* dst);
void print_prefix(char symbol_up, char symbol, int in_upper_case,
                  int print_zero, char* dst);
char hex_char_from_num(int num, int capital);
void print_hexa_oct_num_part(long unsigned int number, int base,
                             int in_upper_case, int pointer, char pading_symbol,
                             char* dst);
void int_to_chars(void* input, int number_length, char* array, int type);
s21_size_t number_length(void* input, int type);
int get_int_sign(void* number, int type);
int is_zero(void* number, int type);
void print_sign(int plus_sgn, int minus_sgn, int space_symbol, int sgn,
                char* dst);
void print_double(long double num, int precision, int width, int right_padding,
                  char pading_symbol, int plus_sgn, int space_symbol,
                  int point_forced, char* dst);
void print_double_scientific(long double num, int precision, int width,
                             int right_padding, char pading_symbol,
                             int plus_sgn, int space_symbol, int capital,
                             int point_forced, char* dst);
long int get_power(long double num);
void print_whole_float(long double num, char* dst);
void print_fractional_float(long double fractional, int precision, int plus_sgn,
                            char* dst);
int check_special_float_nums(double num, int width, int right_padding,
                             char pading_symbol, int plus_sgn, int space_symbol,
                             char* dst);
void print_inf_nan_sgn(int sgn, int plus_sgn, int space_symbol, char* dst);
long double round_double(long double num, long int* power, int precision);
void print_double_shortest(long double num, int precision, int width,
                           int right_padding, char pading_symbol, int plus_sgn,
                           int space_symbol, int capital, int point_forced,
                           char* dst);
void print_shortest_part(long int power, int precision, long double num,
                         int right_padding, char pading_symbol, int plus_sgn,
                         int space_symbol, int capital, int point_forced,
                         char* dst);
void delete_zeros(char* str);
void correct_padding(char* str);
#endif
