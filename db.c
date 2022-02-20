#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

InputBuffer *newInputBuffer();
void printPrompt();
void readInput(InputBuffer *const inputBuffer);
void closeInputBuffer(InputBuffer *const inputBuffer);

int main() {
  InputBuffer *inputBuffer = newInputBuffer();
  while (true) {
    printPrompt();
    readInput(inputBuffer);

    if (strcmp(inputBuffer->buffer, ".exit") == 0) {
      closeInputBuffer(inputBuffer);
      exit(EXIT_SUCCESS);
    } else {
      printf("Unrecognized command '%s'.\n", inputBuffer->buffer);
    }
  }
}

InputBuffer *newInputBuffer() {
  InputBuffer *inputBuffer = malloc(sizeof(InputBuffer));
  if (inputBuffer != NULL) {
    inputBuffer->buffer = NULL;
    inputBuffer->buffer_length = 0;
    inputBuffer->input_length = 0;
  }
  return inputBuffer;
}

void printPrompt() { printf("%s", "db > "); }

void readInput(InputBuffer* const inputBuffer) {
  ssize_t bytesRead =
      getline(&(inputBuffer->buffer), &(inputBuffer->buffer_length), stdin);

  if (bytesRead <= 0) {
    puts("Error reading input");
    exit(EXIT_FAILURE);
  }
  inputBuffer->input_length = bytesRead - 1;
  inputBuffer->buffer[bytesRead - 1] = '\0';
}

void closeInputBuffer(InputBuffer* const inputBuffer) {
  free(inputBuffer->buffer);
  free(inputBuffer);
}
