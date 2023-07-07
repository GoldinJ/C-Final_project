#include "hashtable.h"

unsigned int hash(const char* key) {
    unsigned int hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

Entry* createEntry(const char* key, void* value) {
    Entry* entry = (Entry*)malloc(sizeof(Entry));
    entry->key = duplicateString(key);
    entry->value = value;
    entry->next = NULL;
    return entry;
}

HashTable* createHashTable() {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->entries = (Entry**)calloc(TABLE_SIZE, sizeof(Entry*));
    return table;
}

void insert(HashTable* table, const char* key, void* value) {
    unsigned int index = hash(key) % TABLE_SIZE;
    Entry* newEntry = NULL;

    Entry* entry = table->entries[index];
    Entry* prevEntry = NULL;
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        prevEntry = entry;
        entry = entry->next;
    }

    newEntry = createEntry(key, value);
    if (prevEntry == NULL) {
        table->entries[index] = newEntry;
    } else {
        prevEntry->next = newEntry;
    }
}

void* get(HashTable* table, const char* key) {
    unsigned int index = hash(key) % TABLE_SIZE;

    Entry* entry = table->entries[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}

void removeEntry(HashTable* table, const char* key) {
    unsigned int index = hash(key) % TABLE_SIZE;

    Entry* entry = table->entries[index];
    Entry* prevEntry = NULL;
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            if (prevEntry == NULL) {
                table->entries[index] = entry->next;
            } else {
                prevEntry->next = entry->next;
            }
            free(entry->key);
            free(entry);
            return;
        }
        prevEntry = entry;
        entry = entry->next;
    }
}

void printHashTable(HashTable* table) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        Entry* entry = table->entries[i];
        if (entry != NULL) {
            printf("Index %d:\t", i);
            while (entry != NULL) {
                printf("  Key: %s\t ---- \t", entry->key);
                printf("  Value: %d\n", (int)(long)entry->value);
                entry = entry->next;
            }
        }
    }
    printf("\n");
}

void freeHashtable(HashTable* table) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        Entry* entry = table->entries[i];
        while (entry != NULL) {
            Entry* nextEntry = entry->next;
            free(entry->key);
            free(entry);
            entry = nextEntry;
        }
    }
    free(table->entries);
    free(table);
}

void use_case_example(){
    int i;
    /* // Inserting key-value pairs with array of strings as values */
    char* languagesDeveloper1[MAX_ARRAY_SIZE] = {"C", "C++", "Java"};
    char* languagesDeveloper2[MAX_ARRAY_SIZE] = {"Python", "JavaScript"};
    char* languagesDeveloper3[MAX_ARRAY_SIZE] = {"Go"};
    char** dev1Languages = NULL;
    char** dev2Languages = NULL;
    char** dev3Languages = NULL;
    HashTable* table = createHashTable();

    /* // Example usage

    // Inserting key-value pairs with integer values */
    insert(table, "key1", (void*)1);
    insert(table, "key2", (void*)2);
    insert(table, "key3", (void*)3);

    /* // Retrieving integer values */
    printf("Value for 'key1': %d\n", (int)(long)get(table, "key1"));
    printf("Value for 'key2': %d\n", (int)(long)get(table, "key2"));
    printf("Value for 'key3': %d\n", (int)(long)get(table, "key3"));
    printf("\n");

    insert(table, "developer1", languagesDeveloper1);
    insert(table, "developer2", languagesDeveloper2);
    insert(table, "developer3", languagesDeveloper3);

    /* // Retrieving array of strings */
    printf("Languages for 'developer1':\n");
    dev1Languages = (char**)get(table, "developer1");
    if (dev1Languages != NULL) {
        for (i = 0; dev1Languages[i] != NULL; i++) {
            printf("%s\n", dev1Languages[i]);
        }
    } else {
        printf("No languages found for 'developer1'\n");
    }
    printf("\n");

    printf("Languages for 'developer2':\n");
    dev2Languages = (char**)get(table, "developer2");
    if (dev2Languages != NULL) {
        for (i = 0; dev2Languages[i] != NULL; i++) {
            printf("%s\n", dev2Languages[i]);
        }
    } else {
        printf("No languages found for 'developer2'\n");
    }
    printf("\n");

    printf("Languages for 'developer3':\n");
    dev3Languages = (char**)get(table, "developer3");
    if (dev3Languages != NULL) {
        for (i = 0; dev3Languages[i] != NULL; i++) {
            printf("%s\n", dev3Languages[i]);
        }
    } else {
        printf("No languages found for 'developer3'\n");
    }
    printf("\n");

    freeHashtable(table);

}

/* int main() {
    use_case_example();
    return 0;
}
 */