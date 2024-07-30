#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TCriminal {
    struct TCriminal *m_Next;
    char *m_Name;
    struct TCriminal **m_Contacts;
    size_t m_Cnt;
    size_t m_Capacity;
} TCRIMINAL;


TCRIMINAL *createRecord(const char *name, TCRIMINAL *next) {
    TCRIMINAL *newCriminal = (TCRIMINAL*) malloc(sizeof(TCRIMINAL));
    size_t nameLength = strlen(name) + 1;
    newCriminal->m_Name = (char*) malloc(sizeof(char) * nameLength);
    memcpy(newCriminal->m_Name, name, nameLength);
    newCriminal->m_Capacity = 5;
    newCriminal->m_Cnt = 0;
    newCriminal->m_Next = next;
    newCriminal->m_Contacts = (TCRIMINAL **)malloc(newCriminal->m_Capacity * sizeof(TCRIMINAL *));

    return newCriminal;
}

void mapContacts(TCRIMINAL **tmpArray, size_t count) {
    size_t i = 0;
    do {
        TCRIMINAL *previous = tmpArray[i];
        TCRIMINAL *next = tmpArray[i + 1];
        for (size_t j = 0; j < previous->m_Cnt; j++) {
            TCRIMINAL *oldContact = previous->m_Contacts[j];
            size_t k = 0;
            while (k < count) {
                if (tmpArray[k] == oldContact) {
                    next->m_Contacts[j] = tmpArray[k + 1];
                    break;
                }
                k += 2;
            }
        }
        i += 2;
    } while (i < count);
}

void addCriminalToList(TCRIMINAL **tmpArray, size_t *count, TCRIMINAL *node, TCRIMINAL **right) {
    TCRIMINAL *newCriminal = (TCRIMINAL*)malloc(sizeof(TCRIMINAL));
    size_t crNameLen = strlen(node->m_Name) + 1;
    newCriminal->m_Name = (char*) malloc(sizeof(char) * crNameLen);
    memcpy(newCriminal->m_Name, node->m_Name, crNameLen);
    newCriminal->m_Cnt = node->m_Cnt;
    newCriminal->m_Capacity = node->m_Capacity;
    newCriminal->m_Contacts = (TCRIMINAL**)malloc(newCriminal->m_Capacity * sizeof(TCRIMINAL*));
    newCriminal->m_Next = NULL;

    tmpArray[*count] = node;
    tmpArray[*count + 1] = newCriminal;
    *count += 2;

    (*right)->m_Next = newCriminal;
    *right = newCriminal;
}

TCRIMINAL *cloneList(TCRIMINAL *src) {
    size_t count = 0;
    TCRIMINAL *tmpArray[1000];

    TCRIMINAL *toClone = (TCRIMINAL*)malloc(sizeof(TCRIMINAL));
    size_t nameLength = strlen(src->m_Name) + 1;
    toClone->m_Name = (char*) malloc(sizeof(char) * nameLength);
    memcpy(toClone->m_Name, src->m_Name, nameLength);
    toClone->m_Cnt = src->m_Cnt;
    toClone->m_Capacity = src->m_Capacity;
    toClone->m_Contacts = (TCRIMINAL**)malloc(src->m_Capacity * sizeof(TCRIMINAL *));
    toClone->m_Next = NULL;

    tmpArray[count] = src;
    tmpArray[count + 1] = toClone;
    count += 2;

    TCRIMINAL *timedNode = src->m_Next;
    TCRIMINAL *right = toClone;
    while (timedNode != NULL) {
        addCriminalToList(tmpArray, &count, timedNode, &right);
        timedNode = timedNode->m_Next;
    }

    mapContacts(tmpArray, count);
    return toClone;
}

void addContact(TCRIMINAL *dst, TCRIMINAL *contact) {
    if(dst->m_Capacity == dst->m_Cnt){
        dst->m_Capacity *= 2;
        TCRIMINAL **temporary =(TCRIMINAL**)realloc(dst->m_Contacts, dst->m_Capacity * sizeof(TCRIMINAL*));
        dst->m_Contacts = temporary;
    }
    dst->m_Contacts[dst->m_Cnt] = contact;
    dst->m_Cnt++;
}

void freeList(TCRIMINAL *src) {
    while (src != NULL){
        TCRIMINAL *p = src->m_Next;
        free(src->m_Name);
        if(src->m_Contacts != NULL) {
            free(src->m_Contacts);
        }
        free(src);
        src = p;
    }
}
