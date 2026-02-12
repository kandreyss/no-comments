#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>

#define BUFFER_SIZE 4096
#define TEMP_SUFFIX ".tmp"

typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_ARGS = 1,
    STATUS_ERR_IO = 2,
    STATUS_ERR_MEM = 3
} ExitStatus;

typedef struct {
    bool in_multiline_comment;
    bool preserving_comment;
} ParserContext;

static void trim_trailing_whitespace(char *str, size_t *len) {
    while (*len > 0 && isspace((unsigned char)str[*len - 1])) {
        (*len)--;
    }
    str[*len] = '\0';
}

static bool process_line(const char *input, char *output, ParserContext *ctx) {
    size_t in_len = strlen(input);
    size_t out_idx = 0;

    for (size_t i = 0; i < in_len; ++i) {

        if (ctx->in_multiline_comment) {

            if (ctx->preserving_comment) {
                output[out_idx++] = input[i];
            }

            if (i + 1 < in_len && input[i] == '*' && input[i + 1] == '/') {
                if (ctx->preserving_comment) {
                    output[out_idx++] = '/';
                    i++;
                } else {
                    i++;
                }

                ctx->in_multiline_comment = false;
                ctx->preserving_comment = false;
            }
            continue;
        }

        if (i + 1 < in_len && input[i] == '/' && input[i + 1] == '*') {

            bool is_doxygen = false;
            if (i + 2 < in_len) {
                if (input[i + 2] == '*' && (i + 3 >= in_len || input[i + 3] != '/')) {

                    is_doxygen = true;
                } else if (input[i + 2] == '!') {

                    is_doxygen = true;
                }
            }

            if (is_doxygen) {
                ctx->preserving_comment = true;
                ctx->in_multiline_comment = true;

                output[out_idx++] = '/';
                output[out_idx++] = '*';
                i++;
                continue;
            } else {

                ctx->preserving_comment = false;
                ctx->in_multiline_comment = true;
                i++;
                continue;
            }
        }

        if (i + 1 < in_len && input[i] == '/' && input[i + 1] == '/') {

            bool is_doxygen_single = false;
            if (i + 2 < in_len) {
                 if (input[i + 2] == '/' || input[i + 2] == '!') {
                     is_doxygen_single = true;
                 }
            }

            if (!is_doxygen_single) {
                break;
            }

            output[out_idx++] = '/';
            output[out_idx++] = '/';
            i++;
            continue;
        }

        output[out_idx++] = input[i];
    }

    output[out_idx] = '\0';

    if (!ctx->preserving_comment) {
        trim_trailing_whitespace(output, &out_idx);
    }

    return (out_idx > 0);
}

static void process_stream(FILE *fin, FILE *fout) {
    char *in_buffer = malloc(BUFFER_SIZE);
    char *out_buffer = malloc(BUFFER_SIZE);
    ParserContext ctx = { .in_multiline_comment = false, .preserving_comment = false };

    bool last_line_was_empty = false;

    if (!in_buffer || !out_buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(in_buffer);
        free(out_buffer);
        exit(STATUS_ERR_MEM);
    }

    while (fgets(in_buffer, BUFFER_SIZE, fin) != NULL) {
        size_t len = strlen(in_buffer);
        bool has_newline = (len > 0 && in_buffer[len - 1] == '\n');

        if (has_newline) {
            in_buffer[--len] = '\0';
        }

        bool has_content = process_line(in_buffer, out_buffer, &ctx);

        if (has_content) {
            fputs(out_buffer, fout);
            if (has_newline) fputc('\n', fout);
            last_line_was_empty = false;
        } else if (has_newline && !ctx.in_multiline_comment) {
            if (!last_line_was_empty) {
                fputc('\n', fout);
                last_line_was_empty = true;
            }
        }
    }

    free(in_buffer);
    free(out_buffer);
}

static ExitStatus perform_cleaning(const char *in_path, const char *out_path) {
    FILE *fin = fopen(in_path, "r");
    if (!fin) {
        fprintf(stderr, "Error: Cannot open input file '%s': %s\n", in_path, strerror(errno));
        return STATUS_ERR_IO;
    }

    FILE *fout = fopen(out_path, "w");
    if (!fout) {
        fprintf(stderr, "Error: Cannot open output file '%s': %s\n", out_path, strerror(errno));
        fclose(fin);
        return STATUS_ERR_IO;
    }

    process_stream(fin, fout);

    fclose(fin);
    fclose(fout);
    return STATUS_OK;
}

static void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s <file>           # Overwrite original file\n", prog_name);
    fprintf(stderr, "  %s <input> <output> # Save to new file\n", prog_name);
}

int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        print_usage(argv[0]);
        return STATUS_ERR_ARGS;
    }

    const char *input_file = argv[1];
    ExitStatus status = STATUS_OK;

    if (argc == 3) {
        status = perform_cleaning(input_file, argv[2]);
    } else {
        char temp_filename[FILENAME_MAX];

        int written = snprintf(temp_filename, sizeof(temp_filename), "%s%s", input_file, TEMP_SUFFIX);

        if (written < 0 || (size_t)written >= sizeof(temp_filename)) {
            fprintf(stderr, "Error: Filename too long or encoding error\n");
            return STATUS_ERR_ARGS;
        }

        status = perform_cleaning(input_file, temp_filename);

        if (status == STATUS_OK) {
            if (remove(input_file) != 0) {
                fprintf(stderr, "Error: Failed to remove original file: %s\n", strerror(errno));
                remove(temp_filename);
                status = STATUS_ERR_IO;
            } else if (rename(temp_filename, input_file) != 0) {
                fprintf(stderr, "Error: Failed to rename temp file: %s\n", strerror(errno));
                status = STATUS_ERR_IO;
            }
        } else {
            remove(temp_filename);
        }
    }

    return status;
}