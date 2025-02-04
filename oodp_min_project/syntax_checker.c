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
void write_file(const char *filename, const FileLines *file_lines);
void check_brackets(const FileLines *file_lines);
void count_keywords(const FileLines *file_lines);
void analyze_functions(const FileLines *file_lines);
void count_variables(const FileLines *file_lines);
void remove_comments(FileLines *file_lines);
void trim_spaces(char *line);
void analyze_prototypes(const FileLines *file_lines);
void analyze_everything(const FileLines *file_lines);
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

    printf("File read successfully, removing comments...\n");
    remove_comments(file_lines);

    for (size_t i = 0; i < file_lines->size; ++i) {
        trim_spaces(file_lines->lines[i].content);
    }

    write_keywords_with_lines(file_lines, output_filename);
    write_file(output_filename, file_lines);
    printf("Output written to %s\n", output_filename);

    analyze_everything(file_lines);

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

void write_file(const char *filename, const FileLines *file_lines) {
    FILE *file = fopen(filename, "a");
    if (!file) {
        perror("Error opening output file");
        return;
    }

    printf("Writing lines to %s...\n", filename);
    for (size_t i = 0; i < file_lines->size; ++i) {
        fprintf(file, "%3d: %s\n", file_lines->lines[i].line_number, file_lines->lines[i].content);
    }

    fclose(file);
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
void check_brackets(const FileLines *file_lines) {
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

    printf("Bracket Analysis:\n");
    printf("  Parentheses: %s\n", round == 0 && valid ? "Balanced" : "Unbalanced");
    printf("  Square Brackets: %s\n", square == 0 && valid ? "Balanced" : "Unbalanced");
    printf("  Curly Braces: %s\n", curly == 0 && valid ? "Balanced" : "Unbalanced");
}

void count_keywords(const FileLines *file_lines) {
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

    printf("Keyword Counts:\n");
    for (int k = 0; k < keyword_count; ++k) {
        printf("  %s: %d\n", keywords[k], counts[k]);
    }
}

void analyze_functions(const FileLines *file_lines) {
    int function_count = 0;

    for (size_t i = 0; i < file_lines->size; ++i) {
        char *line = file_lines->lines[i].content;
        if (strstr(line, "(") && strstr(line, ")") && strstr(line, "{")) {
            function_count++;
        }
    }

    printf("Function Analysis:\n");
    printf("  Total Functions: %d\n", function_count);
}

void count_variables(const FileLines *file_lines) {
    int int_count = 0, char_count = 0, float_count = 0;

    for (size_t i = 0; i < file_lines->size; ++i) {
        char *line = file_lines->lines[i].content;
        if (strstr(line, "int ") || strstr(line, "int\t")) int_count++;
        if (strstr(line, "char ") || strstr(line, "char\t")) char_count++;
        if (strstr(line, "float ") || strstr(line, "float\t")) float_count++;
    }

    printf("Variable Analysis:\n");
    printf("  int: %d (Estimated Memory: %d bytes)\n", int_count, int_count * 4);
    printf("  char: %d (Estimated Memory: %d bytes)\n", char_count, char_count * 1);
    printf("  float: %d (Estimated Memory: %d bytes)\n", float_count, float_count * 4);
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

void analyze_prototypes(const FileLines *file_lines) {
    int prototype_count = 0;

    for (size_t i = 0; i < file_lines->size; ++i) {
        char *line = file_lines->lines[i].content;
        if (strchr(line, '(') && strchr(line, ')') && strchr(line, ';') && !strchr(line, '{')) {
            prototype_count++;
        }
    }

    printf("Function Prototypes: %d\n", prototype_count);
}

void analyze_everything(const FileLines *file_lines) {
    printf("Performing complete analysis...\n");
    check_brackets(file_lines);
    count_keywords(file_lines);
    analyze_functions(file_lines);
    count_variables(file_lines);
    analyze_prototypes(file_lines);
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

    fprintf(file, "KEYWORDS WITH LINE NUMBER\n\n");

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
