#ifndef S21_CAT_H
#define S21_CAT_H

#include <ctype.h>
#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define BUF_MAX 4096
#define GETOPT_END -1
#define FOPEN_READ "r"
#define ASCII_DEL 127
#define NONPRINT_SHIFT 64

typedef struct Flags {
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool u;
  bool v;
} Flags;

void number_line(const char prev_sym, const char curr_sym,
                 const Flags* const opts);
void count_empty_lines(const char curr_sym, size_t* const empty_lines_count);
void print_meta(const char curr_sym, const Flags* const opts);

void print_tab(const Flags* const opts);

void print_cntrl(const char curr_sym, const Flags* const opts);
void print_symbol(const char curr_sym, const Flags* const opts);
void mutate_file(FILE* file, const Flags* const opts);
void retrieve_files(int file_count, char* const path[],
                    const Flags* const opts);
void parse(Flags* const opts, int argc, char** argv);
void options_set(Flags* const opts, const char opt);

#endif