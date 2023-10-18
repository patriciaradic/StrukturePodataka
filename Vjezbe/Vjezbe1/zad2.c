#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_LEN 50

/*2. Definirati strukturu osoba (ime, prezime, godina roðenja) i napisati program koji:
A. dinamièki dodaje novi element na poèetak liste,
B. ispisuje listu,
C. dinamièki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable*/

typedef struct person{
	char name[MAX_LEN];
	char surname[MAX_LEN];
	int birthYear;
	struct person * next;
}Person;

int MENU(Person*);
Person* newPerson();
int AddToBeginning(Person*);
int AddToEnd(Person*);
Person* FindPerson(Person*,char[]);
Person* FindPrevPerson(Person*, char[]);
int PrintList(Person*);
int DeleteElement(Person*,char[]);
int DeleteAll(Person*);

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

	while (1){
		printf("\n\n\nChoose an option:\n");
		printf("B Add new item to the beginning of the list \nP Print list \nE Add new item to the end of the list \nF Find elemet in list(by surname) \nD Delete element from list \nX Exit program\n");
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
			if (adress == NULL) {
				printf("That person is not on the list.\n");
			}
			else {
				printf("%s %s, born in %d\n", adress->name, adress->surname, adress->birthYear);
			}
		}
		else if (choice == 'd') {
			printf("Enter the surname of the person you want to remove from the list:\n");
			scanf(" %s", Surname);
			DeleteElement(Head, Surname);
		}
		else if(choice=='x') {

			DeleteAll(Head);
			return 0;
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

int AddToBeginning(Person*peep)
{
	Person* prsn = NULL;
	prsn = newPerson();
	if (prsn != NULL) {
		prsn->next = peep->next;
		peep->next = prsn;

		return 0;
	}
	else return 1;
}

int AddToEnd(Person* peep)
{
	Person* prsn = NULL;
	prsn = newPerson();

	while (peep->next!=NULL){
		peep = peep->next;
	}

	if (prsn != NULL) {
		prsn->next = peep->next;
		peep->next = prsn;

		return 0;
	}
	else return 1;

}

Person* FindPerson(Person*peep, char Surname[])
{
	while (peep != NULL && strcmp(peep->surname,Surname)) {
		peep = peep->next;
	}
	if (peep == NULL) return NULL;
	else return peep;

}

Person* FindPrevPerson(Person* peep, char Surname[])
{
	while (peep->next != NULL && strcmp(peep->next->surname, Surname)) {
		peep = peep->next;
	}
	if (peep->next == NULL) return NULL;
	else return peep;

}

int PrintList(Person* peep)
{
	while (peep != NULL) {

		printf("%s %s, born in %d\n", peep->name, peep->surname, peep->birthYear);
		peep = peep->next;
	}
	return 0;
}


int DeleteAll(Person* peep)
{
	Person* temp;
	while (peep->next != NULL) {

		temp = peep->next;
		peep->next=temp->next;

		free(temp);
	}
}

int DeleteElement(Person* peep, char Surname[])
{
	Person* del;
	Person* temp;
	temp=FindPrevPerson(peep, Surname);
	if (temp == NULL) {
		printf("That person was not on the list.\n");
	}
	else {
		del = temp->next;
		temp->next = del->next;

		free(del);
	}
}