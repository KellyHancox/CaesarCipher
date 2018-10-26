// Project: Caesar Ciphers
// Author: Kelly Hancox

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void readFreq (float given[], FILE *letFreq);
void calcFreq (float found[],FILE* datafile);
char rotate (char ch, int num);
int findKey (float given[], float found[]);
void decrypt (int key, FILE * datafile, FILE* outfile);

/** Load array given with the letter frequencies
 for English from file letFreq.txt
 @param given[] the array storing frequencies
 @param *letfreq the file with frequencies
 @return void
 */
void readFreq (float given[], FILE *letFreq){ 
	char ch;

	for(int i = 0; i< 26; i++){
		fscanf(letFreq, "%c%f%c",&ch,&given[i],&ch); 
	}
}

/** Read the encoded text from an input
 file and accumulate the letter frequency
 data for the encoded text. Store the frequency data in array found.
 @param found[] array storing letter frequencies
 @param *datafile the file to count letters of
 @return void
 */
void calcFreq (float found[],FILE* datafile){

float array[26];
float totalNumChars = 0;
char ch;

	//set array to 0
	for(int i = 0; i < 26; i++){
		array[i] = 0;
	}

	//scans the file and adds to the temporary array
	while(fscanf(datafile, "%c", &ch) != EOF){
		if(ch >= 'a' && ch <= 'z'){
			int temp = ch-'a';
			array[temp]++;
			totalNumChars++;
		}		

		else if (ch >= 'A' && ch<= 'Z'){
			int temp = ch-'A';
			array[temp]++;
			totalNumChars++;
		}
	}

	//takes temporary array values and divides 
	//them by the total number of characters
	//to place into final array
	for(int i = 0; i < 26; i++){
		float a =  array[i]/totalNumChars;
		found[i] = a;
	}

}

/** Rotate the character in parameter
 ch down the alphabet for the number of
 positions as given in parameter num
 and return the resulting character.
 @param ch character being changed
 @param num how far the letter will go
 @return the decrypted character
 */
char rotate (char ch, int num){

	if (isupper(ch))
		return (ch - num + 26 - 'A') % 26 + 'A';
	
	if (islower(ch))
		return (ch - num + 26 - 'a') % 26 + 'a';
	
	else
		return ch;

return ch;
}

/** Compare the data in array found with the
 frequency data in array given, looking
 for a key that will give you the best match.
 To do this, try each of the 26 rotations,
 and remember which gives the smallest difference
 between the frequencies you observed and the
 frequencies given. Return the key.
 @param given[] typical letter frequencies to match
 @param found[] letter frequencies of text
 @return how far away the letters are encrypted
 */
int findKey (float given[], float found[]){
int key = 0;
float sum;
float lowestSum;
int first = 1;

	for(int i = 0; i < 26; i++){
		sum = 0;

		for(int j = 0; j < 26; j++){
			sum += ((given[j] - found[(j + i)%26]) * (given [j] - found[(j + i)%26]));
		}		
		if(first == 1){
			lowestSum = sum;
			first = 0;
		}
		if(sum < lowestSum){
			lowestSum = sum;
			key = i;
		}
	}
return key;
}


/** Decrypt the encoded text in the input file
 using the key and write the decoded text in the output file
 @param key number to rotate letters
 @param *datafile where encrypted text comes from
 @param *outfile where to print decrypted text
 @return none
 */
void decrypt (int key, FILE * datafile, FILE* outfile){
char ch;
char newCh;
	while(fscanf(datafile,"%c", &ch) != EOF){
		newCh = rotate(ch, key);
		fprintf(outfile, "%c", newCh);
	}
}

int main(int argc, char* argv[]){
    int key;
    float givenArray[26];
    float foundArray[26];
    char ch;
    FILE *fin, *fout, *datafileinput;
    
    //opening the frequency values and placing them into an array
    fin = fopen("letFreq.txt", "r"); //"r" for read
    if(fin == NULL){
        printf("File could not be opened\n");
        exit(1);
    }
    
    while (fscanf(fin, "%c", &ch) != EOF){
        readFreq (givenArray, fin);
    }
    fclose(fin);
    
    //opening the data input and counting letter frequency
    datafileinput = fopen(argv[2], "r");
    if(datafileinput == NULL){
        printf("Data input file could not be opened\n");
        exit(1);
    }
    else{
        calcFreq(foundArray, datafileinput);
    }
    fclose(datafileinput);
    
    //reopening the file and decrypting the file
    //places values into "finalFile.txt"
    datafileinput = fopen(argv[2], "r");
    fout = fopen(argv[3], "w");
    
    if(fout == NULL){
        printf("Error opening final file\n");
        exit(1);
    }
    
    key = findKey(givenArray, foundArray);
    decrypt(key, datafileinput, fout);
    
    fclose(datafileinput);
    fclose(fout);
    
    return 0;
}
