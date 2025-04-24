#include "s21_cat.h"

const char* short_options = "AbeEnstTuv";

const struct option long_options[] = {
    {"show-all", no_argument, NULL, 'A'},
    {"number-nonblank", no_argument, NULL, 'b'},
    {"show-ends", no_argument, NULL, 'E'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {"show-tabs", no_argument, NULL, 'T'},
    {"show-nonprinting", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 0},
    {NULL, 0, NULL, 0}};

int main(int argc, char* argv[]) {
  Flags flags = {0};
  parse(&flags, argc, argv);
  retrieve_files(argc - optind, argv + optind, &flags);
  return 0;
}

void options_set(Flags* const flags, const char opt) {
  switch (opt) {
    case 'A':
      flags->v = true;
      flags->e = true;
      flags->t = true;
      break;
    case 'b':
      flags->b = true;
      break;
    case 'e':
      flags->v = true;
      flags->e = true;
      break;
    case 'E':
      flags->e = true;
      break;
    case 'n':
      flags->n = true;
      break;
    case 's':
      flags->s = true;
      break;
    case 't':
      flags->v = true;
      flags->t = true;
      break;
    case 'T':
      flags->t = true;
      break;
    case 'v':
      flags->v = true;
      break;
  }
}

void parse(Flags* const flags, int argc, char** argv) {
  int option_index = 0;
  char curr_opt =
      getopt_long(argc, argv, short_options, long_options, &option_index);
  while (curr_opt != GETOPT_END) {
    options_set(flags, curr_opt);
    curr_opt =
        getopt_long(argc, argv, short_options, long_options, &option_index);
  }
  if (flags->b) {
    flags->n = false;
  }
}

void retrieve_files(int file_count, char* const path[],
                    const Flags* const flags) {
  for (; file_count--; ++path) {
    FILE* curr_file = fopen(*path, FOPEN_READ);
    DIR* pDir;
    pDir = opendir(*path);
    if (!pDir) {
      if (curr_file) {
        mutate_file(curr_file, flags);
      };
    }
    if (curr_file) {
      fclose(curr_file);
    }
  }
}

void mutate_file(FILE* file, const Flags* const flags) {
  static size_t empty_lines_count = 1;
  static char prev_sym = '\n';
  char curr_sym = fgetc(file);
  while (!feof(file)) {
    if (flags->s) {
      count_empty_lines(curr_sym, &empty_lines_count);
    }
    if (curr_sym != '\n' || empty_lines_count <= 2) {
      number_line(prev_sym, curr_sym, flags);
      if (flags->e && curr_sym == '\n') {
        fputc('$', stdout);
      }
      print_symbol(curr_sym, flags);
    }
    prev_sym = curr_sym;
    curr_sym = fgetc(file);
  }
}

void print_symbol(const char curr_sym, const Flags* const flags) {
  if (isprint(curr_sym)) {
    fputc(curr_sym, stdout);
  } else if (curr_sym == '\t') {
    print_tab(flags);
  } else if (curr_sym == '\n') {
    fputc('\n', stdout);
  } else if (iscntrl(curr_sym)) {
    print_cntrl(curr_sym, flags);
  } else {
    print_meta(curr_sym, flags);
  }
}

void print_cntrl(const char curr_sym, const Flags* const flags) {
  if (flags->v) {
    fputc('^', stdout);
    if (curr_sym == ASCII_DEL) {
      fputc((char)(curr_sym - NONPRINT_SHIFT), stdout);
    } else {
      fputc((char)(curr_sym + NONPRINT_SHIFT), stdout);
    }
  } else {
    fputc(curr_sym, stdout);
  }
}

void print_tab(const Flags* const flags) {
  if (flags->t) {
    fputc('^', stdout);
    fputc('\t' + NONPRINT_SHIFT, stdout);
  } else {
    fputc('\t', stdout);
  }
}

void print_meta(const char curr_sym, const Flags* const flags) {
  if (flags->v) {
    const char meta_symbol = (signed char)curr_sym + ASCII_DEL + 1;
    fputc('M', stdout);
    fputc('-', stdout);
    if (isprint(meta_symbol)) {
      fputc(meta_symbol, stdout);
    } else {
      print_cntrl(meta_symbol, flags);
    }
  } else {
    fputc(curr_sym, stdout);
  }
}

void count_empty_lines(const char curr_sym, size_t* const empty_lines_count) {
  if (curr_sym == '\n') {
    *empty_lines_count += 1;
  } else {
    *empty_lines_count = 0;
  }
}

void number_line(const char prev_sym, const char curr_sym,
                 const Flags* const flags) {
  static size_t line_count = 0;
  if (flags->b && prev_sym == '\n' && curr_sym != '\n') {
    fprintf(stdout, "%6zu\t", ++line_count);
  } else if (flags->n && prev_sym == '\n') {
    fprintf(stdout, "%6zu\t", ++line_count);
  }
}
