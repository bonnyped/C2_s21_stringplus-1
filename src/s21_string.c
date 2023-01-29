#include "s21_sprintf_parser.h"

#define _CRT_SECURE_NO_WARNINGS

char *forward_search(const char *src, const char *trim_chars,
                     s21_size_t length_src, s21_size_t length_trim_chars);
char *rear_search(const char *src, const char *trim_chars,
                  s21_size_t length_src, s21_size_t length_trim_chars);

void *s21_memchr(const void *str, int c, s21_size_t n) {
  const char *str_copy = str;
  void *res = (void *)str;
  s21_size_t i = 0;
  int exit = 0;

  while (i < n && !exit) {
    if (str_copy[i] == c) {
      exit = 1;
      res = (void *)(str_copy + i);
    }
    i++;
  }

  return exit ? res : s21_NULL;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int res = 0;
  const char *first = str1;
  const char *second = str2;
  s21_size_t i = 0;
  int exit = 0;

  while (i < n && !exit) {
    if (first[i] != second[i]) {
      if (first[i] < second[i]) {
        res = -1;
        exit = 1;
      } else {
        res = 1;
        exit = 1;
      }
    }
    i++;
  }

  return res;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *dest_copy = dest;
  const char *src_copy = src;

  for (s21_size_t i = 0; i < n; i++) {
    dest_copy[i] = src_copy[i];
  }

  return dest;
}

void *s21_memmove(void *dest, const void *src, s21_size_t n) {
  char *src_copy = calloc(n, sizeof(char));

  s21_memcpy(src_copy, src, n);
  s21_memcpy(dest, src_copy, n);
  free(src_copy);

  return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  char *str_copy = str;
  s21_size_t i = 0;

  while (i < n) {
    str_copy[i] = c;
    i++;
  }

  return str_copy;
}

char *s21_strcat(char *dest, const char *src) {
  s21_size_t len = 0;
  s21_size_t i = 0;

  len = s21_strlen(dest);
  for (; src[i] != '\0'; i++) {
    dest[len + i] = src[i];
  }
  dest[len + i] = '\0';

  return dest;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t len = 0;
  s21_size_t i = 0;

  len = s21_strlen(dest);
  for (; (i < n) && (src[i] != '\0'); i++) {
    dest[len + i] = src[i];
  }

  dest[len + i] = '\0';

  return dest;
}

char *s21_strchr(const char *str, int c) {
  char *res = s21_NULL;
  s21_size_t i = 0;
  int exit = 0;

  do {
    if (str[i] == c) {
      exit = 1;
      res = (char *)(str + i);
    }
  } while (str[i++] != '\0' && !exit);

  return exit ? res : s21_NULL;
}

int s21_strcmp(const char *str1, const char *str2) {
  const char *s21_str1 = (const char *)str1;
  const char *s21_str2 = (const char *)str2;
  int stop = 0;
  int status = 0;
  while (str1 && str2 && stop == 0) {
    if ((*s21_str1 == '\0' || *s21_str2 == '\0') ||
        (*s21_str1 - *s21_str2 != 0)) {
      stop = 1;
      if (*s21_str1 - *s21_str2 != 0) {
        status = *s21_str1 - *s21_str2;
      }
    } else {
      s21_str1++;
      s21_str2++;
    }
  }
  return status;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  char *s21_str1 = (char *)str1;
  char *s21_str2 = (char *)str2;
  unsigned i = 0;
  int stop = 0;
  int status = 0;
  while (stop == 0 && i++ < n) {
    if (*s21_str1 - *s21_str2 != 0) {
      stop = 1;
      status = *s21_str1 - *s21_str2;
    }
    if (*s21_str1 != '\0' && *s21_str2 != '\0') {
      s21_str1++;
      s21_str2++;
    }
  }
  return status;
}

char *s21_strcpy(char *dest, const char *src) {
  char *s21_dest = s21_NULL;
  char *s21_src = s21_NULL;
  if ((dest != s21_NULL) && (src != s21_NULL)) {
    s21_dest = (char *)dest;
    s21_src = (char *)src;
    while (((*s21_dest++ = *s21_src++)) != '\0')
      ;
  }
  return dest;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *s21_dest = s21_NULL;
  char *s21_src = s21_NULL;
  unsigned i = 0;
  if ((dest != s21_NULL) && (src != s21_NULL)) {
    s21_dest = (char *)dest;
    s21_src = (char *)src;
    while (i < n) {
      *s21_dest++ = *s21_src++;
      i++;
    }
  }
  return dest;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  char *s21_str1 = (char *)str1;
  char *s21_str2 = (char *)str2;
  s21_size_t status = 0;
  int stop = 0;
  while (*s21_str1 != '\0' && stop == 0) {
    while (*s21_str2 != '\0' && stop == 0) {
      if (*s21_str1 == *s21_str2) {
        stop = 1;
      }
      s21_str2++;
    }
    if (stop == 0) {
      status += 1;
      s21_str2 = (char *)str2;
    }
    s21_str1++;
  }
  return status;
}

char *s21_strerror(int errnum) {
  char *str_error = s21_NULL;
  static char buffer_error[3000];
  static char *array_error[] = s21_error;
  if (errnum < 0 || errnum > ARRAY_SIZE) {
    s21_sprintf(buffer_error, "%s%d", UNKNOWN_ERROR, errnum);
    str_error = buffer_error;
  } else {
    str_error = (char *)array_error[errnum];
  }
  return str_error;
}

s21_size_t s21_strlen(const char *str) {
  char *s21_str = (char *)str;
  s21_size_t count = 0;
  while (*s21_str != '\0') {
    count++;
    s21_str++;
  }
  return count;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  char *s21_str1 = (char *)str1;
  char *s21_str2 = (char *)str2;
  char *status = s21_NULL;
  int stop = 0;
  while (*s21_str1 != '\0' && stop == 0) {
    while (*s21_str2 != '\0' && stop == 0) {
      if (*s21_str1 == *s21_str2) {
        stop = 1;
        status = s21_str1;
      }
      s21_str2++;
    }
    if (stop == 0) {
      s21_str2 = (char *)str2;
    }
    s21_str1++;
  }
  return status;
}

char *s21_strrchr(const char *str, int ch) {
  char *pointer = s21_NULL;
  if (str && ch >= 0 && ch <= 255) {
    int i = 0;
    if (ch == '\0') {
      pointer = (char *)&str[s21_strlen(str)];
    }
    while (str[i] != '\0') {
      if (str[i] == ch) {
        pointer = (char *)str + i;
      }
      i++;
    }
  }
  return pointer;
}

s21_size_t s21_strspn(const char *str, const char *accept) {
  s21_size_t result = 0;
  for (s21_size_t i = 0; i < s21_strlen(str); i++) {
    s21_size_t match = 0;
    for (s21_size_t j = 0; j < s21_strlen(accept); j++) {
      if (str[i] == accept[j]) {
        result++;
        match++;
        j = s21_strlen(accept);
      } else if (j == (s21_strlen(accept) - 1) && match == 0) {
        i = s21_strlen(str);
      }
    }
  }
  return result;
}

char *s21_strstr(const char *str1, const char *str2) {
  char *pointer = s21_NULL;
  char *tmp = s21_NULL;
  if (str1 && str2) {
    s21_size_t length_str1 = s21_strlen(str1);
    s21_size_t length_str2 = s21_strlen(str2);
    s21_size_t capacity = 0;
    s21_size_t start_to_search = 0;
    if (length_str2 > 0) {
      for (s21_size_t i = 0; i < length_str1; i++) {
        if (start_to_search != 0) {
          i = start_to_search;
          start_to_search = 0;
        }
        for (s21_size_t j = 0; j < length_str2; j++) {
          if (!tmp) {
            if (str1[i + j] == str2[j]) {
              tmp = (char *)(str1 + i + j);
              capacity++;
              if (j == length_str2 - 1) {
                i = length_str1;
              }
            } else {
              j = length_str2;
            }
          }
          if ((tmp && j != 0 && length_str2 > 1) || (tmp && length_str2 == 1)) {
            if (str1[i + j] == str2[j]) {
              capacity++;
              if (start_to_search == 0 && str1[i + j] == str2[0]) {
                start_to_search = i + j;
              }
            }
            if (j <= length_str2 - 1 && str1[i + j] != str2[j] &&
                length_str2 > 1) {
              tmp = s21_NULL;
              i = i + j;
              j = length_str2;
              capacity = 0;
            }
            if ((capacity == length_str2 && length_str2 > 1) ||
                (tmp && length_str2 == 1)) {
              pointer = tmp;
              i = length_str1;
            }
          } else if (!tmp) {
            if (str1[i + j] == str2[j]) {
              pointer = tmp;
            }
          }
        }
      }
    } else {
      pointer = (char *)str1;
    }
  }
  return pointer;
}

char *s21_strtok(char *str, const char *delim) {
  char *pointer = s21_NULL;
  static char *static_pointer = s21_NULL;
  s21_size_t length_match = 0;
  str ? static_pointer = s21_NULL : static_pointer;
  static_pointer ? str = static_pointer : str;
  if (str && delim && (*str != '\0' || *delim != '\0')) {
    char *match = s21_NULL;
    match = s21_strpbrk(str, delim);
    if (match == str) {
      length_match = s21_strspn(str, delim);
      str = match + length_match;
      match = s21_strpbrk(str, delim);
      if (match) {
        static_pointer = match + 1;
        *match = '\0';
        pointer = str;
      } else {
        *str == '\0' ? pointer = s21_NULL : (pointer = str);
      }
    } else if (*str == '\0' || *delim == '\0') {
      *str == '\0' ? pointer = s21_NULL : (pointer = str);
    } else {
      match ? (static_pointer = match + 1) : (static_pointer = s21_NULL);
      if (match)
        *match = '\0';
      else
        static_pointer = match;
      pointer = str;
    }
  }
  return pointer;
}

void *s21_to_upper(const char *str) {
  char *src = s21_NULL;
  if (str) {
    s21_size_t capacity = s21_strlen(str);
    src = calloc(capacity + 1, sizeof(char));
    if (src) {
      for (s21_size_t i = 0; i < capacity; i++) {
        if (str[i] >= 97 && str[i] <= 122) {
          src[i] = str[i] - 32;
        } else {
          src[i] = str[i];
        }
        if (i == capacity - 1) {
          src[i + 1] = '\0';
        }
      }
    }
  }
  return src;
}

void *s21_to_lower(const char *str) {
  char *src = s21_NULL;
  if (str) {
    s21_size_t capacity = s21_strlen(str);
    src = calloc(capacity + 1, sizeof(char));
    if (src) {
      for (s21_size_t i = 0; i < capacity; i++) {
        if (str[i] >= 65 && str[i] <= 90) {
          src[i] = str[i] + 32;
        } else {
          src[i] = str[i];
        }
        if (i == capacity - 1) {
          src[i + 1] = '\0';
        }
      }
    }
  }
  return src;
}

void *s21_insert(const char *src, const char *str,
                 s21_size_t start_index_trimed_buffer) {
  char *srcstr = s21_NULL;
  if (src && str) {
    s21_size_t length_src, length_str;
    s21_size_t count = 0;
    s21_size_t start_index_for_strsrc = 0;
    length_src = s21_strlen(src);
    length_str = s21_strlen(str);
    if (start_index_trimed_buffer <= length_src) {
      srcstr = (char *)calloc((length_src + length_str + 1), sizeof(char));
      if (length_str > 0 && length_src > 0) {
        if (srcstr) {
          for (s21_size_t i = 0; i <= length_src; i++) {
            if (i == start_index_trimed_buffer) {
              for (s21_size_t j = 0; j < length_str; j++) {
                srcstr[start_index_trimed_buffer + j] = str[j];
                count++;
              }
              start_index_for_strsrc = start_index_trimed_buffer + count;
            } else {
              count > 0 ? (srcstr[start_index_for_strsrc] = src[i - 1])
                        : (srcstr[start_index_for_strsrc] = src[i]);
              start_index_for_strsrc++;
              if (start_index_for_strsrc == length_str + length_src) {
                srcstr[length_str + length_src] = '\0';
              }
            }
          }
        }
      } else if (length_str == 0 && length_src > 0) {
        for (size_t i = 0; i < length_src; i++) {
          srcstr[i] = src[i];
        }
      } else if (length_str == 0 && length_src == 0) {
        srcstr[0] = src[0];
      }
    }
  }
  return srcstr;
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *str = s21_NULL;
  char *start = s21_NULL;
  char *end = s21_NULL;
  if (src && trim_chars) {
    s21_size_t length_src = s21_strlen(src);
    s21_size_t length_trim_chars = s21_strlen(trim_chars);

    if (length_trim_chars > 0) {
      char *trimed_buffer = calloc(length_src + 1, sizeof(char));
      start = forward_search(src, trim_chars, length_src, length_trim_chars);
      if (start)
        end = rear_search(src, trim_chars, length_src, length_trim_chars);
      if (end) s21_memcpy(trimed_buffer, start, end - start + 1);
      str = trimed_buffer;
    }
  }
  if (src && (!trim_chars || (trim_chars && s21_strlen(trim_chars) == 0))) {
    char whitespaces[] = " \n\t\x0B\r";
    s21_size_t length_src = s21_strlen(src);
    s21_size_t length_whitespaces = s21_strlen(whitespaces);
    char *trimed_buffer = calloc(length_src + 1, sizeof(char));
    start = forward_search(src, whitespaces, length_src, length_whitespaces);
    if (start)
      end = rear_search(src, whitespaces, length_src, length_whitespaces);
    if (end) s21_memcpy(trimed_buffer, start, end - start + 1);
    str = trimed_buffer;
  }
  return str;
}

char *forward_search(const char *src, const char *trim_chars,
                     s21_size_t length_src, s21_size_t length_trim_chars) {
  const char *start = s21_NULL;
  for (s21_size_t i = 0; i < length_src; i++) {
    for (s21_size_t j = 0; j < length_trim_chars; j++) {
      if (src[i] == trim_chars[j]) {
        j = length_trim_chars;
      }
      if ((j == length_trim_chars - 1 && src[i] != trim_chars[j])) {
        start = &src[i];
        j = length_trim_chars;
        i = length_src;
      }
    }
  }
  return (char *)start;
}

char *rear_search(const char *src, const char *trim_chars,
                  s21_size_t length_src, s21_size_t length_trim_chars) {
  const char *end = s21_NULL;
  for (s21_size_t i = length_src; i > 0; i--) {
    for (s21_size_t j = 0; j < length_trim_chars; j++) {
      if (src[i - 1] == trim_chars[j]) {
        j = length_trim_chars;
      }
      if (j == length_trim_chars - 1 && src[i - 1] != trim_chars[j]) {
        end = &src[i - 1];
        j = length_trim_chars;
        i = 1;
      }
    }
  }
  return (char *)end;
}

int s21_sprintf(char *dst, const char *format_string, ...) {
  int result = 0;
  *dst = 0;
  *(dst + 1) = 0;
  va_list args;
  va_start(args, format_string);
  while (*format_string) {
    if (*format_string != '%') {
      print_char(*format_string, dst, 1, 0, ' ');
      format_string++;
    } else if (*(format_string + 1) == '%') {
      print_char('%', dst, 1, 0, ' ');
      format_string = format_string + 2;
    } else if (*(format_string + 1) == 'n') {
      int *pointer = (int *)va_arg(args, int *);
      *pointer = (int)s21_strlen(dst);
      format_string = format_string + 2;
    } else {
      int data_type = get_format_type(format_string);
      if (data_type != NOT_VALID_RESULT) {
        format_string++;
        pattern executable_pattern = init_pattern(data_type);
        executable_pattern = read_pattern(&format_string, executable_pattern);
        if (executable_pattern.width == READ_REQUIRED) {
          executable_pattern.width = va_arg(args, int);
        }
        if (executable_pattern.precision == READ_REQUIRED) {
          executable_pattern.precision = va_arg(args, int);
        }
        if (executable_pattern.pattern_is_ok) {
          execute_pattern(data_type, &args, dst, executable_pattern);
        }
        format_string++;
      }
    }
  }
  va_end(args);
  result = s21_strlen(dst);
  return result;
}
