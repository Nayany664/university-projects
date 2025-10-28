//take a string (the alphabet in this case
//replace each lower case letter and replace with the assigned new letter
//e.g  original letter is a, new letter is b
//e.g original letter is b, new letter is p
//etc
//leave uppercase letters and the punctuation the same
//show both original and encoded sentences

#include <stdio.h>
#include <string.h>

void code(cahr *pointer)
{
    char code1[] = "abcdefghijklmnopqrstuvwxyz"; //original alphabet
    char code2[] = "bpduhijkaltxwmrzfoysvngeqc"; //encrypted alphabet
    //each letter in the encryption code matches a letter in the original alphabet. so a goes with b, l goes with x etc.
    //only lower case letters are affected
    
    for (size_t i = 0; pointer[i] != '\0'; ++i) //a for loop so it can go through each line of the sentence
    //size_t i = 0 means the very start
    //pointer[i] != '\0' means keep going till i is at the end. Once at end, stop
    // to go onto the next letter and not stay on that one letter, increment with ++i
    { //whilst it's not at the end do this:
        if (pointer[i] >= 'a' && pointer[i] <='z') //checking if the letter is lowercase and in code1 that was started above
        //also each letter has a numerical ascii value. Since this is lowercase, the values are between 97 (a) and 122 (z)
        { //of lowercase do this:
            for (int j = 0; j < 26; ++j) //26 letters so go from numbers 0 - 25. could have done <=25 instead of <26 but this is neater and easier to understand
            //j is what the letter in code2 that you replace the original letter with
            {
                if (pointer[i] == code1[j]) //compare with code1 and checks if j matches
                {
                    pointer[i] = code2[j]; //trplace the original with the encrypted letter
                    break; //this letter is done
                }
            }
        }
    }
} //check next letter until the end 

int main(void)
{
    char sentence[] = "The cat sat on the mat."; //The sentence
    size_t length = strlen(sentence); //find the length of the sentence
    if (length > 0 && sentence[length - 1] == '.') //while the sentence is not at the very beginning AND before the full stop
    {
        sentence[length - 1] = '\0'; //the end of a sentence in the memory is \0
    }
    printf("The original sentence is: %s\n", sentence); //print out the original sentence

    code(sentence); //put the original sentence through the code function

    printf("The encoded sentence is : %s\n", sentence); //print out the encrypted sentence

    return 0;
}
