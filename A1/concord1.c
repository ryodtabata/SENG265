#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>


#define MAX_EX_WORDS 21
#define MAX_EX_WORDS_LEN 21
#define MAX_LINES 101
#define MAX_LINES_LEN 71
#define MAX_WORDS 700 
#define MAX_WORDS_LEN 21

char readline[MAX_LINES_LEN]; /* read input one line at a time*/
char exclusion_words[MAX_EX_WORDS][MAX_EX_WORDS]; /* array holding all exclusion words */
char input_lines[MAX_LINES][MAX_LINES_LEN]; /* array holding all lines minus exclusion words */
char words[MAX_WORDS][MAX_WORDS_LEN]; /* contains all final words// without exclusion word */
char backup[MAX_LINES][MAX_LINES_LEN]; /* backup for because strncpy messes up array */
char uppercaseword[MAX_LINES_LEN]; /* used when replacing lowercase word with uppercse word */
char buffer[MAX_LINES_LEN];

int ex_count;
int line_counter;
int word_counter;

void capitalize_word_in_string(char*);   /* sends each word thats not exclusion word to get capitalized */
int is_exclusion_word(char*);            /* checks if word is exclusion word */
void alphabetize(void);                  /* puts all words in order */
void makes_upper(char *, int);           /* helper function to make word uppercase */
void makes_lower(char *, int);           /* helper function to make the rest of the phrase lowercase again */
char* give_space(char *);                /* used to make input words surrounded by spaces, so would not be triggered if is contained into another word */ 
int forsure(char*, int);
int is_it_in_already(char*);             /* is word already in array of words */



int main (int argc, char *argv[]){
	fgets(readline, MAX_LINES_LEN,stdin);    /* gets rid of 1*/
	fgets(readline, MAX_LINES_LEN, stdin);   /* gets rid of ''''*/	
	int stop;
	while (fgets(readline,MAX_LINES_LEN,stdin)){
	stop = strncmp(readline,"\"\"\"\"\n",MAX_LINES_LEN);
	if (stop!=0){
		ex_count++;
		readline[strcspn(readline,"\n")] =0;
		strncpy(exclusion_words[ex_count-1],readline,MAX_EX_WORDS_LEN);
	}
	if (stop==0){
		break;
	}
	}
	
	
	/* for input lines, made a backup array of input words so it would not be messed up with my tokenization, which seemed to cause errors to my input array?*/
	while(fgets(readline, MAX_LINES_LEN,stdin)){
		readline[strcspn(readline,"\n")] =0;
		strncpy(input_lines[line_counter],readline,MAX_LINES_LEN);
		strncpy(backup[line_counter],readline,MAX_LINES_LEN);
		line_counter++;
	}

	
	/* tokenizing */
	int i;
	int t;
	int k;
	for (i=0; i<MAX_LINES_LEN; i++){
		char* token = strtok(input_lines[i], " ");
			while (token != NULL) {
				t = is_exclusion_word(token);
				k = is_it_in_already(token);
				if(t==0 && k==0) {		
				strncpy(words[word_counter],token, MAX_LINES_LEN);
				word_counter++;
				}
			token = strtok(NULL, " ");
			}		
	}	


	alphabetize();	

	int p;
	for(p=0;p<word_counter;p++){
	char* t;
	t= give_space(words[p]);
	capitalize_word_in_string(t);
	}
	
return 0;		
}



/* helper functions */



char* give_space(char* str) { 
	static char temp[MAX_WORDS_LEN + 2];
	strcpy(temp," "); 
	strcat(temp,str);
	return temp;
}


void makes_lower(char *str,int length){
strcpy(uppercaseword,str);
int i=0;
       while (uppercaseword[i]){
             uppercaseword[i] = tolower(uppercaseword[i]);
             i++;
       }
strncpy(str,uppercaseword,length);
}



void makes_upper(char *str, int length){
strcpy(uppercaseword,str);
int i=0;
       while (uppercaseword[i]){
             uppercaseword[i] = toupper(uppercaseword[i]);
             i++;
       }
strncpy(str,uppercaseword,length);
}


void capitalize_word_in_string(char* index){
int length; 
int i;
length = strlen(index);
for(i=0;i<line_counter;i++){
	char* input = backup[i];
	char* result;
	result = strstr(input,index) ;/* add anohter function which will take pointer and index and strcmp to eachother to ensure word is truely in word */
	if (result != NULL){
		makes_upper(result, length);
		printf(backup[i]);
		printf("\n");
		makes_lower(result,length);
	}
}
}

int forsure(char* index, int lengh) {
index = buffer; 


}


int is_exclusion_word(char* token){
	int i;
	for(i=0;i<ex_count;i++){
	if (strcmp(token,exclusion_words[i])==0){
		return 1;
	}}
return 0;
}
	

int is_it_in_already(char* token){
	int i; 
	for (i=0; i<word_counter; i++){
	if (strcmp(token,words[i]) ==0){
		return 1;
	}}
	return 0;
}
		
void alphabetize(void){
	int z,x;
	char temp[MAX_WORDS_LEN];
	for(z=0; z<word_counter-1; z++){
        for(x=z+1; x<word_counter; x++){
                if(strcmp(words[z],words[x])>0){
                        strcpy(temp,words[z]);
                        strcpy(words[z],words[x]);
                        strcpy(words[x],temp);
                        }
                }               
	}
}	
