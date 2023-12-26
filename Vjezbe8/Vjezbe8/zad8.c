/*8.Napisati program koji omogu�ava rad s binarnim stablom pretra�ivanja. Treba
omogu�iti uno�enje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronala�enje nekog elementa.*/

/*9. Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u �vorove binarnog stabla.
a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vra�a
pokaziva� na korijen stabla.
b) Napisati funkciju replace koja �e svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat �e se u stablo na
slici Slika 2.
c) Prepraviti program na na�in da umjesto predefiniranog cjelobrojnog polja kori�tenjem
funkcije rand() generira slu�ajne brojeve u rasponu <10, 90>. Tako�er, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon kori�tenja funkcije iz a), zatim b)
dijela zadatka.
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _node {
	int value;
	struct _node* LC;
	struct _node* RC;
}Node;

typedef struct _queue {
	Node* levelNode;
	struct _queue* next;
}Queue;

//zad 8
int MENU(Node* rootNode, Queue* headQueue);

Node* createNode(int value);
Node* insertNode(Node* rootNode, int value);

Node* search(Node* rootNode, int value);

void inorder(Node* rootNode);
void preorder(Node* rootNode);
void postorder(Node* rootNode);
void levelOrder(Node* rootNode, Queue* head);

Node* deleteNode(Node* rootNode, int value);
Node* findMin(Node* rootNode);

Queue* createQueue(Node* node);
void enQueue(Queue* head, Node* Child);
Node* deQueue(Queue* head);

//zad 9
int replace(Node* rootNode);
void inorderInFile(Node* rootNode,char* fileName);
void inorderIntoList(Node* rootNode, Node* listHead);
void addToEnd(Node* listHead, Node* newEl);
void clearTree(Node* rootNode);


int  main()
{
	Node headNode = { .value = 0, .LC = NULL,.RC = NULL };
	Queue headQueue = { .levelNode = NULL,.next = NULL };
	
	//MENU(headNode.LC, &headQueue);

	Node* root = NULL;
	root = insertNode(root, 2);
	root = insertNode(root, 5);
	root = insertNode(root, 7);
	root = insertNode(root, 8);
	root = insertNode(root, 11);
	root = insertNode(root, 1);
	root = insertNode(root, 4);
	root = insertNode(root, 2);
	root = insertNode(root, 3);
	root = insertNode(root, 7);

	inorder(root);
	inorderInFile(root, "ogTree.txt");
	printf("\n");

	replace(root);

	inorder(root);
	inorderInFile(root, "replacedTree.txt");

	return 0;
}



int MENU(Node* rootNode, Queue* headQueue)
{
	int value = 0;
	
	while (1) {

		printf("\nMenu:\n");
		printf("1 - Insert new node\n");
		printf("2 - Search node\n");
		printf("3 - Inorder print\n");
		printf("4 - Preorder print\n");
		printf("5 - Postorder print\n");
		printf("6 - Level order print\n");
		printf("7 - Delete Node\n");
		printf("8 - exit\n");

		int choice;
		printf("Enter your choice: ");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			printf("Enter the value: ");
			scanf("%d", &value);
			rootNode = insertNode(rootNode, value);
			break;

		case 2:
			printf("Enter the value: ");
			scanf("%d", &value);

			if (search(rootNode, value) != NULL) {
				printf("Node is found\n");
			}
			else {
				printf("Node doesn't exist\n");
			}
			
			break;

		case 3:
			printf("INORDER: "); 
			inorder(rootNode);
			printf("\n");
			break;

		case 4:
			printf("PREORDER: ");
			preorder(rootNode);
			printf("\n"); 
			break;

		case 5:
			printf("POSTORDER: ");
			postorder(rootNode);
			printf("\n");
			break;

		case 6:
			printf("LEVEL ORDER: ");
			levelOrder(rootNode,headQueue);
			printf("\n");
			break;

		case 7:
			printf("Enter the value: ");
			scanf("%d", &value);
			rootNode = deleteNode(rootNode, value);
			break;
		
		case 8:
			return 0;
			break;

		default:
			printf("Chose an invalid option. Try again.\n");
			break;
		}
	}

}


Node* createNode(int value)
{
	Node* newNode = NULL;
	
	newNode = (Node*)malloc(sizeof(Node));
	if (!newNode) {
		printf("Failed memory allocation");
		return NULL;
	}

	newNode->value = value;
	newNode->LC = NULL;
	newNode->RC = NULL;

	return newNode;
}


Node* insertNode(Node* rootNode, int value)
{
	if (rootNode == NULL) {
		rootNode = createNode(value);
	}
	else if (value < rootNode->value) {
		rootNode->LC = insertNode(rootNode->LC, value);
	}
	else if (value >= rootNode->value) {
		rootNode->RC = insertNode(rootNode->RC, value);
	}

	return rootNode;

}


Node* search(Node* rootNode, int value)
{
	if (rootNode == NULL) {
		return rootNode;
	}
	
	Node* adress = NULL;
	
	if (value < rootNode->value) {
		adress = search(rootNode->LC, value);
	}
	else if (value > rootNode->value) {
		adress = search(rootNode->RC, value);
	}
	else {
		adress = rootNode;
	}

	return adress;

}


void inorder(Node* rootNode)
{
	if (rootNode == NULL) {
		printf("\t(empty)\n");
		return;
	}
	
	
	if (rootNode->LC != NULL) {
		inorder(rootNode->LC);
	}
	
	printf("%d ", rootNode->value);
	
	if (rootNode->RC != NULL) {
		inorder(rootNode->RC);
	}
	return;
}


void preorder(Node* rootNode)
{
	if (rootNode == NULL) {
		printf("\t(empty)\n");
		return;
	}

	printf("%d ", rootNode->value);

	if (rootNode->LC != NULL) {
		inorder(rootNode->LC);
	}

	if (rootNode->RC != NULL) {
		inorder(rootNode->RC);
	}
	return;
}


void postorder(Node* rootNode)
{
	if (rootNode == NULL) {
		printf("\t(empty)\n");
		return;
	}

	if (rootNode->LC != NULL) {
		inorder(rootNode->LC);
	}

	if (rootNode->RC != NULL) {
		inorder(rootNode->RC);
	}

	printf("%d ", rootNode->value);

	return;
}


void levelOrder(Node* rootNode, Queue* head)
{
	Node* current = NULL;

	printf("%d ", rootNode->value);

	if(rootNode->LC)
		enQueue(head, rootNode->LC);
	if(rootNode->RC)
		enQueue(head, rootNode->RC);

	while (head->next) {
		current = deQueue(head);
		printf("%d ", current->value);

		if(current->LC)
			enQueue(head, current->LC);
		if(current->RC)
			enQueue(head, current->RC);
	}
}


void enQueue(Queue* head, Node* Child)
{
	if (!head)
		return;

	Queue* current = head;

	while (current->next != NULL) {
		current = current->next;
	}

	current->next = createQueue(Child);

}


Node* deQueue(Queue* head)
{
	if (!head)
		return NULL;
	if (head->next == NULL)
		return NULL;
	
	Queue* temp = NULL;
	Node* tempNode = NULL;

	temp = head->next;
	head->next = temp->next;

	tempNode = temp->levelNode;
	free(temp);

	return tempNode;
}


Queue* createQueue(Node* node)
{
	Queue* newQueueElement = NULL;

	newQueueElement = (Queue*)malloc(sizeof(Queue));
	if (!newQueueElement) {
		printf("Failed memory allocation");
		return NULL;
	}

	newQueueElement->levelNode = node;
	newQueueElement->next = NULL;

	return newQueueElement;
}


Node* deleteNode(Node* rootNode, int value)
{
	if (rootNode == NULL) {
		printf("Doesn't exist");
		return rootNode;
	}

	Node* temp = NULL;

	if (value < rootNode->value) {
		rootNode->LC = deleteNode(rootNode->LC, value);
	}
	else if (value > rootNode->value) {
		rootNode->RC = deleteNode(rootNode->RC, value);
	}
	else if (rootNode->LC && rootNode->RC) {
		temp = findMin(rootNode->RC);
		rootNode->value = temp->value;
		rootNode->RC = deleteNode(rootNode->RC, rootNode->value);
	}
	else {
		temp = rootNode;
		
		if (rootNode->LC == NULL) {
			rootNode = rootNode->RC;
		}
		else {
			rootNode = rootNode->LC;
		}

		free(temp);
	}

	return rootNode;
}


Node* findMin(Node* rootNode)
{
	if (rootNode == NULL) {
		return rootNode;
	}

	if (rootNode->LC == NULL) {
		return rootNode;
	}

	return findMin(rootNode->LC);
}

int replace(Node* rootNode)
{
	if (rootNode == NULL)
		return 0;

	int originalValue = rootNode->value;

	int LCValue = replace(rootNode->LC);
	int RCValue = replace(rootNode->RC);
	
	rootNode->value = LCValue + RCValue;

	return originalValue + rootNode->value;
}


void inorderInFile(Node* rootNode, char* fileName)
{
	FILE* filePointer = NULL;
	filePointer = fopen(fileName, "w");

	if (rootNode == NULL) {
		fprintf(filePointer, "\t(empty)\n");
		return;
	}

	Node* inorderListHead = createNode(0);

	inorderIntoList(rootNode, inorderListHead);
	Node* current = inorderListHead->RC;

	while (current != NULL) {
		fprintf(filePointer, "%d ", current->value);
		current = current->RC;
	}
	
	fclose(filePointer);

	clearTree(inorderListHead);
}


void inorderIntoList(Node* rootNode, Node* listHead) {

	if (rootNode == NULL) 
		return;
		
	if (rootNode->LC != NULL) {
		inorderIntoList(rootNode->LC, listHead);
	}

	Node* newEl = createNode(rootNode->value);
	addToEnd(listHead, newEl);

	if (rootNode->RC != NULL) {
		inorderIntoList(rootNode->RC, listHead);
	}
}

void addToEnd(Node* listHead, Node* newEl)
{
	Node* current = listHead;

	while (current->RC != NULL) {
		current = current->RC;
	}

	newEl->RC = current->RC;
	current->RC = newEl;
}


void clearTree(Node* rootNode)
{
	if (rootNode == NULL)
		return;

	clearTree(rootNode->LC);

	clearTree(rootNode->RC);

	free(rootNode);
}