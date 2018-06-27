#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct SortedIntList{
    int id;
    struct SortedIntList *next;
};

typedef struct SortedIntList *SortedList;

/*
 * Returns new SortedList element containing "id".
 * Communicates possible memory overflow with "overflow" pointer.
 */
static SortedList newSortedListElement(int id, bool *overflow);

/*
 * Returns if element with given "id" exist in given "list.
 */
static bool isElement(SortedList list, int id);

/*
 * Inserts element into list preserving order. Starts from "destination" element.
 */
static void addElement(SortedList destination, SortedList element);

/*
 * Copies "source" list and straight adds it to "destination" list.
 * Doesn't check order.
 * Communicates possible memory overflow with "overflow" pointer.
 */
static void addCopiedElements(SortedList destination, SortedList source,
                              int k, bool *overflow);

/*
 * Deletes next element from "list" if it exists.
 */
static void deleteNextElement(SortedList list);

/*
 * Returns element containing biggest id from lists in given n-long array.
 */
static SortedList recordFromArray(SortedList lists[], int n);

/*
 * Moves all list pointers in "lists" n-long array containing "target_id"
 * one element forward.
 */
static void moveForward(SortedList lists[], int n, int target_id);

/*
 * Returns if we should still look for more sons in "sonList" for merging
 * sonList with fatherList. Support function for "mergeLists" library function.
 */
static bool sonListHasBiggerElements(SortedList sonList, int fatherMax,
                                     int sonSegmentLength, int listLength);


static SortedList newSortedListElement(int id, bool *overflow){
    SortedList list = NULL;

    list = (SortedList)malloc(sizeof(struct SortedIntList));

    if (list != NULL) {
        list->next = NULL;
        list->id = id;
    }
    else {
        *overflow = true;
    }
    return list;
}

static bool isElement(SortedList list, int id){
    if (list == NULL || list->next == NULL) {
        return false;
    }
    else if (list->next->id == id) {
        return true;
    }
    else {
        return isElement(list->next, id);
    }
}

static void addElement(SortedList destination, SortedList element){
    SortedList list = destination;

    if (element != NULL) {

        while (list->next != NULL && element->id < list->next->id){
            list = list->next;
        }

        element->next = list->next;
        list->next = element;
    }
}

static void addCopiedElements(SortedList destination, SortedList source, int k, bool *overflow){
    SortedList destinationPointer = destination, sourcePointer = source;

    for (int i = 0; i < k; i++){
        if (sourcePointer != NULL && !(*overflow)) {
            destinationPointer->next = newSortedListElement(sourcePointer->id, overflow);

            if (!(*overflow)) {
                destinationPointer = destinationPointer->next;
                sourcePointer = sourcePointer->next;
            }
        }
    }
}

static void deleteNextElement(SortedList list){
    SortedList target;

    if (list->next != NULL){
        target = list->next;
        list->next = target->next;
        free(target);
    }
}

static SortedList recordFromArray(SortedList lists[], int n){
    SortedList record = NULL;
    int i_record = 0;

    for (int i = 0; i < n; i++){
        if (lists[i] != NULL && lists[i]->id >= i_record) {
            i_record = lists[i]->id;
            record = lists[i];
        }
    }
    return record;
}

static void moveForward(SortedList lists[], int n, int target_id){
    for (int i = 0; i < n; i++){
        if (lists[i] != NULL && lists[i]->id == target_id){
            lists[i] = lists[i]->next;
        }
    }
}

static bool sonListHasBiggerElements(SortedList sonList, int fatherMax, int sonSegmentLength, int listLength){
    return sonSegmentLength < listLength
           && sonList->next != NULL
           && sonList->next->id > fatherMax;
}



bool addNewElement(SortedList destination, int id, bool *overflow){
    if(isElement(destination, id)) {
        return false;
    }
    else {
        addElement(destination, newSortedListElement(id, overflow));

        if (!(*overflow)) {
            return true;
        } else {
            return false;
        }
    }
}

bool deleteElementById(SortedList list, int id) {
    SortedList listVar = NULL;

    if (!isElement(list, id)) {
        return false;
    }
    else {
        listVar = list;

        while (listVar->next != NULL && listVar->next->id != id) {
            listVar = listVar->next;
        }

        deleteNextElement(listVar);
        return true;
    }
}

void deleteList(SortedList start){
    SortedList next = NULL;

    if (start != NULL) {
        next = start->next;
        free(start);
        deleteList(next);
    }

}

SortedList newSortedList(bool *overflow){
    return newSortedListElement(INT_MAX, overflow);
}

SortedList sortedMultiList(SortedList lists[], int n, int outLength, bool *overflow) {
    SortedList out = newSortedList(overflow), record = NULL, last = out;

    moveForward(lists, n, INT_MAX); //moving lists forward, leaving dummies behind

    for (int i = 0; i < outLength; i++) {
        record = recordFromArray(lists, n);

        if (record != NULL){
            addNewElement(last, record->id, overflow);

            if (!(*overflow)) {
                last = last->next;
                moveForward(lists, n, record->id);
            }
        }
    }

    return out;
}

SortedList mergeLists(SortedList fatherList, SortedList sonList, int listLength, bool *overflow) {
    int fatherMax = 0;
    int sonLength = 0;
    SortedList sonListPointer = sonList;

    if (fatherList != NULL) {
        if (fatherList->next != NULL) {
            fatherMax = fatherList->next->id;
        }

        while (sonListHasBiggerElements(sonListPointer, fatherMax, sonLength, listLength)) {
            sonListPointer = sonListPointer->next;
            sonLength++;
        }

        deleteList(sonListPointer->next);
        addCopiedElements(sonListPointer, fatherList->next, listLength - sonLength, overflow);
    }

    return sonList;
}

void printList(SortedList list){
    SortedList listPointer = list;

    if(list == NULL || list->next == NULL){
        printf("NONE");
    }
    else {
        listPointer = listPointer->next;
        printf("%d ", listPointer->id);
        listPointer = listPointer->next;

        while (listPointer != NULL){
            printf("%d ", listPointer->id);
            listPointer = listPointer->next;
        }
    }
    printf("\n");
}