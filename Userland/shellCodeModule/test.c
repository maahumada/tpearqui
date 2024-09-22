#include <stdint.h>
#include <test_mm.h>
#include <strings.h>
#include <syscall.h>

void testMemoryManager(){
  char* argv[] = { "256" };
  puts("Starting test... (1000000 iterations)\n", 0x0000FF);
  printScreen();
  uint64_t status = test_mm(1, argv);
  puts("\nTest has finished with exit code: ", 0x0000FF);
  char exitcode[10];
  numToStr(status, exitcode);
  puts(exitcode, 0x0000FF);
  puts("\n", 0x0000FF);
  printScreen();
}