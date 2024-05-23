#include <strings.h>

int strcmp(const char* a, const char* b){
	int i = 0;
	while(a[i] != 0 && b[i] != 0 && a[i] == b[i]){
		i++;
	}
	return (int)(a[i] - b[i]);
}