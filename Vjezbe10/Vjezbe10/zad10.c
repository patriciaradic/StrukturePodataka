/*10. Napisati program koji èita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države. Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.

a) Potrebno je formirati sortiranu vezanu listu država po nazivu. Svaki èvor vezane liste
sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.

b) Potrebno je formirati stablo država sortirano po nazivu. Svaki èvor stabla sadrži vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.

Nakon formiranja podataka potrebno je ispisati države i gradove te omoguæiti korisniku putem
tastature pretragu gradova odreðene države koji imaju broj stanovnika veæi od unosa na
tastaturi.*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE 1024
#define MAX_LEN 50

typedef struct _city {
	char cityName[MAX_LEN];
	int population;
	struct _city* next;
	struct _city* lc;
	struct _city* rc;
}City;

typedef struct _country {
	char countryName[MAX_LEN];
	City* cityRoot;
	City* cityHead;
	struct _country* next;
	struct _country* lc;
	struct _country* rc;
}Country;

City* createCity(char name[], int population);
Country* createCountry(char name[]);
Country* readCountriesFromFile(Country* countryHead, Country* countryRoot);

void countryInorder(Country* countryRoot);
void cityListPrint(City* firstCity);
void cityInorder(City* root, int populationLimit);
void printCitiesWithPopul(Country* firstCountry, char countryName[], int populationLimit);
Country* findCountry(Country* firstCountry, char countryName[]);

//blok funkcija za a)
void insertSortCountryList(char countryName[], char fileName[], Country* head);
City* readCitiesIntoTree(char fileName[], City* root);
City* insertSortCitiesTree(City* root, City* newCity);

//blok funkcija za b)
Country* insertSortCountryTree(Country* root, Country* newCountry, char fileName[]);
void readCitiesIntoList(char fileName[], City* head);
void insertSortCitiesList(City* head, City* newCity);


int main()
{
	Country countryHead = { .countryName = "",
							.cityRoot = NULL,
							.cityHead = NULL,
							.next = NULL,
							.lc = NULL,
							.rc = NULL 
	};
	Country* countryRoot = NULL;
	char enteredCountry[MAX_LEN] = "";
	int eneteredPopulation = 0;


	countryRoot = readCountriesFromFile(&countryHead, countryRoot);

	while (1) {
		system("cls");
		countryInorder(countryRoot);

		printf("Unesite zemlju koju zelite pretraziti: ");
		scanf("%s", enteredCountry);
		printf("Unesite donju granicu stanovnistva: ");
		scanf("%d", &eneteredPopulation);

		printCitiesWithPopul(countryHead.next, enteredCountry, eneteredPopulation);


		system("pause");
	}
	
	return 0;
}


City* createCity(char name[], int population)
{
	City* newCity = (City*)malloc(sizeof(City));
	if (!newCity) {
		printf("Memory allocation error");
		return NULL;
	}
	strcpy(newCity->cityName, name);
	newCity->population = population;
	newCity->next = NULL;
	newCity->lc = NULL;
	newCity->rc = NULL;
	return newCity;
}

Country* createCountry(char name[])
{
	Country* newCountry = (Country*)malloc(sizeof(Country));
	if (!newCountry) {
		printf("Memory allocation error");
		return NULL;
	}
	strcpy(newCountry->countryName, name);
	newCountry->cityHead = NULL;
	newCountry->cityRoot = NULL;
	newCountry->next = NULL;
	newCountry->lc = NULL;
	newCountry->rc = NULL;
	return newCountry;
}


Country* readCountriesFromFile(Country* countryHead, Country* countryRoot)
{
	char buffer[MAX_LINE] ="";
	char countryName[MAX_LEN] = "";
	char countryFile[MAX_LEN] = "";
	FILE* filePointer = fopen("drzave.txt", "r");

	while (!feof(filePointer)) {
		fgets(buffer, MAX_LINE, filePointer);
		if (sscanf(buffer, "%s %s", countryName, countryFile) == 2) {
			if (countryFile[strlen(countryFile) - 1] == '\n')
				countryFile[strlen(countryFile) - 1] = '\0';

			insertSortCountryList(countryName, countryFile, countryHead);
			countryRoot = insertSortCountryTree(countryRoot, createCountry(countryName), countryFile);
		}
	}
	
	fclose(filePointer);

	return countryRoot;
}

void countryInorder(Country* countryRoot)
{
	if (countryRoot == NULL) {
		printf("\t(empty)\n");
		return;
	}

	if (countryRoot->lc != NULL) {
		countryInorder(countryRoot->lc);
	}

	printf("%s\n", countryRoot->countryName);
	cityListPrint(countryRoot->cityHead->next);

	if (countryRoot->rc != NULL) {
		countryInorder(countryRoot->rc);
	}
}

void cityListPrint(City* firstCity)
{
	while (firstCity != NULL) {
		printf(" - %s\n", firstCity->cityName);
		firstCity = firstCity->next;
	}		
	printf("\n");
}

void printCitiesWithPopul(Country* firstCountry, char countryName[], int populationLimit)
{
	if (firstCountry == NULL)
		return;

	Country* foundCountry = findCountry(firstCountry, countryName);
	if (!foundCountry) {
		printf("Drzava nije pronadena\n");
		return;
	}
	printf("\n%s\n", foundCountry->countryName);

	cityInorder(foundCountry->cityRoot, populationLimit);
}

void cityInorder(City* root, int populationLimit)
{
	if (root == NULL) {
		printf("\t(empty)\n");
		return;
	}

	if (root->population < populationLimit)
		return;

	if (root->lc != NULL) {
		cityInorder(root->lc, populationLimit);
	}

	printf(" - %s, %d\n", root->cityName, root->population);

	if (root->rc != NULL) {
		cityInorder(root->rc, populationLimit);
	}
}

Country* findCountry(Country* firstCountry, char countryName[])
{
	while (firstCountry != NULL) {
		if (!strcmp(firstCountry->countryName, countryName))
			break;

		firstCountry = firstCountry->next;
	}
	return firstCountry;
}


//a
void insertSortCountryList(char countryName[], char fileName[], Country* head)
{
	Country* currentCountry = head;
	Country* newCountry = createCountry(countryName);

	while (currentCountry->next != NULL &&
		strcmp(countryName, currentCountry->next->countryName) > 0) 
	{
		currentCountry = currentCountry->next;
	}

	newCountry->next = currentCountry->next;
	currentCountry->next = newCountry;

	newCountry->cityRoot = readCitiesIntoTree(fileName, newCountry->cityRoot);
}

City* readCitiesIntoTree(char fileName[], City* root)
{
	char buffer[MAX_LINE] = "";
	char cityName[MAX_LEN] = "";
	char popul[MAX_LEN] = "";
	int population=0;
	FILE* filePointer = fopen(fileName, "r");

	while (!feof(filePointer)) {
		fgets(buffer, MAX_LINE, filePointer);
		if (sscanf(buffer, "%s %s", cityName, popul) == 2) {
			if (cityName[strlen(cityName) - 1] == ',')
				cityName[strlen(cityName) - 1] = '\0';
			population = atoi(popul);
			root = insertSortCitiesTree(root, createCity(cityName, population));
		}
	}

	fclose(filePointer);

	return root;
}

City* insertSortCitiesTree(City* root, City* newCity)
{
	
	if (root == NULL)
		return newCity;

	if (root->population > newCity->population)
		root->lc = insertSortCitiesTree(root->lc, newCity);
	else
		root->rc = insertSortCitiesTree(root->rc, newCity);

	//sort po imenu grada
	/*if (strcmp(newCity->cityName, root->cityName) < 0)
		root->lc = insertSortCitiesTree(root->lc, newCity);
	else
		root->rc = insertSortCitiesTree(root->rc, newCity);*/
			
	return root;
}



//b
Country* insertSortCountryTree(Country* root, Country* newCountry, char fileName[])
{
	if (root == NULL) {
		newCountry->cityHead = createCity("", 0);
		readCitiesIntoList(fileName, newCountry->cityHead);
		return newCountry;
	}
		
	if (strcmp(root->countryName, newCountry->countryName) > 0)
		root->lc = insertSortCountryTree(root->lc, newCountry, fileName);
	else
		root->rc = insertSortCountryTree(root->rc, newCountry, fileName);

	return root;
}

void readCitiesIntoList(char fileName[], City* head)
{
	char buffer[MAX_LINE] = "";
	char cityName[MAX_LEN] = "";
	char popul[MAX_LEN] = "";
	int population = 0;
	FILE* filePointer = fopen(fileName, "r");

	while (!feof(filePointer)) {
		fgets(buffer, MAX_LINE, filePointer);
		if (sscanf(buffer, "%s %s", cityName, popul) == 2) {
			if (cityName[strlen(cityName) - 1] == ',')
				cityName[strlen(cityName) - 1] = '\0';
			population = atoi(popul);
			insertSortCitiesList(head, createCity(cityName, population));
		}
	}

	fclose(filePointer);
}

void insertSortCitiesList(City* head, City* newCity)
{
	City* currentCity = head;

	while (currentCity->next != NULL && 
		strcmp(newCity->cityName, currentCity->next->cityName) > 0)
	{
		currentCity = currentCity->next;
	}

	//sort po broju stanovnika
	/*while (currentCity->next != NULL &&
		newCity->population > currentCity->next->population)
	{
		currentCity = currentCity->next;
	}*/

	newCity->next = currentCity->next;
	currentCity->next = newCity;
}