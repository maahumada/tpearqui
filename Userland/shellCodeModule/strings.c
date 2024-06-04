#include <strings.h>

int strcmp(const char* a, const char* b){
	int i = 0;
	while(a[i] != 0 && b[i] != 0 && a[i] == b[i]){
		i++;
	}
	return (int)(a[i] - b[i]);
}


//Only positive numbers
int strtoint(const char *str) {
  int toReturn = 0;
  int i = 0;

  while (str[i] >= '0' && str[i] <= '9') {
    toReturn = toReturn * 10 + (str[i] - '0');
    i++;
  }
  
  return toReturn;
}

char* strcpy(char* dest, const char* src) {
	while(*src != '\0'){
		*(dest++) = *(src++);
	}
	*dest = '\0';
	return dest;
}

void reverseStr(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        // Swap characters
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void numToStr(uint64_t number, char* str){
		int index = 0;
    
    // Special case for 0
    if (number == 0) {
        str[index++] = '0';
        str[index] = '\0';
        return;
    }

    // Process each digit
    while (number != 0) {
        str[index++] = (number % 10) + '0';
        number /= 10;
    }
    
    // Null-terminate the string
    str[index] = '\0';
    
    // Reverse the string to get the correct order
    reverseStr(str, index);
}