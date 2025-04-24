#include "s21_grep.h"

static const char short_options[] = "e:f:isvnholc";

static const struct option long_options[] = {
    {"regexp", required_argument, NULL, 'e'},
    {"file", no_argument, NULL, 'f'},
    {"ignore-case", no_argument, NULL, 'i'},
    {"no-messages", no_argument, NULL, 's'},
    {"invert-match", no_argument, NULL, 'v'},
    {"line-number", no_argument, NULL, 'n'},
    {"no-filename", no_argument, NULL, 'h'},
    {"only-matching", no_argument, NULL, 'o'},
    {"files-with-matches", no_argument, NULL, 'l'},
    {"count", no_argument, NULL, 'c'},
    {"help", no_argument, NULL, 0},
    {NULL, 0, NULL, 0}};

void e_process(Flags *const flags, int *e_count, char *pattern) {
  if (*e_count) {
    strcat(pattern, "|");
  }
  if (!*optarg) {
    optarg = ".";
    flags->empty_line += 1;
  }
  strcat(pattern, optarg);
  *e_count += 1;
}
void options_set(Flags *const flags, const char opt, char *pattern,
                 int *e_count) {
  switch (opt) {
    case 'e':
      flags->e = true;
      e_process(flags, e_count, pattern);
      break;
    case 'i':
      flags->i = true;
      break;
    case 'v':
      flags->v = true;
      break;
    case 'n':
      flags->n = true;
      break;
    case 'l':
      flags->l = true;
      break;
    case 'c':
      flags->c = true;
      break;
  }
}

void parse(Flags *const flags, int argc, char **argv, char *pattern) {
  int e_count = 0;
  int option_index = 0;
  char curr_opt =
      getopt_long(argc, argv, short_options, long_options, &option_index);
  while (curr_opt != GETOPT_END) {
    options_set(flags, curr_opt, pattern, &e_count);
    curr_opt =
        getopt_long(argc, argv, short_options, long_options, &option_index);
  }
  if (!flags->e) {
    if (!*argv[optind]) argv[optind] = ".";
    strcat(pattern, argv[optind]);
    ++optind;
  }
}

void retrieve_files(const Flags *const flags, char *const path[],
                    int file_count, char *pattern) {
  int files_left = file_count;
  for (; files_left--; ++path) {
    FILE *curr_file = fopen(*path, FOPEN_READ);
    DIR *pDir;
    pDir = opendir(*path);
    if (!pDir) {
      if (curr_file) {
        output(flags, curr_file, path, file_count, pattern);
      }
    }
  }
}

void output(const Flags *const flags, FILE *file, char *const path[],
            int file_count, char *pattern) {
  regex_t regex;
  int regflag = REG_EXTENDED;
  char str[BUFFSIZE] = {0};
  size_t line_number = 1;
  size_t lines_count = 0;
  regmatch_t pmatch[1] = {0};
  size_t nmatch = 1;
  if (flags->i) regflag |= REG_ICASE;
  regcomp(&regex, pattern, regflag);
  while (!feof(file)) {
    if (fgets(str, BUFFSIZE, file)) {
      int success = regexec(&regex, str, nmatch, pmatch, 0);
      if (flags->v) success = success ? 0 : 1;
      if (success != REG_NOMATCH) {
        print_usual(flags, path, file_count, line_number, str);
        lines_count += 1;
      }
      line_number += 1;
    }
  }
  print_unus(flags, path, file_count, lines_count);
  regfree(&regex);
}

void print_usual(const Flags *const flags, char *const path[], int file_count,
                 size_t line_number, char *str) {
  if (!flags->c && !flags->l) {
    if (file_count > 1) {
      printf("%s:", *path);
    }
    if (flags->n) {
      printf("%zu:", line_number);
    }
    printf("%s", str);
    if (str[strlen(str) - 1] != '\n') {
      printf("\n");
    }
  }
}

void print_unus(const Flags *const flags, char *const path[], int file_count,
                size_t lines_count) {
  if (true == flags->l && lines_count) {
    printf("%s\n", *path);
  }
  if (true == flags->c && !flags->l) {
    if (file_count > 1) {
      printf("%s:", *path);
    }
    if (true == flags->l && lines_count)
      printf("1\n");
    else
      printf("%zu\n", lines_count);
  }
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    Flags flags = {0};
    char pattern[BUFFSIZE] = {0};
    parse(&flags, argc, argv, pattern);
    if (argc - optind) {
      retrieve_files(&flags, argv + optind, argc - optind, pattern);
    }
  }
}