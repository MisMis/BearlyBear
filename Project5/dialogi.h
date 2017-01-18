#pragma once
#ifndef dialogi_h
#define dialogi_h
struct wlasciwosci_pola {
	bool przeszkoda;
	int  npc;
	int  przejscie_mapy;
	int npc_id;
	int przeciwnik;
	int przeciwnik_id;
	int przedmiot;
	int zdarzenia;
	bool przyszle_poz_przeciwnikow;
	bool przyszle_poz_npc;
};
typedef struct wlasciwosci_pola wlasciwosci_pola_t;
void dialogi(wlasciwosci_pola_t**dane,ALLEGRO_FONT *czcionka, int rodzaj_npc, int* nr_odp, int *rozmowa, int *zad, float czas, bool *wziecie_czasu, bool *wybor, int *ilosc_opcji, int *drugi_wybor, int numer_mapy, int *przedmioty_zalozone, int przedmioty_w_ekwipunku[][3], bool *zmianamapy, int*stary_numer_bitmapy, char***nazwy_plikow,bool*stara_pozycja);
void zamiana_itemu_w_ekwipunku(int *przedmioty_zaolozone, int przedmioty_w_ekwipunku[][3], int z_czego, int na_co, int typ_itemu);
void eventy(int ktory_event, int poz_gracza_x, int poz_gracza_y, int *przedmioty_zaolozone, int przedmioty_w_ekwipunku[][3]);
bool sprawdzenie_czy_przedmiot_jest_w_ekwipunku(int*przedmioty_zalozone, int przedmioty_w_ekwipunku[][3], int czego_szukamy, int typ_itemu);
void zabranie_przedmioty_z_ekwipunku(int*przedmioty_zalozone, int przedmioty_w_ekwipunku[][3], int czego_szukamy, int typ_itemu);
bool sprawdzenie_czy_sa_przeciwnicy_na_aktualnej_mapie(wlasciwosci_pola_t** mapa, int wysokosc, int szerokosc, int ktorego_wroga_szukamy);
bool sprawdzenie_czy_sa_przeciwnicy_na_innej_mapie(char* nazwa_mapy, int ktorego_wroga_szukamy);
void dodanie_npc_do_innej_mapy(char*** nazwy_plikow, int na_poz_x, int na_poz_y, int ktory_npc, int na_ktora_mape);
void zapis_pliku_leafe_z_pliku_do_pliku(char*nazwa_wejscia, char*nazwa_wyjscia);
void zapis_wszystkich_plikow(char*** nazwy_plikow, int ilosc_plikow, int pliki_wejscia, int pliki_wyjscia);
void menu(ALLEGRO_FONT *czczionka, ALLEGRO_COLOR *kolory, int wybor, ALLEGRO_BITMAP*grafika, bool jest_gra);
void wybor_z_menu(int pole_wybrane, bool *zakonczenie, bool *gra, char*** nazwy_plikow, int ilosc_plikow, bool *jest_gra, bool *jest_pozycja, int przedmioty_w_ekwipunku[][3], int *przedmioty_zalozone, int *zdrowie, int *nr_zad, int *numerbitmapy, int *stary_numer_bitmapy, int *pozycja_kontunulowaniax, int *pozycja_kontunulowaniay);
void zapis_do_pliku(FILE *wyjscie, wlasciwosci_pola_t **dane, int wysokosc, int szerokosc, int xspawnu, int yspawnu);
void zapis_danych_poczatkowych(int przedmioty_w_ekwipunku[][3], int *przedmioty_zalozone, int zdrowie, int nr_zad, int numerbitmapy, int stary_numer_bitmapy, int pozycja_kontunulowaniax, int pozycja_kontunulowaniay, bool *jest_pozycja);
void wczytanie_danych_poczatkowych(int przedmioty_w_ekwipunku[][3], int *przedmioty_zalozone, int *zdrowie, int *nr_zad, int *numerbitmapy, int *stary_numer_bitmapy, int *pozycja_kontunulowaniax, int *pozycja_kontunulowaniay);
void wczytanie_z_pliku(FILE *wyjscie, wlasciwosci_pola_t **dane, int wysokosc, int szerokosc);
#endif // !dialogi.h
