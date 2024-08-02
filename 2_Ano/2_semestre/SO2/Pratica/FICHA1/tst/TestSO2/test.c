#include <stdio.h>

int main(int argc, char argv[]) {
	if (argc == 0) {
		printf("Hello world!");
		return 0;
	}
	//else if(argc==1){
		printf("Input Arg.: %s", argv[1]);
		return 0;
	//}
	//return 1;
}