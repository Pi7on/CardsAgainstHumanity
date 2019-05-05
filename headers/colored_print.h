#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printf_clr_bg(char const str[], char const col[]){
	char command[128];
	
	if(strcmp(col,"RED")==0){
		snprintf(command, sizeof(command), "echo \033[103m\033[41m%s\033[0m", str);
		system(command);
	}
	if(strcmp(col,"YELLOW")==0){
		snprintf(command, sizeof(command), "echo \033[103m\033[30m%s\033[0m", str);
		system(command);
	}
	if(strcmp(col,"BLU")==0){
		snprintf(command, sizeof(command), "echo \033[104m%s\033[0m", str);
		system(command);
	}
	if(strcmp(col,"GREEN")==0){
		snprintf(command, sizeof(command), "echo \033[102m\033[30m%s\033[0m", str);
		system(command);
	}
}

void printf_clr_txt(char const str[], char const col[]){
	char command[128];
	
	if(strcmp(col,"RED")==0){
		snprintf(command, sizeof(command), "echo \033[91m%s\033[0m", str);
		system(command);
	}
	if(strcmp(col,"YELLOW")==0){
		snprintf(command, sizeof(command), "echo \033[93m%s\033[0m", str);
		system(command);
	}
	if(strcmp(col,"BLU")==0){
		snprintf(command, sizeof(command), "echo \033[94m%s\033[0m", str);
		system(command);
	}
	if(strcmp(col,"GREEN")==0){
		snprintf(command, sizeof(command), "echo \033[92m%s\033[0m", str);
		system(command);
	}
	if(strcmp(col,"CYAN")==0){
		snprintf(command, sizeof(command), "echo \033[96m%s\033[0m", str);
		system(command);
	}
}

void putch_clr(int n, const char col[]){
	char command[128];
	char c=n;
	
	if(strcmp(col,"RED")==0){
		snprintf(command, sizeof(command), "echo \033[91m%c\033[0m", c);
		system(command);
	}
	
}


