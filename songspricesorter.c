//get min 5 songs from user and display in order of price (low to high)
//need to store and display all the data
//also needs to exit on user command
//maybe suggest a micocontroller where your code can be implemented
//sir mentioned bubble sort alorithm

#include <stdio.h> //standard
#include <stdlib.h> //standard library
#include <string.h> //string library

#define min 5   // minimum num of songs is 5
#define max 10 // maximum num of songs is 100

#define DATA_FILE "songs.txt" //data file name

typedef struct // song details
{
	char title[100]; //title of song max 100 characters
	char artist[100]; //artist/group name max 100 characters
	float price; //price of song
	int year; //year released
} Song;

Song songs[max]; //array of song structures
int num = 0; //currently 0 songs but this will increase as songs added (hence just 1 =)

void read_songs(void) //1 get the songs
{
	while (num < 5) //while less than 5 songs
	{
		printf("Please enter a song\n"); //enter a song

		printf("Enter the artist/band/group name: "); //asking for artist name
		fgets(songs[num].artist, sizeof(songs[num].artist), stdin); //store artist
		songs[num].artist[strcspn(songs[num].artist, "\n")] = '\0'; // remove newline

		printf("Enter the song's name: "); //ask song's name
		fgets(songs[num].title, sizeof(songs[num].title), stdin); //store title
		songs[num].title[strcspn(songs[num].title, "\n")] = '\0'; // remove newline

		printf("Enter the price (£): "); //ask for price
		scanf("%f", &songs[num].price); //store price
		while (getchar() != '\n'); // clear buffer

		printf("Enter the year it was released: "); //ask for year released
		scanf("%d", &songs[num].year); //store year
		while (getchar() != '\n'); // clear buffer

		num++; //add 1 more to num cause 1 more song added
	}

	printf("You have entered %d songs.\n", num); //print number of songs entered)
}


void Bubble_Sort_By_Price(Song arr[], int n) //sorting song prices lowest to highest through bubblesort
{
	Song temp; //temporary store
	for (int i = 0; i < n - 1; i++) //outer loop
	{
		for (int j = 0; j < n - i - 1; j++) //inner loop
		{
			if (arr[j].price > arr[j + 1].price) //if price is more than next price
			{
				temp = arr[j]; //store in temp
				arr[j] = arr[j + 1]; //move the next one to current
				arr[j + 1] = temp; //move the one in temp to the next one cause more
			} //repeat for next item until end of list
		} //repeat until all sorted
	} //repeat until all sorted
}

void display_songs(void) //2 show all songs in order of price
{
	if (num == 0) //if no songs
	{
		printf("\nNo songs available. Please add or load songs first.\n"); //tell them no songs
		return;
	}

	Song temp[max]; //// Make a temporary copy to sort the songs
	memcpy(temp, songs, num * sizeof(Song)); //copy songs to temp

	Bubble_Sort_By_Price(temp, num); //bubble sort by price and temporarily store

	printf("\n List of Songs (Lowest to highest price)\n"); //title
	for (int i = 0; i < num; i++) //for each song
	{
		printf("Artist/Band: %s\n", temp[i].artist); //1st artist
		printf("Title      : %s\n", temp[i].title); //2nd title
		printf("Price (£)  : %.2f\n", temp[i].price); //3rd price
		printf("Year       : %d\n", temp[i].year); //4th year
		printf(" \n"); //a gap
	} //repeat till all songs displayed
}

void save_data(void) //3 save songs to 1 file
{
	FILE *fp = fopen(DATA_FILE, "w"); //open file in write mode
	if (fp == NULL) //if file is null
	{
		printf("Error.\n"); //print error message
		return;
	}
	for (int i = 0; i < num; i++) //for each song
	{
		fprintf(fp, "%s,%s,%.2f,%d\n", songs[i].artist, songs[i].title, songs[i].price, songs[i].year); //write song details to file
		//format: artist (string),title (string),price (float),year (integer)

	}

	fclose(fp); //close file
	printf("Songs saved successfully.\n", DATA_FILE); //print success message

}


void load_file(void) //4 load songs from file
{
	FILE* fp = fopen_s("songs.txt", "r"); // open file in read mode
	if (fp == NULL) //if null
	{
		printf("File not found.\n"); //error message
		return;
	}

	char line[256];
	num = 0; //reset number of songs to 0

	while (fgets(line, sizeof(line), fp)) //read each line from file
	{
		char* token = strtok(line, ","); //token is each section of line separated by |
		if (token == NULL) //if empty
		{
			continue; //if that token is null, continue to next line
			strcpy(songs[num].artist, token); //add in artist name to that token
		}

		token = strtok(NULL, ","); //next token
		if (token == NULL)
		{
			continue;
			strcpy(songs[num].title, token); //add title
		}

		token = strtok(NULL, ","); //next token
		if (token == NULL)
		{
			continue;
			songs[num].price = atof(token); //add price
		}

		token = strtok(NULL, ","); //next token
		if (token == NULL)
		{
			continue;
			songs[num].price = atoi(token); //add year
		}

		num++; //increase number of songs

		if (num == max)

		{
			break;
		}

		fclose(fp);
		printf("\nLoaded %d songs from '%s'.\n", num, DATA_FILE);
	}

}


void menu(void)
{
	int menu_number_choosing; //choosing when menu option you want
	do {
		printf("\n SONG MENU \n"); //title
		printf("1. Enter song data (min 5 songs)\n"); //entering songs
		printf("2. Display all songs (sorted by price)\n"); //displaying songs in order
		printf("3. Save songs to file\n"); //save songs to file
		printf("4. Load songs from file\n"); //load songs from file
		printf("5. Exit\n"); //exit program
		printf("\n"); //empty line
		printf("Enter a number from 1-5 from the menu: "); //if not chosesn correctly
		scanf("%d", &menu_number_choosing); //store choice
		while (getchar() != '\n'); // clear buffer

		switch (menu_number_choosing) //switch case for menu options
		{
		case 1: read_songs(); break; //call read songs function
		case 2: display_songs(); break; //call display songs function
		case 3: save_data(); break; //call save data function
		case 4: load_file(); break; //call load file function
		case 5: printf("\nExiting program.\n"); //exit message

			break; //exit program

		default: printf("Pleas enter a number between 1 and 5.\n"); //if not correct number
		}
	} while (menu_number_choosing != 5); //repeat until user wants to exit
}

int main(void)
{
	menu(); //call menu function
	return 0;
}
