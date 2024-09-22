#include <syscall.h>
#include <test_util.h>
#include <stdlib.h>
#include <strings.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_BLOCKS 32
#define A_ASCII 65 
#define MAX_ITERATION 1000000
#define LOADING_LINES 20
#define TEST_OUTPUT_COLOR 0xFF00FF

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;



char nibble_to_hex(uint8_t nibble) {
    if (nibble < 10)
        return '0' + nibble;
    else
        return 'a' + (nibble - 10);
}

// Function to convert a void pointer to a string (hexadecimal representation)
void pointer_to_string(void* ptr, char* buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) {
        return; // Do nothing if buffer is null or size is 0
    }

    // uintptr_t can safely hold the pointer as an integer
    uintptr_t address = (uintptr_t)ptr;

    // Number of hexadecimal digits needed (2 per byte)
    size_t num_digits = sizeof(uintptr_t) * 2;

    // Check if the buffer has enough space (including null terminator)
    if (buffer_size < num_digits + 1) {
        return; // Not enough space in buffer
    }

    // Null-terminate the string
    buffer[num_digits] = '\n';
    buffer[num_digits+1] = '\0';

    // Convert the address to hexadecimal string
    for (int i = num_digits - 1; i >= 0; --i) {
        buffer[i] = nibble_to_hex(address & 0xF); // Get last 4 bits (1 nibble)
        address >>= 4; // Shift right by 4 bits to process the next nibble
    }
}


uint64_t test_mm(uint64_t argc, char *argv[]) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 1)
    return -1;

  if ((max_memory = satoi(argv[0])) <= 0)
    return -1;

  max_memory = 256;

  int iterationCount = 0;
  for(int i = 0; i < LOADING_LINES; i++){
    puts("-", TEST_OUTPUT_COLOR);
  }
  printScreen();
  while (iterationCount++ < MAX_ITERATION) {
    if(iterationCount % (MAX_ITERATION / LOADING_LINES) == 0){
      for(int i = 0; i < LOADING_LINES; i++){
        removeChar();
      }
      for(int i = 0; i < LOADING_LINES; i++){
        if(iterationCount * LOADING_LINES / MAX_ITERATION > i){
          putChar(0x80, 0xFF00FF);
        } else {
          puts("_", 0xFF00FF);
        }
      }
      printScreen();
    }
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = malloc(mm_rqs[rq].size);
      char buffer[100];
      pointer_to_string(mm_rqs[rq].address, buffer, 100);
      puts(buffer, 0xFF0000);
      printScreen();
      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        test_memset(mm_rqs[i].address, i, mm_rqs[i].size);

    for(int i = 0; i < rq; i++){
      char buffer[100];
      pointer_to_string(mm_rqs[i].address, buffer, 100);
      puts(buffer, 0xFF0000);
      printScreen();
    }

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, (uint8_t)i, mm_rqs[i].size))
          return 1;

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        free(mm_rqs[i].address);
  }

  return 0;
}