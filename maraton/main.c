#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "Viewer.h"

/*
 * Returns if current line is empty. Doesnt change stdin state.
 */
bool emptyLine();

/*
 * Moves stdin to the end of the current line;
 */
void toEOL();

/*
 * Moves stdin to the end of current line and prints "ERROR" to stderr
 */
void errorToEOL();

/*
 * Checks if next char is EOF. Doesn't change stdin.
 */
bool eof();

/*
 * Handles memory overflow exception.
 * Frees all allocated memory and exits with code '1'.
 */
void memoryOverflow(Viewer* viewers);

/*
 * Checks what kind of command is put. Returns different int for ech case.
 */
int properCommand(char* string);

/*
 * Handles adding User from viewer.h library
 */
void addUserCommand(Viewer *viewers);

/*
 * Handles deleting user from viewer.h library
 */
void delUserCommand(Viewer* viewers);

/*
 * Handles adding movie from viewer.h library
 */
void addMovieCommand(Viewer* viewers);

/*
 * Handles deleting movie from viewer.h library
 */
void delMovieCommand(Viewer* viewers);

/*
 * Handles marathon function from viewer.h library
 */
void marathonCommand(Viewer* viewers);

/*
 * Calls command associated with given "n" int (from -1 to 5)
 */
void callCommand(int n, Viewer *viewers);

/*
 * Processes one line from stdin. Calls a command, ignores line,
 * or signals error.
 */
void processLine(Viewer* viewers);

/*
 * Listens for input from stdin and reacts to it.
 */
void read(Viewer viewers[]);


bool emptyLine(){
    char c;
    c = getchar();
    ungetc(c, stdin);
    return c == 10;
}

void toEOL(){
    char c;
    c = getchar();
    while (c != '\n' && c != EOF){
        c = getchar();
    }
    ungetc(c, stdin);
}

void errorToEOL(){
    toEOL();
    fprintf(stderr, "ERROR\n");
}

bool eof(){
    char c;
    c = getchar();
    ungetc(c, stdin);
    return c == EOF;
}

void memoryOverflow(Viewer* viewers){
    if (viewers != NULL){
        eraseViewers(viewers[0]);
        free(viewers);
    }
    exit(1);
}

int properCommand(char* string){
    char* properCommands[5] = {"addUser", "delUser", "addMovie", "delMovie", "marathon"};
    int output = -1;

    if (string[0] != '#') {

        for (int i = 0; i < 5; i++) {
            if (strcmp(string, properCommands[i]) == 0) {
                output = i;
            }
        }

    } else {
        output = 5;
    }

    return output;
}

void addUserCommand(Viewer *viewers) {
    int parentId, newId;
    bool overflow = false;

    scanf("%d", &parentId);
    scanf("%d", &newId);

    if (emptyLine() && addUser(parentId, newId, viewers, &overflow)) {
        printf("OK\n");
    }
    else {
        if (overflow) {
            memoryOverflow(viewers);
        }
        else {
            errorToEOL();
        }
    }
}

void delUserCommand(Viewer* viewers) {
    int userId;

    scanf("%d", &userId);
    if (delUser(userId, viewers)) {
        printf("OK\n");
    }
    else {
        fprintf(stderr, "ERROR\n");
    }
}

void addMovieCommand(Viewer* viewers){
    int userId;
    int movieId;
    bool overflow = false;

    scanf("%d", &userId);
    scanf("%d", &movieId);
    if (addMovie(userId, movieId, viewers, &overflow)) {
        printf("OK\n");
    }
    else {
        if (!(overflow)) {
            fprintf(stderr, "ERROR\n");
        } else {
            memoryOverflow(viewers);
        }
    }
}

void delMovieCommand(Viewer* viewers){
    int userId;
    int movieId;

    scanf("%d", &userId);
    scanf("%d", &movieId);
    if (delMovie(userId, movieId, viewers)) {
        printf("OK\n");
    }
    else {
        fprintf(stderr, "ERROR\n");
    }
}

void marathonCommand(Viewer* viewers){
    int userId;
    int k;
    bool overflow = false;

    scanf("%d", &userId);
    scanf("%d", &k);
    if (!movieMarathon(userId, k, viewers, &overflow)){
        if (!overflow) {
            fprintf(stderr, "ERROR\n");
        } else {
            memoryOverflow(viewers);
        }
    }
}

void callCommand(int n, Viewer *viewers){
    switch (n){
        case 0:
            addUserCommand(viewers);
            break;
        case 1:
            delUserCommand(viewers);
            break;
        case 2:
            addMovieCommand(viewers);
            break;
        case 3:
            delMovieCommand(viewers);
            break;
        case 4:
            marathonCommand(viewers);
            break;
        case 5:
            toEOL();
            break;
        default:
            errorToEOL();
            break;
    }
}

void processLine(Viewer* viewers){
    if (!emptyLine()){
        char buf[10];
        scanf("%9s", buf);
        callCommand(properCommand(buf), viewers);
    }

}

void read(Viewer viewers[]){
    char c;
    c = getchar();

    if(c != EOF) {
        ungetc(c, stdin);

        while(!eof()){
            processLine(viewers);
            c = getchar();
        }
    }
}

int main(){
    bool overflow = false;
    Viewer* viewers = viewersArray();
    Viewer root = newTree(viewers, &overflow);
    if(overflow){
        free(viewers);
        exit(1);
    }

    read(viewers);

    eraseViewers(root);
    free(viewers);
    return 0;
}

