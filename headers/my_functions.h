#include <windows.h>
void title(){

	printf("%c",201);
	for(int i=0; i<22; i++){
		printf("%c",205);	
	}
	printf("%c\n",187);
	
	printf("%c",186);
	printf("CARDS AGAINST HUMANITY");
	printf("%c\n",186);
	
	printf("%c",200);
	for(int i=0; i<22; i++){
		printf("%c",205);	
	}
	printf("%c\n",188);
}

void disclamer(){
	printf_clr_txt("DISCLAMER:","YELLOW");
	printf("Questo gioco di carte contiene ");
	printf_clr_txt("DARK HUMOR.","YELLOW");
	printf("Ogni riferimento a fatti o persone reali e' puramente causale.\n\n");
	printf("Premi un tasto per iniziare!\n");
	getchar();
	fflush(stdin);
	system("cls");
}

void n_round(int n){
	int k; //controlla spaziatura dentro quadrato
	
	if(n>=10){
		k=9;
	}
	else{
		k=8;
	}
	
	printf("%c",201);
	for(int i=0; i<k; i++){
		printf("%c",205);	
	}
	printf("%c\n",187);
	
	printf("%c",186);
	printf("%d%c ROUND",n,167);
	printf("%c\n",186);

	printf("%c",200);
	for(int i=0; i<k; i++){
		printf("%c",205);	
	}
	printf("%c\n",188);
}

void print_turno(char p[]){
	int k=strlen(p)+8;
	
	printf("%c",201);
	for(int i=0; i<k; i++){
		printf("%c",205);	
	}
	printf("%c\n",187);

	printf("%c",186);
	printf("Turno : %s",p);
	printf("%c\n",186);

	printf("%c",200);
	for(int i=0; i<k; i++){
		printf("%c",205);	
	}
	printf("%c\n",188);
}

void print_winner_screen(char nome[]){
	int k=strlen(nome)+23;

	printf("%c",201);          //
	for(int i=0; i<k+4; i++){  //
		printf("%c",205);	   //  ROW 1
	}                          //
	printf("%c\n",187);        //

	printf("%c",186);          //
	printf(" %c",201);         //
	for(int i=0; i<k; i++){    //
		printf("%c",205);	   //  ROW 2
	}                          //
	printf("%c",187);          //
	printf(" %c\n",186);       //

	printf("%c",186);                         //
	printf(" %c",186);                        //
	printf(" %s VINCE LA PARTITA !!!",nome);  //  ROW 3
	printf(" %c",186);                        //
	printf(" %c\n",186);                      //

	printf("%c",186);         //
	printf(" %c",200);        //
	for(int i=0; i<k; i++){   //
		printf("%c",205);	  //  ROW 4
	}                         //
	printf("%c",188);         //
	printf(" %c\n",186);      //
		
	printf("%c",200);           //
	for(int i=0; i<k+4; i++){   //
		printf("%c",205);	    //  ROW 5
	}                           //
	printf("%c\n",188);         //
}

void print_player_status(int PN, char nome[], bool czar, int punti, int maxp){
	//per dtampare correttamente le "barre"
	int k=maxp-punti;
	
	//calcola il padding a destra del nome
	//pad = lunghezza del nome piu lungo - lunghezza del nome piu corto
	//int pad=10;
	
	printf("%s: ",nome);
	if(czar==true){
		printf_clr_txt("CZAR","GREEN");
	}
	else{
		//printf_clr_txt("PLAYER","CYAN");
		printf("PLAYER ");
		for(int i=0; i<punti*2; i++){
			printf("%c",178);
		}
		for(int i=0; i<k*2; i++){
			printf("%c",176);
		}
		printf("\n");
	}	
}

void check_file(){
	FILE *fp;
	FILE *fp2;
	fp=fopen("files\\frasi.txt","r");
	fp2=fopen("files\\parole.txt","r");
	
	while(1){
		
		if(fp==NULL){
			printf("File delle FRASI non trovato!\n");		
			printf("Assicurati che sia presente nella stessa cartella di questo eseguibile.\n");
			printf("Quando hai fatto, premi qualsiasi tasto per continuare.\n");
			getchar();
		}
		else{
			break;
		}
		
		if(fp2==NULL){
			printf("File delle PAROLE non trovato!\n");		
			printf("Assicurati che sia presente nella stessa cartella di questo eseguibile.\n");
			printf("Quando hai fatto, premi qualsiasi tasto per continuare.\n");
			getchar();
		}
		else{
			break;
		}
	}
	fclose(fp);
	fclose(fp2);
}

int conta_buchi(char f[]){ //ritorna il numero di gap in una frase
	int gaps=0;
	for(int i=0; i<strlen(f); i++){
		if(f[i]=='_'){
			if(f[i+7]=='_'){
				gaps++;
				i+=7; // :)
			}
		}
	}
	return gaps;
}


char *str_replace_once(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements
    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);
    // count the number of replacements needed
    ins = orig;
	for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    } 
    tmp = result = (char *)malloc(strlen(orig) + (len_with - len_rep) * count + 1);
    if (!result)
        return NULL;
	ins = strstr(orig, rep);
    len_front = ins - orig;
    tmp = strncpy(tmp, orig, len_front) + len_front;
    tmp = strcpy(tmp, with) + len_with;
	orig += len_front + len_rep; // move to next "end of rep"
    strcpy(tmp, orig);
    return result;
}

char *str_replace_always(char *orig, char *rep, char *with){
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements
    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);
    // count the number of replacements needed
    ins = orig;
	for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    } 
    tmp = result = (char *)malloc(strlen(orig) + (len_with - len_rep) * count + 1);
    if (!result)
        return NULL;
    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    } 
    strcpy(tmp, orig);
    return result;
}




int get_window_width(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int get_window_height(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}
