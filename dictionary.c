// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <strings.h> // strcasecmp is in this library, case-insensitively compare two strings
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

// Declare "globally scoped" hashing and iterative variables
int i;
int m;
unsigned int wordLoadCount;
unsigned int hashWord;
unsigned int checkHash;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 4096;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash word, access linked list, traverse linked list looking for matching 'word' with 'strcasecmp'.
    checkHash = hash(word);
    if (table[checkHash] == NULL)
    {
        return false;
    }
    node *ptr = table[checkHash];
    while (ptr != NULL)
    {
        if (strcasecmp(word, ptr->word) == 0)
        {
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

// Calculate and return hash.
unsigned int hash(const char *word)
{
    // Totals ASCII-values of chars as an integer.
    unsigned long total = 0;
    for (i = 0, m = strlen(word); i < m; i++)
    {
        total = total + tolower(word[i]);
    }
    return (total % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open file, returning false if unable to open file.
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Unabled to open %s\n", dictionary);
        return false;
    }
    // Read strings from file, hash, and copy string to node 'n'. Then, insert node (preppended)
    // into appropriate linked list. If table empty at hash, insert node at base.
    else
    {
        // Buffer for word to be read into.
        char wordLoad[LENGTH + 1];
        // Start reading strings from file.
        while (fscanf(file, "%s", wordLoad) != EOF)
        {
            // Allocate memory for new node.
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                return false;
            }
            // Copy string to node
            strcpy(n->word, wordLoad);
            // Hash word and initialize node's next field to NULL
            hashWord = hash(n->word);
            n->next = NULL;
            // If table empty at index, new node is inserted at base
            if (table[hashWord] == NULL)
            {
                table[hashWord] = n;
                wordLoadCount++;
            }
            // If table is occupied at index, new node is prepended
            else if (table[hashWord] != NULL)
            {
                n->next = table[hashWord];
                table[hashWord] = n;
                wordLoadCount++;
            }
        }
    }
    // Close file and return true
    fclose(file);
    return true;

}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Simply return the count from the load function.
    return wordLoadCount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate through table, freeing lists at each index
    for (i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            node *tmp = ptr;
            ptr = ptr->next;
            free(tmp);
        }
        // If the ptr is null, and the list has been iterated through, return true.
        if (ptr == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}