#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order; 
};

// Node for linked list
struct ListNode
{
    struct RecordType data;
    struct ListNode* next;
};

// HashType
struct HashType
{
    struct ListNode* head;
};

// Hash table size
#define HASH_SIZE 10

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType pHashArray[], int hashSz)
{
    int i;
    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        struct ListNode* current = pHashArray[i].head;
        while (current != NULL)
        {
            printf("%d %c %d -> ", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize hash table
    struct HashType hashTable[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        hashTable[i].head = NULL;
    }

    // Insert records into hash table
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);
        struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
        newNode->data = pRecords[i];
        newNode->next = hashTable[index].head;
        hashTable[index].head = newNode;
    }

    // Display records in hash table
    displayRecordsInHash(hashTable, HASH_SIZE);

    // Free memory
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        struct ListNode* current = hashTable[i].head;
        while (current != NULL)
        {
            struct ListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(pRecords);

    return 0;
}
