#include "test_mm.h"
#include "mm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 256 * 1024

int main(int argc, char* argv[]){
  void* startAddress = malloc(MEMORY_SIZE);
  initializeMemoryManager(startAddress);
  printf("Running test...\n");
  uint64_t status = test_mm(argc, argv);
  printf("Test has finished with status: %d\n", (int) status);
  return 0;
}