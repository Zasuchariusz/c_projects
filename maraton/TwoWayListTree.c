#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "TwoWayListTree.h"
#include "SortedIntList.h"

extern struct TwoWayListTree{
    int id;
    DoubleTree son;
    DoubleTree right;
    DoubleTree left;
    SortedList list;
};

typedef struct TwoWayListTree *DoubleTree;

/*
 * Inserts new "brother" element right from given "tree"
 */
static void addBrother(DoubleTree tree, DoubleTree brother);

/*
 * Removes "brother" element from list, not deleting it
 */
static void removeBrother(DoubleTree brother);

/*
 * Returns total number of sons tree element "father" has
 * Does not count dummy
 */
static int numberOfSons(DoubleTree father);

/*
 * Inserts list starting with "childrenDummy" element into new list
 * called "fosterFamily"
 */
static void adoptChildren(DoubleTree fosterFamily, DoubleTree childrenDummy);

/*
 * Removes father from his list and places his children into his ex-list
 */
static void removeFather(DoubleTree father);

/*
 * Completely removes and deletes "brother" element from list
 */
static void deleteBrother(DoubleTree brother);


static void addBrother(DoubleTree tree, DoubleTree brother){
    brother->left = tree;
    brother->right = tree->right;
    tree->right->left = brother;
    tree->right = brother;
}

static void removeBrother(DoubleTree brother){
    brother->left->right = brother->right;
    brother->right->left = brother->left;
}

static int numberOfSons(DoubleTree father){
    int number = 0;
    DoubleTree son = father->son;
    if (son == NULL) {
        return 0;
    }
    else{
        while (son->right != father->son){
            number++;
            son = son->right;
        }
    }
    return number;
}

static void adoptChildren(DoubleTree fosterFamily, DoubleTree childrenDummy){
    childrenDummy->left->right = fosterFamily->right;
    fosterFamily->right->left = childrenDummy->left;
    childrenDummy->left = fosterFamily;
    fosterFamily->right = childrenDummy;
    deleteBrother(childrenDummy);
}

static void removeFather(DoubleTree father){
    if(father->son != NULL){
        adoptChildren(father, father->son);
    }
    removeBrother(father);
}

static void deleteBrother(DoubleTree brother){
    removeBrother(brother);
    deleteList(brother->list);
    free(brother);
}


DoubleTree newDoubleTreeDummy(bool *overflow){
    DoubleTree d = (DoubleTree)malloc(sizeof(struct TwoWayListTree));
    if (d != NULL) {
        d->id = 0;
        d->son = NULL;
        d->right = d;
        d->left = d;
        d->list = NULL;
    }
    else {
        *overflow = true;
    }
    return d;
}

DoubleTree newDoubleTree(int id, bool *overflow){
    DoubleTree d = newDoubleTreeDummy(overflow);
    if (!(*overflow)) {
        d->id = id;
    }
    return d;
};

void addSon(DoubleTree tree, DoubleTree son, bool *overflow){
    if (tree->son == NULL){
        tree->son = newDoubleTreeDummy(overflow);
    }
    if (!(*overflow)) {
        addBrother(tree->son->left, son);
    }
}

bool addListElement(DoubleTree tree, int id, bool *overflow){
    if (tree->list == NULL)
        tree->list = newSortedList(overflow);
    return addNewElement(tree->list, id, overflow);
}

bool deleteListElement(DoubleTree tree, int id){
    return deleteElementById(tree->list, id);
}

void deleteFather(DoubleTree father){
    removeFather(father);
    deleteList(father->list);
    free(father);
}

void deleteLineage(DoubleTree grandFather){
    if (grandFather != NULL) {
        if (grandFather->son != NULL) {
            while (grandFather->son->right != grandFather->son) {
                deleteLineage(grandFather->son->right);
            }
            free(grandFather->son);
        }
        deleteBrother(grandFather);
    }
}

SortedList marathon(DoubleTree tree, int length, bool *overflow){
    int numOfSons = numberOfSons(tree);
    DoubleTree sonPointer = tree->son;
    SortedList sonMarathon = NULL;
    SortedList sonsLists[numOfSons], tempSonsLists[numOfSons];

    for (int i = 0; i < numOfSons; i++){
        sonPointer = sonPointer->right;
        sonsLists[i] = marathon(sonPointer, length, overflow);
        tempSonsLists[i] = sonsLists[i];
    }

    sonMarathon = sortedMultiList(tempSonsLists, numOfSons, length, overflow);
    mergeLists(tree->list, sonMarathon, length, overflow);

    for (int i = 0; i < numOfSons; i++){
        deleteList(sonsLists[i]);
    }

    return sonMarathon;

}