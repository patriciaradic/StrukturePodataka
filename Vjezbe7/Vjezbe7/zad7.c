#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

/*7. Napisati program koji pomoæu vezanih listi (stabala) predstavlja strukturu direktorija.
Omoguæiti unos novih direktorija i pod-direktorija, ispis sadržaja direktorija i
povratak u prethodni direktorij. Toènije program treba preko menija simulirati
korištenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.*/

typedef struct _directory {
    char name[MAX_NAME_LENGTH];
    struct _directory* subDirectories;
    struct _directory* next;
} Directory;

typedef struct _levelStack {
    struct _directory* directoryLevel;
    struct _levelStack* next;
} LevelStack;


int MENU(Directory* rootDirectory, LevelStack* headLevelStack);
void currentPath(LevelStack* headLevelStack);

Directory* createDirectory(char name[MAX_NAME_LENGTH]);
Directory* createSubdirectory(char name[MAX_NAME_LENGTH], Directory* currentDirectory);
Directory* changeDirectory(char name[MAX_NAME_LENGTH], Directory* currentDirectory);
void listDirectoryContents(Directory* currentDirectory);

Directory* pop(LevelStack* headLevelStack);
int push(LevelStack* headLevelStack, Directory* directoryLevel);
LevelStack* createNewLevelStackElement(Directory* directoryLevel);

int main()
{
    Directory headDirectory = {
        .name = "",
        .subDirectories = NULL,
        .next = NULL
    };
    Directory* rootDirectory = createDirectory("C:");
    headDirectory.next = rootDirectory;


    LevelStack headLevelStack = {
        .directoryLevel = NULL,
        .next = NULL
    };
    push(&headLevelStack, rootDirectory);

    MENU(rootDirectory, &headLevelStack);
    
    free(rootDirectory);

    return 0;
}


int MENU(Directory* rootDirectory, LevelStack* headLevelStack)
{
    Directory* currentDirectory = rootDirectory;
    char directoryName[MAX_NAME_LENGTH];

    while (1) {
        printf("\nMenu:\n");
        printf("1 - md (Create Directory)\n");
        printf("2 - cd dir (Change Directory)\n");
        printf("3 - cd.. (Go Up)\n");
        printf("4 - dir (List Contents)\n");
        printf("5 - exit\n");

        //currentPath
        currentPath(headLevelStack);

        int choice;
        //printf("Enter your choice: ");
        scanf("%d", &choice);


        switch (choice)
        {
        case 1:
            printf("Enter directory name: ");
            scanf("%s", directoryName);
            createSubdirectory(directoryName, currentDirectory); 
            break;

        case 2:
            printf("Enter directory name: ");
            scanf("%s", directoryName);
            currentDirectory = changeDirectory(directoryName, currentDirectory);
            push(headLevelStack, currentDirectory); 
            break;

        case 3:
            if (strcmp(currentDirectory->name, rootDirectory->name)!=0) {
                currentDirectory = pop(headLevelStack);
                printf("Currently in '%s' \n", currentDirectory->name);
            }
            else {
                printf("Already in the root directory.\n");
            }
            break;

        case 4:
            listDirectoryContents(currentDirectory); 
            break;

        case 5:
            printf("Exiting the program.\n"); 
            return 0;
            break;

        default:
            printf("Invalid choice. Please enter a valid option.\n"); 
            break;
        }

    }

}


void currentPath(LevelStack* headLevelStack)
{
    if (headLevelStack->next == NULL)
        return;
    
    LevelStack* lastLevel = NULL;
    LevelStack* firstLevel = headLevelStack->next;
    LevelStack* currentLevel = firstLevel;

    while (firstLevel->next != lastLevel) {
        while (currentLevel->next != lastLevel) {
            currentLevel = currentLevel->next;
        }
        printf("%s/", currentLevel->directoryLevel->name);
        lastLevel = currentLevel;
        currentLevel = firstLevel;
    }
    
    printf("%s>  ", currentLevel->directoryLevel->name);
    
}


Directory* createDirectory(char name[MAX_NAME_LENGTH]) {
    Directory* newDirectory = NULL;
    newDirectory = (Directory*)malloc(sizeof(Directory));
    if (!newDirectory) {
        printf("Can't allocate memory!\n");
        return NULL;
    }
    strcpy(newDirectory->name, name);
    newDirectory->subDirectories = NULL;
    newDirectory->next = NULL;
    return newDirectory;
}

Directory* createSubdirectory(char name[MAX_NAME_LENGTH], Directory* currentDirectory) {
    Directory* newDirectory = NULL;
    newDirectory = createDirectory(name);
    if (!newDirectory) {
        printf("New directory wasn't created!\n");
        return NULL;
    }
    newDirectory->next = currentDirectory->subDirectories;
    currentDirectory->subDirectories = newDirectory;
    return newDirectory;
}

Directory* changeDirectory(char name[MAX_NAME_LENGTH], Directory* currentDirectory) {
    Directory* subdirectory = currentDirectory->subDirectories;
    while (subdirectory != NULL) {
        if (strcmp(subdirectory->name, name) == 0) {
            return subdirectory;
        }
        subdirectory = subdirectory->next;
    }
    printf("Directory '%s' not found.\n", name);
    return currentDirectory;
}

void listDirectoryContents(Directory* currentDirectory) {
    printf("Contents of directory '%s':\n", currentDirectory->name);
    
    if (currentDirectory->subDirectories == NULL) {
        printf("   (empty)\n");
        return;
    }
        
    Directory* subdirectory = currentDirectory->subDirectories;
    while (subdirectory != NULL) {
        printf(" - %s\n", subdirectory->name);
        subdirectory = subdirectory->next;
    }
    
   
}






Directory* pop(LevelStack* headLevelStack) {
    LevelStack* toDelete = NULL;
    Directory* directoryLevel = NULL;

    toDelete = headLevelStack->next;
    if (!toDelete) {
        printf("Stack is empty! Nothing to pop!\n");
        return NULL;
    }

    headLevelStack->next = toDelete->next;
    directoryLevel = headLevelStack->next->directoryLevel;
    free(toDelete);

    return directoryLevel;
}

int push(LevelStack* headLevelStack, Directory* directoryLevel) {
    LevelStack* newLevelStackElement = NULL;

    newLevelStackElement = createNewLevelStackElement(directoryLevel);
    if (!newLevelStackElement) {
        perror("Error in creating new element!\n");
        return 1;
    }

    newLevelStackElement->next = headLevelStack->next;
    headLevelStack->next = newLevelStackElement;
    return 0;
}

LevelStack* createNewLevelStackElement(Directory* directoryLevel) {
    LevelStack* newLevelStackElement = NULL;

    newLevelStackElement = (LevelStack*)malloc(sizeof(LevelStack));
    if (!newLevelStackElement) {
        perror("Can't allocate memory!\n");
        return NULL;
    }

    newLevelStackElement->directoryLevel = directoryLevel;
    newLevelStackElement->next = NULL;

    return newLevelStackElement;
}