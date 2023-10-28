#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE 1024
#define MAX_LEN 50

/* Napisati program koji prvo proèita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamièki alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i uèitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100
*/

typedef struct
{
	char name[MAX_LEN];
	char surname[MAX_LEN];
	double points;

}Student;

int countRows();
Student* memoryAlloc(int, Student*);
double readFile(int, Student*);
int printStudents(int, Student*, double);

int main()
{
	int noRows = 0;
	double maxPoint = 0.0;
	int i = 0;

	Student* studenti = NULL;


	noRows = countRows();
	printf("%d", noRows);

	if (noRows > 0) {
		studenti = memoryAlloc(noRows, studenti);

		maxPoint = readFile(noRows, studenti);

		printStudents(noRows, studenti, maxPoint);

		free(studenti);
	}

	return 0;
}

int countRows()
{
	int count = 0;
	char buffer[MAX_LINE] = "";
	FILE* filePointer = fopen("studenti.txt", "r");

	//error handling => ako se ne moze otvoriti file
	if (filePointer == NULL) {
		perror("fopen");
		return -1;
	}


	while (!feof(filePointer)) {
		fgets(buffer, MAX_LINE, filePointer);
		count++;
	}

	fclose(filePointer);

	return count;
}

Student* memoryAlloc(int noRows, Student* studenti)
{
		
	studenti = (Student*)malloc(noRows * sizeof(Student));

	if (studenti == NULL) {
		printf("Neuspjela alokacija memorije");
		return NULL;
	}
	else {
		return studenti;
	}


}

double readFile(int noRows, Student* studenti)
{
	FILE* filePointer = NULL;
	char Name[MAX_LEN] = "";
	char Surname[MAX_LEN] = "";
	double Point, maxPoint = 0;



	filePointer = fopen("studenti.txt", "r");

	//error handling => ako se ne moze otvoriti file
	if (filePointer == NULL) {
		printf("Datoteka nije pronadena.");
		return -1.0;
	}

	for (int i = 0; i < noRows; i++) {
		fscanf(filePointer, "%s %s %lf", Name, Surname, &Point);

		strcpy(studenti[i].name, Name);
		strcpy(studenti[i].surname, Surname);
		studenti[i].points = Point;

		if (maxPoint < Point) {
			maxPoint = Point;
		}
	}


	fclose(filePointer);

	return maxPoint;

}

int printStudents(int noRows, Student* studenti, double maxPoints)
{
	printf("IME\tPREZIME\tAPS. BOD\tREL. BOD\n");

	for (int i = 0; i < noRows; i++) {
		printf("%s\t%s\t%.2lf\t\t%.2lf\n", studenti[i].name, studenti[i].surname, studenti[i].points, studenti[i].points / maxPoints * 100);
	}

	return 0;

}