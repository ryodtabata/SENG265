/*
 * concord3.c
 *
 * Starter file provided to students for Assignment #3, SENG 265,
 * Fall 2022.
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "emalloc.h"
#include "seng265-list.h"


#define MAX_WORD_LEN 41
#define MAX_INPUT_LEN 101


void print_correctly(node_t* ,node_t*);
char* word_find(node_t*, node_t*);
void capitalize(char*, int, node_t*, node_t*);
char* is_in_list(char*, node_t*);
void front_end(char*, char*, int);
void back_end(char*);
void properspacing(char*, char*);
void lowercase(char*, int, node_t* , node_t*);
void trimtrailing(char *);
char *make_lower(char* );


void print_word(node_t *node, void *arg)
{
    char *format = (char *)arg;
    printf(format, node->text);
}


int main(int argc, char *argv[])
{   
    int error;
    char buffer[MAX_INPUT_LEN];
    fgets(buffer,MAX_INPUT_LEN,stdin); 
    error = strncmp(buffer, "2\n", MAX_INPUT_LEN);
    if (error != 0){
        printf("Input is version 1, concord3 expected version 2");     //if file is of wrong type
        return;
    }
    //removes first line of quotes 
    fgets(buffer, MAX_INPUT_LEN,stdin);    
    int stop;
    int count_exclusion_words = 0;
    int input_lines = 0;

    node_t *temp_node = NULL;
    node_t *head_exclusion = NULL;
    int buffer_length;
    // enters values into exclusion words 
    while (fgets(buffer,MAX_INPUT_LEN,stdin))
    {
        stop = strncmp(buffer, "\"\"\"\"\n", MAX_INPUT_LEN);
        if (stop!=0)
        {
            buffer_length = strlen(buffer);
            buffer[buffer_length-1]= '\0';
            temp_node = new_node(buffer);
            head_exclusion = add_end(head_exclusion, temp_node);
            count_exclusion_words++;
        }
        else if (stop == 0)
        {
            break;
        }
    }

    // now make input lines linked list

    node_t *head_input_lines= NULL;
    node_t *temp_node2 = NULL;
    char * ptr;

    while(fgets(buffer,MAX_INPUT_LEN,stdin))
    {    
        buffer_length = strlen(buffer);
        if (buffer_length != 1)
        {
        buffer[buffer_length-1]= '\0';
        ptr = buffer;
        trimtrailing(ptr);
        temp_node2 = new_node(buffer);    
        head_input_lines = add_end(head_input_lines, temp_node2);
        input_lines++; 
        }
    }

    node_t *curr = NULL;
    curr = head_input_lines;
    node_t *head_key_words = NULL; 
    int key_words_count = 0;
    char* i;
    char* k;
    //adding key words list in order so prints alphabetically 
    while (curr!=NULL)
    {
        strcpy(buffer,curr);  
        char* token = strtok(buffer, " ");
        while (token != NULL){
            i = is_in_list(token,head_exclusion);             //check if word is in exclusion words or already in tokens 
            k = is_in_list(token,head_key_words);
            if (i==NULL){
                if (k==NULL){

                    temp_node2 = new_node(token);
                    head_key_words = add_inorder(head_key_words, temp_node2);    //adding in order 
                    key_words_count++;
                }
            }
            token = strtok(NULL, " ");
        }
        curr = curr->next;
    }

    print_correctly(head_input_lines, head_key_words);
    freeList(curr);
    freeList(temp_node2);
    freeList(temp_node);
    freeList(head_key_words);        //freeing all nodes 
    freeList(head_exclusion);
    freeList(head_input_lines);
    
}




 void print_correctly(node_t* head_input_lines, node_t* head_key_words) //sendes key words to be capitialized
{

    int length;
    node_t *curr_input = NULL;
    curr_input = head_input_lines;
    node_t *curr_key = NULL;
    curr_key = head_key_words; 
    char *p = NULL;
   

    while (curr_key->text!=NULL) {
        while(curr_input!= NULL)
        {
            length = strlen(curr_key);
            p = word_find(curr_key,curr_input);     //finding occurance of word in sentence 
            if (p!=NULL)
            {
                capitalize(p,length,curr_input,head_key_words);    //capitialized the word being pointed to 
            }
            curr_input = curr_input->next;
        }
        curr_input = head_input_lines;
     curr_key = curr_key->next;    
    }
 
return;
}

void capitalize(char* p, int len, node_t* input, node_t* head_key_words) //puts values into uppercase and replaces its value with, in original string
{ 
    char* temp = NULL;
    char buffer[MAX_INPUT_LEN];
    strcpy(buffer,p);
    for (int i; i<len; i++){
        buffer[i] = toupper(buffer[i]);
    }
    temp = buffer;
    int length = strlen(temp);
    memmove(p,temp,length);       //replacing word with capital word
    properspacing(input,p);
    lowercase(p,len,input,head_key_words);   //replacing it back with lowercase 
}

void lowercase(char* p, int len, node_t* input, node_t* key_head){   //makes value lowercase again so it is back to nommal in original string
    char string[MAX_WORD_LEN] = "";
    char* temp = NULL;
    int i = 0;
    char buffer[MAX_INPUT_LEN];
    strcpy(buffer,p);  
    for (i; i<len; i++){
        buffer[i] = tolower(buffer[i]);
        string[i] = buffer[i];
    }
    temp = string;
    temp = is_in_list(temp,key_head);
    int length = strlen(temp);
    memmove(p,temp,length);
}

void properspacing(char* sentence, char* p){                         //figures out how many spaces needed infront of string
    int length_sentence = strlen(sentence);
    int length_word_and_after = strlen(p);
    int starting_index = length_sentence - length_word_and_after;    //finds differenece between values 
    front_end(sentence,p,starting_index);
}

void front_end(char* sentence, char* p, int starting_index){         //adds the spaces given by properspacing to the front of the string
    int i;
    int j=0;
    char string[MAX_INPUT_LEN] = "";
    if (starting_index >29){
       sentence = sentence + starting_index - 29;
    }
    for(i=0;i<29-starting_index;i++){         //adding the difference between 30 and index in spaces 
        strcat(string," ");
    }
    strcat(string, sentence);
        while (j<8 || string[j]!= ' '){       //removes cutoff words 
            string[j] = ' ';
            j++;
        }
    back_end(string);       //sends to function to work on the backend of code 
}

void back_end(char* string){    //removes curoff words on back of string 
    int length =0;
    char* ptr;
    if (strlen(string)<= 60){
        printf("%s",string);
        printf("\n");
    }
    else
    {
        int i = 60;
        while (string[i] != '\0' && string[i] != ' '){
            i--;
        }
        string[i] = '\0';
        printf("%s", string);
        printf("\n");
    }
}

void trimtrailing(char * str) 
{
    int index;
    int i =0;

    index = -1;

    // rmeoves all the whitespace 

    while(str[i] != '\0')
    {
        if(str[i] != ' ')
        {
            index= i;
        }
        i++;
    }
    str[index + 1] = '\0';
}

char* word_find(node_t* key, node_t* input_line)    //finds word in sentenece 
{
    char* pointer = strstr(input_line,key);     //using strstr without case sensitivity
    if (pointer == NULL && strcasestr(input_line,key)==NULL){
        return NULL;
    }
    if (pointer!=NULL){


        char buffer[MAX_WORD_LEN];
        char buffer2[MAX_WORD_LEN];
        strcpy(buffer,pointer-1);
        strcpy(buffer2,pointer+strlen(key));
        int i = buffer[0] == ' ';
        int b = buffer2[0] == ' ';
        int c = buffer2[0] == '\0';
        int pointer_length = strlen(pointer);
        int input_length = strlen(input_line);

        if (pointer_length == input_length && (b || c)){       //checking before and after word if it is a space or null character 
            return pointer;
        }

        if (i && (b || c)){
            return pointer;
        }
        else {
            pointer = strstr(input_line,key);   //otherwise checking the next occurance of word 
            while (pointer){
               pointer = strstr(pointer+1, key);
               if (strlen(key)<3){
                    return pointer;
                }
            }

        }

    }
    
    return NULL;
}

char* is_in_list(char* token, node_t* head)    //fucntion used to see if words already in given linked lists 
{         
    node_t *curr = NULL;
    curr = head;
    while (curr!=NULL) {
        if (strcasecmp(token,curr)==0)
        {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void freeList(node_t *head){          //frees all nodes in list 
    node_t *temp_node = NULL;
    temp_node = head;
    while(temp_node != NULL){
        assert(temp_node != NULL);
        head = remove_front(head);
        free(temp_node);
        temp_node = head;
    }
    assert(head==NULL);
}
