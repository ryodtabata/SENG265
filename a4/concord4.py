import sys
import re
import fileinput

class concord:

    def __init__(self, input=None, output=None):
        self.input = input
        self.output = output
        self.exclusion_words = []        
        self.sentences = []
        self.words = []
        self.list_part_of_sentence = []  #holds list of all tuples of 3 parts of sentence 
        concord.__iteratelines(self)     #go through each line of input 
        if input!=None:                  #if input name is given then we know to write it to a file
            concord.__tofile(self)


    def __tofile(self):
        lines = concord.full_concordance(self)    #since full_concordance is not called in new driver, we have to call it
        f = open(self.output, "w")                #open the given name 
        for line in lines:                        #write each of our lines to the file
            f.write(line)
            f.write("\n")
        f.close()
        

    def full_concordance(self):
        final_list = []
        concord.__makeindexwords(self)               #will split into correct tokens 
        self.words.sort() 
        sentences = concord.__capitalize(self)
        for elements in sentences:
            tup=()
            index = elements[0].find(elements[1])
            tup = (elements[0][0:index],elements[0][index:index+len(elements[1])],elements[0][index+len(elements[1]):])
            s = concord.__print_properly(self,tup)
            final_list.append(s)
        return final_list


    def __iteratelines(self):
       exclusion_words_complete = False              #how we know when we pass the exclusion words 
       for line in fileinput.input(self.input):      #if file name is given we will use it, else we will go to stdin automatically
            if fileinput.isfirstline():
                format_number = (line)
                format_number = format_number.rstrip()
                if(format_number !='2'):
                    print("Input is version 1, concord2.py expected version 2")
                    return 0
                continue
            line= line.rstrip()
            if line=="\"\"\"\"":
                exclusion_words_complete = True
                continue
            elif (exclusion_words_complete==False):
                line = line.lower()
                line= line.rstrip()
                self.exclusion_words.append(line)
            elif (exclusion_words_complete==True):
                line = line.rstrip()
                self.sentences.append(line) 
       fileinput.close()               


    def __makeindexwords(self):
        for line in self.sentences:
            list_of_words = line.split(" ")
            for word in list_of_words:
                if word not in self.exclusion_words and word not in self.words:
                    self.words.append(word)
    

    def __capitalize(self):
        list_all_sentences = []
        for word in self.words:
            for sentence in self.sentences:
                TEXTO = word
                regex = r"\b" + re.escape(TEXTO) + r"\b"
                match = re.search(regex, sentence, re.IGNORECASE)
                if match != None:
                    capital = word.upper()
                    new_sentence = (re.sub(regex, capital, sentence, re.IGNORECASE), capital)
                    list_all_sentences.append(new_sentence)
        return(list_all_sentences) 


    def __print_properly(self,tup):
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
            formatted_string = concord.__remove_cut_offs_first(self,formatted_string,tup[0],tup[1])
        
        max_length = 31-length_word                              #max length of string allowed 
        if (length_after_word <= max_length):                    #case 1, where words after index word are less than max_length 
            formatted_string = formatted_string + tup[2]

        else:
            returnstring = concord.__delete_ends(self,tup[2][0:max_length+1])   #case 2, where words after index are greater than max_length and need to be cut-off
            formatted_string = formatted_string + returnstring
        return(formatted_string)


    def __delete_ends(self,returnstring):                               #function will delete all the cut off strings at the end of a string 
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


    def __remove_cut_offs_first(self,string,words,index_word):          #function removes all words that are cut off before the word
        index_word = index_word.rstrip()
        list_words = words.split(" ")
        list_string_words = string.split(" ")                    #split into lists to easily mod
        for i in range(len(list_string_words)):
            if ((list_string_words[i] not in list_words) and list_string_words[i] != index_word):
                list_string_words[i] = ' '*len(list_string_words[i])
        format_string = ' '.join(list_string_words)              #iterate through list of words, and if we have a word not in the list of words ie a cutoff, we repalce with spaces, to keep our correct spacing 
        return format_string


