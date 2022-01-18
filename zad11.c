#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024
#define table_size 11
#define _CRT_SECURE_NO_WARNINGS

struct _CityElement;
typedef struct _CityElement* CityPosition;
struct _CountryElement;
typedef struct _CountryElement* CountryPosition;
typedef struct _CountryElement
{
	char name[MAX_LINE];
	CountryPosition next;
	CityPosition cityHead;
}CountryElement;

typedef struct _CityElement
{
	char name[MAX_LINE];
	int population;
	CityPosition right;
	CityPosition left;
}CityElement;

int HashFunction(char countryName[MAX_LINE]);                                        
CountryPosition CreateNewCountryElement(char name[MAX_LINE]);                            
CityPosition CreateNewCityElement(char name[MAX_LINE], int population);
int InsertCountryElement(CountryPosition first[table_size], char name[MAX_LINE]);       
int Insert(CountryPosition current, CountryPosition new);                              
int InsertCityElement(CountryPosition head, char name[MAX_LINE], int population);     
CityPosition InsertCity(CityPosition current, CityPosition new);                         
int ReadFromFirstFile(char fileName[MAX_LINE], CountryPosition first[table_size]);       
int ReadFromNewFile(CountryPosition first[table_size], char countryName[MAX_LINE], char newFileName[MAX_LINE]);  
CountryPosition FindCountry(CountryPosition first[table_size], char name[MAX_LINE]); 
int Print(CountryPosition first[table_size]);                                         
int PrintCities(CityPosition head);

int HashFunction(char countryName[MAX_LINE])
{
	int sum = 0;
	for (int i = 0; i < 5; i++)
	{
		if (toupper(countryName[i]) >= 'A' && toupper(countryName[i]) <= 'Z')
		{
			sum += countryName[i];
		}
		else sum += 0;
	}
	return sum % table_size;
}

CountryPosition CreateNewCountryElement(char name[MAX_LINE])
{
	CountryPosition q = NULL;
	q = (CountryPosition)malloc(sizeof(CountryElement));
	if (!q)
	{
		perror("Couldn't allocate memory!\n");
		return NULL;
	}
	strcpy(q->name, name);
	q->cityHead = NULL;
	q->next = NULL;
	return q;
}

CityPosition CreateNewCityElement(char name[MAX_LINE], int population)
{
	CityPosition q = NULL;
	q = (CityPosition)malloc(sizeof(CityElement));
	if (!q)
	{
		perror("Couldn't allocate memory\n");
		return NULL;
	}
	strcpy(q->name, name);
	q->population = population;
	q->right = NULL;
	q->left = NULL;
	return q;
}

int InsertCountryElement(CountryPosition first[table_size], char name[MAX_LINE])
{
	int hashNumber = HashFunction(name);
	CountryPosition new = CreateNewCountryElement(name);
	Insert(first[hashNumber], new);
	return EXIT_SUCCESS;
}

int Insert(CountryPosition current, CountryPosition new)
{
	if (!current->next)
	{
		current->next = new;
		return EXIT_SUCCESS;
	}
	while(current->next != NULL && strcmp(current->next->name, new->name) < 0)
	{
		current = current->next;
	}
	new->next = current->next;
	current->next = new;
	return EXIT_SUCCESS;
}

int InsertCityElement(CountryPosition head, char name[MAX_LINE], int population)
{
	CityPosition q = CreateNewCityElement(name, population);
	head->next->cityHead = InsertCity(head->next->cityHead, q);
	return EXIT_SUCCESS;
}

CityPosition InsertCity(CityPosition current, CityPosition new)
{
	if (current == NULL)
	{
		return new;
	}
	else if (current->population < new->population)
	{
		current->right = InsertCity(current->right, new);
	}
	else if (current->population > new->population)
	{
		current->left = InsertCity(current->left, new);
	}
	else
	{
		if (strcmp(current->name, new->name) < 0)
		{
			new->left = current;
			return new;
		}
		else
		{
			new->right = current;
			return new;
		}
	}
	return current;
}

int ReadFromFirstFile(char fileName[MAX_LINE], CountryPosition first[table_size])
{
	char countryName[MAX_LINE] = { 0 };
	char newFileName[MAX_LINE] = { 0 };
	FILE* fp = fopen(fileName, "r");
	if (!fp) 
	{
		perror("Couldn't allocate memory!\n");
		return EXIT_FAILURE;
	}
	char* buffer = (char*)malloc(MAX_LINE * sizeof(char));
	while (!feof(fp))
	{
		fgets(buffer, MAX_LINE, fp);
		if (!sizeof(buffer) == 0)
		{
			sscanf(buffer, "%s %s\n", countryName, newFileName);
			InsertCountryElement(first, countryName);
			ReadFromNewFile(first, countryName, newFileName);
		}
	}
	fclose(fp);
	return EXIT_SUCCESS;
}

int ReadFromNewFile(CountryPosition first[table_size], char countryName[MAX_LINE], char newFileName[MAX_LINE])
{
	FILE* fp = fopen(newFileName, "r");
	if (!fp)
	{
		perror("Couldn't allocate memory!\n");
		return EXIT_FAILURE;
	}

	char* buffer = (char*)malloc(MAX_LINE * sizeof(char));
	int population = 0;
	char cityName[MAX_LINE] = { 0 };
	while (!feof(fp))
	{
		fgets(buffer, MAX_LINE, fp);
		if (sizeof(buffer) > 0)
		{
			sscanf(buffer, "%s %d\n", cityName, &population);
			InsertCityElement(first[HashFunction(countryName)], cityName, population);
		}
	}
	fclose(fp);
}

CountryPosition FindCountry(CountryPosition first[table_size], char name[MAX_LINE])
{
	CountryPosition find = first[HashFunction(name)];
	while (strcmp(find->name, name) != 0)
	{
		find = find->next;
	}
	if (!find) 
	{
		printf("No country with given name!\n");
		return NULL;
	}
	return find;
}

int Print(CountryPosition first[table_size])
{
	for (int i = 0; i < table_size; i++)
	{
		CountryPosition temp = first[i]->next;
		while (temp)
		{
			printf("\n%s:\n", temp->name);
			PrintCities(temp->cityHead);
			temp = temp->next;
		}
	}
	return EXIT_SUCCESS;
}

int PrintCities(CityPosition head)
{
	CityPosition current = head;
	if (current)
	{
		PrintCities(current->left);
		printf("%s (%d), ", current->name, current->population);
		PrintCities(current->right);
	}
	return EXIT_SUCCESS;
}

int main()
{
	CountryElement head[table_size] = { 0 };
	CountryPosition first[table_size] = { 0 };
	for (int i = 0; i < table_size; i++)
	{
		strcpy(head[i].name, " ");
		head[i].cityHead = NULL;
		head[i].next = NULL;
		first[i] = &head[i];
	}
	ReadFromFirstFile("drzave.txt", first);
	Print(first);
	return 0;
}