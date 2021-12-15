#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct _Directory;
typedef struct _Directory* Position;
typedef struct _Directory
{
	char name[MAX_LINE];
	Position sibling;
	Position child;
}Directory;


struct _StackElement;
typedef struct _StackElement* StackP;
typedef struct _StackElement
{
	Position dir;
	StackP next;
}StackElement;

int SwitchCase(Position, StackP);

StackP CreateNewStackElement(Position);
int Push(StackP, StackP);
int Pop(StackP, Position*);
int DeleteStack(StackP);

Position CreateNewElement(char);
Position Insert(Position, Position);
int List(Position);
Position EnterDirectory(Position, char, StackP);
Position GoToParent(StackP); 
int DeleteDirAfter(Position);
int DeleteDirAll(Position);
int InsertAfter(Position, Position);

int main() {
	Directory cHead = { .name = "C", .sibling = NULL, .child = NULL };
	Position c = &cHead;
	StackElement sHead = { .dir = c, .next = NULL };
	StackP s = &sHead;
	
	SwitchCase(c, s);
	
	return 0;
}

int SwitchCase(Position c, StackP head)
{
	Position currentDir = c;
	int flag = 0;
	int br = 0;
	char name[MAX_LINE] = { 0 };

	while (flag == 0)
	{
		printf("1 -> make directory (md)\n2 -> print the contents of the directory (dir)\n3 -> enter the directory (cd dir)\n4 -> go to parent directory (cd..)\n5 -> exit\n");
		scanf("%d", &br);

		switch (br)
		{
		case 1:
		{
			printf("New directory:\n");
			scanf("%s", name);
			Position new = CreateNewElement(name);
			currentDir->child = Insert(currentDir->child, new);
			break;
		}

		case 2:
		{

			printf("%s:", currentDir->name);
			List(currentDir);
			break;
		}

		case 3:
		{
			printf("Directory that you want to enter:\n");
			printf("%s:", currentDir->name);
			scanf_s("%s", name, MAX_LINE);
			Position prev = currentDir;
			currentDir = EnterDirectory(currentDir, name, head);
			if (!currentDir)
			{
				currentDir = prev;
			}
			break;
		}

		case 4:
		{
			if (currentDir == c)
			{
				break;
			}
			currentDir = GoToParent(head);
			break;
		}

		case 5:
		{
			if (c->child)
			{
				DeleteDirAll(c->child);
			}
			DeleteStack(head);
			flag = 1;
			break;
		}

		default:
		{
			printf("Wrong input, please try again!\n");
			break;
		}
		}
	}

	return EXIT_SUCCESS;
}

StackP CreateNewStackElement(Position dir)
{
	StackP q = (StackP)malloc(sizeof(StackElement));
	if (!q)
	{
		perror("Couldn't allocate memory\n");
		return NULL;
	}
	q->next = NULL;
	q->dir = dir;
	return q;
}

int Push(StackP head, StackP newS)
{
	newS->next = head->next;
	head->next = newS;
	return 0;
}
int Pop(StackP head, Position* mem)
{
	if (head->next == NULL)
	{
		printf("Stack is empty!\n");
		return 1;
	}

	StackP temp = head->next;
	head->next = head->next->next;
	*mem = temp->dir;
	free(temp);
	return 0;
}

int DeleteStack(StackP head)
{
	Position mem = CreateNewElement("");
	while (head->next)
	{
		Pop(head, &mem);
	}

	return EXIT_SUCCESS;
}

Position CreateNewElement(char name[])
{
	Position q = (Position)malloc(sizeof(Directory));
	if (!q)
	{
		perror("Couldn't allocate memory\n");
		return NULL;
	}

	strcpy(q->name, name);
	q->sibling = NULL;
	q->child = NULL;
	return q;
}

Position Insert(Position current, Position newElement)
{
	if (!current)
	{
		return newElement;
	}
	else if (strcmp(current->name, newElement->name) < 0)
	{
		current->sibling = Insert(current->sibling, newElement);
	}
	else if (strcmp(current->name, newElement->name) == 0)
	{
		free(newElement);
		return current;
	}
	else
	{
		newElement->sibling = current;
		return newElement;
	}

	return current;
}

int List(Position parent)
{
	Position temp = parent->child;
	while (temp)
	{
		printf("%s\n", temp->name);
		temp = temp->sibling;
	}

	printf("\n");

	return 0;
}

Position EnterDirectory(Position parent, char name[], StackP head)
{
	Position temp = parent->child;
	StackP q = NULL;
	while (temp != NULL && strcmp(temp->name, name) != 0)
	{
		temp = temp->sibling;
	}

	if (temp == NULL)
	{
		printf("Directory not found!\n");
		return NULL;
	}
	q = CreateNewStackElement(parent);
	Push(head, q);

	return temp;
}

Position GoToParent(StackP head)
{
	Position mem = CreateNewElement("");
	if (Pop(head, &mem))
		return head->dir;

	return mem;
}

int DeleteDirAfter(Position del)
{
	Position temp = del->sibling;
	del->sibling = del->sibling->sibling;
	free(temp);
	return EXIT_SUCCESS;
}

int DeleteDirAll(Position first)
{
	if (!first)
		return 0;
	else if (first->sibling)
	{
		DeleteDirAll(first->sibling);
	}
	else if (first->child)
	{
		DeleteDirAll(first->child);
	}
	free(first);

	return 0;
}

int InsertAfter(Position first, Position new)
{
	new->sibling = first->sibling;
	first->sibling = new;
	return EXIT_SUCCESS;
}