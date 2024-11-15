// James Parrott
// 11/14/2024
// CSE 374
// This program is a T9 predictive text
// Each number from 2-9 on the keypad represents three or four letters,
// the number 0 represents a space, and 1 represents a set of symbols such
// as { , . ! ? } etc.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef strdup

// Define strdup manually
// Duplicates a String
// strdup takes a string as input and creates a new copy of it
//  in dynamically allocated memory.
char* strdup(const char* s) {
    size_t len = strlen(s) + 1;
    char* new_str = malloc(len);
    if (new_str) {
        memcpy(new_str, s, len);
    }
    return new_str;
}
#endif

// Define the T9Node structure for trie nodes
typedef struct T9Node {
    struct T9Node* children[8];
    char* word;
} T9Node;

// Define the T9 structure and add a typedef alias
typedef struct T9 {
    T9Node* root;
} T9;

// Helper function to map a character to a T9 digit
int CharToDigit(char c) {
    const char* T9_mapping[8] = {"abc", "def", "ghi", "jkl", "mno",
                                 "pqrs", "tuv", "wxyz"};
    for (int i = 0; i < 8; i++) {
        if (strchr(T9_mapping[i], c) != NULL) {
            // Return 2 to 9
            return i + 2;
        }
    }
    return -1;
}

// Initializes an empty T9 instance
T9* InitializeEmptyT9() {
    T9* t9 = malloc(sizeof(T9));
    if (t9 == NULL) {
        perror("Failed to allocate T9");
        return NULL;
    }
    // Empty root node
    t9->root = calloc(1, sizeof(T9Node));
    return t9;
}

// Adds a word to the T9 trie
void AddWordToT9(T9* dict, const char* word) {
    if (dict == NULL || word == NULL || *word == '\0') {
        return;
    }

    T9Node* curr = dict->root;

    for (const char* p = word; *p != '\0'; p++) {
        if (!islower(*p)) {
            return;
        }

        // Calls the helper function
        int digit = CharToDigit(*p) - 2;

        if (digit < 0 || digit >= 8) {
            return;
        }

        if (curr->children[digit] == NULL) {
            // Assigning value to children node of curr
            curr->children[digit] = calloc(1, sizeof(T9Node));
        }

        curr = curr->children[digit];
    }

    if (curr->word == NULL) {
        // Assigning value to a word of curr
        curr->word = strdup(word);
    }
}

// Initializes T9 from file
T9* InitializeFromFileT9(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("File open failed");
        return NULL;
    }

    T9* t9 = InitializeEmptyT9();
    if (t9 == NULL) {
        return NULL;
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        AddWordToT9(t9, buffer);
    }

    fclose(file);
    return t9;
}

// Predicts a word based on T9 sequence
char* PredictT9(T9* dict, const char* nums) {
    if (dict == NULL || nums == NULL || *nums == '\0') return NULL;

    T9Node* curr = dict->root;

    for (const char* p = nums; *p != '\0'; p++) {
        if (*p == '#') {
            continue;
        }

        int digit = *p - '2';
        if (digit < 0 || digit >= 8) {
            return NULL;
        }

        // Assigning value to children node of curr
        if (curr->children[digit] == NULL) {
            return NULL;
        }

        curr = curr->children[digit];
    }

    return curr->word;
}

// Helper function to free a T9Node
void FreeNode(T9Node* node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < 8; i++) {
        FreeNode(node->children[i]);
    }
    free(node->word);
    free(node);
}

// Frees the entire T9 trie structure
void DestroyT9(T9* dict) {
    if (dict == NULL) {
        return;
    }

    FreeNode(dict->root);
    free(dict);
}
