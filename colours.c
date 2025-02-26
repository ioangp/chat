#include <string.h>
#include "colours.h"

char* string_to_colour(char *str){
	if(strcmp(str, "r") == 0){
		return RED;
	}
	else if (strcmp(str, "g") == 0){
		return GRN;
	}
	else if (strcmp(str, "b") == 0){
		return BLU;
	}
	else if (strcmp(str, "y") == 0){
		return YEL;
	}
	else if (strcmp(str, "m") == 0){
		return MAG;
	}
	else if (strcmp(str, "c") == 0){
		return CYN;
	}
	else if (strcmp(str, "w") == 0){
		return WHT; // default
	}
}
