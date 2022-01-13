#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME 25
#define MAX_SIZE 1024

struct _City;
typedef struct _City* Position;
typedef struct _City{
    char name[MAX_SIZE];
    int population;
    Position next;
}City;

struct _Country;
typedef struct _Country* Pointer;
typedef struct _Country{
    char name[MAX_NAME];
    Pointer left;
    Pointer right;
    Position list;
}Country;

Pointer CreateCountry(char countryname[], char cityfile[]);
void InsertSortedCountry(Pointer current, char countryname[], char cityfile[]);
Position Cities(char cityfile[]);
Position CreateCity(char cityname[], int population);
int InsertSortedCity(Position city, Position element);
void PrintList(Pointer head, Position current);
void PrintCities(Position current);
Pointer FindCountry(Pointer head, char name[]);
void PrintCertainCities(Position current, int num);

int main(void) {
    Pointer head = NULL;
	char buffer[MAX_SIZE] = { 0 };
	char filename[MAX_NAME] = { 0 };
	char countryname[MAX_NAME] = { 0 };
	char cityfile[MAX_NAME] = { 0 };
	printf("Enter file name: ");
	scanf(" %s", filename);
	FILE* fp = NULL;
	fp = fopen(filename, "r");
	if(!fp) {
	    perror("Can't open file!\n");
	    return EXIT_FAILURE;
	}
	while (!feof(fp)) {
		fgets(buffer, MAX_SIZE, fp);
		char* line = buffer;
		if(sscanf(line, " %s %s", countryname, cityfile) != 2) {
		    printf("File line not written correctly!\n");
		    return EXIT_FAILURE;
		}
		sscanf(line, " %s %s", countryname, cityfile);
		if(!head)
		    head = CreateCountry(countryname, cityfile);
        else
            InsertSortedCountry(head, countryname, cityfile);
	}
	fclose(fp);
	printf("Countries:\t\tCities:\n");
	PrintList(head, head->list);
	int choice, status = 1, num;
	char name[MAX_NAME] = { 0 };
	Pointer current = NULL;
	printf("\n1 -> Select country\n2 -> List cities\n3 -> Exit program\n");
	while(status) {
		printf("\nYour choice: ");
		scanf("%d", &choice);
		switch (choice) {
			case 1:
				printf("\nEnter country name: ");
				scanf(" %s", name);
				current = FindCountry(head, name);
				if(!current)
					printf("\nCountry doesn't exist!");
				else
					printf("\nCountry found!");
				break;
			case 2:
				if(!current) {
					printf("Country not selected!\n");
					break;
				}
				printf("\nInsert population number: ");
				scanf("%d", &num);
				printf("\nCities with bigger population than %d are:\n", num);
				PrintCertainCities(current->list, num);
				break;
			case 3:
				status--;
				printf("\nExiting program...");
				break;
			default:
				printf("\nWrong input! Please select valid input.\n");
				break;
		}
	}
    return EXIT_SUCCESS;
}
Pointer CreateCountry(char countryname[], char cityfile[]) {
    Pointer country = NULL;
    country = (Pointer)malloc(sizeof(Country));
    if(!country) {
        perror("Can't allocate memory!\n");
        return country;
    }
    strcpy(country->name, countryname);
    country->left = NULL;
    country->right = NULL;
    country->list = Cities(cityfile);
    return country;
}
void InsertSortedCountry(Pointer current, char countryname[], char cityfile[]) {
    if(strcmp(countryname, current->name) < 0 && !current->right)
        current->right = CreateCountry(countryname, cityfile);
    else if(strcmp(countryname, current->name) > 0 && !current->left)
        current->left = CreateCountry(countryname, cityfile);
    else if(strcmp(countryname, current->name) < 0)
        InsertSortedCountry(current->right, countryname, cityfile);
    else if(strcmp(countryname, current->name) > 0)
        InsertSortedCountry(current->left, countryname, cityfile);
}
Position Cities(char cityfile[]) {
    City head = { .name = { 0 }, .population = 0, .next = NULL};
    Position element = NULL;
    char buffer[MAX_SIZE] = { 0 };
	char cityname[MAX_NAME] = { 0 };
	int population;
    FILE* fp = NULL;
	fp = fopen(cityfile, "r");
	if(!fp) {
		perror("Can't open file!\n");
		return head.next;
	}
	while(!feof(fp)) {
		fgets(buffer, MAX_SIZE, fp);
		char* line = buffer;
		if(sscanf(line, " %s %d", cityname, &population) != 2) {
			printf("File line not written correctly!\n");
			return head.next;
		}
		sscanf(line, " %s %d", cityname, &population);
        element = CreateCity(cityname, population);
        InsertSortedCity(&head, element);
	}
	fclose(fp);
	return head.next;
}
Position CreateCity(char cityname[], int population) {
    Position city = NULL;
    city = (Position)malloc(sizeof(City));
    if(!city) {
        perror("Can't alocate memory!\n");
        return city;
    }
    strcpy(city->name, cityname);
    city->population = population;
    city->next = NULL;
    return city;
}
int InsertSortedCity(Position head, Position element) {
    if(!head->next) {
        head->next = element;
        return EXIT_SUCCESS;
    }
    Position tmp = head->next;
    while(tmp) {
        if(element->population > tmp->population) {
            Position current = head;
            while(current->next != tmp)
                current = current->next;
            current->next = element;
            element->next = tmp;
            return EXIT_SUCCESS;
        }
        if(!tmp->next) {
            tmp->next = element;
            return EXIT_SUCCESS;
        }
        tmp = tmp->next;
    }
}
void PrintList(Pointer head, Position current) {
    if(!head)
        return;
    PrintList(head->right, current);
    printf("%s\t\t", head->name);
    PrintCities(head->list);
    printf("\n");
    PrintList(head->left, current);
}
void PrintCities(Position current) {
    while(current) {
        printf("%s (%d)\t", current->name, current->population);
        current = current->next;
    }
}
Pointer FindCountry(Pointer head, char name[]) {
    Pointer tmp = head;
    if(!strcmp(tmp->name, name))
        return tmp;
    else if(strcmp(tmp->name, name) > 0 && tmp->right)
         return FindCountry(tmp->right, name);
    else if(strcmp(tmp->name, name) < 0 && tmp->left)
        return FindCountry(tmp->left, name);
    return NULL;
}
void PrintCertainCities(Position current, int num) {
    while(current) {
        if(current->population > num)
            printf("%s (%d)\t", current->name, current->population);
        current = current->next;
    }
}