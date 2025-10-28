//replace all the gaps in the sentence with hyphens
//also output how many spaces were replaced by hyphens

#include <stdio.h>

//return function hs to be above main as I am calling the return function in the main

int replace(char* str) // replacing any spaces
{
 //need a pointer so the code can go through the full sentence and figure out where the gaps are to replace with -
 char* pointer = str;
 int num = 0;

 while (*pointer != '\0') //while the pointer is not at the end
 {
     if (*pointer == ' ') //if gap
     {
       *pointer = '-'; //put - instead of gap
        num++;//add 1 to num of spaces that have been replaced
     }
     pointer++; //move onto the next character in the sentence
 }
 
 return num;
}

int main(void)
{
    char sentence[] = "The cat sat on the mat."; //The sentence
    printf("Originally, the sentence was: %s\n", sentence);
    //even though the sentence is made through characters, it is still a string originall
    // '%s\n' means string type then new line
    
    int newsentence = replace(sentence); //replace the original sentence with the new through replace function
    printf("Now, the sentence is: %s\n", sentence); //output modified sentence
    printf("The number of spaces that were replaced is: %d\n", newsentence); //output num of spaces replaced (calculated in return function)
    
    return 0;
}
