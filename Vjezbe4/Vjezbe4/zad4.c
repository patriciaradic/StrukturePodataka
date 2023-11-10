#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LEN (1024)

/*4. Napisati program za zbrajanje i množenje polinoma. Koeficijenti i eksponenti se
èitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.*/

typedef struct _Polynomial {
	int coefficient;
	int exponent;
	struct _Polynomial* Next;
}Polynomial;

int readFile(Polynomial* headPoly1, Polynomial* headPoly2);
Polynomial* createElement(int coefficient, int exponent);
int parseStringIntoList(Polynomial* Head, char* buffer);
int insertSorted(Polynomial* Head, Polynomial* element);
int polyMultiply(Polynomial* firstElement1, Polynomial* firstElement2, Polynomial* headProduct);
int polySum(Polynomial* firstElement1, Polynomial* firstElement2, Polynomial* headSum);
int printList(char* text, Polynomial* firstElement);
int freeElement(Polynomial* Head);



int main()
{
	Polynomial HeadPoly1 = { .coefficient = 0, .exponent = 0, .Next = NULL };
	Polynomial HeadPoly2 = { .coefficient = 0, .exponent = 0, .Next = NULL };
	Polynomial HeadSum = { .coefficient = 0, .exponent = 0, .Next = NULL };
	Polynomial HeadProduct = { .coefficient = 0, .exponent = 0, .Next = NULL };

	readFile(&HeadPoly1, &HeadPoly2);
	printList("first polynomial: ", HeadPoly1.Next);
	printList("second polynomial: ", HeadPoly2.Next);
	
	polySum(HeadPoly1.Next, HeadPoly2.Next, &HeadSum);
	printList("sum of the polynomials: ",HeadSum.Next);

	polyMultiply(HeadPoly1.Next, HeadPoly2.Next, &HeadProduct);
	printList("product of the polynomials: ",HeadProduct.Next);

	return 0;
}


int readFile(Polynomial* HeadPoly1, Polynomial* HeadPoly2)
{
	FILE* filePointer = NULL;
	char buffer[MAX_LEN] = "";

	filePointer = fopen("polynomials.txt", "r");

	if (!filePointer) {
		perror("fopen");
		return 1;
	}

	//citamo liniju filea
	fgets(buffer, MAX_LEN, filePointer);
	//kidamo buffer na dijelove
	if (parseStringIntoList(HeadPoly1, buffer)) return 1;

	fgets(buffer, MAX_LEN, filePointer);
	if (parseStringIntoList(HeadPoly2, buffer)) return 1;

	fclose(filePointer);

	return 0;
}


int parseStringIntoList(Polynomial* Head, char* buffer)
{
	Polynomial* currentElement = Head;
	Polynomial* element = NULL;
	char* currentBuffer = buffer;
	int status = 0;
	int coefficient = 0, exponent = 0;
	int numBytes = 0;

	while (strlen(currentBuffer)>0) {

		//sscanf broji kolko smo varijabli popunili(u ovom zad mora bit 2)
		//stores the number of characters read from the input up to where the %n was encountered into the integer variable pointed to by the corresponding argument
		status = sscanf(currentBuffer, "%dx^%d %n", &coefficient, &exponent, &numBytes);

		if (status != 2) {
			printf("This file is not good!\n");
			return 1;
		}

		element = createElement(coefficient, exponent);

		if (!element) return 1;

		insertSorted(Head, element);

		//currentBuffer je adresa koju pomaknemo za numBytes tj. u iducoj iteraciji currentBuffer je za numBytes kraci
		currentBuffer += numBytes;
	}

	freeElement(Head);

	return 0;
}



Polynomial* createElement(int coefficient, int exponent)
{
	Polynomial* newElement = NULL;

	newElement = (Polynomial*)malloc(sizeof(Polynomial));

	if (!newElement) {
		perror("malloc");
		return NULL;
	}

	newElement->coefficient = coefficient;
	newElement->exponent = exponent;

	return newElement;
}


int insertSorted(Polynomial* Head, Polynomial* element)
{
	Polynomial* current = Head;

	//trazimo kraj liste ili dok je nas element ne bude >= od iduceg u listi
	while (current->Next != NULL && element->exponent < current->Next->exponent) {
		current = current->Next;
	}

	//ako je nas element == iducem u listi samo im zbrojimo koeficijente, a ako nije ide klasicno ubacivanje
	if (current->Next != NULL && current->Next->exponent == element->exponent) {
		current->Next->coefficient = current->Next->coefficient + element->coefficient;
	}
	else {
		element->Next = current->Next;
		current->Next = element;
	}

	
	return 0;
}

int polyMultiply(Polynomial* firstElement1, Polynomial* firstElement2, Polynomial* headProduct)
{
	Polynomial* start = firstElement2;
	Polynomial* newElement = NULL;

	if (!firstElement1 || !firstElement2) {
		printf("empty polynomials\n");
		return 1;
	}

	while (firstElement1) {
		while (firstElement2) {
			
			newElement = createElement(firstElement1->coefficient * firstElement2->coefficient, firstElement1->exponent + firstElement2->exponent);
			
			if (!newElement) return 1;

			insertSorted(headProduct, newElement);

			firstElement2 = firstElement2->Next;
		}
		//dosli smo do kraja 2 poli i sad ga vracamo na pocetak
		firstElement2 = start;
		firstElement1 = firstElement1->Next;
	}

	return 0;
}


int polySum(Polynomial* firstElement1, Polynomial* firstElement2, Polynomial* headSum)
{
	Polynomial* newElement = NULL;

	if (!firstElement1 || !firstElement2) {
		printf("empty polynomials\n");
		return 1;
	}

	while (firstElement1) {
		newElement = createElement(firstElement1->coefficient, firstElement1->exponent);
		if (!newElement) return 1;

		insertSorted(headSum, newElement);
		firstElement1 = firstElement1->Next;
	}

	while (firstElement2) {
		newElement = createElement(firstElement2->coefficient, firstElement2->exponent);
		if (!newElement) return 1;

		insertSorted(headSum, newElement);
		firstElement2 = firstElement2->Next;
	}

	freeElement(headSum);

	return 0;
}

int printList(char* text, Polynomial* firstElement)
{
	printf(" %s ", text);

	//samo za prvi element (ako je koef>0 onda da ne stavimo predznak viska)
	if (firstElement) {
		if (firstElement->exponent < 0) {
			if (firstElement->coefficient == 1) {
				printf("x^(%d)", firstElement->exponent);
			}
			else {
				printf("%dx^(%d)", firstElement->coefficient, firstElement->exponent);
			}
		}
		else {
			if (firstElement->coefficient == 1) {
				printf("x^%d", firstElement->exponent);
			}
			else {
				printf("%dx^%d", firstElement->coefficient, firstElement->exponent);
			}
		}

		firstElement = firstElement->Next;
	}

	while (firstElement != NULL) {
		if (firstElement->coefficient < 0) {
			if (firstElement->exponent < 0) {
				printf(" - %dx^(%d)", abs(firstElement->coefficient), firstElement->exponent);
			}
			else {
				printf(" - %dx^%d", abs(firstElement->coefficient), firstElement->exponent);
			}
		}
		else {
			if (firstElement->exponent < 0) {
				if (firstElement->coefficient == 1) {
					printf(" + x^(%d)", firstElement->exponent);
				}
				else {
					printf(" + %dx^(%d)", firstElement->coefficient, firstElement->exponent);
				}
			}
			else {
				if (firstElement->coefficient == 1) {
					printf(" + x^%d", firstElement->exponent);
				}
				else {
					printf(" + %dx^%d", firstElement->coefficient, firstElement->exponent);
				}
			}
		}
		firstElement = firstElement->Next;
	}

	printf("\n");

	return 0;
}


int freeElement(Polynomial* Head)
{
	Polynomial* delete = NULL;
	Polynomial* current = Head;

	while (current->Next != NULL) {
		
		if (current->Next->coefficient == 0) {
			delete = current->Next;
			current->Next = delete->Next;

			free(delete);
		}
		else {
			current = current->Next;
		}
		
	}


	return 0;
}