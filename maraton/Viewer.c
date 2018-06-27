#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "TwoWayListTree.h"
#include "Viewer.h"
#include "SortedIntList.h"

#define MAX_VIEWER 65535
#define MAX_MOVIE 2147483647

typedef DoubleTree Viewer;

Viewer* viewersArray(){
    Viewer* viewers = (Viewer*)malloc((MAX_VIEWER + 1) * sizeof(Viewer));

    for (int i = 0; i <= MAX_VIEWER; i++){
        viewers[i] = NULL;
    }
    return viewers;
}

Viewer newTree(Viewer viewers[], bool *overflow){
    Viewer root = newDoubleTreeDummy(overflow);
    viewers[0] = root;
    return root;
}

void eraseViewers(Viewer root){
    deleteLineage(root);
}

bool userInRange(int userId){
    return userId >= 0 && userId <= MAX_VIEWER;
}

bool movieInRange(int movieId){
    return movieId >= 0 && movieId <= MAX_MOVIE;
}

/*
 * False may mean that element already exist or that the memory couldn't be allocated.
 * Check latter case with viewers[userId] == NULL.
 */
bool addUser(int parentUserId, int userId, Viewer viewers[], bool *overflow){
    if (userInRange(parentUserId) && userInRange(userId)
       && viewers[parentUserId] != NULL && viewers[userId] == NULL) {
        Viewer new = newDoubleTree(userId, overflow);

        if (!(*overflow)) {
            addSon(viewers[parentUserId], new, overflow);
            viewers[userId] = new;
            return true;
        } else {
            return false;
        }
    }
    else return false;
}

bool delUser(int userId, Viewer viewers[]){
    if(userInRange(userId) && userId != 0 && viewers[userId] != NULL) {
        deleteFather(viewers[userId]);
        viewers[userId] = NULL;
        return true;
    }
    else return false;
}

bool addMovie(int userId, int movieRating, Viewer viewers[], bool *overflow){
    if(userInRange(userId) && movieInRange(movieRating) &&
       viewers[userId] != NULL) {
        return addListElement(viewers[userId], movieRating, overflow);
    }
    else {
        return false;
    }
}

bool delMovie(int userId, int movieRating, Viewer viewers[]){
    if (userId >= 0 && movieRating >= 0 && viewers[userId] != NULL) {
        return deleteListElement(viewers[userId], movieRating);
    } else {
        return false;
    }
}

bool movieMarathon(int viewerId, int length, Viewer* viewers, bool *overflow){
    if (viewerId >= 0 && length >= 0 && viewers[viewerId] != NULL) {
        SortedList marathonList = marathon(viewers[viewerId], length, overflow);
        printList(marathonList);
        deleteList(marathonList);
        return true;
    }
    else{
        return false;
    }
}