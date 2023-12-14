/*Napisati program koji omoguæava rad s binarnim stablom pretraživanja. Treba
omoguæiti unošenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronalaženje nekog elementa.*/

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


int  main()
{
	Node headNode = { .value = 0, .LC = NULL,.RC = NULL };
	Queue headQueue = { .levelNode = NULL,.next = NULL };
	
	MENU(headNode.LC, &headQueue);


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
	else if (value > rootNode->value) {
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