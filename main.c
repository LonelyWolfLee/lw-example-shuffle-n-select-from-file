#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINE_COUNT      20
#define MAX_LINE_SIZE       100
#define SELECTED_LINE_COUNT 5

// to get start positions of sentence
int get_start_positions(FILE* stream, long positions[]);
// to shuffle start positions of sentence
void shuffle_positions(long positions[], int line_count);
// record sentences
int select_n_record(FILE* stream, const char* record_filename, long positions[], int selected_lint_count);

int main(int argc, char* argv[]) {
    char *filename = "sample", *record_filename = "result";
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("There is no '%s' file\n", filename);
        exit(1);
    }

    int line_count;
    long *positions = calloc(MAX_LINE_COUNT, sizeof(long));

    line_count = get_start_positions(file, positions);
    shuffle_positions(positions, line_count);
    int status = select_n_record(file, record_filename, positions, SELECTED_LINE_COUNT);
    if (status != 0) {
        printf("Failed to record data in '%s' file\n", record_filename);
    }

    printf("\nRelease memory ...\n");
    free(positions);
    int close_status = fclose(file);
    if (close_status != 0) {
        printf("Failed to close '%s' file\n", filename);
        exit(1);
    }

    printf("Done.\n");
    return status;
}

int get_start_positions(FILE* stream, long positions[]) {
    printf("Finding start positions ...\n");
    char sentence[MAX_LINE_SIZE+1];
    long cur_pos = 0;
    int line_count = 0;

    fseek(stream, 0, SEEK_END);
    long last_pos = ftell(stream);
    fseek(stream, 0, SEEK_SET);

    while(cur_pos != last_pos) {
        positions[line_count++] = cur_pos;
        fgets(sentence, MAX_LINE_SIZE, stream);
        cur_pos = ftell(stream);
    }

    printf("Found %d sentences.\n\n", line_count);
    return line_count;
}

void shuffle_positions(long positions[], int line_count) {
    printf("Shuffling %d sentences ...\n", line_count);
    srand(time(NULL));
    long temp;
    int rand_idx;
    for (int i = 0; i < line_count-1; i++) {
        rand_idx = rand() % (line_count - i) + i;
        temp = positions[i];
        positions[i] = positions[rand_idx];
        positions[rand_idx] = temp;
    }
}

int select_n_record(FILE* stream, const char* record_filename, long positions[], int selected_lint_count) {
    printf("Recording %d sentences ...\n", selected_lint_count);
    FILE *record = fopen(record_filename, "wb");
    if(record == NULL) {
        printf("Failed to create '%s' file\n", record_filename);
        return 1;
    }

    rewind(stream);
    char sentence[MAX_LINE_SIZE+1];
    for (int i = 0; i < selected_lint_count; i++) {
        fseek(stream, positions[i], SEEK_SET);
        fgets(sentence, MAX_LINE_SIZE, stream);
        fprintf(record, "%s", sentence);
    }

    return fclose(record);
}