#ifndef GREP_H
#define GREP_H

#include <dirent.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define GETOPT_END -1
#define FOPEN_READ "r"

#define PATTERNS_INIT 128
#define PATTERNS_ADD 128
#define BUFFSIZE 8192

#define BUFFER_INIT 128
#define BUFFER_MULT 2
typedef struct Flags {
  bool e;
  bool i;
  bool v;
  bool n;
  bool l;
  bool c;
  int empty_line;
} Flags;

void e_process(Flags *const flags, int *e_count, char *pattern);
void options_set(Flags *const flags, const char opt, char *pattern,
                 int *e_count);
void parse(Flags *const flags, int argc, char **argv, char *pattern);
void retrieve_files(const Flags *const flags, char *const path[],
                    int file_count, char *pattern);
void output(const Flags *const flags, FILE *file, char *const path[],
            int file_count, char *pattern);
void print_usual(const Flags *const flags, char *const path[], int file_count,
                 size_t line_number, char *str);
void print_unus(const Flags *const flags, char *const path[], int file_count,
                size_t lines_count);

#endif