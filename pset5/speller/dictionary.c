// Implements a dictionary's functionality
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"



// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 130003;
unsigned int word_count = 0;



// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //gets the hash value of the word in text
    unsigned int key = hash(word);
    node* cursor = table[key];
    bool match = false;

    // if table[key] is not NULL iterates through linked list
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor -> word) == 0)
        {
            match = true;
        }
        cursor = cursor -> next;
    }
    return match;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //this is the DJB2 hash function first reported by Dan Bernstein
    unsigned int hash = 5381;
    int c;

    while ((c = tolower(*word++)))
    {

        hash = ((hash << 5) + hash) + c;    /* hash * 33 + c */
        hash = hash % 100000;

    }

    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary in read mode and error checks
    FILE *load_dictionary = fopen(dictionary, "r");
    if (dictionary == NULL) // error check
    {
        printf("unable to open file\n");
        return 1;
    }

    // variable to keep track of hash funtion values
    unsigned int key;
    //array to read words from dictionary into temporary array
    char new_word [LENGTH + 1];

    //temporary node pointer to head of link list
    node *head = NULL;

    //pointer to store when word is found
    node *found_word = NULL;
    // int bucket = 0;
    while ((fscanf(load_dictionary, "%s", new_word)) == 1)
    {
        //malloc memory for node and error checks
        found_word = malloc(sizeof(node));
        if (found_word == NULL)
        {
            printf("memory alloc failed\n");
            return 1;
        }

        // copy word scanned
        strcpy(found_word -> word, new_word);

        //implements hash funtion to get an int value
        key = hash(new_word);

        //if no collision is ecountered stores word in table and sets new_word as head of linked list
        if (table[key] == NULL)
        {
            table[key] = found_word;
            head = found_word;
            found_word -> next = NULL;
            word_count++;
            // bucket++;
        }
        // in case of collision set new word as new head of linked list and and old head becomes next
        else
        {
            head = found_word;
            found_word -> next = table[key];
            table[key] = found_word;
            word_count++;
        }
    }

    bool success = false;
    //printf("# of buckets: %i", bucket);
    if (feof(load_dictionary))
    {
        fclose(load_dictionary);
        success = true;
    }
    return success;
}
// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //returns word_count as implemented in load
    return word_count;
}
// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *temp = NULL;
    node *cursor = NULL;
    for (int i = 0; i <= N; i++)
    {
        if (table[i] != NULL)
        {
            // sets cursor to point to the same mem address as table[i]
            for (cursor = table[i]; cursor -> next != NULL; free(temp))
            {
                //if there more liked nodes in the tables it set tempt to the same mem address as cursor and frees temp
                temp = cursor;
                //sets cursor to point to next once temp is assigned
                cursor = cursor -> next;
            }
            //frees cursor is no more nodes in "bucket"
            free(cursor);
        }
    }
    return true;
}
