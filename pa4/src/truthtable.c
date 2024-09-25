#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Enum for gate types
typedef enum {
    AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER
} kind_t;

// Structure to represent a gate
typedef struct Gate {
    kind_t kind;
    int size;
    char **params;
    char **inputs;
    char **outputs;
    struct Gate *next;
} Gate;

struct Gate *head = NULL;

void createGate(kind_t type, int size, char** params, char** inputs, char** outputs) {
    
    Gate *newGate = (Gate *) malloc(sizeof(Gate));

    if(newGate != NULL) {
        newGate->kind = type;
        newGate->size = size;
        if (type == MULTIPLEXER) {
            newGate->params = (char**)malloc((1<<size) * sizeof(char*));
            for (int i = 0; i < (1<<size); i++) {
                newGate->params[i] = (char *)malloc((strlen(params[i]) + 1) * sizeof(char));
                if (newGate->params[i] != NULL) {
                    strcpy(newGate->params[i], params[i]);
                }
            }
        } else {
            newGate->params = params;
        }
        newGate->inputs = (char**)malloc(size * sizeof(char*));
        for (int i = 0; i < size; i++) {
            newGate->inputs[i] = (char *)malloc((strlen(inputs[i]) + 1) * sizeof(char));
            strcpy(newGate->inputs[i], inputs[i]);
        }
        if (type == DECODER) {
            newGate->outputs = (char**)malloc((1<<size) * sizeof(char*));
            for (int i = 0; i < (1<<size); i++) {
                newGate->outputs[i] = (char *)malloc((strlen(outputs[i]) + 1) * sizeof(char));
                strcpy(newGate->outputs[i], outputs[i]);
            }
        } else {
            newGate->outputs = (char**)malloc(sizeof(char*));
            newGate->outputs[0] = (char*)malloc((strlen(outputs[0]) + 1) * sizeof(char));
            newGate->outputs[0] = strcpy(newGate->outputs[0], outputs[0]);
        }
        newGate->next = NULL;
    }

    if(head == NULL) {
        head = newGate;
    } else if(head->next == NULL) {
        head->next = newGate;
    } else {
        struct Gate *temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newGate;
    }
}

void freeGate(Gate *gate) {
    if (gate == NULL) {
        return;
    }

    if (gate->kind == MULTIPLEXER) {
        for (int i = 0; i < (1 << gate->size); i++) {
            free(gate->params[i]);
        }
        free(gate->params);
    } else {
        free(gate->params);
    }

    for (int i = 0; i < gate->size; i++) {
        free(gate->inputs[i]);
    }
    free(gate->inputs);

    if (gate->kind == DECODER) {
        for (int i = 0; i < (1 << gate->size); i++) {
            free(gate->outputs[i]);
        }
        free(gate->outputs);
    } else {
        free(gate->outputs[0]);
        free(gate->outputs);
    }

    free(gate);
}

void freeGates() {
    Gate *current = head;
    while (current != NULL) {
        Gate *temp = current;
        current = current->next;
        freeGate(temp);
    }
    head = NULL;
}

#define SIZE 20

struct KeyValue {
    char* key;
    int value;
    struct KeyValue* next;
};

struct HashMap {
    struct KeyValue* array[SIZE];
};

unsigned int hash(char* key) {
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = (hash * 31) + key[i];
    }
    return hash % SIZE;
}

struct HashMap* createHashMap() {
    struct HashMap* map = (struct HashMap*)malloc(sizeof(struct HashMap));
    for (int i = 0; i < SIZE; i++) {
        map->array[i] = NULL;
    }
    return map;
}

void insert(struct HashMap* map, char* key, int value) {
    
    unsigned int index = hash(key);
    
    struct KeyValue* current = map->array[index];
    
    while (current != NULL) {
        
        if (strcmp(current->key, key) == 0) {
            
            current->value = value;
            return;
        }
        current = current->next;
    }
    
    struct KeyValue* newPair = (struct KeyValue*)malloc(sizeof(struct KeyValue));
    newPair->key = (char *)malloc((strlen(key) + 1) * sizeof(char));
    
    if (newPair->key != NULL) {
        strcpy(newPair->key, key);
    }

    newPair->value = value;
    newPair->next = NULL;

    if (map->array[index] == NULL) {
        map->array[index] = newPair;
    } else {
        
        current = map->array[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newPair;
    }
}

int get(struct HashMap* map, char* key) {
    
    unsigned int index = hash(key);
    
    struct KeyValue* current = map->array[index];
    
    while (current != NULL) {
        
        if (strcmp(current->key, key) == 0) {
            
            return current->value;
        }
        current = current->next;
    }
    
    return -1;
}

void removeKey(struct HashMap* map, char* key) {
    
    unsigned int index = hash(key);
    
    struct KeyValue* current = map->array[index];
    struct KeyValue* prev = NULL;
    
    while (current != NULL) {
        
        if (strcmp(current->key, key) == 0) {
            
            if (prev == NULL) {
                map->array[index] = current->next;
            } else {
                prev->next = current->next;
            }
            
            free(current->key);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void freeHashMap(struct HashMap* map) {
    
    for (int i = 0; i < SIZE; i++) {
        
        struct KeyValue* current = map->array[i];
        while (current != NULL) {
            
            struct KeyValue* temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(map);
}

struct HashMap* map; //Declare hashmap

// Read and create GATES

void readFileLine(FILE *file, char *type) {
    if (strcmp("MULTIPLEXER", type) == 0) { 

        int n;
        fscanf(file, "%d", &n); //Reading number of inputs
        int num_selector = 1 << n; //Adjusting for 2^n selector inputs
        char **selector = (char**) malloc(num_selector * sizeof(char*));
        for (int i = 0; i < num_selector; i++) { //reading selectors
            selector[i] = (char*) malloc(16 * sizeof(char));
            fscanf(file, "%s", selector[i]);
        }
        char **inputs = (char**) malloc(n*sizeof(char*)); //repeating for inputs (n)
        for (int i = 0; i < n; i++) {
            inputs[i] = (char*) malloc(16 * sizeof(char));
            fscanf(file, "%s", inputs[i]);
        }
        char **outputs = (char**) malloc(sizeof(char*)); //repeating for outputs (1)
        outputs[0] = (char *) malloc(16 * sizeof(char));
        fscanf(file, "%s", outputs[0]);
        createGate(MULTIPLEXER, n, selector, inputs, outputs); //creating gate struct.

        for (int i = 0; i < n; i++) { //freeing data.
            free(inputs[i]);
        }
        for (int i = 0; i < num_selector; i++) {
            free(selector[i]);
        }
        free(selector);
        free(inputs);
        free(outputs[0]);
        free(outputs);
    } else if(strcmp("DECODER",type) == 0) {

        int n;
        fscanf(file, "%d", &n); //reading number of inputs
        int num_o = 1 << n; //discovering number of outputs
        char **inputs = (char**) malloc(n * sizeof(char*)); //reading inputs
        for (int i = 0; i < n; i++) {
            inputs[i] = (char*) malloc(16 * sizeof(char));
            fscanf(file, " %s", inputs[i]);
        }
        char **outputs = (char**) malloc(num_o * sizeof(char*)); //reading outputs
        for (int i = 0; i < num_o; i++) {
            outputs[i] = (char*) malloc(16*sizeof(char));
            fscanf(file, " %s", outputs[i]);
        }
        createGate(DECODER, n, NULL, inputs, outputs); //creating gate struct

        for (int i = 0; i < n; i++) { //freeing data
            free(inputs[i]);
        }
        free(inputs);
        for (int i = 0; i < num_o; i++) {
            free(outputs[i]);
        }
        free(outputs);
    } else if((strcmp("PASS",type) == 0) || (strcmp("NOT", type) == 0)) {

        char **inputs = (char**) malloc(sizeof(char*));
        inputs[0] = (char *) malloc(16 * sizeof(char));
        fscanf(file, " %s", inputs[0]);
        char **outputs = (char**) malloc(sizeof(char*));
        outputs[0] = (char *) malloc(16 * sizeof(char));
        fscanf(file, " %s", outputs[0]);
        
        if(strcmp("PASS", type) == 0) {
            createGate(PASS, 1, NULL, inputs, outputs);
        } else {
            createGate(NOT, 1, NULL, inputs, outputs);
        }

        free(inputs[0]);
        free(inputs);
        free(outputs[0]);
        free(outputs);
    } else {

        char **inputs = (char**) malloc(2*sizeof(char*));
        inputs[0] = (char*) malloc(16*sizeof(char));
        inputs[1] = (char*) malloc(16*sizeof(char));
        fscanf(file, " %s",inputs[0]);
        fscanf(file, " %s",inputs[1]);
        char **outputs = (char**) malloc(16 * sizeof(char*));
        outputs[0] = (char*) malloc(16*sizeof(char));
        fscanf(file, " %s", outputs[0]);

        if(strcmp("AND", type) == 0) {

            createGate(AND, 2, NULL, inputs, outputs);
        } else if(strcmp("OR", type) == 0) {

            createGate(OR, 2, NULL, inputs, outputs);
        } else if(strcmp("NAND", type) == 0) {

            createGate(NAND, 2, NULL, inputs, outputs);
        } else if(strcmp("NOR", type) == 0) {

            createGate(NOR, 2, NULL, inputs, outputs);
        } else if(strcmp("XOR", type) == 0) {
                
            createGate(XOR, 2, NULL, inputs, outputs);
        }

        free(inputs[0]);
        free(inputs[1]);
        free(outputs[0]);
        free(inputs);
        free(outputs);
    }
}

void printGates(Gate *head) {
    Gate *temp = (Gate *) malloc(sizeof(Gate));
    temp = head;
    const char *gateNames[] = {
        "AND", "OR", "NAND", "NOR", "XOR", "NOT", "PASS", "DECODER", "MULTIPLEXER"
    };

    while(temp != NULL) {
        printf("%s ", gateNames[temp->kind]);
        printf("%d\n", temp->size);
        temp = temp->next;
    }

    freeGate(temp);
}

int* paramToInt(char** params, int size) {
    
    //initialize intger array to represent symbolic ones.
    int* newArray = (int*) malloc(size * sizeof(int));

    //Ignores existing 1s and 0s, only converts symbols to new 1s or 0s.
    for (int i = 0; i < size; i++) {

        if (strcmp(params[i],"0") == 0) {
            newArray[i] = 0;
        } else if(strcmp(params[i],"1") == 0) {
            newArray[i] = 1;
        } else {
            newArray[i] = get(map, params[i]);
        }
    }
    return newArray;
}

void andLogic(Gate *currentGate) {

    int *inputs = paramToInt(currentGate->inputs, currentGate->size);
    int result = inputs[0] && inputs[1];
    insert(map, currentGate->outputs[0], result);
    free(inputs);
}

void orLogic(Gate *currentGate) {

    int *inputs = paramToInt(currentGate->inputs, currentGate->size);
    int result = inputs[0] || inputs[1];
    insert(map, currentGate->outputs[0], result);
    free(inputs);
}

void nandLogic(Gate *currentGate) {

    int *inputs = paramToInt(currentGate->inputs, currentGate->size);
    int result = !(inputs[0] && inputs[1]);
    insert(map, currentGate->outputs[0], result);
    free(inputs);
}

void norLogic(Gate *currentGate) {

    int *inputs = paramToInt(currentGate->inputs, currentGate->size);
    int result = !(inputs[0] || inputs[1]);
    insert(map, currentGate->outputs[0], result);
    free(inputs);
}

void xorLogic(Gate *currentGate) {

    int *inputs = paramToInt(currentGate->inputs, currentGate->size);
    int result = inputs[0] ^ inputs[1];
    insert(map, currentGate->outputs[0], result);
    free(inputs);
}

void notLogic(Gate *currentGate) {

    int *inputs = paramToInt(currentGate->inputs, currentGate->size);
    int result = !inputs[0];
    insert(map, currentGate->outputs[0], result);
    free(inputs);
}

void passLogic(Gate *currentGate) {

    int *inputs = paramToInt(currentGate->inputs, currentGate->size);
    int result = inputs[0];
    insert(map, currentGate->outputs[0], result);
    free(inputs);
}

void decoderLogic(Gate *currentGate) {
    int inputSize = currentGate->size;
    int numOutputs = 1 << inputSize;

    int *outputs = (int *)malloc(sizeof(int) * numOutputs);

    int *inputArray = paramToInt(currentGate->inputs, inputSize);
    int input = 0;
    for (int i = 0; i < inputSize; i++) {
        input = input * 2 + inputArray[i];
    }

    for (int i = 0; i < numOutputs; i++) {
        outputs[i] = 0;
    }

    outputs[input] = 1;

    for (int i = 0; i < numOutputs; i++) {
        insert(map, currentGate->outputs[i], outputs[i]);
    }

    free(outputs);
    free(inputArray);
}



void multiLogic(Gate *currentGate) {

    int num_inp = currentGate->size;
    
    int num_select = 1 << num_inp;

    // Convert selector strings to integers
    int *selectees = paramToInt(currentGate->params, num_select);

    // Convert input strings to integers
    int *inputs = paramToInt(currentGate->inputs, num_inp);

    // Get the output index based on the selector values
    int output_index = 0;
    for (int i = 0; i < num_inp; i++) {
        output_index = (output_index << 1) + inputs[i];
    }

    // Get the selected output based on the selector values
    int selected_output = selectees[output_index];

    // Update the output value in the HashMap
    insert(map, currentGate->outputs[0], selected_output);

    // Free dynamically allocated memory
    free(selectees);
    free(inputs);
}


void performLogic(Gate *currentGate) {

    kind_t kind = currentGate->kind;
        switch(kind) {
            case AND:
                andLogic(currentGate);
                break;
            case OR:
                orLogic(currentGate);
                break;
            case NAND:
                nandLogic(currentGate);
                break;
            case NOR:
                norLogic(currentGate);
                break;
            case XOR:
                xorLogic(currentGate);
                break;
            case NOT:
                notLogic(currentGate);
                break;
            case PASS:
                passLogic(currentGate);
                break;
            case DECODER:
                decoderLogic(currentGate);
                break;
            case MULTIPLEXER:
                multiLogic(currentGate);
                break;
            default:
                printf("Ya done fucked now kid.\n");
                break;
        }
}

void generateTable(int numInputs, char** inputs, int numOutputs, char** outputs) {

    int maxValue = 1 << numInputs; //finding max combos
    for (int i = 0; i < maxValue; i++) {
        int k = 0;
        for (int j = numInputs - 1; j >= 0; j--) {
            insert(map, inputs[k], (i >> j) & 1); //generating inputs line by line and adding to hash
            k++;
        }
        
        struct Gate *temp = head;

        while(temp != NULL) {
            performLogic(temp);
            temp = temp->next;
        }

        for(int j = 0; j < numInputs; j++) {
            printf("%d ", get(map, inputs[j]));
        }
        printf("|");
        for(int j = 0; j < numOutputs; j++) {
            printf(" %d", get(map, outputs[j]));
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {

    FILE *file = fopen(argv[1],"r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int num_inputs, num_outputs;
    map = createHashMap();

    fscanf(file,"INPUT %d",&num_inputs); //Scanning inputs and adding to map
    char** inputs = (char**)malloc(num_inputs * sizeof(char*));
    for (int i = 0; i < num_inputs; i++) {
        inputs[i] = (char*)malloc(16*sizeof(char));
        fscanf(file, "%s", inputs[i]);
        insert(map,inputs[i],0);
    }

    fgetc(file); //Throwing out EOL character

    fscanf(file,"OUTPUT %d",&num_outputs); //Scaning outputs and adding to map
    char** outputs = (char**)malloc(num_outputs * sizeof(char*));
    for (int i = 0; i < num_outputs; i++) {
        outputs[i] = (char*)malloc(16*sizeof(char));
        fscanf(file, "%s", outputs[i]);
        insert(map,outputs[i],0);
    }

    fgetc(file); //Throwing out EOL character

    char *type = (char *) malloc(16 * sizeof(char)); //Dynamically allocating space for type
    while (fscanf(file, "%s", type) != EOF) {

        readFileLine(file,type);
        fgetc(file); //Throwing out EOL character
    }
    free(type);

    generateTable(num_inputs, inputs, num_outputs, outputs);
    for (int i = 0; i < num_inputs; i++) {
        free(inputs[i]);
    }
    for (int i = 0; i < num_outputs; i++) {
        free(outputs[i]);
    }
    free(inputs);
    free(outputs);
    freeHashMap(map);
    freeGates();
}