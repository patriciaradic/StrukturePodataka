#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>

/* Napisati program koji prvo proèita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamièki alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i uèitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100
*/

int main()
{
	int brojRedova = 0;
	char buffer[1024]="";
	FILE* filePointer=NULL;

	filePointer = fopen("studenti.txt","r");

	//error handling -> ako se ne moze otvoriti file
	if (filePointer == NULL) {
		printf("Datoteka nije pronadena.");
		return 1;
	}

	//brojanje redova
	while (!feof(filePointer)){
		fgets(buffer,1024,filePointer);
		brojRedova++;
	}

	

	fclose(filePointer);

	return 0;
}