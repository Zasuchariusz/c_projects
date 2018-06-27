#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_DLUGOSC_ETYKIETY 2000
#define MAX_ETYKIETY 1000
#define MAX_INSTRUKCJE 2000
#define MIN_ADRES -5000
#define MAX_ADRES 4999
#define MAX_STOS 4000

//STRUKTURY DANYCH
/*
Użycie globalnej pamięci w maszynie wirtualnej wydaje mi się uzasadnione
Maszyna wirtualna ma udawać komputer, czyli jak komputer powinna mieć pamięć
*/


/*
Sktruktura danych dla etykiet, każda etykieta dostaje int (od 0)
W tablicy char[] ciąg nazw etykiet, w int[] wskazniki na kolejne etykiety
*/
char etykiety_identyfikatory[MAX_DLUGOSC_ETYKIETY];
int etykiety_wskazniki[MAX_ETYKIETY];
int* ilosc_id_ety;

/*
Struktura dla definicji taka jak dla etykiet
Dodatkowa tablica wskazująca miejsca w kodzie, gdzie występują definicje
*/
char definicje_identyfikatory[MAX_DLUGOSC_ETYKIETY];
int definicje_wskazniki[MAX_ETYKIETY];
int* ilosc_id_def;
int definicje_miejsca[MAX_ETYKIETY];

/*
Tablica trzymająca przyporządkowane do int-owych etykiet miejsca do których odsyłają
*/
int definicje_etykiet[MAX_ETYKIETY];

//Bufory służące do zczytywania najmniejszych składowych programu (identyfikatory, adresy, symbole)
char bufor1[MAX_DLUGOSC_ETYKIETY];
char bufor2[MAX_DLUGOSC_ETYKIETY];

//trzyma dane, dla każdego elementu dwa pola np. adres1 adres2 dla odejmowania
int dane[2 * MAX_INSTRUKCJE];
//każdemu elementowi programu przyporządkowuje literę mówiącą jaki to element
char element[MAX_INSTRUKCJE];
//odpowiada "długości programu"
int* ilosc_elementow;
//wskaznik miejsca w programie podczas wykonania
int* aktualny_element;

//wejscie programu (&....)
char wejscie[MAX_INSTRUKCJE];
//liczba wszystkich argumentów wejscia
int* ilosc_wejscia;
//wskaznik używany do wczytywania z wejścia
int* wejscie_wskaznik;

//Pamięć programu
int pamiec[MAX_ADRES - MIN_ADRES + 1];

//STOS
int stos[MAX_STOS];
int *stos_wskaznik;

//Standardowe funkcje stosu
void stos_push(int n){
  stos[*stos_wskaznik] = n;
  (*stos_wskaznik)++;
}

int stos_pop(){
  (*stos_wskaznik)--;
  return stos[*stos_wskaznik];
}

bool stos_pusty(){
  return *stos_wskaznik == 0;
}

//Zapisuje w pamięci pod adresem @i wartość @n
void zapisz_w_pamieci(int i, int n){
  pamiec[i - MIN_ADRES] = n;
}

//Zwraca wartość spod adresu i, dla i wartości spoza zakresu pamięci zwraca -i -1
int pamiec_daj(int i){
  int wynik;
  if(i < MIN_ADRES || i > MAX_ADRES){
    wynik = -1 - i;
  } else {
    wynik = pamiec[i - MIN_ADRES];
  }
  return wynik;
}

//Wypełnia pamięc początkowymi wartościami -1 - adres
void inicjalizuj_pamiec(){
  for(int i = MIN_ADRES; i <= MAX_ADRES; i++){
    zapisz_w_pamieci(i, -1 - i);
  }
}

/*
Zapisuje informacje dla instrukcji jako dane, w przypadku gdy instrukcja
przyjmuje tylko jeden argument, drugie pole danych jest równe 0
*/
void zapisz_dane(int d1, int d2){
  dane[*ilosc_elementow * 2] = d1;
  dane[*ilosc_elementow * 2 + 1] = d2;

}

/*
Dane[] udają tablicę, która pod każdym indeksem @i ma dwa pola: @j = 0 lub @j = 1
*/
int daj_dane(int i, int j){
  return dane[2 * i + j];
}

//Alokuje pamięć wszystkich wskaźników i ustawia je na 0, inicjalizuje pamięć
void inicjalizuj(){
  etykiety_wskazniki[0] = 0;
  ilosc_id_ety = malloc(sizeof(int));
  *ilosc_id_ety = 0;
  definicje_wskazniki[0] = 0;
  ilosc_id_def = malloc(sizeof(int));
  *ilosc_id_def = 0;
  ilosc_elementow = malloc(sizeof(int));
  *ilosc_elementow = 0;
  ilosc_wejscia = malloc(sizeof(int));
  *ilosc_wejscia = 0;
  aktualny_element = malloc(sizeof(int));
  *aktualny_element = 0;
  stos_wskaznik = malloc(sizeof(int));
  *stos_wskaznik = 0;
  wejscie_wskaznik = malloc(sizeof(int));
  *wejscie_wskaznik = 0;
  inicjalizuj_pamiec();
}

//Zwalnia wszystkie alokowane wskaźniki
void zwolnij(){
  free(ilosc_id_ety);
  free(ilosc_id_def);
  free(ilosc_elementow);
  free(ilosc_wejscia);
  free(aktualny_element);
  free(stos_wskaznik);
}

/*DYNAMICZNA TABLICA
Struktura to char[] trzymająca wszystkie nazwy po kolei i int[]
trzymająca wskaźniki na kolejne nazwy. Ta struktura jest użyta do zapisywania
nazw etykiet i definicji, poniżej funkcje.
*/

//Dodaje kolejne "słowo" do tablicy
void dyntab_dodaj(char dyntab_id[], int dyntab_pointer[], char identyfikator[], int* ilosc_id){
  int i = 0, wolne_miejsce = dyntab_pointer[*ilosc_id];

  while(identyfikator[i] != '\0'){
    dyntab_id[wolne_miejsce + i] = identyfikator[i]; //zapisuje słowo począwszy od pierwszego wolnego indeksu
    i++;
  }

  (*ilosc_id)++; //zwieksza licznik identyfikatorów
  dyntab_pointer[*ilosc_id] = wolne_miejsce + i; //zapisuje polozenie nowego identyfikatora
}

//Szuka słowa w tablicy, zwraca indeks lub -1 jeśli nie znajdzie
int wyszukaj_identyfikator(char dyntab_id[], int dyntab_pointer[], char identyfikator[], int* ilosc_id){
  int i = 0, j = 0, dlugosc_w_dyntab = 0, wynik = -1;
  while(i < *ilosc_id && wynik == -1){
    j = 0;
    dlugosc_w_dyntab = dyntab_pointer[i + 1] - dyntab_pointer[i];
    while(j < dlugosc_w_dyntab && identyfikator[j] != '\0' && identyfikator[j] == dyntab_id[ dyntab_pointer[i] + j ] && wynik == -1){
      j++;
    }
    if (j == dlugosc_w_dyntab && identyfikator[j] == '\0')
      wynik = i;
    i++;
  }
  return wynik;
}

//Zwraca "słowo" o danym indeksie
void get_identyfikator(char dyntab_id[], int dyntab_pointer[], char identyfikator[], int i){
  int dlugosc = dyntab_pointer[i+1] - dyntab_pointer[i];
  for(int j = 0; j < dlugosc; j++){
    identyfikator[j] = dyntab_id[dyntab_pointer[i] + j];
  }
}

//Poniżej funkcje "tłumaczące" funkcje dynamicznej tablicy dla  etykiet i definicji, dla wygody
int wyszukaj_etykiete(char identyfikator[]){
  return wyszukaj_identyfikator(etykiety_identyfikatory, etykiety_wskazniki, identyfikator, ilosc_id_ety);
}

void dodaj_etykiete(char identyfikator[]){
  if(wyszukaj_etykiete(identyfikator) == -1)
      dyntab_dodaj(etykiety_identyfikatory, etykiety_wskazniki, identyfikator, ilosc_id_ety);
}

void get_identyfikator_ety(char identyfikator[], int i){
  get_identyfikator(etykiety_identyfikatory, etykiety_wskazniki, identyfikator, i);
}

int wyszukaj_definicje(char identyfikator[]){
  return wyszukaj_identyfikator(definicje_identyfikatory, definicje_wskazniki, identyfikator, ilosc_id_def);
}

//Ta funkcja jest mniej trywialna, zapisuje dodatkowo w definicje_miejsca adres definicji
void dodaj_definicje(char identyfikator[], int adres){
  definicje_miejsca[*ilosc_id_def] = adres;
  dyntab_dodaj(definicje_identyfikatory, definicje_wskazniki, identyfikator, ilosc_id_def);
}

void get_identyfikator_def(char identyfikator[], int i){
  get_identyfikator(definicje_identyfikatory, definicje_wskazniki, identyfikator, i);
}

//Funkcja parująca indeksy etykiet z adresami definicji (do tablicy definicje_etykiet)
void przyporzadkuj_definicje(){
  char etykieta[MAX_DLUGOSC_ETYKIETY + 1] = "";
  int i_definicji, adres_definicji;
  for(int i = 0; i < *ilosc_id_ety; i++){
    for(int i = 0; i <= MAX_DLUGOSC_ETYKIETY; i++){
      etykieta[i] = 0;
    }
    get_identyfikator_ety(etykieta, i);
    i_definicji = wyszukaj_definicje(etykieta);
    adres_definicji = definicje_miejsca[i_definicji];
    definicje_etykiet[i] = adres_definicji;

  }
}

/*FUNKCJE DO ZCZYTANIA KODU*/

//Zwraca true, jeśli @c jest separatorem, w p. p. false. Można zmodyfikować by zmienić separatory;
bool separator(char c){
  return c == ' ' || c == '\n' || c == '|' || c == '\t';
}

/*
Zczytuje na bufor (bufor1 lub bufor2) najmniejszą jednostkę programu
Zwraca false jeśli zczytał słowo, które może wskazywać na instrukcję "jedno-słowową",
czyli wywołanie bądź powrót.
W p. p. true
*/
bool zczytaj_na_bufor(char bufor[]){
  char c;
  bool element_pojedynczy = false;
  int i = 0;

  for(int i = 0; i < MAX_DLUGOSC_ETYKIETY; i++){ //czyszczenie bufora
    bufor[i] ='\0';
  }
  do{
    c = getchar();
  }
  while(separator(c));
  if(c == ':' || c == '^' || c == ';'){ //słowo to symbol
    bufor[0] = c;
    if (c == ';') element_pojedynczy = true;
  } else if (isdigit(c) || c == '-' || c == '+') { //słowo to adres
    while (isdigit(c) || c == '-' || c == '+'){
      bufor[i] = c;
      c = getchar();
      i++;
    }
    ungetc(c, stdin);
  } else if (isalpha(c) || c == '_') { //słowo to identyfikator
    while (isdigit(c) || isalpha(c) || c == '_'){
      bufor[i] = c;
      c = getchar();
      i++;
    }
    element_pojedynczy = true; //identyfikator może być wywołaniem
    ungetc(c, stdin);
  } else {
    ungetc(c, stdin); //dla dobrych danych to się wykonuje jeśli zczytamy "&"
  }
  return element_pojedynczy;
}

/*
Zczytuje cały element programu na bufory,
rozpoznając czy jest on jedno, czy dwu-słowowy
*/
void zczytaj_element(){
  bool pojedynczy = false;
  pojedynczy = zczytaj_na_bufor(bufor1);
  if (!pojedynczy){
    zczytaj_na_bufor(bufor2);
  } else { //po każdym zczytaniu trzeba się upewnić że bufory są czyste
    for(int i = 0; i < MAX_DLUGOSC_ETYKIETY; i++){
      bufor2[i] ='\0';
    }
  }
}

/*
Zwraca informację o tym, czy dany bufor zawiera identyfikator, adres,
lub zwraca konkretny symbol jeśli bufor zawiera tylko jeden symbol (:, ;, ^).
*/
char zawartosc_bufora(char bufor[]){
  char c = bufor[0], wynik = '\0';
  if(c == '-' || c == '+' || isdigit(c)){
    wynik = 'n';//
  } else if(isalpha(c)) {
    wynik = 'c';
  } else if(c == ';' || c == ':' || c == '^'){
    wynik = c;
  }
  return wynik;
}


//Zamienia tablicę char na inta
//(Wiem, że jest taka funkcja w bibliotece standardowej, ale pisanie tego było pouczające)
int char_to_int(char liczba[]){
  int znak = 1, start = 0, j = 0, wynik = 0, mnoznik = 1;
  if(liczba[0] == '-'){
    znak = -1;
    start++;
    j++;
  } else if(liczba[0] == '+'){
    start++;
    j++;
  }
  while (isdigit(liczba[j])){
    j++;
  }
  j--;
  while(j >= start){
    wynik += mnoznik * (liczba[j] - '0');
    mnoznik = mnoznik * 10;
    j--;
  }
  wynik = wynik * znak;

  return wynik;
}

//W zależności od zawartości bufora zapisuje element (char[]) i dane dla niego
void zapisz_element(){
  char e1, e2;
  int i_etykiety;
  zczytaj_element();
  e1 = zawartosc_bufora(bufor1);
  e2 = zawartosc_bufora(bufor2);
  if(e1 == 'n' && e2 == 'n'){ //ODEJMOWANIE
    zapisz_dane(char_to_int(bufor1), char_to_int(bufor2));
    element[*ilosc_elementow] = 'o';
  } else if (e1 == 'n' && e2 == 'c'){ //SKOK
    i_etykiety = wyszukaj_etykiete(bufor2);
    if(i_etykiety == -1){
      zapisz_dane(char_to_int(bufor1), *ilosc_id_ety);
      dodaj_etykiete(bufor2);
    } else {
      zapisz_dane(char_to_int(bufor1), i_etykiety);
    }
    element[*ilosc_elementow] = 's';
  } else if (e1 == 'c' && e2 == '\0'){ //WYWOŁANIE
    i_etykiety = wyszukaj_etykiete(bufor1);
    if(i_etykiety == -1){
      zapisz_dane(0, *ilosc_id_ety);
      dodaj_etykiete(bufor1);
    } else {
      zapisz_dane(0, i_etykiety);
    }
    element[*ilosc_elementow] = 'w';
  } else if (e1 == ';' && e2 == '\0'){ //POWRÓT
    zapisz_dane(0, 0);
    element[*ilosc_elementow] = ';';
  } else if (e1 == '^' && e2 == 'n'){ //CZYTANIE
    zapisz_dane(0, char_to_int(bufor2));
    element[*ilosc_elementow] = 'c';
  } else if (e1 == 'n' && e2 == '^'){ //PISANIE
    zapisz_dane(char_to_int(bufor1), 0);
    element[*ilosc_elementow] = 'p';
  } else if (e1 == ':' && e2 == 'c'){ //DEFNICJA
    zapisz_dane(0, *ilosc_id_def);
    dodaj_definicje(bufor2, *ilosc_elementow);
    element[*ilosc_elementow] = 'd';
  } else {
    (*ilosc_elementow)--; //jeśli nie ma żadnego legalnego elementu na buforze to trzeba "anulować następną operację ++
  }
  (*ilosc_elementow)++;
}

//Wczytuje wejście dla programu (&..)
void wczytaj_wejscie(){
  char c;
  c = getchar();
  while(c != EOF){
    wejscie[*ilosc_wejscia] = c;
    (*ilosc_wejscia)++;
    c = getchar();
  }
  wejscie[*ilosc_wejscia] = c;
  (*ilosc_wejscia)++;
}

//Wczytuje wszystkie elementy, a potem wejście jeśli istnieje
void wczytaj(){
  char c;
  c = getchar();
  while(c != '&' && c != EOF){
    while(separator(c))
      c = getchar();
    ungetc(c, stdin);
    //Założenie: bufor jest po wczytaniu ostatniego separatora
    zapisz_element();
    //Założenie: bufor jest w stanie po wczytaniu ostatniego znaku do bufora
    c = getchar();
  }
  if(c == '&'){
    wczytaj_wejscie();
  }
}

/*WYKONYWANIE
Po wczytaniu wszystkich elementów można zacząć wykonywać instrukcje
Działanie funkcji jak w treści zadania
*/

void odejmij(int adres1, int adres2){
  int wynik = pamiec_daj(pamiec_daj(adres1)) - pamiec_daj(pamiec_daj(adres2));
  zapisz_w_pamieci(pamiec_daj(adres1), wynik);
}

void skok(int adres, int etykieta){
  if (pamiec_daj(pamiec_daj(adres)) > 0){
    *aktualny_element = definicje_etykiet[etykieta];
  }
}

void wywolanie(int etykieta){
  stos_push(*aktualny_element);
  *aktualny_element = definicje_etykiet[etykieta];
}

//W przypadku pustego stosu ustawia wskaznik na koncu programu co kończy jego działanie
void powrot(){
  if(!stos_pusty()){
    *aktualny_element = stos_pop();
  } else {
    *aktualny_element = *ilosc_elementow;
  }
}

void czytanie(int adres){
  char c = wejscie[*wejscie_wskaznik];
  if(c == EOF){
    zapisz_w_pamieci(pamiec_daj(adres), -1);
  } else {
    zapisz_w_pamieci(pamiec_daj(adres), c);
  }
  (*wejscie_wskaznik)++;
}

void pisanie(int adres){
  int c = pamiec_daj(pamiec_daj(adres));
  putchar(c);
}

/*
W zależności od tego co znajduje się pod element[*aktualny_element]
wykonuje odpowiednią instrukcję dla danych pod tym samym adresem. Pomija definicje.
*/
void wykonaj_instrukcje(){
  char instrukcja;
  int d1, d2;
  instrukcja = element[*aktualny_element];
  d1 = daj_dane(*aktualny_element, 0);
  d2 = daj_dane(*aktualny_element, 1);
  switch (instrukcja){
    case 'o':
      odejmij(d1, d2);
      break;
    case 's':
      skok(d1, d2);
      break;
    case 'w':
      wywolanie(d2);
      break;
    case ';':
      powrot();
      break;
    case 'c':
      czytanie(d2);
      break;
    case 'p':
      pisanie(d1);
      break;
    case 'd':
      break;
  }
  (*aktualny_element)++;
}

/*
Wykonuje program dopóki wskaznik jest mniejszy od całkowitej liczby elementów programu
*/
void wykonaj_program(){
  while(*aktualny_element < *ilosc_elementow){
    wykonaj_instrukcje();
  }
}

int main(){
  inicjalizuj();
  wczytaj();
  przyporzadkuj_definicje();
  wykonaj_program();
  zwolnij();
  return 0;
}
