#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEN (50)

/*2. Definirati strukturu osoba (ime, prezime, godina ro�enja) i napisati program koji:
A. dinami�ki dodaje novi element na po�etak liste,
B. ispisuje listu,
C. dinami�ki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. bri�e odre�eni element iz liste,
U zadatku se ne smiju koristiti globalne varijable*/

/*3. Prethodnom zadatku dodati funkcije:
A. dinami�ki dodaje novi element iza odre�enog elementa,
B. dinami�ki dodaje novi element ispred odre�enog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. �ita listu iz datoteke.*/

typedef struct node {
	char name[MAX_LEN];
	char surname[MAX_LEN];
	int birthYear;
	struct node* next;
}Person;

int MENU(Person*);
Person* newPerson();
int AddToBeginning(Person*);
int AddToEnd(Person*);
Person* FindPerson(Person*, char[]);
Person* FindPrevPerson(Person*, char[]);
int PrintList(Person*);
int DeleteElement(Person*, char[]);
int DeleteAll(Person*);
int AddAfter(Person*,char[]);
int AddInFrontOf(Person*,char[]);
int ReadFromFile(Person*);
int WriteIntoFile(Person*);


int main()
{
	Person Head;
	Head.next = NULL;
	MENU(&Head);

	return 0;

}

int MENU(Person* Head)
{
	char choice;
	char Surname[MAX_LEN];
	Person* adress;

	while (1) {
		printf("\n\n\nChoose an option:\n");
		printf("B Add new item to the beginning of the list \nP Print list \nE Add new item to the end of the list \nF Find elemet in list(by surname) \nD Delete element from list \nA Add after element in list\nI Add in front of element in list\nW Write list in file\nR Read list from file\nX Exit program\n");
		scanf(" %c", &choice);

		choice = tolower(choice);

		if (choice == 'b') {
			if (!AddToBeginning(Head)) {
				printf("Action executed succesfully\n");
			}
			else printf("Action failed. Try again.\n");
		}
		else if (choice == 'p') {
			PrintList(Head->next);
		}
		else if (choice == 'e') {
			if (!AddToEnd(Head)) {
				printf("Action executed succesfully\n");
			}
			else printf("Action failed. Try again.\n");
		}
		else if (choice == 'f') {

			printf("Enter the surname of the person you want to find:\n");
			scanf(" %s", Surname);
			adress = FindPerson(Head->next, Surname);
			
		}
		else if (choice == 'd') {
			printf("Enter the surname of the person you want to remove from the list:\n");
			scanf(" %s", Surname);
			DeleteElement(Head, Surname);
		}
		else if (choice == 'x') {

			DeleteAll(Head);
			return 0;
		}
		else if (choice == 'a') {

			printf("Enter the surname after wich you want to add the new person:\n");
			scanf(" %s", Surname);
			AddAfter(Head->next,Surname);
		}
		else if (choice == 'i') {
			
			printf("Enter the surname in front of wich you want to add the new person:\n");
			scanf(" %s", Surname);
			AddInFrontOf(Head,Surname);
		}
		else if (choice == 'w') {
			WriteIntoFile(Head->next);
		}
		else if (choice == 'r') {
			ReadFromFile(Head);
		}
		else {
			printf("You chose a non-existent option. Try again.\n");
		}
	}

}

Person* newPerson()
{
	Person* newPrsn = NULL;

	newPrsn = (Person*)malloc(sizeof(Person));

	printf("Enter the new person's name: ");
	scanf(" %s", newPrsn->name);
	printf("Enter their surname: ");
	scanf(" %s", newPrsn->surname);
	printf("Enter their birth year: ");
	scanf(" %d", &newPrsn->birthYear);


	return newPrsn;
}

int AddToBeginning(Person* element)
{
	Person* prsn = NULL;
	prsn = newPerson();
	if (prsn != NULL) {
		prsn->next = element->next;
		element->next = prsn;

		return 0;
	}
	else return 1;
}

int AddToEnd(Person* element)
{
	Person* prsn = NULL;
	prsn = newPerson();

	while (element->next != NULL) {
		element = element->next;
	}

	if (prsn != NULL) {
		prsn->next = element->next;
		element->next = prsn;

		return 0;
	}
	else return 1;

}

Person* FindPerson(Person* element, char Surname[])
{
	while (element != NULL && strcmp(element->surname, Surname)) {
		element = element->next;
	}
	if (element == NULL){
		printf("That person is not on the list.\n");
		return NULL;
	}
	return element;

}

Person* FindPrevPerson(Person* element, char Surname[])
{
	while (element->next != NULL && strcmp(element->next->surname, Surname)) {
		element = element->next;
	}
	if (element->next == NULL) {
		printf("That person was not on the list.\n"); 
		return NULL;
	}
	return element;

}

int PrintList(Person* element)
{
	while (element != NULL) {

		printf("%s %s, born in %d\n", element->name, element->surname, element->birthYear);
		element = element->next;
	}
	return 0;
}


int DeleteAll(Person* element)
{
	Person* temp;
	while (element->next != NULL) {

		temp = element->next;
		element->next = temp->next;

		free(temp);
	}
}

int DeleteElement(Person* element, char Surname[])
{
	Person* del;

	element = FindPrevPerson(element, Surname);
	if (element == NULL) {
		return 1;
	}
	else {
		del = element->next;
		element->next = del->next;

		free(del);
		return 0;
	}
}

int AddAfter(Person* element, char Surname[])
{
	Person* prsn = NULL;
	
	prsn = newPerson();
	
	element = FindPerson(element, Surname);
	

	if (prsn != NULL) {
		prsn->next = element->next;
		element->next = prsn;

		return 0;
	}
	else return 1;

}


int AddInFrontOf(Person* element, char Surname[])
{
	Person* prsn = NULL;

	prsn = newPerson();

	element = FindPrevPerson(element, Surname);


	if (prsn != NULL) {
		prsn->next = element->next;
		element->next = prsn;

		return 0;
	}
	else return 1;

}


int ReadFromFile(Person* element)
{
	FILE* filePointer;
	char name[MAX_LEN], surname[MAX_LEN];
	int year;

	filePointer = fopen("list.txt", "r");

	while (!feof(filePointer)){

		fscanf(filePointer, "%s %s %d", name, surname, &year);

	}

	fclose(filePointer);

	return 0;
}


int WriteIntoFile(Person* element)
{
	FILE* filePointer;

	filePointer = fopen("list.txt", "w");

	if (filePointer == NULL) {
		printf("write nije dobro otvoria dat");
		return 1;

	}

	while (element != NULL) {

		fprintf(filePointer, "%s %s\t%d\n", element->name, element->surname, element->birthYear);
		element = element->next;
	}


	fclose(filePointer);

	return 0;
}