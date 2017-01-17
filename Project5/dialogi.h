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
void dialogi(ALLEGRO_FONT *czcionka, int rodzaj_npc, int* nr_odp, int *rozmowa, int *zad, float czas, bool *wziecie_czasu, bool *wybor, int *ilosc_opcji, int *drugi_wybor, int numer_mapy, int *przedmioty_zalozone, int przedmioty_w_ekwipunku[][3]);
void zamiana_itemu_w_ekwipunku(int *przedmioty_zaolozone, int przedmioty_w_ekwipunku[][3], int z_czego, int na_co, int typ_itemu);
void eventy(int ktory_event, int poz_gracza_x, int poz_gracza_y, int *przedmioty_zaolozone, int przedmioty_w_ekwipunku[][3]);
bool sprawdzenie_czy_przedmiot_jest_w_ekwipunku(int*przedmioty_zalozone, int przedmioty_w_ekwipunku[][3], int czego_szukamy, int typ_itemu);
void zabranie_przedmioty_z_ekwipunku(int*przedmioty_zalozone, int przedmioty_w_ekwipunku[][3], int czego_szukamy, int typ_itemu);
bool sprawdzenie_czy_sa_przeciwnicy_na_aktualnej_mapie(wlasciwosci_pola_t** mapa, int wysokosc, int szerokosc, int ktorego_wroga_szukamy);
bool sprawdzenie_czy_sa_przeciwnicy_na_innej_mapie(char* nazwa_mapy, int ktorego_wroga_szukamy);
void zapis_pliku_leafe_z_pliku_do_pliku(char*nazwa_wejscia, char*nazwa_wyjscia);
#endif // !dialogi.h
