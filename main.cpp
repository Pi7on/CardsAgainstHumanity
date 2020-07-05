//CARDS AGAINS HUMANITY
// Scritto da: Bulgarelli Marco, Barbieri Kevin, Perboni Nicholas
//NOTA: pulire i \n dalle frasi lette (strtok)
//      allineare bene le barre dei giocatori quando le stampi

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "headers\\colored_print.h"
#include "headers\\my_functions.h"

#define LEN 1024      //nota: se settato a 2048 fa scattare una seg. fault
#define MIN_PN 3      //numero minimo di players
#define MIN_POINTS 1  //numero minimo di round da giocare
#define CPP 10        //cards per player

struct Parola{
	char parola[LEN];
	bool usata=false;
};

struct Frase{
	char frase[LEN];
	bool usata=false;
};

struct Giocatore{
	char nome[LEN];
	int punti=0;
	char mazzo[CPP][LEN];       // 10 carte (10 parole)
	char mazzo_usate[CPP][LEN]; //tiene conto della carte usate
	char frase[LEN];           // frase estratta a caso (verra poi modificata con le parole scelte dal player)
	bool czar = false;         // il giudice
	bool winner = false;
}; 

int main(){
	srand(time(NULL));
	int PN=0;                            //player number
	int MAX_POINTS=0;					 //punti da fare per vincere
	char buffer[LEN];                    //buffer generico
	char frase_in_gioco[LEN];            //memorizza la frase con cui si sta giocando nel turno corrente (VERRA' POI MODIFICATA)
	char frase_in_gioco_originale[LEN];  //memorizza la frase con cui si sta giocando nel turno corrente (NON VIENE MODIFICATA MODIFICATA)
	char yn;                             //prende risposta a una domanda yes/no
	int r;      		                 //random generico
	int tot_parole=0;      
	int tot_frasi=0;  
	int gaps=0;                          //memorizza quanti buchi ha la frase in gioco
	int scelta=0;                        //per la scelta della difficolta
	bool difficulty;					 // true=normal(chad) :: false=easy(virgin)
	bool nome_ok = false;                // per il controllo sull'inserimento del czar
	FILE *fp;

	time_t t = time(NULL);               //serve per scrivere il log delle vittorie
	struct tm tm = *localtime(&t);
	

	title();
	disclamer();   
	check_file();    //controlla integrità dei file 
	
	
//##################################################################################################
//           CREZIONE VETTORI E VARIBILI VARIE CHE POI SERVIRANNO NEL MAIN GAME LOOP 
//##################################################################################################

	//conteggio parole presenti nel txt (una riga è intesa come una parola)
	fp=fopen("files\\parole.txt","r");
	while(1){
		if(feof(fp)){
			break;
		}
		fgets(buffer,LEN,fp);
		tot_parole++;
	}
	fclose(fp);
	//creazione instanza
	Parola lista_p[tot_parole];
	//riempio vettore con tutte le parole nel file
	fp=fopen("files\\parole.txt","r");
	for(int i=0;;i++){
		if(feof(fp)){
			break;
		}
		fgets(lista_p[i].parola, LEN, fp);
		strtok(lista_p[i].parola, "\n"); //rimuove \n alla fine delle parole per evitare a capo a caso
	}	
	fclose(fp);


	//conteggio frasi presenti nel file
	fp=fopen("files\\frasi.txt","r");
	while(1){
		if(feof(fp)){
			break;
		}
		fgets(buffer, LEN, fp);
		tot_frasi++;
	}
	fclose(fp);
	//crezione istanza
	Frase lista_f[tot_frasi];
	//riempio vettore con tutte le frasi nel file
	fp=fopen("files\\frasi.txt","r");
	for(int i=0;;i++){
		if(feof(fp)){
			break;
		}
		fgets(lista_f[i].frase, LEN, fp);
	}
	fclose(fp);


	//chiedi numero di players e controlli vari
	title();
	printf("In quanti giocatori siete? (Massimo %d)\n>",(tot_parole/CPP)/2);
	do{
		scanf("%d",&PN);
		fflush(stdin);
		if(PN<3){
			printf_clr_txt("Bisogna essere almeno in 3 per giocare!","RED");
			printf(">");
		}
		if(PN>tot_parole/CPP){
			printf_clr_txt("Siete in troppi! Non ci sono abbastanza parole per tutti!","RED");
			printf(">");
		}
	}while(PN<MIN_PN || PN>tot_parole/CPP);
	
	
	//chiedi quanti round che si volgiono giocare (questo numero influenza ovviamente i punti da fare per vincere)
	int MAX_POSSIBLE_POINTS=(PN-1)*CPP;  //massime carte usate in un round
	MAX_POSSIBLE_POINTS=(tot_parole/MAX_POSSIBLE_POINTS)-1;  
	
	printf("\nQuanti punti bisogna fare per vincere? (Massimo %d)\n>",MAX_POSSIBLE_POINTS);
	do{
		scanf("%d",&MAX_POINTS);
		fflush(stdin);
		if(MAX_POINTS<MIN_POINTS){
			printf_clr_txt("Bisogna minimo fare 2 round per giocare!","RED");
			printf(">");
		}
		if(MAX_POINTS>MAX_POSSIBLE_POINTS){
			printf_clr_txt("Numero troppo alto per la quantita' di giocatori","RED");
			printf(">");
		}
	}while(MAX_POINTS<MIN_POINTS || MAX_POINTS>MAX_POSSIBLE_POINTS);
		
	//creazione array di giocatori
	printf("creazione array giocatori...");
	Giocatore g[PN];
	
	//scelta difficoltà
	system("cls");
	title();
	printf("Scegli la difficolta'\n\n");
	printf_clr_txt("1) CHAD MODE (normal)","YELLOW");
	printf("   Ti verranno assegnate 10 parole all'inizio della partita, e non potrai cambiarle fino alla fine.\n");
	printf("   Gestisci bene le tue parole migliori perche' una volta usate, non saranno piu' disponibili!\n");
	printf("   Questa e' la modalita' per chi predilige la SKILL alla mera fortuna.\n\n");
	printf_clr_txt("2) VIRGIN MODE (easy)","YELLOW");
	printf("   Ti verranno assegnate 10 parole all'inizio della partita, ma ne avrai di nuove ogni turno.\n");
	printf("   Questa e' la modalita' per chi predilige la FORTUNA alla skill.\n\n");
	do{
		printf("> ");
		scanf("%d",&scelta);
		fflush(stdin);
		if(scelta<1 || scelta>2){
			printf_clr_txt("Scelta non valida!","RED");
		}
	}while(scelta<1 || scelta>2);
	if(scelta==1){
		difficulty=true; //normal
	}
	else{
		difficulty=false; //easy
	}

		
	//registrazione nomi players
	system("cls");
	title();
	printf("Inserite il vostro nome:\n\n");
	for(int i=0; i<PN; i++){
		printf("Giocatore %d> ",i+1);
		gets(g[i].nome);
		//controlla che i nomi non siano uguali
		if(i>=1 && strcmp(g[i].nome, g[i-1].nome)==0){
			printf_clr_txt("Nome gia' in uso!","RED");
			i--;
		}
		if(strcmp(g[i].nome,"")==0){
			printf_clr_txt("Il nome non puo' essere vuoto!","RED");
			i--;
		}
	}
	//stampa impostazioni
	printf("\nDifficolta': ");
	if(difficulty){
		printf("CHAD");
	}
	else{
		printf("VIRGIN");
	}
	printf("\nPunti per vincere: %d\nChe vinca il migliore!\n\npremi un tasto per partire",MAX_POINTS); getchar();
	system("cls");
	
	//scelta casuale del czar
	r=rand()%PN;
	g[r].czar=true;
	
	//assegno a ogni giocatore 10 parole random
	//azzeramento dell array delle parole usate
	//tutti i giocatori devono avere parole uniche
	for(int i=0; i<PN; i++){
		if(g[i].czar==false){
			for(int j=0; j<CPP; j++){
				r=rand()%tot_parole;
				if(lista_p[r].usata==false){
					strcpy(g[i].mazzo[j], lista_p[r].parola); //copia nel mazzo
					strcpy(g[i].mazzo_usate[j], "0");         //azzeramento array
					lista_p[r].usata=true;
				}
				else{
					j--; 
				}
			}
		}
	}

//##################################################################################################
//          MAIN GAME LOOP - TERMINA QUANDO UN PLAYER RAGGIUNGE I MAX_POINTS
//##################################################################################################

		
	//main loop, usato un for per poter contare le iterazioni senza variabili esterne
	for(int z=0;;z++){
		
		//scegli una farse bucata random, salvala in frase_in_gioco
		while(1){
			r=rand()%tot_frasi;
			if(lista_f[r].usata==false){
				strcpy(frase_in_gioco, lista_f[r].frase);
				strcpy(frase_in_gioco_originale, frase_in_gioco);
				lista_f[r].usata=true;
				break;
			}
		}
		
		//conta i buchi della frase, in modo da poter fare il parse della scelta correttamente
		gaps=conta_buchi(frase_in_gioco);
		
		//il giocatore dovra scegliere tante parole quanti sono i buchi della frase
		int scelta[gaps]; //crea con new, cosi poi puoi cencellar
		
		//azzera array delle scelte
		for(int j=0; j<gaps; j++){
			scelta[j]=0;
		}

		//cicla i turni dei PN giocatori
		for(int i=0; i<PN; i++){
			
			if(g[i].czar==false){
				title();
				
				// stampa stato giocatori
				for(int j=0; j<PN; j++){
					print_player_status(PN, g[j].nome, g[j].czar, g[j].punti, MAX_POINTS);	
				}
				printf("\n");
				
				//stampa round corrente
				n_round(z+1);
			
				//stampa frase in gioco (nel round corrente)
				printf("%s\n",frase_in_gioco);
				
				//stampa a che giocatore tocca inserire una parola
				print_turno(g[i].nome);
				
				//if che divide le due difficolta
				if(difficulty==true){ //se gioco a difficolta normale
				
					//stampa menu delle parole del giocatore, numerate
					for(int j=0; j<CPP; j++){
						if(strcmp(g[i].mazzo_usate[j], "0")==0){
							//printf("%02d) %s [disponibile]\n",j+1, g[i].mazzo[j]);
							printf("%02d) ",j+1);
							printf_clr_txt(g[i].mazzo[j], "GREEN");
						}
						else{
							//printf("%02d) %s [usata]\n",j+1, g[i].mazzo[j]);
							printf("%02d) ",j+1);
							printf_clr_txt(g[i].mazzo[j], "RED");
						}
					}
				
					//aspetta input
					if(gaps==1){
						N_ASK_ONE: //N sta per normal
						printf("\nSceglia una parola\n> ");
						scanf("%d",&scelta[0]);
						fflush(stdin);
					
						if(strcmp(g[i].mazzo_usate[scelta[0]-1],"0")==0){ //se la carta non è ancora stata usata
							strcpy(g[i].frase, str_replace_once(frase_in_gioco, "________", g[i].mazzo[scelta[0]-1])); //inserisci parola in frase
							strcpy(g[i].mazzo_usate[scelta[0]-1], "1");												   //rendi parola usata non piu disponibile
						}
						else{
							printf_clr_txt("Parola gia usata!","RED");
							goto N_ASK_ONE;
						}
					}
					if(gaps>1){
						printf("\nScegli %d parole\n",gaps);
						for(int j=0; j<gaps; j++){
							N_ASK_MORE:
							printf("> ");
							scanf("%d",&scelta[j]);
							fflush(stdin);
						
							if(strcmp(g[i].mazzo_usate[scelta[j]-1],"0")==0){ //se la carta non è ancora stata usata
								//per ogni gap, sostituisci la scelta ai trattini
								strcpy(g[i].frase, str_replace_once(frase_in_gioco, "________", g[i].mazzo[scelta[j]-1])); //inserisci parola in frase
								strcpy(frase_in_gioco, g[i].frase);			                                               //aggiorna frase in gioco per il prossimo ciclo di j
								strcpy(g[i].mazzo_usate[scelta[j]-1], "1");                                                //rendi parola usata non piu disponibile
							}
							else{
								printf_clr_txt("Parola gia usata!","RED");
								goto N_ASK_MORE;  
								
							}			    
						}
					}
				
					// errore in caso la frase non contenga gaps
					if(gaps<1){
						printf_clr_txt("ERRORE, Sembra che una delle frasi nel file FRASI.txt non contanga un buco, rendendola quindi inutilizzabile.","RED");
						printf_clr_txt("Controlla il file e correggi l'errore.","RED");
						return -1;
					}
				}
		
				else{ //se la difficolta' è easy
					
					//ad ogni turno do parole nuove ai giocatori
					for(int j=0; j<PN; j++){
						if(g[j].czar==false){
							for(int k=0; k<CPP; k++){
								r=rand()%tot_parole;
								if(lista_p[r].usata==false){
									strcpy(g[j].mazzo[k], lista_p[r].parola); //copia nel mazzo
									lista_p[r].usata=true;
								}
								else{
									k--; 
								}
							}
						}
					}
					
					//stampa menu delle parole del giocatore, numerate
					for(int j=0; j<CPP; j++){
						printf("%d) %s\n",j+1, g[i].mazzo[j]);
					}
					
					//aspetta input
					if(gaps==1){
						E_ASK_ONE: //E sta per easy
						printf("\nSceglia una parola\n> ");
						scanf("%d",&scelta[0]);
						fflush(stdin);
						if(scelta[0]>=1 && scelta[0]<=10){
							strcpy(g[i].frase, str_replace_once(frase_in_gioco, "________", g[i].mazzo[scelta[0]-1])); //inserisci parola in frase                                 
						}
						else{
							printf_clr_txt("Scelta non valida!","RED");
							goto E_ASK_ONE;
						}	 
						
						
					}
					if(gaps>1){
						printf("\nScegli %d parole\n",gaps);
						for(int j=0; j<gaps; j++){
							E_ASK_MORE:
							printf("> ");
							scanf("%d",&scelta[j]);
							fflush(stdin);
						
							if(strcmp(g[i].mazzo_usate[scelta[j]-1],"0")==0){ //se la carta non è ancora stata usata
								//per ogni gap, sostituisci la scelta ai trattini
								strcpy(g[i].frase, str_replace_once(frase_in_gioco, "________", g[i].mazzo[scelta[j]-1])); //inserisci parola in frase
								strcpy(frase_in_gioco, g[i].frase);			                                               //aggiorna frase in gioco per il prossimo ciclo di j
								strcpy(g[i].mazzo_usate[scelta[j]-1], "1");                                                //rendi parola usata non piu disponibile
							}
							else{
								printf_clr_txt("Non puoi scegliere due parole uguali nello stesso turno!","RED");
								goto E_ASK_MORE;  
								
							}			    
						}
					}
					
					// errore in caso la frase non contenga gaps
					if(gaps<1){
						printf_clr_txt("ERRORE, Sembra che una delle frasi nel file FRASI.txt non contanga un buco, rendendola quindi inutilizzabile.","RED");
						printf_clr_txt("Controlla il file e correggi l'errore.","RED");
						return -1;
					}
					
					//resetta le parole usate perche siamo in easy mode
					for(int j=0; j<PN; j++){
						for(int k=0; j<CPP; j++){
							strcpy(g[i].mazzo_usate[j], "0");
						}
					}
					
					//resettare la frase in gioco a quella vecchia coi buchi prima di cambiare giocatore
					//strcpy(frase_in_gioco, frase_in_gioco_originale);
				}
				
				//resettare la frase in gioco a quella vecchia coi buchi prima di cambiare giocatore
				strcpy(frase_in_gioco, frase_in_gioco_originale);
			}
			system("cls");
		
		}
		
		//turno del CZAR e assegnazione punteggio
		for(int i=0; i<PN; i++){
			if(g[i].czar==true){
				title();
				
				// stampa stato giocatori
				for(int j=0; j<PN; j++){
					print_player_status(PN, g[j].nome, g[j].czar, g[j].punti, MAX_POINTS);	
				}
				printf("\n");
				
				//stampa round corrente
				n_round(z+1);
			
				//stampa frase in gioco (nel round corrente)
				printf("%s\n",frase_in_gioco);
				
				print_turno("CZAR");
				
				//stampa le frasi generate dai players
				for(int j=0; j<PN; j++){
					if(g[j].czar==false){
						printf("%s:\n %s\n", g[j].nome, g[j].frase);
					}
				}
				
				//scelta del vincitore del round e controllo su inserimento nome
				do{
					printf("Nome del vincitore> ");
					gets(buffer);
					for(int i=0; i<PN; i++){
						if(g[i].czar==false && strcmp(g[i].nome, buffer)==0){
							nome_ok=true;
						}
					}
					if(nome_ok==false){
						printf_clr_txt("Questo giocatore non esiste!","RED");
					}
				}while(nome_ok==false);
				nome_ok=false; //reset per il prossimo ciclo;
				
				//stampa
				for(int j=0; j<PN; j++){
					if(g[j].czar==false && strcmp(buffer, g[j].nome)==0){
						printf("\n\n%s ",g[j].nome);
						printf_clr_txt("VINCE IL ROUND!","YELLOW");
						g[j].punti++;
						printf("Premi un tasto qualsiasi per avanzare al prossimo round.\n");
						getchar();
						system("cls");
						break;
					}
				}
				break;
			}
		}
		

		//controllo per rompere il main game loop
		for(int j=0; j<PN; j++){
			if(g[j].punti>=MAX_POINTS){
				print_winner_screen(g[j].nome);
				
				//salva log vittoria
				fp=fopen("files\\log.txt","a+");
				fprintf(fp, "[%d-%d-%d | %d:%d] %s VINCE | %s", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, g[j].nome, g[j].frase);
				fclose(fp);
				goto END_SESSION;
			}
		}
		
	}
	
	END_SESSION:
	system("pause");
	
	return 0;
}







