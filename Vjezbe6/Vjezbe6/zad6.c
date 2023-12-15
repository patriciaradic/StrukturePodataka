//6. Napisati program koji èita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
//predstavljaju pojedini raèun.Na poèetku svake datoteke je zapisan datum u kojem vremenu je
//raèun izdat u formatu YYYY - MM - DD.Svaki sljedeæi red u datoteci predstavlja artikl u formatu
//naziv, kolièina, cijena.Potrebno je formirati vezanu listu raèuna sortiranu po datumu.Svaki èvor
//vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla.
//
//Nakon toga potrebno je omoguæiti upit kojim æe korisnik saznati koliko je novaca sveukupno 
//potrošeno na specifièni artikl u odreðenom vremenskom razdoblju i u kojoj je kolièini isti kupljen.

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE (1024)
#define MAX_LEN (50)


typedef struct _article {
	char name[MAX_LEN];
	int quantity;
	float price;
	struct _article* Next;
}Article;

typedef struct _date {
	int year;
	int month;
	int day;
	Article* ArtHead;
	struct _date* Next;
}Date;

Date* createDate(int year, int month, int day);
Article* createArticle(char* name, int quantity, float price);
int readFile2(Date* dateHead);
int readReciept(char* recieptName, Date* dateHead);
int insertSortArticle(Article* head, Article* newArticle);
int addArticleAfter(Article* current, Article* newArticle);
int datecmp(Date* date1, Date* date2);
int insertSortDate(Date* head, Date* newDate);
int addDateAfter(Date* current, Date* newDate);

void searchArticle(Date* firstEl, char name[], Date* date1, Date* date2);

int printlist(Date* firstEl);
int strToDate(char date[], int* year, int* month, int* day);

int main()
{
	Date DateHead = { .year = 0, .month = 0, .day = 0, .ArtHead = NULL, .Next = NULL };
	char name[MAX_LEN] = "";
	int year = 0, month = 0, day = 0;
	Date* date1 = NULL;
	Date* date2 = NULL;

	readFile2(&DateHead);

	//printlist(DateHead.Next);

	printf("Article name: ");
	scanf("%s",name);
	
	printf("Between dates(YYYY-MM-DD)\n");
	
	printf("Date 1: ");
	scanf("%d-%d-%d", &year, &month, &day);
	date1 = createDate(year, month, day);

	printf("Date 2: ");
	scanf("%d-%d-%d", &year, &month, &day);
	date2 = createDate(year, month, day);

	searchArticle(DateHead.Next, name, date1, date2);

	free(date1);
	free(date2);

	return 0;
}


Date* createDate(int year, int month, int day)
{
	Date* newDateElement = NULL;
	newDateElement = (Date*)malloc(sizeof(Date));
	if (newDateElement == NULL) {
		perror("malloc");
		return NULL;
	}
	newDateElement->year = year;
	newDateElement->month = month;
	newDateElement->day = day;
	newDateElement->Next = NULL;

	newDateElement->ArtHead = createArticle("", 0, 0);

	return newDateElement;
}

Article* createArticle(char* name, int quantity, float price)
{
	Article* newArticleElement = NULL;
	newArticleElement = (Article*)malloc(sizeof(Article));
	if (newArticleElement == NULL) {
		perror("malloc");
		return NULL;
	}
	newArticleElement->price = price;
	newArticleElement->quantity = quantity;
	strcpy(newArticleElement->name, name);
	newArticleElement->Next = NULL;

	return newArticleElement;
}

int readFile2(Date* dateHead)
{
	char buffer[MAX_LINE] = "";
	FILE* filePointer = NULL;

	filePointer = fopen("racuni.txt", "r");

	while (!feof(filePointer)) {
		fgets(buffer, MAX_LINE, filePointer);

		int length = strlen(buffer);
		if (buffer[length - 1] == '\n') {
			buffer[length - 1] = '\0';
		}

		readReciept(buffer, dateHead);
	}

	fclose(filePointer);


	return 0;
}

int readReciept(char* recieptName, Date* dateHead)
{
	int year = 0, month = 0, day = 0, quantity = 0;
	float price = 0.0;
	char articleName[MAX_LEN] = "";
	char date[11]="";
	Date* newDate = NULL;
	Article* newArticle = NULL;

	FILE* filePointer = NULL;

	filePointer = fopen(recieptName, "r");


	fscanf(filePointer, "%d-%d-%d ", &year, &month, &day);
	/*fgets(date, 11, filePointer);
	strToDate(date, &year, &month, &day);*/
	newDate = createDate(year, month, day);

	while (!feof(filePointer)) {
		fscanf(filePointer, "%s %d %f ", &articleName, &quantity, &price);
		newArticle = createArticle(articleName, quantity, price);
		insertSortArticle(newDate->ArtHead, newArticle);
	}

	insertSortDate(dateHead, newDate);

	fclose(filePointer);

	return 0;
}

int insertSortArticle(Article* head, Article* newArticle)
{
	Article* current = head;
	while (current->Next != NULL && strcmp(current->Next->name, newArticle->name) < 0) {
		current = current->Next;
	}

	if (current->Next != NULL) {
		if (strcmp(current->Next->name, newArticle->name) == 0 && current->Next->price == newArticle->price) {
			current->Next->quantity += newArticle->quantity;
		}
		else {
			addArticleAfter(current, newArticle);
		}
	}
	else {
		addArticleAfter(current, newArticle);
	}


	return 0;
}

int addArticleAfter(Article* current, Article* newArticle)
{
	newArticle->Next = current->Next;
	current->Next = newArticle;

	return 0;

}

int datecmp(Date* date1, Date* date2)
{
	if (date1->year == date2->year) {
		if (date1->month == date2->month) {
			if (date1->day == date2->day) {
				return 0;
			}
			else if (date1->day > date2->day) {
				return 1;
			}
		}
		else if (date1->month > date2->month) {
			return 1;
		}
	}
	else if (date1->year > date2->year) {
		return 1;
	}

	return -1;
}


int insertSortDate(Date* head, Date* newDate)
{
	Date* current = head;
	Article* temp = NULL;
	
	while (current->Next != NULL && datecmp(current->Next, newDate) < 0) {
		current = current->Next;
	}

	if (current->Next != NULL) {
		if (datecmp(current->Next, newDate) == 0) {
			while (newDate->ArtHead->Next != NULL) {
				temp = newDate->ArtHead->Next;
				newDate->ArtHead->Next = temp->Next;
				insertSortArticle(current->Next->ArtHead, temp);
			}
		}
		else {
			addDateAfter(current, newDate);
		}
	}
	else {
		addDateAfter(current, newDate);
	}

	return 0;
}


int addDateAfter(Date* current, Date* newDate)
{
	newDate->Next = current->Next;
	current->Next = newDate;

	return 0;
}


int printlist(Date* firstEl)
{
	Date* currentD = firstEl;
	Article* currentA = NULL;

	while (currentD != NULL) {
		printf("%d-%d-%d\n", currentD->year, currentD->month, currentD->day);
		currentA = currentD->ArtHead->Next;
		while (currentA != NULL) {
			printf("-->%s, %d, %.2f\n", currentA->name, currentA->quantity, currentA->price);
			currentA = currentA->Next;
		}
		printf("\n");
		currentD = currentD->Next;
	}

	return 0;
}

int strToDate(char date[], int* year, int* month, int* day)
{
	char temp[5] = "";
	int i, j = 0;

	for (i = 0; i < 11; i++) {
		if (date[i] >= 0 && date[i] <= 9) {
			temp[j] = date[i];
			j++;
		}
		else {
			if (i == 5) {
				*year = atoi(temp);
			}
			else if (i == 8) {
				*month = atoi(temp);
			}
			else {
				*day = atoi(temp);
			}
			strcpy(temp, "");
			j = 0;
		}
		
	}

	return 0;
}

void searchArticle(Date* firstEl, char name[], Date* date1, Date* date2)
{
	if (firstEl == NULL) {
		printf("Empty list\n");
		return;
	}

	Date* currentD = firstEl;
	Article* currentA = NULL;
	int totalQuantity = 0;
	float totalPrice = 0;

	int length = strlen(name);
	if (name[length - 1] == '\n') {
		name[length - 1] = '\0';
	}

	while (currentD != NULL && datecmp(currentD, date1) < 0) {
		currentD = currentD->Next;
	}

	while (currentD != NULL && datecmp(currentD, date2) <= 0) {
		
		currentA = currentD->ArtHead->Next;
		while (currentA != NULL) {

			if (!strcmp(currentA->name, name)) {
				totalQuantity += currentA->quantity;
				totalPrice += currentA->price * currentA->quantity;
			}

			currentA = currentA->Next;
		}
		currentD = currentD->Next;
	}

	if (!totalQuantity) {
		printf("that article hasn't been purchased in this time period.\n");
	}
	else {
		printf("that article has been purchased in %d copies for the total value of %.2f\n", totalQuantity, totalPrice);
	}

}