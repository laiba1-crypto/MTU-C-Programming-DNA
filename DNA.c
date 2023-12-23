#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to represent a DNA base
typedef struct Base {
    char type;
} Base;

// Structure to represent a DNA sequence node
typedef struct Node {
    Base base;
    struct Node* next;
    struct Node* prev;
} Node;

// Structure to represent a DNA sequence
typedef struct Sequence {
    Node* head;
    int current_age;
    int max_lifespan;
    double growth_probability;
    double deletion_probability;
    double mutation_probability;
} Sequence;

// Function to create a new DNA base
Base* createBase(char type) {
    Base* newBase = (Base*)malloc(sizeof(Base));
    newBase->type = type;
    return newBase;
}

// Function to create a new DNA sequence
Sequence* createSequence() {
    Sequence* newSequence = (Sequence*)malloc(sizeof(Sequence));
    newSequence->head = NULL;
    newSequence->current_age = 0;
    newSequence->max_lifespan = (rand() % (50 - 8 + 1)) + 8; // Random lifespan between 8 and 50
    newSequence->growth_probability = 0.25;
    newSequence->deletion_probability = 0.25;
    newSequence->mutation_probability = 0.0;
    return newSequence;
}

// Function to add a base at the end of a sequence
void addBase(Sequence* sequence, Base* base) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->base = *base;
    newNode->next = NULL;

    if (sequence->head == NULL) {
        newNode->prev = NULL;
        sequence->head = newNode;
    } else {
        Node* current = sequence->head;
        while (current->next != NULL) {
            current = current->next;
        }
        newNode->prev = current;
        current->next = newNode;
    }
}

// Function to insert a base at a specific position in the sequence
void insertBase(Sequence* sequence, Base* base, int position) {
    if (position < 0) {
        printf("Invalid position.\n");
        return;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->base = *base;

    if (position == 0) {
        newNode->next = sequence->head;
        newNode->prev = NULL;
        if (sequence->head != NULL) {
            sequence->head->prev = newNode;
        }
        sequence->head = newNode;
    } else {
        Node* current = sequence->head;
        int i = 0;
        while (current != NULL && i < position) {
            current = current->next;
            i++;
        }
        if (current == NULL) {
            printf("Invalid position.\n");
            return;
        }
        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
    }
}

// Function to delete a base from a specific position in the sequence
void deleteBase(Sequence* sequence, int position) {
    if (position < 0) {
        printf("Invalid position.\n");
        return;
    }

    if (sequence->head == NULL) {
        printf("Sequence is empty.\n");
        return;
    }

    Node* current = sequence->head;
    int i = 0;
    while (current != NULL && i < position) {
        current = current->next;
        i++;
    }
    if (current == NULL) {
        printf("Invalid position.\n");
        return;
    }

    if (current->prev != NULL) {
        current->prev->next = current->next;
    } else {
        sequence->head = current->next;
    }

    if (current->next != NULL) {
        current->next->prev = current->prev;
    }

    free(current);
}

// Function to search for a given DNA pattern within the sequence
int searchPattern(Sequence* sequence, char* pattern) {
    if (sequence->head == NULL) {
        printf("Sequence is empty.\n");
        return -1;
    }

    Node* current = sequence->head;
    int position = 0;
    int patternLength = strlen(pattern);

    while (current != NULL) {
        if (current->base.type == pattern[0]) {
            Node* temp = current;
            int i = 0;
            while (temp != NULL && i < patternLength && temp->base.type == pattern[i]) {
                temp = temp->next;
                i++;
            }
            if (i == patternLength) {
                return position;
            }
        }
        current = current->next;
        position++;
    }

    return -1;
}

// Function to print the DNA sequence
void printSequence(Sequence* sequence) {
    if (sequence->head == NULL) {
        printf("Sequence is empty.\n");
        return;
    }

    Node* current = sequence->head;
    while (current != NULL) {
        printf("%c ", current->base.type);
        current = current->next;
    }
    printf("\n");
}

// Function to randomly assign a DNA base
Base* getRandomBase() {
    char types[] = {'A', 'C', 'G', 'T'};
    int randomIndex = rand() % 4;
    return createBase(types[randomIndex]);
}

// Function to check for special patterns and apply their effects
void checkSpecialPatterns(Sequence* sequence) {
    int patternPosition = searchPattern(sequence, "AAA");
    if (patternPosition != -1) {
        int additionalIterations = 0;
        Node* current = sequence->head;
        int position = 0;
        while (current != NULL) {
            if (position >= patternPosition && current->base.type == 'A') {
                additionalIterations++;
            }
            current = current->next;
            position++;
        }
        sequence->max_lifespan += additionalIterations;
    }

    patternPosition = searchPattern(sequence, "CCC");
    if (patternPosition != -1) {
        int reductionIterations = 0;
        Node* current = sequence->head;
        int position = 0;
        while (current != NULL) {
            if (position >= patternPosition && current->base.type == 'C') {
                reductionIterations++;
            }
            current = current->next;
            position++;
        }
        sequence->max_lifespan -= reductionIterations;
        if (sequence->max_lifespan < 1) {
            sequence->max_lifespan = 1;
        }
    }

    patternPosition = searchPattern(sequence, "GGG");
    if (patternPosition != -1) {
        Node* current = sequence->head;
        int position = 0;
        while (current != NULL) {
            if (position >= patternPosition && current->base.type == 'G') {
                deleteBase(sequence, position);
                current = sequence->head;
                position = -1;
            }
            if (current != NULL) {
                current = current->next;
            }
            position++;
        }
    }

    patternPosition = searchPattern(sequence, "TTT");
    if (patternPosition != -1) {
        Node* current = sequence->head;
        while (current->next != NULL) {
            current = current->next;
        }

        Node* temp = current;
        while (temp != NULL) {
            Node* newNode = (Node*)malloc(sizeof(Node));
            newNode->base.type = temp->base.type;
            newNode->next = NULL;
            if (current->prev != NULL) {
                newNode->prev = current;
                current->next = newNode;
            } else {
                newNode->prev = NULL;
                sequence->head = newNode;
            }
            current = newNode;
            temp = temp->prev;
        }
    }
}

// Function to perform DNA lifecycle
void performDNALifecycle() {
	start:
		srand(time(NULL));
	    Sequence* sequence = createSequence();
	    for (int i = 0; i < 20; i++) { // Create initial 20 bases
	        Base* newBase = getRandomBase();
	        addBase(sequence, newBase);
	    }
	
	    while (sequence->current_age <= sequence->max_lifespan) {
	        printf("Current Age: %d\n", sequence->current_age);
	        printf("Max Lifespan: %d\n", sequence->max_lifespan);
	        printf("Growth Probability: %f\n", sequence->growth_probability);
	        printf("Deletion Probability: %f\n", sequence->deletion_probability);
	        printf("Mutation Probability: %f\n", sequence->mutation_probability);
	
	        printSequence(sequence);
	        checkSpecialPatterns(sequence);
	
	        if (sequence->current_age >= 5) {
	            sequence->mutation_probability += 0.01;
	        }
	
	        double randomValue = (double)rand() / RAND_MAX;
	
	        if (randomValue < sequence->growth_probability) {
	            Base* newBase = getRandomBase();
	            int position = rand() % (sequence->current_age + 1);
	            insertBase(sequence, newBase, position);
	        }
	
	        randomValue = (double)rand() / RAND_MAX;
	
	        if (randomValue < sequence->deletion_probability) {
	            int position = rand() % sequence->current_age;
	            deleteBase(sequence, position);
	        }
	
	        randomValue = (double)rand() / RAND_MAX;
	
	        if (randomValue < sequence->mutation_probability) {
	            int position = rand() % sequence->current_age;
	            Node* current = sequence->head;
	            int count = 0;
	            while (count < position) {
	                current = current->next;
	                count++;
	            }
	            current->base = *getRandomBase();
	        }
	
	        sequence->current_age++;
	
	        if (sequence->current_age > sequence->max_lifespan || sequence->head == NULL) {
	            printf("DNA sequence has reached the end of its lifecycle or died.\n");
	            char choice;
	            printf("Do you want to Start a new life cycle (y/n)? ");
	            scanf(" %c", &choice);
	            if(choice=='Y' || choice=='y'){
	            	goto start;
				}
				else{
					break;
				}
	            break;
	        }
	
	        printf("\n");
	    }
	
	    free(sequence);
}

int main() {
    performDNALifecycle();
    return 0;
}
