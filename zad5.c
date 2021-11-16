#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _stackElement;
typedef struct _stackElement* Position;
typedef struct _stackElement{
    int number;
	Position next;
}stackElement;

Position CreateNewElement(double number);
int Push(Position head, double number);
int InsertAfter(Position first, Position new);
int DeleteAfter(Position first);
int DeleteAll(Position head);
int Pop(Position head);
int PerformOperation(char operation, Position head);
int CalculateFromFile(char* fileName, double* result);

int main(int argc, char* argv[])
{
	double result = 0;
	int status = 0;
	char fileName[MAX_LINE] = { 0 };
	printf("Insert file name:\n");
	scanf("%s", fileName);
	status = CalculateFromFile(fileName, &result);
	if (status == EXIT_SUCCESS)
	{
		printf("Postfix result is: %lf", result);
	}
	return EXIT_SUCCESS;
}

Position CreateStackElement(double number)
{
	Position newElement = NULL;
	newElement = (Position*)malloc(sizeof(StackElement));
	
	if (!newElement)
	{
		perror("Can't allocate memory! \n");
		return NULL;
	}
	
	newElement->number = number;
	newElement->next = NULL;

	return newElement;
}

int InsertAfter(Position position, Position newElement)
{
	newElement->next = position->next;
	position->next = newElement;

	return EXIT_SUCCESS;
}

int Push(Position head, double number)
{
	Position newElement = NULL;
	newElement = CreateNewElement(number);
	if (!newElement)
	{
		perror("Couldn't allocate memory!\n");
		return -1;
	}
	InsertAfter(head, newElement);
	return EXIT_SUCCESS;
}

int DeleteAfter(Position position)
{
	Position temp = position->next;
	if (!temp)
		return -1;

	position->next = temp->next;
	free(temp);

	return EXIT_SUCCESS;
}

int DeleteAll(Position head)
{
	while (head->next != NULL)
	{
		DeleteAfter(head);
	}
	return EXIT_SUCCESS;
}

int Pop(double* destination, Position head)
{
	Position first = head->next;
	if (!first)
	{
		printf("Postfix not valid!\n");
		return -1;
	}

	*destination = first->number;
	DeleteAfter(head);
	return EXIT_SUCCESS;
}

int PerformOperation(char operation, Position head)
{
	double operand1 = 0;
	double operand2 = 0;
	double result = 0;
	int status1 = EXIT_SUCCESS;
	int status2 = EXIT_SUCCESS;

	status2 = Pop(&operand2, head);
	if (status2 != EXIT_SUCCESS)
	{
		return -2;
	}
	status1 = Pop(&operand1, head);
	if (status1 != EXIT_SUCCESS)
	{
		return -1;
	}
	switch (operation)
	{
	case '+':
	{
		result = operand1 + operand2;
		break;
	}
	case '*':
	{
		result = operand1 * operand2;
		break;
	}
	case '-':
	{
		result = operand1 - operand2;
		break;
	}
	case '/':
	{
		if (operand2 == 0)
		{
			perror("Can't divide with zero!\n");
			return -3;
		}
		result = operand1 / operand2;
		break;
	}
	default:
	{
		printf("This operation is not available yet!\n");
		return -4;
	}
	}

	Push(head, result);
	return EXIT_SUCCESS;
}

int CalculateFromFile(char* fileName, double* result)
{
	FILE* file = NULL;
	char* buffer = NULL;
	char* currentBuffer = NULL;
	char operation = 0;
	int fileLength = 0;
	int status = 0;
	double number = 0;
	int numBytes = 0;
	stackElement head = { .number = 0, .next = NULL };

	file = fopen(fileName, "rb"); 
	if (!file)
	{
		perror("Can't open the file!\n");
		return -1;
	}

	fseek(file, 0, SEEK_END); 
	fileLength = ftell(file); 

	buffer = (char*)calloc(fileLength + 1, sizeof(char));
	if (!buffer)
	{
		perror("Can't allocate memory!\n");
		return -2;
	}

	rewind(file);  
	fread(buffer, sizeof(char), fileLength, file);
	printf("|%s|\n", buffer);
	
	fclose(file); 
	
	currentBuffer = buffer;

	while (strlen(currentBuffer) > 0)
	{
		status = sscanf(currentBuffer, "%lf %n", &number, &numBytes);
		if (status == 1)
		{
			status = Push(&head, number);
			if (status != EXIT_SUCCESS)
			{
				free(buffer);
				DeleteAll(&head);
				return -3;
			}
			currentBuffer += numBytes;
		}
		else
		{
			status = sscanf(currentBuffer, "%c %n", &operation, &numBytes);
			if (status == 1)
			{
				status = PerformOperation(operation, &head);
				if (status != EXIT_SUCCESS) 
				{
					free(buffer);
					DeleteAll(&head);
					return -4;
				}
				currentBuffer += numBytes;
			}
			else
			{
				printf("Postfix not valid!\n");
				DeleteAll(&head);
				return -7;
			}
		}
	}
	free(buffer);

	status = Pop(result, &head);
	if (status != EXIT_SUCCESS)
	{
		DeleteAll(&head);
		return -5;
	}

	if (head.next)
	{
		printf("Postfix not valid! Please check your file!\n");
		DeleteAll(&head);
		return -6;
	}

	return EXIT_SUCCESS;
}