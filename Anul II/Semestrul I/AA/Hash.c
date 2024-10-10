#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define CAPACITY 1000000 // Size of the Hash Table

unsigned long hash_function(int x) {
    return x % CAPACITY;
}

typedef struct Ht_item Ht_item;

// Define the Hash Table Item here
struct Ht_item {
    int key;
    long value;
};

typedef struct LinkedList LinkedList;

// Define the Linkedlist here
struct LinkedList {
    Ht_item* item; 
    LinkedList* next;
};

typedef struct HashTable HashTable;

// Define the Hash Table here
struct HashTable {
    // Contains an array of pointers
    // to items
    Ht_item** items;
    LinkedList** overflow_buckets;
    int size;
    int count;
};

static LinkedList* allocate_list () {
    // Allocates memory for a Linkedlist pointer
    LinkedList* list = (LinkedList*) malloc (sizeof(LinkedList));
    return list;
}

static LinkedList* linkedlist_insert(LinkedList* list, Ht_item* item) {
    // Inserts the item onto the Linked List
    if (!list) {
        LinkedList* head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    } 
    
    else if (list->next == NULL) {
        LinkedList* node = allocate_list();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    LinkedList* temp = list;
    while (temp->next->next) {
        temp = temp->next;
    }
    
    LinkedList* node = allocate_list();
    node->item = item;
    node->next = NULL;
    temp->next = node;
    
    return list;
}

static Ht_item* linkedlist_remove(LinkedList* list) {
    // Removes the head from the linked list
    // and returns the item of the popped element
    if (!list)
        return NULL;
    if (!list->next)
        return NULL;
    LinkedList* node = list->next;
    LinkedList* temp = list;
    temp->next = NULL;
    list = node;
    Ht_item* it = NULL;
    memcpy(temp->item, it, sizeof(Ht_item));
    free(temp->item);
    free(temp);
    return it;
}

static void free_linkedlist(LinkedList* list) {
    LinkedList* temp = list;
    while (list) {
        temp = list;
        list = list->next;
        free(temp->item);
        free(temp);
    }
}

static LinkedList** create_overflow_buckets(HashTable* table) {
    // Create the overflow buckets; an array of linkedlists
    LinkedList** buckets = (LinkedList**) calloc (table->size, sizeof(LinkedList*));
    for (int i=0; i<table->size; i++)
        buckets[i] = NULL;
    return buckets;
}

static void free_overflow_buckets(HashTable* table) {
    // Free all the overflow bucket lists
    LinkedList** buckets = table->overflow_buckets;
    for (int i=0; i<table->size; i++)
        free_linkedlist(buckets[i]);
    free(buckets);
}

Ht_item* create_item(int key) {
    // Creates a pointer to a new hash table item
    Ht_item* item = (Ht_item*) malloc (sizeof(Ht_item));
    item->key = key;
    item->value = key;
    
    return item;
}

HashTable* create_table(int size) {
    // Creates a new HashTable
    HashTable* table = (HashTable*) malloc (sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**) calloc (table->size, sizeof(Ht_item*));
    for (int i=0; i<table->size; i++)
        table->items[i] = NULL;
    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}

void free_item(Ht_item* item) {
    // Frees an item
    free(item);
}

void free_table(HashTable* table) {
    // Frees the table
    for (int i=0; i<table->size; i++) {
        Ht_item* item = table->items[i];
        if (item != NULL)
            free_item(item);
    }

    free_overflow_buckets(table);
    free(table->items);
    free(table);
}

void ht_insert(HashTable* table, int key);

void handle_collision(HashTable *table, unsigned long index, Ht_item *item) {
        LinkedList* head = table->overflow_buckets[index];

        if (head == NULL) {
                head = allocate_list();
                head->item = item;
                table->overflow_buckets[index] = head;
                return;
        } else {
                LinkedList* curr = table->overflow_buckets[index];
                while (curr) {
                        if (curr->item->key == item->key) {
                            free_item(curr->item);
                            curr->item = item;
                            return;
                        }
                        curr = curr->next;
                }
                ht_insert(head, item->key);
                return;
        }
}

void ht_insert(HashTable* table, int key) {
    // Create the item
    Ht_item* item = create_item(key);

    // Compute the index
    unsigned long index = hash_function(key);

    Ht_item* current_item = table->items[index];
    
    if (current_item == NULL) {
        // Key does not exist.
        if (table->count >= table->size) {
            return;
        }
        
        // Insert directly
        table->items[index] = item; 
        table->count++;
        return;
    } else {
            // Scenario 1: We only need to update value
            if (current_item->key == key) {
                table->items[index]->value = key;
                return;
            } else {
                // Scenario 2: Collision
                handle_collision(table, index, item);
                return;
            }
    }
}

int ht_search(HashTable* table, int key) {
    // Searches the key in the hashtable
    // and returns NULL if it doesn't exist
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkedList* head = table->overflow_buckets[index];

    // Ensure that we move to items which are not NULL
    while (item != NULL) {
        if (item->key == key)
            return 1;
        if (head == NULL)
            return 0;
        item = head->item;
        head = head->next;
    }
    return 0;
}

void ht_delete(HashTable* table, int key) {
    // Deletes an item from the table
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkedList* head = table->overflow_buckets[index];

    if (item == 0) {
        // Does not exist. Return
        return;
    }
    if (head == NULL && item->key == key) {
        // No collision chain. Remove the item
        // and set table index to NULL
        table->items[index] = NULL;
        free_item(item);
        table->count--;
        return;
    }
    else if (head != NULL) {
        // Collision Chain exists
        if (item->key == key) {
            // Remove this item and set the head of the list
            // as the new item
            free_item(item);
            LinkedList* node = head;
            head = head->next;
            node->next = NULL;
            table->items[index] = create_item(key);
            free_linkedlist(node);
            table->overflow_buckets[index] = head;
            return;
        }

        LinkedList* curr = head;
        LinkedList* prev = NULL;
        
        while (curr) {
            if (curr->item->key == key) {
                if (prev == NULL) {
                    // First element of the chain. Remove the chain
                    free_linkedlist(head);
                    table->overflow_buckets[index] = NULL;
                    return;
                }
                else {
                    // This is somewhere in the chain
                    prev->next = curr->next;
                    curr->next = NULL;
                    free_linkedlist(curr);
                    table->overflow_buckets[index] = head;
                    return;
                }
            }
            curr = curr->next;
            prev = curr;
        }

    }
}

void print_search(FILE* output, HashTable* table, int key) {
    fprintf(output, "%d\n", ht_search(table, key));
}

void print_table(HashTable* table, FILE *output) {
    for (int i = 0; i < table->size; i++) {
        if (table->items[i]) {
            fprintf(output, "%d ", table->items[i]->key);
            if (table->overflow_buckets[i]) {
                LinkedList* head = table->overflow_buckets[i];
                while (head) {
                    printf("%d ", head->item->key);
                    head = head->next;
                }
            }
        }
    }
}

int main() {
    FILE *input, *output;
    char in[20], out[20];

    strcpy(in, "test.in");
    strcpy(out, "test.out");

    input = fopen(in, "r");
    output = fopen(out, "w");

    int N = 0;
    fscanf(input, "%d", &N);

    HashTable* ht = create_table(CAPACITY);

    int start_time = clock();

    for (int i = 0; i < N; i++) {
        int choice = 0, a = 0, b = 0; 
        fscanf(input, "%d", &choice);
        if (choice != 4) {
            fscanf(input, "%d", &a);
        }
        if (choice == 3) {
            fscanf(input, "%d", &b);
        }

        switch (choice) {
            case 0: {
                ht_insert(ht, a);
                break;
            }     
            case 1: {
                ht_delete(ht, a);
                break;
            }
            case 2: {
                print_search(output, ht, a);
                break;
            }
            case 3: {
                if (ht_search(ht, a)) {
                    ht_delete(ht, a);
                    ht_insert(ht, b);
                }
                break;
            }
            case 4: {
                print_table(ht, output);
                if (ht->count > 0) {
                    fprintf(output, "\n");
                }
                break;
            }
        }
    }
    
    int end_time = clock();

    FILE *time = fopen("timeHash", "a");

    fprintf(time, "%f\n", 
    (double) (end_time - start_time) * 1000 / CLOCKS_PER_SEC);
    
    fclose(time);
    
    free_table(ht);
    fclose(input); fclose(output);    
    
    return 0;
}