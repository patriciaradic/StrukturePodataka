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

/*6. Napisati program koji èita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini raèun. Na poèetku svake datoteke je zapisan datum u kojem vremenu je
raèun izdat u formatu YYYY-MM-DD. Svaki sljedeæi red u datoteci predstavlja artikl u formatu
naziv, kolièina, cijena. Potrebno je formirati vezanu listu raèuna sortiranu po datumu. Svaki èvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omoguæiti upit kojim æe korisnik saznati koliko je novaca sveukupno potrošeno na specifièni
artikl u odreðenom vremenskom razdoblju i u kojoj je kolièini isti kupljen.
*/

Date* createDate(int year, int month, int day);
Article* createArticle(char* name, int quantity, float price);

int main()
{
	Date DateHead = { .year = 0, .month = 0, .day = 0, .ArtHead = NULL, .Next = NULL };

	

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
	newDateElement->ArtHead = NULL;
	newDateElement->Next = NULL;

	return newDateElement;
}

Article* createArticle(char* name, int quantity, float price)
{
	Article* newArticleElement = NULL;
	newArticleElement = (Date*)malloc(sizeof(Date));
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

/*read racuni.txt
->petlja do kraja filea
->procita liniju i otvori novi file
	->procita prvu liniju(datum) createa datum i unese
		->petlja do kraja filea->linija novi artikal->create+sortUnos
	->sortUnos datuma*/