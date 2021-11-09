#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

typedef struct poli* pozicija;
struct poli {
	int k;
	int e;
	pozicija next;
};

pozicija NoviElement(int k, int e)
int unosPrvi(pozicija a);
int unosDrugi(pozicija a);
int ispis(pozicija a);
int zbroj(pozicija a, pozicija b, pozicija c);
int umnozak(pozicija a, pozicija b, pozicija c);

int main() {
    struct poli Head1;
	struct poli Head2;
	struct poli HeadZ;
	struct poli HeadU;
	Head1.next = NULL;
	Head2.next = NULL;
	HeadZ.next = NULL;
	HeadU.next = NULL;
	unosPrvi(&Head1);
	unosDrugi(&Head2);
	printf("Prvi polinom:\n");
	ispis(Head1.next);
	printf("Drugi polinom:\n");
	ispis(Head2.next);
	zbroj(&Head1, &Head2, &HeadZ);
	printf("Zbroj polinoma:\n");
	ispis(HeadZ.next);
	umnozak(&Head1, &Head2, &HeadU);
	printf("Umnozak polinoma:\n");
	ispis(HeadU.next);
	return 0;
}

pozicija NoviElement(int k, int e)
{
	pozicija novi = (pozicija)malloc(sizeof(poli));
	if (novi == NULL)
	{
		printf("Neuspjesno stvaranje novog elementa");
		return NULL;
	}
	novi->k = k;
	novi->e = e;
	return novi;
}

int unosPrvi(pozicija a)
{
	FILE* fp = NULL;
	fp = fopen("pol1.txt", "r");
	if (fp == NULL)
	{
		printf("Neuspjesno otvaranje datoteke!\n");
		return -1;
	}
	while (!feof(fp))
	{
		pozicija b = NULL;
		b = (pozicija)malloc(sizeof(struct poli));
		if (b == NULL)
		{
			printf("Neuspjesna alokacija memorije!\n");
			return -1;
		}
		pozicija prev, temp;
		temp = a->next;
		prev = a;
		fscanf(fp, "%d %d", &b->k, &b->e);
		while (temp != NULL && temp->e < b->e)
		{
			prev = temp;
			temp = temp->next;
		}
		prev->next = b;
		if (temp != NULL)
			b->next = temp;
		else
			b->next = NULL;
	}
	return 0;
}

int unosDrugi(pozicija a)
{
	FILE* fp = NULL;
	fp = fopen("pol2.txt", "r");
	if (fp == NULL)
	{
		printf("Neuspjesno otvaranje datoteke!\n");
		return -1;
	}
	while (!feof(fp))
	{
		pozicija b = NULL;
		b = (pozicija)malloc(sizeof(struct poli));
		if (b == NULL)
		{
			printf("Neuspjesna alokacija memorije!\n");
			return -1;
		}
		pozicija prev, temp;
		temp = a->next;
		prev = a;
		fscanf(fp, "%d %d", &b->k, &b->e);
		while (temp != NULL && temp->e < b->e)
		{
			prev = temp;
			temp = temp->next;
		}
		prev->next = b;
		if (temp != NULL)
			b->next = temp;
		else
			b->next = NULL;
	}
	return 0;
}

int ispis(pozicija a)
{
	while (a != NULL)
	{
		printf("Koeficijent : %d Eksponent : %d\n", a->k, b->e);
		a = a->next;
	}
	return 0;
}

int zbroj(pozicija a, pozicija b, pozicija c)
{
	pozicija a1 = a->next;
	pozicija b1 = b->next;
	pozicija c1 = c;
	pozicija novi = NULL;

	while (a1 != NULL && b1 != NULL)
	{
		if (a1->e == b1->e)
		{
			if (a1->k + b1->k == 0)
			{
				a1 = a1->next;
				b1 = b1->next;
			}
			else 
			{
				novi = NoviElement(a1->k + b1->k, a1->e);
				a1 = a1->next;
				b1 = b1->next;
			}
		}
		else if (a1->e > b1->e)
		{
			novi = NoviElement(a1->k, a1->e);
			a1 = a1->next;
		}
		else if (a1->e < b1->e)
		{
			novi = NoviElement(b1->k, b1->e);
			b1 = b1->next;
		}
		c1 = c1->next;
	}

	if (a1 == NULL)
	{
		while (b1 != NULL)
		{
			novi = NoviElement(b1->k, b1->e);
			c1 = c1->next;
			b1 = b1->next;
		}
	}

	if (b1 == NULL)
	{
		while (b1 != NULL)
		{
			novi = NoviElement(a1->k, a1->e);
			c1 = c1->next;
			a1 = a1->next;
		}
	}
	return c;
}


pozicija umnozak(pozicija a, pozicija b, pozicija c)
{
	pozicija a1 = a->next;
	pozicija b1 = b->next;
	pozicija c1 = c;
	pozicija novi = NULL;

	while (a1 != NULL)
	{
		b1 = b->next;
		while (b1 != NULL)
		{
			novi = NoviElement(a1->k * b1->k, a1->e + b1->e);
			temp2 = temp2->next;
		}
		a1 = a1->next;
	}
	return c;
}