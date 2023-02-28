#!/usr/bin/env python3
import sys
import re 

exclusion_words = []        #List to hold all excusion words
sentences = []              #Holds all sentences
words = []                  #Hold all words without exclusion words
split_words = []            #used to split words into seperate words 
list_part_of_sentence = []  #holds list of all tuples of 3 parts of sentence 
single_word_value = []      #holds index words that are 1 len

def main():
    format_number = (sys.stdin.readline())
    format_number = format_number.rstrip()
    if(format_number !='2'):
        print("Input is version 1, concord2.py expected version 2")
        return 0
    sys.stdin.readline()
    line = sys.stdin.readline()
    while (line):
        line= line.rstrip()
        if line=="\"\"\"\"":
            line = sys.stdin.readline()
            break
        else:
            line = line.lower()
            exclusion_words.append(line)      #adding exclusion words to list 
            line =sys.stdin.readline()       
    while (line):                             #will skip over blank lines                             
        line = line.rstrip()      
        sentences.append(line)                #adding all other lines after to list of sentences 
        line = sys.stdin.readline()          
     
    split_words()                             #will split into correct tokens 
    words.sort()                              #puts words into alphebtic order
    make_capital_word(words,sentences)        #will capitalize index words, and tokenize each input line into desired tuples 

    for tup in list_part_of_sentence:         #goes through each token and prints out correctly, unless tuple is empty
            if tup[1] != "":
                print_properly(tup)
        
   
def split_words():                            #goes through each input line and splits words into own tokens, only if word is not in exlcusion words, or if its not already added to words list      
    for phrase in sentences:                  
        phrase = phrase.lower()
        split_words = phrase.split(" ")
        for x in split_words:
            if x not in exclusion_words and x not in words: 
                words.append(x)
            if len(x)== 1:
                single_word_value.append(x)
    
   

def print_properly(tup):
    MAX_WORDS = 19                                           #iterates through list of tuples and prints tuple in desired output 
    length_before_word = len(tup[0])
    length_word = len(tup[1])
    length_after_word = len(tup[2])
    if (length_before_word <=MAX_WORDS):                     #case 1 where the string is less than max number of words, 19.
        formatted_string = " "*10
        formatted_string = ''.join([formatted_string," "*(19-length_before_word),tup[0]])
        formatted_string = ''.join([formatted_string,tup[1]])

    else:                                                    #case 2, where the string is greater than max number of words and needs to be cut off       
        formatted_string = " "*9
        formatted_string = ''.join([formatted_string,tup[0][-20:]])
        formatted_string = ''.join([formatted_string,tup[1]])
        formatted_string.rstrip() 
        formatted_string = remove_cut_offs_first(formatted_string,tup[0],tup[1])
           
    max_length = 31-length_word                              #max length of string allowed 
      
    if (length_after_word <= max_length):                    #case 1, where words after index word are less than max_length 
        formatted_string = formatted_string + tup[2]
        
    else: 
        returnstring = delete_ends(tup[2][0:max_length+1])   #case 2, where words after index are greater than max_length and need to be cut-off
        formatted_string = formatted_string + returnstring

    print(formatted_string)

  
def delete_ends(returnstring):                               #function will delete all the cut off strings at the end of a string 
    if returnstring[len(returnstring)-1] == " ":
        returnstring = returnstring.rstrip()                 #if string just needs be be restripped
        return returnstring  
    list_chars = list(returnstring)
    i = len(returnstring)-1
    while list_chars[i] != " ":                              #otherwise, delete words until we have a non space character 
        list_chars[i] = ''
        i = i-1
    returnstring = ''.join(list_chars)
    returnstring = returnstring.rstrip()                     #rejoin string and return 
    return returnstring

def remove_cut_offs_first(string,words,index_word):          #function removes all words that are cut off before the word
    index_word = index_word.rstrip()
    list_words = words.split(" ")
    list_string_words = string.split(" ")                    #split into lists to easily mod
    for i in range(len(list_string_words)):
        if ((list_string_words[i] not in list_words) and list_string_words[i] != index_word): 
            list_string_words[i] = ' '*len(list_string_words[i])  
    format_string = ' '.join(list_string_words)              #iterate through list of words, and if we have a word not in the list of words ie a cutoff, we repalce with spaces, to keep our correct spacing 
    if format_string[9] in single_word_value:                #if the first word of string is an a single word, and not a cutoff                          
       if format_string[10] == " ":                          #check if letter after is a space i.e not a cut-off from the front
           if words[-21] != " ":                             #chck if letter before is a space, i.e not a cut-off from the back
               list_string_words[9] = " "                    
               format_string = ' '.join(list_string_words)   #reaplce with a space  
    return format_string 
                   

def make_capital_word(words,sentences):                     #goes through all key words and sends to capitalize function if not a keyword 
    for word in words:                                         
       for sentence in sentences:
            if not_subword(sentence,word): 
                capitalize_format(sentence,word)            #sends to fucntion to place capital in right spot


    
def not_subword(sentence,word):                             #function compares the word and sentence to ensure the word is not a subword 
     sentence2 = sentence.lower()
     sentence_words = sentence2.split(" ")
     for i in sentence_words:
         if i==word:
             return True
     return False

def tokenize(sentence,word,start_index)   :                 #function takes each line of input, and splits between before the word, index word itself, and after the word 
    first_part = sentence[:start_index]
    second_part = sentence[start_index:start_index+len(word)+1]
    third_part = sentence[start_index+len(word)+1:]
    sentence_tuple = (first_part, second_part, third_part)
    list_part_of_sentence.append(sentence_tuple)
    

def capitalize_format(sentence,word):                       #goes through sentence and splits word into before word,after word and word itself, and capitalized the word 
    sentence2 = sentence + "a"                              #making another string to not mess up the uppercases
    sentence2[:-1]
     
    sentence2 = sentence2.lower()
    occurances = re.findall(word, sentence2)
    if len(occurances) <=1 :                                #this checks to see if we have the word pop up twice *subword*
        start_index = sentence2.find(word)
    else:
        start_index = double_occurance(occurances,sentence2)#if so we make sure to find the actual word not the subword 
    list_words = sentence2.split(" ")
    i = 0 
    list_words2 = sentence.split(" ")
    for i in range(len(list_words)-1):
        if list_words[i] == word:
            break                                           #if we find the word, break out so we have the right count 
        else:
            i = i+1
    list_words2[i] = list_words2[i].upper()    
    new_string = ' '.join(list_words2)
    if start_index == -1:                                   #if word is at end of string and cannot be found
        start_index = len(sentence) -len(word)              #making index = proper number
        word = word.lower()                                 #making all words lowercase to find match 
    tokenize(new_string,word,start_index)                   #sends strings to be tokenized                
   
def double_occurance(occurances, sentence):                 #if we run into a subword, we add a space to find the word by itself
    word =  occurances[0] + " "                                      
    return sentence.find(word)
    

if __name__ == '__main__':
        main()                  
