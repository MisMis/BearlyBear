#pragma once
#ifndef dialogi_h
#define dialogi_h
void dialogi(ALLEGRO_FONT *czcionka, int rodzaj_npc, int* nr_odp, int *rozmowa, int *zad, float czas, bool *wziecie_czasu, bool *wybor, int *ilosc_opcji, int *drugi_wybor, int numer_mapy, int *przedmioty_zalozone, int przedmioty_w_ekwipunku[][3]);
void zamiana_itemu_w_ekwipunku(int *przedmioty_zaolozone, int przedmioty_w_ekwipunku[][3], int z_czego, int na_co, int typ_itemu);
void eventy(int ktory_event, int poz_gracza_x, int poz_gracza_y, int *przedmioty_zaolozone, int przedmioty_w_ekwipunku[][3]);
bool sprawdzenie_czy_przedmiot_jest_w_ekwipunku(int*przedmioty_zalozone, int przedmioty_w_ekwipunku[][3], int czego_szukamy, int typ_itemu);
void zabranie_przedmioty_z_ekwipunku(int*przedmioty_zalozone, int przedmioty_w_ekwipunku[][3], int czego_szukamy, int typ_itemu);
#endif // !dialogi.h
