//Zadatak za oslobaðanje praktiènog dijela ispita :
//
//"BookHaven" je knjižnica koja želi unaprijediti svoj sustav praæenja knjiga, autora i korisnika.
//Svaka knjiga ima informacije o naslovu, autoru, godini izdanja i dostupnim primjercima.
//Korisnici mogu posuðivati knjige i vratiti ih nakon nekog vremena.
//
//Program mora korisniku omoguæiti :
//a) ispis svih knjiga abecedno(tako da se mogu vidjeti svi podatci o knjizi i korisnici koji trenutno posuðuju knjigu)
//b) ispis svih korisnika abecedno(tako da se mogu vidjeti sve knjige koje je neki korisnik posudio)
//c) pretraživanje knjiga po godini izdanja(tako da se mogu vidjeti sve knjige iz te godine i njihova kolièina)
//d) pretraživanje knjiga po nazivu autora(tako da se mogu vidjeti sve knjige tog autora i njihova kolièina)
//e) unos novog korisnika
//f) posudba knjige korisniku
//- može više primjeraka iste knjige posuditi od puta
//- korisnik u sebe nikad ne smije imati više od 5 knjiga sve skupa
//- ako pokušava napravit posudbu koja æe ga dovesti preko tog broja program treba upozoriti korisnika na to i ispisati mu broj koliko još knjiga posudit
//- ako korisnik pita za više primjeraka knjige, a knjižnica na stanju nema nema dovoljno treba korisniku omoguæiti izbor hoæe li posudit to što ima ili ne.
//g) povratak knjiga na stanje
//h) spremi(u datoteku / datoteke spremiti trenutno stanje u knjižnici, tako da kad se iduæi put pokrene program moguæe nastaviti od spremljenog stanja)
//
//Sami odaberite strukturu podataka i format datoteka koju æe te koristiti za ovo izvesti.

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>

#define MAX_LINE (1024)
#define MAX_LEN (100)
#define ERROR (-123)
#define ERROR_PTR (NULL)

typedef struct _borrowedBook {
	int bookId;
	int userId;
	int copies;
	struct _borrowedBook* next;
}BorrowedBook;

typedef struct _user {
	int id;
	char name[MAX_LEN];
	int totalBorrowedBooks;
	struct _user* next;
}User;

typedef struct _book {
	int id;
	char title[MAX_LEN];
	char author[MAX_LEN];
	int publishYear;
	int totalCopies;
	int availableCopies;
	struct _book* next;
}Book;

Book* createBook(int id, char title[MAX_LEN], char author[MAX_LEN], int publishYear, int totalCopies, int availableCopies);
User* createUser(int id, char name[MAX_LEN], int bookAmount);
BorrowedBook* createBorrowed(int bookId, int userId, int copies);

void insertSortedBook(Book* head, Book* newBook);
void insertSortedUser(User* head, User* newUser);
void insertSortedBorrowed(BorrowedBook* head, BorrowedBook* newBorrowed);

int GetBooksFromFile(Book* head); 
int GetUsersFromFile(User* head);
int GetBorrowedFromFile(BorrowedBook* head);

void MENU(Book* bHead, User* uHead, BorrowedBook* bbHead);

void sortBookAlph(Book* head);
void sortBookById(Book* head);
void printBooksAlph(Book* head, User* uHead, BorrowedBook* bbHead);
int bookUsers(User* uHead, BorrowedBook* bbHead, int bookId);
void sortBorrowedByUser(BorrowedBook* head);

void sortUserAlph(User* head);
void sortUserById(User* head);
void printUsersAlph(Book* bHead, User* head, BorrowedBook* bbHead);
int userBooks(Book* bHead, BorrowedBook* bbHead, int userId);
void sortBorrowedByBook(BorrowedBook* head);

int searchBooksByYear(Book* firstBook, int year);
int searchBooksByAuthor(Book* firstBook, char* author);

int addNewUser(User* uHead, char* name);

int borrowBook(Book* bHead, User* uHead, BorrowedBook* bbHead);
int getUserIdAndFreeSlots(User* head, int* userId, int isBorrowing);
int findUserByName(User* uHead, char* name, int* userBookAmount);
void getBookIdAndCopyAmount(Book* head, int* bookId, int* availableCopies);
int findBookByName(Book* bHead, char* name, int* availableCopies);
void reduceBookCopyAvaillability(Book* firstBook, int id, int copiesToRemove);
void increaseUserBookAmount(User* firstUser, int id, int copiesToAdd);

int returnBook(Book* bHead, User* uHead, BorrowedBook* bbHead);
BorrowedBook* findReturningPrev(BorrowedBook* bbHead, int userId, int* bookId, int choice);
void increaseBookCopyAvaillability(Book* firstBook, int id, int copiesToAdd);
void reduceUserBookAmount(User* firstUser, int id, int copiesToRemove);

int SetBooksIntoFile(Book* FirstBook);
int SetUsersIntoFile(User* FirstUser);
int SetBorrowedIntoFile(BorrowedBook* firstBorrowed);

void main()
{
	Book bHead = { .id = 0, .title = "", .publishYear = 0, .author = "", .totalCopies = 0, .availableCopies = 0, .next = NULL };
	User uHead = { .id = 0, .name = "", .totalBorrowedBooks = 0, .next = NULL };
	BorrowedBook bbHead = { .bookId = 0, .userId = 0, .copies = 0, .next = NULL };
		
		
	GetBooksFromFile(&bHead);
	GetUsersFromFile(&uHead);
	GetBorrowedFromFile(&bbHead);

	MENU(&bHead, &uHead, &bbHead);
	
}


void MENU(Book* bHead, User* uHead, BorrowedBook* bbHead)
{
	int choice = 0;
	int year = 0;
	char author[MAX_LEN] = "";
	char userName[MAX_LEN] = "";
	char c;


	while (1) {

		printf("\n\nMENU:\n");
		printf("1. List books alphabetically\n");
		printf("2. List library memberes alphabetically\n");
		printf("3. Search books by publishing year\n");
		printf("4. Search books by author\n");
		printf("5. Make a new library membership\n");
		printf("6. Borrow books\n");
		printf("7. Return books\n");
		printf("8. Leave library\n");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			printBooksAlph(bHead, uHead, bbHead);
			break;

		case 2:
			printUsersAlph(bHead, uHead, bbHead);
			break;

		case 3:
			printf("Enter year(YYYY): ");
			scanf("%d", &year);
			if(!searchBooksByYear(bHead->next, year))
				printf("We don't have any books published in this year.\n");
			break;

		case 4:
			printf("Enter author: ");
			scanf("%c", &c);
			fgets(author, MAX_LEN, stdin);
			if (author[strlen(author) - 1] == '\n')
				author[strlen(author) - 1] = '\0';
			if (!searchBooksByAuthor(bHead->next, author))
				printf("We don't have any books by this author.\n");
			break;

		case 5:
			printf("Enter your name: ");
			scanf("%c", &c);
			fgets(userName, MAX_LEN, stdin);
			if (userName[strlen(userName) - 1] == '\n')
				userName[strlen(userName) - 1] = '\0';
			addNewUser(uHead, userName);
			break;

		case 6:
			borrowBook(bHead, uHead, bbHead);
			break;

		case 7:
			returnBook(bHead, uHead, bbHead);
			break;

		case 8:
			return;
			break;

		default:
			printf("You chose an invalid option. Try again.\n");
			break;
		}
	}

}



#pragma region create list elements

Book* createBook(int id, char title[MAX_LEN], char author[MAX_LEN], int publishYear, int totalCopies, int availableCopies)
{
	Book* newBook = (Book*)malloc(sizeof(Book));

	if (!newBook) {
		printf("Memory allocation error");
		return ERROR_PTR;
	}

	newBook->id = id;
	strcpy(newBook->title, title);
	newBook->publishYear = publishYear;
	strcpy(newBook->author, author);
	newBook->totalCopies = totalCopies;
	newBook->availableCopies = availableCopies;
	newBook->next = NULL;

	return newBook;
}

User* createUser(int id, char name[MAX_LEN], int bookAmount)
{
	User* newUser = (User*)malloc(sizeof(User));

	if (!newUser) {
		printf("Memory allocation error");
		return ERROR_PTR;
	}

	newUser->id = id;
	strcpy(newUser->name, name);
	newUser->totalBorrowedBooks = bookAmount;
	newUser->next = NULL;

	return newUser;
}

BorrowedBook* createBorrowed(int bookId, int userId, int copies)
{
	BorrowedBook* newBorrowed = (BorrowedBook*)malloc(sizeof(BorrowedBook));

	if (!newBorrowed) {
		printf("Memory allocation error");
		return ERROR_PTR;
	}

	newBorrowed->bookId = bookId;
	newBorrowed->userId = userId;
	newBorrowed->copies = copies;
	newBorrowed->next = NULL;

	return newBorrowed;
}

#pragma endregion



#pragma region initialize library

int GetBooksFromFile(Book* head)
{
	int id = 0, publishYear = 0, totalCopies = 0, availableCopies = 0;
	char title[MAX_LEN] = "", author[MAX_LEN] = "";
	char buffer[MAX_LINE] = "";
	Book* newBook = NULL;

	FILE* filePointer = fopen("Book.txt", "r");
	
	if (!filePointer) {
		printf("File error");
		return ERROR;
	}

	while (!feof(filePointer)) {
		fgets(buffer, MAX_LINE, filePointer);

		id = atoi(strtok(buffer, "#"));
		strcpy(title, strtok(NULL, "#"));
		publishYear = atoi(strtok(NULL, "#"));
		strcpy(author, strtok(NULL, "#"));
		totalCopies = atoi(strtok(NULL, "#"));
		availableCopies = atoi(strtok(NULL, "#"));

		newBook = createBook(id, title, author, publishYear, totalCopies, availableCopies);
		if (!newBook) {
			return ERROR;
		}

		insertSortedBook(head, newBook);
	}

	fclose(filePointer);
	return EXIT_SUCCESS;
}

int GetUsersFromFile(User* head)
{
	int id = 0, bookAmount = 0;
	char name[MAX_LEN] = "";
	char buffer[MAX_LINE] = "";
	User* newUser=NULL;

	FILE* filePointer = fopen("User.txt", "r");

	if (!filePointer) {
		printf("File error");
		return ERROR;
	}

	while (!feof(filePointer)) {
		fgets(buffer, MAX_LINE, filePointer);

		id = atoi(strtok(buffer, "#"));
		strcpy(name, strtok(NULL, "#"));
		bookAmount = atoi(strtok(NULL, "#"));

		newUser = createUser(id, name, bookAmount);
		if (!newUser) {
			return ERROR;
		}

		insertSortedUser(head, newUser);
	}

	fclose(filePointer);
	return EXIT_SUCCESS;
}

int GetBorrowedFromFile(BorrowedBook* head)
{
	int bookId = 0, userId = 0, copies = 0;
	char buffer[MAX_LINE] = "";
	BorrowedBook* newBorrowed=NULL;

	FILE* filePointer = fopen("BorrowedBook.txt", "r");

	if (!filePointer) {
		printf("File error");
		return ERROR;
	}

	while (!feof(filePointer)) {
		fgets(buffer, MAX_LINE, filePointer);

		bookId = atoi(strtok(buffer, "#"));
		userId = atoi(strtok(NULL, "#"));
		copies = atoi(strtok(NULL, "#"));

		newBorrowed = createBorrowed(bookId, userId, copies);
		if (!newBorrowed) {
			return ERROR;
		}

		insertSortedBorrowed(head, newBorrowed);
	}

	fclose(filePointer);
	return EXIT_SUCCESS;
}


void insertSortedBook(Book* head, Book* newBook)
{
	Book* current = head;
	while (current->next != NULL && current->next->id < newBook->id) {
		current = current->next;
	}
	newBook->next = current->next;
	current->next = newBook;
}

void insertSortedUser(User* head, User* newUser)
{
	User* current = head;
	while (current->next != NULL && current->next->id < newUser->id) {
		current = current->next;
	}
	newUser->next = current->next;
	current->next = newUser;
}

void insertSortedBorrowed(BorrowedBook* head, BorrowedBook* newBorrowed)
{
	BorrowedBook* current = head;
	while (current->next != NULL && current->next->bookId < newBorrowed->bookId) {
		current = current->next;
	}
	newBorrowed->next = current->next;
	current->next = newBorrowed;
}

#pragma endregion



#pragma region print out books alphabeticaly

void printBooksAlph(Book* head, User* uHead, BorrowedBook* bbHead)
{
	sortBookAlph(head);

	Book* current = head->next;

	while (current != NULL) {
		printf("\n%s(%d); %s; %d.; %d/%d\n", current->title, current->id, current->author, current->publishYear, current->availableCopies, current->totalCopies);
		bookUsers(uHead, bbHead, current->id);
		printf("\n");
		current = current->next;
	}

	sortBookById(head);
}

//finds all users who have a specific book and lists them out on the console
int bookUsers(User* uHead, BorrowedBook* bbHead, int bookId)
{
	int i = 1;
	BorrowedBook* currentBorrow = bbHead->next;
	BorrowedBook* filterListH = createBorrowed(0, 0, 0);
	if (!filterListH) {
		return ERROR;
	}

	//makes a filter list with all the borrowings of the book(id) 
	while (currentBorrow != NULL) {
		if (currentBorrow->bookId == bookId) {
			
			BorrowedBook* newBorrowed = createBorrowed(currentBorrow->bookId, currentBorrow->userId, currentBorrow->copies);
			if (!newBorrowed) {
				return ERROR;
			}

			insertSortedBorrowed(filterListH, newBorrowed);
		}
		currentBorrow = currentBorrow->next;
	}
	sortBorrowedByUser(filterListH);

	User* currentUser = uHead->next;
	currentBorrow = filterListH->next;

	//matches users with the id from the filter list and prints out their names
	while (currentUser != NULL && currentBorrow != NULL) {

		if (currentBorrow->userId == currentUser->id) {
			printf("%d - %s\n", i, currentUser->name);
			currentBorrow = currentBorrow->next;
			i++;
		}
		currentUser = currentUser->next;
	}
	return EXIT_SUCCESS;
}

void sortBorrowedByUser(BorrowedBook* head)
{
	BorrowedBook* current = head;
	BorrowedBook* temp = current->next;
	BorrowedBook* last = NULL;

	while (current->next != last) {
		while (temp->next != last) {
			if (temp->userId > temp->next->userId) {
				current->next = temp->next;
				temp->next = current->next->next;
				current->next->next = temp;
			}

			current = current->next;
			temp = current->next;
		}
		last = temp;
		current = head;
		temp = current->next;
	}
}

void sortBookAlph(Book* head)
{
	Book* current = head;
	Book* temp = current->next;
	Book* last = NULL;

	while (current->next != last) {
		while (temp->next != last) {
			if (strcmp(temp->title, temp->next->title) > 0) {
				current->next = temp->next;
				temp->next = current->next->next;
				current->next->next = temp;
			}

			current = current->next;
			temp = current->next;
		}
		last = temp;
		current = head;
		temp = current->next;
	}
}

void sortBookById(Book* head)
{
	Book* current = head;
	Book* temp = current->next;
	Book* last = NULL;

	while (current->next != last) {
		while (temp->next != last) {
			if (temp->id > temp->next->id) {
				current->next = temp->next;
				temp->next = current->next->next;
				current->next->next = temp;
			}

			current = current->next;
			temp = current->next;
		}
		last = temp;
		current = head;
		temp = current->next;
	}
}

#pragma endregion



#pragma region print out users alphabeticaly

void sortUserAlph(User* head)
{
	User* current = head;
	User* temp = current->next;
	User* last = NULL;

	while (current->next != last) {
		while (temp->next != last) {
			if (strcmp(temp->name, temp->next->name) > 0) {
				current->next = temp->next;
				temp->next = current->next->next;
				current->next->next = temp;
			}

			current = current->next;
			temp = current->next;
		}
		last = temp;
		current = head;
		temp = current->next;
	}
}

void sortUserById(User* head)
{
	User* current = head;
	User* temp = current->next;
	User* last = NULL;

	while (current->next != last) {
		while (temp->next != last) {
			if (temp->id > temp->next->id) {
				current->next = temp->next;
				temp->next = current->next->next;
				current->next->next = temp;
			}

			current = current->next;
			temp = current->next;
		}
		last = temp;
		current = head;
		temp = current->next;
	}
}
void printUsersAlph(Book* bHead, User* head, BorrowedBook* bbHead)
{
	sortUserAlph(head);

	User* current = head->next;

	while (current != NULL) {
		printf("\n%s(%d); %d/5\n", current->name, current->id, current->totalBorrowedBooks);
		userBooks(bHead, bbHead, current->id);
		printf("\n");
		current = current->next;
	}

	sortUserById(head);
}

//finds and prints out all the books a specific user has
int userBooks(Book* bHead, BorrowedBook* bbHead, int userId)
{
	int i = 1;
	BorrowedBook* currentBorrow = bbHead->next;
	BorrowedBook* filterListH = createBorrowed(0, 0, 0);
	if (!filterListH) {
		return ERROR;
	}

	//filter list of all the active borrowings our user has
	while (currentBorrow != NULL) {
		if (currentBorrow->userId == userId) {
			BorrowedBook* newBorrow = createBorrowed(currentBorrow->bookId, currentBorrow->userId, currentBorrow->copies);
			if (!newBorrow) {
				return ERROR;
			}
			insertSortedBorrowed(filterListH, newBorrow);
		}
		currentBorrow = currentBorrow->next;
	}
	sortBorrowedByBook(filterListH);

	Book* currentBook = bHead->next;
	currentBorrow = filterListH->next;

	//matches filter list book id-s and prints out the title
	while (currentBook != NULL && currentBorrow != NULL) {

		if (currentBorrow->bookId == currentBook->id) {
			printf("%d - %s\n", i, currentBook->title);
			currentBorrow = currentBorrow->next;
			i++;
		}
		currentBook = currentBook->next;
	}

	//returns how many titles a user has(this is used for returning books)
	return i - 1;
}
void sortBorrowedByBook(BorrowedBook* head)
{
	BorrowedBook* current = head;
	BorrowedBook* temp = current->next;
	BorrowedBook* last = NULL;

	while (current->next != last) {
		while (temp->next != last) {
			if (temp->bookId > temp->next->bookId) {
				current->next = temp->next;
				temp->next = current->next->next;
				current->next->next = temp;
			}

			current = current->next;
			temp = current->next;
		}
		last = temp;
		current = head;
		temp = current->next;
	}
}

#pragma endregion



#pragma region search books by

int searchBooksByYear(Book* firstBook, int year)
{
	Book* current = firstBook;
	int count = 0;

	while (current != NULL) {
		if (current->publishYear == year) {
			printf(" - %s; %s; %d/%d\n", current->title, current->author, current->availableCopies, current->totalCopies);
			count++;
		}
		current = current->next;
	}
	return count;
}

int searchBooksByAuthor(Book* firstBook, char* author)
{
	Book* current = firstBook;
	int count = 0;
	
	while (current != NULL) {

		if (strcmp(current->author, author) == 0) {
			printf(" - %s; %d/%d\n", current->title, current->availableCopies, current->totalCopies);
			count++;
		}
		current = current->next;
	}
	return count;
}

#pragma endregion



int addNewUser(User* uHead, char* name)
{
	User* current = uHead;
	int userLastId = 0;
	
	while (current->next != NULL)
		current = current->next;

	userLastId = current->id;
		
	User* newUser = createUser(userLastId + 1, name, 0);
	if (!newUser) {
		return ERROR;
	}

	insertSortedUser(uHead, newUser);

	if(SetUsersIntoFile(uHead->next)==ERROR)
		return ERROR;

	return EXIT_SUCCESS;
}



#pragma region borrow books

int borrowBook(Book* bHead, User* uHead, BorrowedBook* bbHead)
{
	
	int bookId = 0, userId = 0, choice = 0, wantedCopies = 0;
	int availableCopies = 0;
	int freeSlots = 5;
	int allowedNumOfCopies = 0;
	int isBorrowing = 1;
		
	freeSlots = getUserIdAndFreeSlots(uHead, &userId, isBorrowing);
	
	getBookIdAndCopyAmount(bHead, &bookId, &availableCopies);
	

	if (userId < 0 || bookId < 0)
		return ERROR;

	if (freeSlots == 0 || availableCopies == 0)
		return ERROR;

	if (availableCopies < freeSlots)
		allowedNumOfCopies = availableCopies;
	else
		allowedNumOfCopies = freeSlots;

	
	printf("How many copies of this book do you wat to borrow?\n");
	scanf("%d", &wantedCopies);

	if (wantedCopies > allowedNumOfCopies) {
		printf("We apologise, but you are allowed to take a maximum of %d copies of this book.\n", allowedNumOfCopies);
		printf("Do you still want to check out this book?\n");
		printf("1 - Yes\n");
		printf("2 - No\n");
		scanf("%d", &choice);
		printf("\n");

		if (choice == 2)
			return ERROR;
		else if (choice == 1)
			wantedCopies = allowedNumOfCopies;
	}

	//if a book is borrowed save changes in library
	BorrowedBook* newBorrowed = createBorrowed(bookId, userId, wantedCopies);
	if (!newBorrowed) {
		return ERROR;
	}
	insertSortedBorrowed(bbHead, newBorrowed);
	if(SetBorrowedIntoFile(bbHead->next)==ERROR)
		return ERROR;

	reduceBookCopyAvaillability(bHead->next, bookId, wantedCopies);
	if (SetBooksIntoFile(bHead->next) == ERROR)
		return ERROR;

	increaseUserBookAmount(uHead->next, userId, wantedCopies);
	if (SetUsersIntoFile(uHead->next) == ERROR)
		return ERROR;
	
	return EXIT_SUCCESS;
}

int getUserIdAndFreeSlots(User* head, int* userId, int isBorrowing)
{
	char userName[MAX_LEN] = "";
	int userBookAmount = 0, choice = 0;
	int freeSlots = 5;
	char c;

	printf("Enter your name: ");
	scanf("%c", &c);
	fgets(userName, MAX_LEN, stdin);
	if (userName[strlen(userName) - 1] == '\n')
	userName[strlen(userName) - 1] = '\0';

	*userId = findUserByName(head, userName, &userBookAmount);

	if (isBorrowing < 0) {
		if (*userId < 0) {
			printf("You don't have a membership in this library.\n");
			return ERROR;
		}
		else
			return EXIT_SUCCESS;
	}

	if (*userId < 0) {
		printf("You don't have a membership in this library.\n");
		printf("Do you want to make one?\n");
		printf("1 - Yes\n");
		printf("2 - No\n");
		scanf("%d", &choice);
		printf("\n");

		if (choice == 1) {
			addNewUser(head, userName);
			*userId = findUserByName(head, userName, &userBookAmount);
		}
		else {
			printf("You can't borrow books unless you have a membership.\n");
			return 0;
		}
	}

	freeSlots -= userBookAmount;

	if (freeSlots == 0) {
		printf("Unfortunately, you can't borrow any more books.\nPlease return some of the books you already have so you can take new ones.\n");
		return 0;
	}

	return freeSlots;
}

int findUserByName(User* uHead, char* name, int* userBookAmount)
{
	sortUserAlph(uHead);

	int userId = 0;
	User* current = uHead->next;
	

	while (current != NULL && strcmp(current->name, name) < 0) 
		current = current->next;
			
	if (current != NULL && strcmp(current->name, name) == 0) {
		userId = current->id;
		*userBookAmount = current->totalBorrowedBooks;
	}
	else
		userId = -1;

	sortUserById(uHead);

	return userId;
}


void getBookIdAndCopyAmount(Book* head, int* bookId, int* availableCopies)
{
	char title[MAX_LEN] = "";
	int choice = 0;
	char c;
	int flag = 0;

	do
	{
		printf("Enter title of book: ");
		if(flag) 
			scanf("%c", &c);
		fgets(title, MAX_LEN, stdin);
		if (title[strlen(title) - 1] == '\n')
			title[strlen(title) - 1] = '\0';

		*bookId = findBookByName(head, title, availableCopies);

		if (*bookId < 0) {
			printf("This library doesn't have a book with that title.\n");
			printf("Do you want to try searching another book?\n");
			printf("1 - Yes\n");
			printf("2 - No\n");
			scanf("%d", &choice);
			printf("\n");
			flag++;
		}
		else
			choice = 2;

	} while (choice == 1);
			

	if (*bookId < 0)
		return;

	if (*availableCopies == 0) {
		printf("Unfortunately, this book is out of stock.\nPlease come back at a later date to try again.\n");
		return;
	}

}


int findBookByName(Book* bHead, char* name, int* availableCopies)
{
	sortBookAlph(bHead);

	int bookId = 0;
	Book* current = bHead->next;
	

	while (current != NULL && strcmp(current->title, name) < 0) 
		current = current->next;
	
	if (current != NULL && strcmp(current->title, name) == 0) {
		bookId = current->id;
		*availableCopies = current->availableCopies;
	}		
	else
		bookId = -1;

	sortBookById(bHead);

	return bookId;
}

void reduceBookCopyAvaillability(Book* firstBook, int id, int copiesToRemove)
{
	Book* current = firstBook;

	while (current != NULL && current->id < id)
		current = current->next;

	if (current != NULL && current->id == id) {
		current->availableCopies -= copiesToRemove;
	}
}

void increaseUserBookAmount(User* firstUser, int id, int copiesToAdd)
{
	User* current = firstUser;

	while (current != NULL && current->id < id)
		current = current->next;

	if (current != NULL && current->id == id) {
		current->totalBorrowedBooks += copiesToAdd;
	}
}

#pragma endregion



#pragma region return books

int returnBook(Book* bHead, User* uHead, BorrowedBook* bbHead)
{
	int userId = 0, bookId = 0, isBorrowing = -1, bookTitleAmount = 0;
	int choice = 0, returnCopies = 0;
	BorrowedBook* returningBBprevious = NULL;

	if (getUserIdAndFreeSlots(uHead, &userId, isBorrowing) == ERROR)
		return ERROR;

	bookTitleAmount = userBooks(bHead, bbHead, userId);
	if (bookTitleAmount == ERROR)
		return ERROR;

	while (choice==0)
	{
		printf("What book do you want to return?\n");
		printf("Enter the number:");
		scanf("%d", &choice);

		if (choice<1 || choice>bookTitleAmount) {
			printf("You chose an invalid option. try again.\n");
			choice = 0;
		}
	}

	returningBBprevious = findReturningPrev(bbHead, userId, &bookId, choice);
	if (returningBBprevious->next == NULL)
		return ERROR;

	printf("How many copies of this book do you want to return?\n");
	printf("Enter the number: ");
	scanf("%d", &returnCopies);

	returningBBprevious->next->copies -= returnCopies;
	if (returningBBprevious->next->copies < 1) {
		returnCopies += returningBBprevious->next->copies;
		returningBBprevious->next = returningBBprevious->next->next;
	}
	if (SetBorrowedIntoFile(bbHead->next) == ERROR)
		return ERROR;


	increaseBookCopyAvaillability(bHead->next, bookId, returnCopies);
	if (SetBooksIntoFile(bHead->next) == ERROR)
		return ERROR;

	reduceUserBookAmount(uHead->next, userId, returnCopies);
	if (SetUsersIntoFile(uHead->next) == ERROR)
		return ERROR;

	return EXIT_SUCCESS;
}

BorrowedBook* findReturningPrev(BorrowedBook* bbHead, int userId, int* bookId, int choice) {
	
	BorrowedBook* current = bbHead;

	while (current->next != NULL && choice > 0) {
		if (current->next->userId == userId) {
			*bookId = current->next->bookId;
			choice--;
		}
	}

	return current;
}


void increaseBookCopyAvaillability(Book* firstBook, int id, int copiesToAdd)
{
	Book* current = firstBook;

	while (current != NULL && current->id < id)
		current = current->next;

	if (current != NULL && current->id == id) {
		current->availableCopies += copiesToAdd;
	}
}


void reduceUserBookAmount(User* firstUser, int id, int copiesToRemove)
{
	User* current = firstUser;

	while (current != NULL && current->id < id)
		current = current->next;

	if (current != NULL && current->id == id) {
		current->totalBorrowedBooks -= copiesToRemove;
	}
}


#pragma endregion



#pragma region save library state

int SetBooksIntoFile(Book* FirstBook)
{
	FILE* filePointer = fopen("Book.txt", "w");
	if (!filePointer) {
		printf("File error");
		return ERROR;
	}

	Book* current = FirstBook;

	while (current->next != NULL) {
		fprintf(filePointer, "%d#%s#%d#%s#%d#%d#\n", current->id, current->title, current->publishYear, current->author, current->totalCopies, current->availableCopies);
		current = current->next;
	}
	fprintf(filePointer, "%d#%s#%d#%s#%d#%d#", current->id, current->title, current->publishYear, current->author, current->totalCopies, current->availableCopies);
	

	fclose(filePointer);
	return EXIT_SUCCESS;
}


int SetUsersIntoFile(User* FirstUser)
{
	FILE* filePointer = fopen("User.txt", "w");
	if (!filePointer) {
		printf("File error");
		return ERROR;
	}

	User* current = FirstUser;

	while (current->next != NULL) {
		fprintf(filePointer, "%d#%s#%d#\n", current->id, current->name, current->totalBorrowedBooks);
		current = current->next;
	}
	fprintf(filePointer, "%d#%s#%d#", current->id, current->name, current->totalBorrowedBooks);

	fclose(filePointer);
	return EXIT_SUCCESS;
}


int SetBorrowedIntoFile(BorrowedBook* firstBorrowed)
{
	FILE* filePointer = fopen("BorrowedBook.txt", "w");
	if (!filePointer) {
		printf("File error");
		return ERROR;
	}

	BorrowedBook* current = firstBorrowed;

	while (current->next != NULL) {
		fprintf(filePointer, "%d#%d#%d#\n", current->bookId, current->userId, current->copies);
		current = current->next;
	}
	fprintf(filePointer, "%d#%d#%d#", current->bookId, current->userId, current->copies);
	
	fclose(filePointer);
	return EXIT_SUCCESS;
}

#pragma endregion
