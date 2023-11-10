#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE (50)
#define MAX_LENGTH (1024)


typedef struct _stackElement {
	double number;
	struct _stackElement* next;
} StackElement;

StackElement* createStackElement(double number); 
int readFile(char* buffer);
int parseStringIntoPostfix(StackElement* head, char* buffer, double* result);
int popAndPerformOperation(StackElement* head, char operation, double* result);
int pop(StackElement* head, double* result);
int push(StackElement* head, StackElement* newStackElement);



int main()
{
	StackElement Head = { .number = 0.0, .next = NULL };
	char buffer[MAX_LENGTH] = "";
	double result = 0.0;

	if (readFile(buffer)) {
		return 1;
	}
	printf("Postfix: %s \n", buffer);

	if (parseStringIntoPostfix(&Head, buffer, &result)) {
		return 1;
	}
	printf("Result: %0.1lf\n", result);
	

	return 0;
}

StackElement* createStackElement(double number)
{
	StackElement* newElement = NULL;

	newElement = (StackElement*)malloc(sizeof(StackElement));

	if (newElement == NULL) {
		perror("malloc");
		return NULL;
	}

	newElement->number = number;
	newElement->next = NULL;

	return newElement;
}

int readFile(char* buffer)
{
	FILE* filePointer = NULL;
	filePointer = fopen("postfix.txt", "r");

	if (!filePointer) {
		perror("fopen");
		return 1;
	}

	fgets(buffer, MAX_LENGTH, filePointer);

	fclose(filePointer);

	return 0;
}


int parseStringIntoPostfix(StackElement* head, char* buffer, double* result)
{
	char* currentBuffer = buffer;
	int status = 0;
	int numBytes = 0;
	double number = 0.0;
	char operation = 0;
	StackElement* newElement = NULL;

	while (strlen(currentBuffer) > 0) {

		status = sscanf(currentBuffer, " %lf %n", &number, &numBytes);

		if (status != 1) {
			sscanf(currentBuffer, " %c %n", &operation, &numBytes);

			if (popAndPerformOperation(head, operation, result)) {
				return 1;
			}

			number = *result;
		}

		newElement = createStackElement(number);
		if (!newElement) {
			return 1;
		}
		push(head, newElement);

		currentBuffer += numBytes;
	}

	return 0;
}


int popAndPerformOperation(StackElement* head, char operation, double* result)
{
	double operand1 = 0.0;
	double operand2 = 0.0;

	if (pop(head, &operand1)) {
		return 1;
	}

	if (pop(head, &operand2)) {
		return 1;
	}

	switch (operation)
	{
	case '+':
		*result = operand2 + operand1;
		break;
	case '-':
		*result = operand2 - operand1;
		break;
	case '*':
		*result = operand2 * operand1;
		break;
	case '/':
		*result = operand2 / operand1;
		break;
	default:
		printf("Operation %c not supported yet!\r\n", operation);
		break;
	}

	return 0;

}


int pop(StackElement* head, double* result)
{
	StackElement* toPop = NULL;

	toPop = head->next;
	if (!toPop) {
		printf("Stack is empty!");
		return 1;
	}

	head->next = toPop->next;

	*result = toPop->number;

	free(toPop);

	return 0;
}


int push(StackElement* head, StackElement* newStackElement)
{
	newStackElement->next = head->next;
	head->next = newStackElement;

	return 0;
}