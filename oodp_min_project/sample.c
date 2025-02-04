#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define INITIAL_CAPACITY 100

typedef struct {
    int line_number;
    int line_length;
    char *content;
} FileLine;

typedef struct {
    FileLine *lines;
    size_t size;
    size_t capacity;
} FileLines;

// Function Prototypes
FileLines *read_file(const char *filename);
void free_filelines(FileLines *file_lines);
void check_brackets(const FileLines *file_lines, FILE *output_file);
void count_keywords(const FileLines *file_lines, FILE *output_file);
void analyze_functions(const FileLines *file_lines, FILE *output_file);
void count_variables(const FileLines *file_lines, FILE *output_file);
void remove_comments(FileLines *file_lines);
void trim_spaces(char *line);
void analyze_prototypes(const FileLines *file_lines, FILE *output_file);
void analyze_everything(const FileLines *file_lines, const char *output_filename);
void write_keywords_with_lines(const FileLines *file_lines, const char *filename);

// Main Function
int main() {
    const char *input_filename = "input.txt";
    const char *output_filename = "output.txt";

    FileLines *file_lines = read_file(input_filename);
    if (!file_lines) {
        fprintf(stderr, "Error reading input file\n");
        return EXIT_FAILURE;
    }

    remove_comments(file_lines);

    for (size_t i = 0; i < file_lines->size; ++i) {
        trim_spaces(file_lines->lines[i].content);
    }

    write_keywords_with_lines(file_lines, output_filename);
    analyze_everything(file_lines, output_filename);

    free_filelines(file_lines);
    return EXIT_SUCCESS;
}

// File Reading and Writing Functions
FileLines *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return NULL;
    }

    FileLines *file_lines = malloc(sizeof(FileLines));
    if (!file_lines) {
        perror("malloc");
        fclose(file);
        return NULL;
    }

    file_lines->lines = malloc(INITIAL_CAPACITY * sizeof(FileLine));
    file_lines->capacity = INITIAL_CAPACITY;
    file_lines->size = 0;

    char buffer[MAX_LINE_LENGTH];
    int line_number = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (file_lines->size >= file_lines->capacity) {
            file_lines->capacity *= 2;
            file_lines->lines = realloc(file_lines->lines, file_lines->capacity * sizeof(FileLine));
            if (!file_lines->lines) {
                perror("realloc");
                free_filelines(file_lines);
                fclose(file);
                return NULL;
            }
        }

        FileLine *line = &file_lines->lines[file_lines->size++];
        line->line_number = line_number++;
        line->line_length = strlen(buffer);
        line->content = strdup(buffer);
        if (!line->content) {
            perror("strdup");
            free_filelines(file_lines);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return file_lines;
}

void free_filelines(FileLines *file_lines) {
    if (!file_lines) return;

    for (size_t i = 0; i < file_lines->size; ++i) {
        free(file_lines->lines[i].content);
    }
    free(file_lines->lines);
    free(file_lines);
}

// Analysis Functions
void check_brackets(const FileLines *file_lines, FILE *output_file) {
    int round = 0, square = 0, curly = 0;
    int valid = 1;

    for (size_t i = 0; i < file_lines->size && valid; ++i) {
        for (char *ch = file_lines->lines[i].content; *ch; ++ch) {
            if (*ch == '(') round++;
            else if (*ch == ')') round--;
            else if (*ch == '[') square++;
            else if (*ch == ']') square--;
            else if (*ch == '{') curly++;
            else if (*ch == '}') curly--;

            if (round < 0 || square < 0 || curly < 0) {
                valid = 0;
                break;
            }
        }
    }

    fprintf(output_file, "Bracket Analysis:\n");
    fprintf(output_file, "  Parentheses: %s\n", round == 0 && valid ? "Balanced" : "Unbalanced");
    fprintf(output_file, "  Square Brackets: %s\n", square == 0 && valid ? "Balanced" : "Unbalanced");
    fprintf(output_file, "  Curly Braces: %s\n", curly == 0 && valid ? "Balanced" : "Unbalanced");
}

void count_keywords(const FileLines *file_lines, FILE *output_file) {
    const char *keywords[] = {
        "int", "char", "float", "double", "if", "else", "while", "for",
        "return", "switch", "case", "default", "break", "continue", "void", "struct"
    };
    const int keyword_count = sizeof(keywords) / sizeof(keywords[0]);
    int counts[keyword_count];
    memset(counts, 0, sizeof(counts));

    for (size_t i = 0; i < file_lines->size; ++i) {
        for (int k = 0; k < keyword_count; ++k) {
            char *line = file_lines->lines[i].content;
            char *pos = line;
            while ((pos = strstr(pos, keywords[k])) != NULL) {
                if ((pos == line || !isalnum(*(pos - 1))) && !isalnum(*(pos + strlen(keywords[k])))) {
                    counts[k]++;
                }
                pos += strlen(keywords[k]);
            }
        }
    }

    fprintf(output_file, "Keyword Counts:\n");
    for (int k = 0; k < keyword_count; ++k) {
        fprintf(output_file, "  %s: %d\n", keywords[k], counts[k]);
    }
}

void analyze_functions(const FileLines *file_lines, FILE *output_file) {
    int function_count = 0;

    for (size_t i = 0; i < file_lines->size; ++i) {
        char *line = file_lines->lines[i].content;
        if (strstr(line, "(") && strstr(line, ")") && strstr(line, "{")) {
            function_count++;
        }
    }

    fprintf(output_file, "Function Analysis:\n");
    fprintf(output_file, "  Total Functions: %d\n", function_count);
}

void count_variables(const FileLines *file_lines, FILE *output_file) {
    int int_count = 0, char_count = 0, float_count = 0;

    for (size_t i = 0; i < file_lines->size; ++i) {
        char *line = file_lines->lines[i].content;
        if (strstr(line, "int ") || strstr(line, "int\t")) int_count++;
        if (strstr(line, "char ") || strstr(line, "char\t")) char_count++;
        if (strstr(line, "float ") || strstr(line, "float\t")) float_count++;
    }

    fprintf(output_file, "Variable Analysis:\n");
    fprintf(output_file, "  int: %d (Estimated Memory: %d bytes)\n", int_count, int_count * 4);
    fprintf(output_file, "  char: %d (Estimated Memory: %d bytes)\n", char_count, char_count * 1);
    fprintf(output_file, "  float: %d (Estimated Memory: %d bytes)\n", float_count, float_count * 4);
}

void remove_comments(FileLines *file_lines) {
    int in_comment = 0;

    for (size_t i = 0; i < file_lines->size; ++i) {
        char *line = file_lines->lines[i].content;
        char *result = malloc(strlen(line) + 1);
        char *dest = result;

        for (char *ch = line; *ch; ++ch) {
            if (!in_comment && *ch == '/' && *(ch + 1) == '*') {
                in_comment = 1;
                ch++;
            } else if (in_comment && *ch == '*' && *(ch + 1) == '/') {
                in_comment = 0;
                ch++;
            } else if (!in_comment && *ch == '/' && *(ch + 1) == '/') {
                break;
            } else if (!in_comment) {
                *dest++ = *ch;
            }
        }

        *dest = '\0';
        free(file_lines->lines[i].content);
        file_lines->lines[i].content = result;
    }
}

void trim_spaces(char *line) {
    char *start = line;
    while (isspace((unsigned char)*start)) start++;

    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) *end-- = '\0';

    memmove(line, start, end - start + 2);
}

void analyze_prototypes(const FileLines *file_lines, FILE *output_file) {
    int prototype_count = 0;

    for (size_t i = 0; i < file_lines->size; ++i) {
        char *line = file_lines->lines[i].content;
        if (strchr(line, '(') && strchr(line, ')') && strchr(line, ';') && !strchr(line, '{')) {
            prototype_count++;
        }
    }

    fprintf(output_file, "Function Prototypes: %d\n", prototype_count);
}

void analyze_everything(const FileLines *file_lines, const char *output_filename) {
    FILE *output_file = fopen(output_filename, "a");
    if (!output_file) {
        perror("Error opening output file");
        return;
    }

    fprintf(output_file, "\nPerforming complete analysis...\n\n");
    check_brackets(file_lines, output_file);
    count_keywords(file_lines, output_file);
    analyze_functions(file_lines, output_file);
    count_variables(file_lines, output_file);
    analyze_prototypes(file_lines, output_file);

    fclose(output_file);
}

void write_keywords_with_lines(const FileLines *file_lines, const char *filename) {
    const char *keywords[] = {
        "int", "char", "float", "double", "if", "else", "while", "for",
        "return", "switch", "case", "default", "break", "continue", "void", "struct"
    };
    const int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening output file");
        return;
    }
    for (size_t i = 0; i < file_lines->size; ++i) {
        for (int k = 0; k < keyword_count; ++k) {
            char *line = file_lines->lines[i].content;
            char *pos = line;
            while ((pos = strstr(pos, keywords[k])) != NULL) {
                if ((pos == line || !isalnum(*(pos - 1))) && !isalnum(*(pos + strlen(keywords[k])))) {
                    fprintf(file, "line %3d: %s\n", file_lines->lines[i].line_number, keywords[k]);
                }
                pos += strlen(keywords[k]);
            }
        }
    }

    fprintf(file, "\nKEYWORDS USED IN THE PROGRAM\n");
    fclose(file);
}
