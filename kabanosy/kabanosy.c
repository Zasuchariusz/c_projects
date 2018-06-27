#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Reprezentacja liczby całkowitej.
 */
typedef struct liczba *Liczba;

/**
 * Daje reprezentację @n.
 */
Liczba reprezentacja(int n);

/**
 * Daje wartość @a lub 0, jeśli nie mieści się ona w zakresie typu int.
 */
int wartosc(Liczba a);

/**
 * Usuwa z pamięci liczbę @a.
 */
void usun(Liczba a);

/**
 * Czyta liczbę z wejścia.
 */
Liczba czytaj(void);

/**
 * Pisze @a na wyjście.
 */
void pisz(Liczba a);

/**
 * Daje sumę @a + @b.
 */
Liczba suma(Liczba a, Liczba b);

/**
 * Daje różnicę @a - @b.
 */
Liczba roznica(Liczba a, Liczba b);

/**
 * Daje iloczyn @a * @b.
 */
Liczba iloczyn(Liczba a, Liczba b);

/**
 * Daje -1 dla ujemnego @a, 0 jeśli @a jest zerem, 1 jeśli @a jest dodatnie.
 */
int znak(Liczba a);

struct liczba{
    int w;
    Liczba next;
};

Liczba nowaLista(int w){
    Liczba perm;
    perm = (Liczba)malloc(sizeof(struct liczba));
    perm->w = w;
    perm->next = NULL;
    return perm;
}

void dodaj(Liczba l, int w){
    Liczba perm;
    perm = nowaLista(w);
    l->next = perm;
}

void printLiczba(Liczba a){
    Liczba l = a->next;
    while(l != NULL){
        printf("%d ", l->w);
        l = l->next;
    }
    printf("\n");
}

Liczba reverseList(Liczba a){
    Liczba prev = NULL, akt = a, nast;
    while(akt != NULL){
        nast = akt->next;
        akt->next = prev;
        prev = akt;
        akt = nast;
    }
    return prev;
}

int pozycja(int i){
    if(i < 0) return (-1)*i - 1;
    else return i;
}

int czytajInt(){
    char liczba[20];
    int n = 20, i = 0;
    char c;
    c = (char) getchar();
    while(isdigit(c) && i < n){
        liczba[i] = c;
        i++;
        c = (char) getchar();
    }
    ungetc(c, stdin);
    liczba[i] = '\0';
    return atoi(liczba);
}

Liczba sumaProsta(Liczba a1, Liczba a2){
    Liczba sum = nowaLista(0), l1 = a1->next, l2 = a2->next, akt = sum;

    while(l1 != NULL || l2 != NULL){
        if(l1 == NULL || (l2 != NULL && pozycja(l2->w) < pozycja(l1->w))) {
            dodaj(akt, l2->w);
            l2 = l2->next;
        } else {
            dodaj(akt, l1->w);
            l1 = l1->next;
        }
        akt = akt->next;
    }
    return sum;
}

void uporzadkuj(Liczba *prev, Liczba *akt){ //za każdym razem albo coś usuwa, albo przesuwa wskazniki
    Liczba perm;
    if((*akt)->next == NULL || pozycja((*akt)->next->w) >= pozycja((*akt)->w) + 2){
        *prev = *akt;
        *akt = (*akt)->next;
    } else if (pozycja((*akt)->w) == pozycja((*akt)->next->w)){
        if ((*akt)->w == (*akt)->next->w) { //n, n -> n + 1
            if ((*akt)->w >= 0) ((*akt)->w)++;
            else ((*akt)->w)--;
            perm = (*akt)->next;
            (*akt)->next = (*akt)->next->next;
            free(perm);
        } else { //akt->w = -(akt->next->w) - 1
            perm = *akt; //skladniki sumy byly poprawne, wiec mozna spokojnie przesunac
            *akt = (*akt)->next->next;
            (*prev)->next = *akt;
            free(perm->next); //oba skladniki sie zniosa
            free(perm);
        }
    } else if(pozycja((*akt)->w) + 1 == pozycja((*akt)->next->w)){
        if((*akt)->w == (*akt)->next->w - 1 || (*akt)->w == (*akt)->next->w + 1){
            if((*akt)->w >= 0) ((*akt)->next->w)++;
            else ((*akt)->next->w)--;
            (*akt)->w = (-1) * ((*akt)->w) - 1; //n -> -n
            *prev = *akt;
            *akt = (*akt)->next;
        } else { //akt i akt->next maja przeciwne znaki
            (*akt)->w = (-1) * ((*akt)->w) - 1;
            perm = (*akt)->next;
            (*akt)->next = (*akt)->next->next;
            free(perm);
        }
    } else if(pozycja((*akt)->w) == pozycja((*akt)->next->w) + 1) { //poz(akt) = poz(akt->next) + 1, mozliwe gdy -n, -(n+1), -(n+3), +(n+3)
        if(pozycja((*prev)->w) == pozycja((*akt)->next->w) - 1){
            if((*prev)->w == (*akt)->next->w - 1 || (*prev)->w == (*akt)->next->w + 1){
                if((*prev)->w >= 0) (*akt)->next->w++;
                else (*akt)->next->w--;
                (*prev)->w = (-1) * (*prev)->w - 1;
            }
            else {
                (*prev)->w = (-1) * (*prev)->w - 1;
                perm = (*akt)->next;
                (*akt)->next = (*akt)->next->next;
                free(perm);
            }
        }
    }
}

void zujemnij(Liczba a){
    Liczba akt = a->next;
    while(akt != NULL){
        akt->w = akt->w * (-1) - 1;
        akt = akt -> next;
    }
}

Liczba prosteMnozenie(Liczba a, int w){
    int abs;
    Liczba wynik = nowaLista(0), act = wynik, l = a->next;
    if(w >= 0) abs = w;
    else abs = (-1) * w - 1;
    while (l != NULL){
        if(l->w >= 0) dodaj(act, l->w + abs);
        else dodaj(act, l->w - abs);
        act = act->next;
        l = l->next;
    }
    if(w < 0) zujemnij(wynik);
    return wynik;
}

/*
void dodajSimple(Liczba l, int w){
    Liczba perm = NULL, akt = l;
    perm = nowaLista(w);
    while(akt->next != NULL) akt = akt->next;
    akt->next = perm;
}*/


Liczba reprezentacja(int n){
    Liczba start = nowaLista(0), akt;
    int i = 0, l = n;
    akt = start;
    while(l != 0){
        if(l % 2 == 0) l = l/2;
        else {
            if((l-1) % 4 == 0){
                dodaj(akt, i);
                l = (l - 1)/2;
            } else { //(l+1) % 4 == 0
                dodaj(akt, (-1)*i - 1);
                l = (l + 1)/2;
            }
            akt = akt->next;
        }
        i++;
    }
    return start;
}

int wartosc(Liczba a){
    Liczba start, akt;
    int l, i, k;
    bool zaDuzo = false;
    if(a->next == NULL) return 0;
    start = reverseList(a);
    akt = start;
    l = 0;
    i = pozycja(start->w);
    k = i;
    while(i >= 0 && !zaDuzo){
        if(i == k && akt->next != NULL){
            if(akt->w >= 0){
                if(l > (INT_MAX - 1)/2) zaDuzo = true;
                else l = 2 * l + 1;
            }
            else {
                if(l > (INT_MAX - 1)/2 + 1) zaDuzo = true;
                else l = 2 * l - 1;
            }
            akt = akt->next;
            k = pozycja(akt->w);
        } else {
            if(l > INT_MAX / 2) zaDuzo = true;
            else l = l * 2;
        }
        i--;
    }
    reverseList(start);
    if(zaDuzo) return 0;
    else return l;
}

void usun(Liczba a){
    Liczba prev = a, akt;
    while(prev != NULL){
        akt = prev->next;
        free(prev);
        prev = akt;
    }
    free(prev);
}

Liczba czytaj(void){
    Liczba a = nowaLista(0), akt = a;
    int w;
    char c;
    c = (char)getchar();
    while (c != ';'){
        w = czytajInt();
        if(c == '-') w = (-1)*w;
        dodaj(akt, w);
        akt = akt->next;
        c = (char) getchar();
    }
    return a;
}

void pisz(Liczba a){
    Liczba akt = a->next;
    while(akt != NULL){
        if(akt->w >= 0) printf("+%d", akt->w);
        else printf("%d", akt->w);
        akt = akt->next;
    }
    printf(";");
}

Liczba suma(Liczba a1, Liczba a2){
    Liczba prev, akt;
    Liczba sum = sumaProsta(a1, a2);
    prev = sum;
    akt = sum->next;
    while(akt != NULL) {
        uporzadkuj(&prev, &akt);
    }
    return sum;
}

Liczba roznica(Liczba a1, Liczba a2){
    Liczba roznica;
    zujemnij(a2);
    roznica = suma(a1, a2);
    zujemnij(a2);
    return roznica;
}

Liczba iloczyn(Liczba a1, Liczba a2){
    Liczba wynik = nowaLista(0), act = a2->next, skladnik, perm;
    while(act != NULL){
        perm = wynik;
        skladnik = prosteMnozenie(a1, act->w);
        wynik = suma(wynik, skladnik);
        usun(perm);
        usun(skladnik);
        act = act->next;
    }
    return wynik;
}

int znak(Liczba a){
    Liczba act = a->next;
    if (act == NULL) return 0;
    else while(act->next != NULL){
            act = act->next;
        }
    if(act->w >= 0) return 1;
    else return -1;
}

/*
void pokaz(Liczba x) {
    printf("%d %d ", znak(x), wartosc(x));
    pisz(x);
    putchar('\n');
}*/

/*
int main() {
  Liczba a = czytaj();
  Liczba b = czytaj();
  Liczba c = reprezentacja(127);
  Liczba d = suma(a, b);
  Liczba e = roznica(a, c);
  Liczba f = iloczyn(b, c);
  pokaz(a);
  pokaz(b);
  pokaz(c);
  pokaz(d);
  pokaz(e);
  pokaz(f);
  usun(a);
  usun(b);
  usun(c);
  usun(d);
  usun(e);
  usun(f);
  return 0;
}*/

/*
for(int i = 1; i < 21; i++){
    a = reprezentacja(i);
    printf("%d = ", i);
    pisz(a);
    printf("\n");
    usun(a);
}*/
/*
Liczba l = reprezentacja(120);
Liczba k = reprezentacja(-120);
Liczba m = iloczyn(l, k);
pisz(l);
printf("\n");
pisz(k);
printf("\n");
pisz(m);
printf("\n");
printf("%d\n", wartosc(l));
printf("%d\n", wartosc(k));
printf("%d\n", wartosc(m));
usun(l);
usun(k);
usun(m); */
