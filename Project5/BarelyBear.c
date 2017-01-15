#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "dialogi.h"
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
struct dane_npc {
	bool rozmawia;
	int npc_id;
	int pozx;
	int pozy;
	int poz_docelowa_x;
	int poz_docelowa_y;
	int maxx;
	int maxy;
	int minx;
	int miny;
	clock_t czas;
	int skok_miedzy_zmianami;
	int kierunek;
	int ostatni_kierunek;
};
struct dane_przeciwnikow {
	int nr_przeciwnika;
	int poz_x;
	int poz_y;
	int poz_docelowa_x;
	int poz_docelowa_y;
	int skok_miedzy_zmianami;
	int kierunek;
	int ostatni_kierunek;
	int zdrowie;
	clock_t czas_ataku;
};
struct przeciwnicy_statystyki {
	int atak;
	int obrona;
	int predkosc;
};
struct przedmiot {
	int typ;
	int atak;
	int obrona;
	char nazwa[20];
	char opis[50];
};
struct typy_przedmiotow {
	char nazwa[20];
	int x;
	int y;
};
struct dane_do_alg_A
{
	bool czy_rozpatrzono;
	bool jest_na_liscie;
	int koszt;
	int heu;
	int xmatki;
	int ymatki;
};
struct lista
{
	int x;
	int y;
	struct lista *adres;
	struct lista *adres_poprzedniego;
};
struct wspolrzedne_pola
{
	int x;
	int y;
};
typedef struct dane_do_alg_A dane_do_A;
typedef struct lista s_lista;
typedef struct wspolrzedne_pola s_wspolrzedne_pola;
typedef struct wlasciwosci_pola wlasciwosci_pola_t;
typedef struct dane_npc dane_npc_t;
typedef struct przedmiot przedmiot_t;
typedef struct typy_przedmiotow typy_przedmiotow_t;
typedef struct dane_przeciwnikow dane_przeciwnikow_t;
typedef struct przeciwnicy_statystyki przeciwnicy_statystyki_t;

void czytanie_nazw_npc(char **nazwy);
void czytanie_nazw_przeciwnikow(char **nazwy);
void czytanie_nazw(char ***nazwy);
void ladowanie_opisow_przedmiotow(przedmiot_t *tablica);
void ladowanie_typow_przedmiotow(typy_przedmiotow_t *tablica);
void rysowanie_postaci_w_ruchu(ALLEGRO_BITMAP *obraz, int kierunek, int pix, int poz_x, int poz_y, int ostatni_kierunek);
void wczytanie_z_pliku(FILE *wyjscie, wlasciwosci_pola_t **dane, int wysokosc, int szerokosc);
bool dodaj_przedmiot_do_ekwipunku(int ekwipunek[][3], int id_przedmiotu);
bool kolizja(int poz_x, int poz_y, wlasciwosci_pola_t **struktura_mapy);
int sprawdzenie_do_rozmowy(wlasciwosci_pola_t **dane, int poz_x, int poz_y, int wysokosc_mapy, int szerokosc_mapy, dane_npc_t *dane_do_npc);
void zalozenie_przedmiotu(int* zalozone_przedmioty, int przedmioty_w_ekwipunku[][3], przedmiot_t *tablica, int pozx, int pozy);
void zerowanie_klawiszy_ruchu(bool*klawisze);
void wyczyszczenie_listy_do_alg_A(s_lista**adres_pierwszy, s_lista**adres_ostatni);
s_wspolrzedne_pola pobierz_najmniejszy(s_lista **adres_pierwszy, s_lista **adres_ostatniego, dane_do_A **dane);
s_wspolrzedne_pola wytyczenie_trasy_dla_przeciwnikow(wlasciwosci_pola_t **mapadane, s_wspolrzedne_pola pozycja, int szerokosc, int wysokosc, s_wspolrzedne_pola poz_gracza);
void dodaj_do_listy(s_lista **adres_pierwszy, s_lista**adres_ostatni, int x, int y);
bool poza_mapa(int wysokosc, int szerokosc, int poz_x, int poz_y);
bool czy_jest_obok(wlasciwosci_pola_t **mapa, s_wspolrzedne_pola poz_gracza, int wysokosc, int szerokosc, int nr_przeciwnika);
void ladowanie_statystyk_przeciwnikow(przeciwnicy_statystyki_t*statystyki);
void atak(wlasciwosci_pola_t**mapadane, int ostatni_kierunek, int poz_x, int poz_y, dane_przeciwnikow_t *przeciwnicy, int *ekwipunek, przeciwnicy_statystyki_t *statystyki_przeciwnikow, przedmiot_t *stat_przedmiotow);


int main() {
	int ilosc_map = 29;
	int mapax = 0;
	int mapay = 0;
	int tmp;
	int predkosc_bohatera = 5;
	int przerwa_miedzy_przejsciami_npc=3;
	int zasiegnpc = 3;
	int bohaterx = 350;
	int bohatery = 350;
	int x_ramki = 0, y_ramki = 0;
	int misjednostka = 50;
	int pozdocelowa = 350;
	int xdocelowe = mapax;
	int ydocelowe = mapay;
	int wysokosc, szerokosc,i,j,x,y;
	int aktualna_pozycjax;
	int aktualna_pozycjay;
	int przedmioty_w_ekwipunku[5][3];
	int przedmioty_zalozone[6];
	int zdrowie = 100;
	int nr_zad = 1,nr_odp;
	int ilosc_npc=14, ilosc_rysowanych_npc, ilosc_wrogow = 3, ilosc_rysowanych_wrogow;
	int aktualna_poz_npc_x, aktualna_poz_npc_y;
	int numerbitmapy=0;
	int xspawnu, yspawnu,xstarejpoz=0,ystarejpoz=0;
	int do_wyboru_dialogu;
	int ostatni_kier_bohatera=1,kierunek=5;
	int skok_zamiany_bitmapy_w_ruchu=0;
	int ilosc_opcji, drugi_wybor = 0;
	przeciwnicy_statystyki_t statystyki_przec[3];
	s_wspolrzedne_pola pomoc, aktualna_poz_przeciwnika, poz_gracza;
	char ***nazwy_plikow;
	char **nazwy_npc;
	char **nazwy_przeciwnikow;
	int rozmowa,id,ktory_rozmawia;
	float czas_aktualny;
	clock_t czas_startowy;
	int xszukania, yszukania, szerszukania=16;
	bool zmianamapy=false;
	bool czy_ekwipunek = false;
	bool przemieszczenie = false;
	bool ruch =true;
	bool wziecie_czasu=true;
	bool wybor = false;
	al_init();
	ALLEGRO_DISPLAY *okno = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_EVENT_QUEUE *kolejka = NULL;
	ALLEGRO_BITMAP *bohater = NULL;
	ALLEGRO_BITMAP *ekwipunek = NULL;
	ALLEGRO_BITMAP *serce = NULL;
	ALLEGRO_BITMAP *przedmioty = NULL;
	ALLEGRO_BITMAP *ramka_wyboru = NULL;
	ALLEGRO_BITMAP *ramka_do_dialogow = NULL;
	ALLEGRO_BITMAP *ramka_wyboru_do_dialogow = NULL;
	ALLEGRO_BITMAP *glowy = NULL;
	wlasciwosci_pola_t** mapadane = NULL;
	dane_npc_t *dane_do_ruchu_npc;
	dane_przeciwnikow_t *dane_przeciwnikow_wartosci= NULL;
	FILE *dane_do_mapy;
	ALLEGRO_FONT *czcionka_48;
	ALLEGRO_FONT *czcionka_18;
	ALLEGRO_FONT *czcionka_24;
	ALLEGRO_FONT *czcionka_18_cienka;
	ALLEGRO_BITMAP **wrogowie = NULL;
	ALLEGRO_BITMAP **NPC = NULL;
	przedmiot_t opisy_przedmiotow[7];
	typy_przedmiotow_t typy_przedmiotow_tab[6];
	bool przyciski[4] = { false,false,false,false };
	okno = al_create_display(750, 750);
	al_init_image_addon();
	al_init_font_addon();
	al_install_keyboard();
	al_init_ttf_addon();
	ladowanie_opisow_przedmiotow(opisy_przedmiotow);
	ladowanie_typow_przedmiotow(typy_przedmiotow_tab);
	kolejka = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);
	al_register_event_source(kolejka, al_get_timer_event_source(timer));
	al_register_event_source(kolejka, al_get_keyboard_event_source());
	bool zakonczenie=false;
	//wczytywanie niezmiennych bitmap
	bohater = al_load_bitmap("data/postacie/bohater.png");
	ekwipunek = al_load_bitmap("data/inne/ekwipunek.png");
	serce = al_load_bitmap("data/inne/serce.png");
	przedmioty = al_load_bitmap("data/inne/przedmioty.png");
	czcionka_48 = al_load_ttf_font("data/czcionka/kleptocracy titling bd.ttf", 48, 0);
	czcionka_18 = al_load_ttf_font("data/czcionka/kleptocracy titling rg.ttf", 18, 0);
	czcionka_18_cienka =al_load_ttf_font("data/czcionka/kleptocracy titling lt.ttf", 18, 0);
	czcionka_24 = al_load_ttf_font("data/czcionka/kleptocracy titling rg.ttf", 24, 0);
	ramka_wyboru = al_load_bitmap("data/inne/ramka_wyboru.png");
	glowy = al_load_bitmap("data/inne/glowy.png");
	ramka_do_dialogow = al_load_bitmap("data/inne/okno dialogowe.png");
	ramka_wyboru_do_dialogow = al_load_bitmap("data/inne/podkreslenie_tekstu.png");
	//zerowanie tablicy z przedmoitami;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 3; j++) {
			przedmioty_w_ekwipunku[i][j] = 0;
		}
	}
	//zerowanie przedmoitow zalozonych
	for (i = 0; i < 6; i++) {
		przedmioty_zalozone[i] = 0;
	}
	ladowanie_statystyk_przeciwnikow(statystyki_przec);

	if (ilosc_npc != 0) {
		NPC = (ALLEGRO_BITMAP**)malloc(ilosc_npc * sizeof(ALLEGRO_BITMAP*));
		nazwy_npc = (char**)malloc(ilosc_npc * sizeof(char*));
	}
	if (ilosc_wrogow != 0) {
		wrogowie = (ALLEGRO_BITMAP**)malloc(ilosc_wrogow * sizeof(ALLEGRO_BITMAP*));
		nazwy_przeciwnikow = (char**)malloc(ilosc_wrogow * sizeof(char*));
	}
	if (ilosc_map != 0) {
		nazwy_plikow = (char***)malloc(ilosc_map * sizeof(char**));
		for (i = 0; i < ilosc_map; i++) {
			nazwy_plikow[i] = (char**)malloc(2 * sizeof(char*));
			}
	}
	czytanie_nazw(nazwy_plikow);
	czytanie_nazw_npc(nazwy_npc);
	czytanie_nazw_przeciwnikow(nazwy_przeciwnikow);
	for (i = 0; i < ilosc_npc; i++) {
		NPC[i] = al_load_bitmap(nazwy_npc[i]);
	}
	for (i = 0; i < ilosc_wrogow; i++) {
		wrogowie[i] = al_load_bitmap(nazwy_przeciwnikow[i]);
	}
	while (!zakonczenie) {
		ALLEGRO_BITMAP *mapa = NULL;
		zmianamapy = false;
		fopen_s(&dane_do_mapy, nazwy_plikow[numerbitmapy][1], "r");
		fscanf_s(dane_do_mapy, "%i", &wysokosc);
		fscanf_s(dane_do_mapy, "%i", &szerokosc);
		fscanf_s(dane_do_mapy, "%i", &xspawnu);
		fscanf_s(dane_do_mapy, "%i", &yspawnu);
		//zmiana miejsc pojawiania siê potrzebna do przejsc miedzy mapami
		if (numerbitmapy == 0) {
			mapax = xstarejpoz*misjednostka*-1;
			mapay = ystarejpoz*misjednostka*-1;
		}
		else {
			mapax = xspawnu*misjednostka*-1;
			mapay = yspawnu*misjednostka*-1;
		}
		//ustawienie miejsc docelowych na obecna pozycje bohatera na mapie
		xdocelowe = mapax;
		ydocelowe = mapay;
		mapadane = (wlasciwosci_pola_t **)malloc(wysokosc * sizeof(wlasciwosci_pola_t*));
		for (i = 0; i < wysokosc; i++) {
			mapadane[i] = (wlasciwosci_pola_t*)malloc(szerokosc * sizeof(wlasciwosci_pola_t));
		}
		for (i = 0; i < wysokosc; i++) {
			for (j = 0; j < szerokosc; j++) {
				mapadane[i][j].przeciwnik = 0;
			}
		}
		wczytanie_z_pliku(dane_do_mapy, mapadane, wysokosc, szerokosc);
		mapadane[5][6].przeciwnik = 3;
		ilosc_rysowanych_npc = 0;
		//zliczenie ilosci rysowanych npc
		for (i = 0; i < wysokosc; i++) {
			for (j = 0; j < szerokosc; j++) {
				if (mapadane[i][j].npc != 0) {
					ilosc_rysowanych_npc++;
				}
			}
		}
		//zliczenie ilosci rysowanych wrogow
		ilosc_rysowanych_wrogow = 0;
		for (i = 0; i < wysokosc; i++) {
			for (j = 0; j < szerokosc; j++) {
				if (mapadane[i][j].przeciwnik != 0) {
					ilosc_rysowanych_wrogow++;
				}
			}
		}
		//deklaracja pamieci na zmienne przeciwnikow
		dane_przeciwnikow_wartosci = (dane_przeciwnikow_t*)malloc(ilosc_rysowanych_wrogow * sizeof(dane_przeciwnikow_t));
		//deklaracja pamieci na zmienne portzebne do przemieszczanie npc
		dane_do_ruchu_npc = (dane_npc_t*)malloc(ilosc_rysowanych_npc * sizeof(dane_npc_t));
		//zerowanie npc.id na mapie i wyliczenie danych potrzebnych do ruchu
		x = 1;
		y = 1;
		for (i = 0; i < wysokosc; i++) {
			for (j = 0; j < szerokosc; j++) {
				mapadane[i][j].przyszle_poz_npc = false; //zerowanie przyszlych poz npc potrzebnych do kolizji z npc;
				mapadane[i][j].przyszle_poz_przeciwnikow = false;
				mapadane[i][j].przedmiot = 0;
				if (mapadane[i][j].npc == 0) {
					mapadane[i][j].npc_id = 0;
				}
				else
				{
					mapadane[i][j].npc_id = x;
					x++;
					tmp = x - 2;//arg
					dane_do_ruchu_npc[tmp].rozmawia = false;
					dane_do_ruchu_npc[tmp].pozx = j * misjednostka*-1;
					dane_do_ruchu_npc[tmp].pozy = i * misjednostka*-1;
					dane_do_ruchu_npc[tmp].poz_docelowa_x = dane_do_ruchu_npc[tmp].pozx;
					dane_do_ruchu_npc[tmp].poz_docelowa_y = dane_do_ruchu_npc[tmp].pozy;
					dane_do_ruchu_npc[tmp].maxx = j + zasiegnpc;
					dane_do_ruchu_npc[tmp].minx = j - zasiegnpc;
					dane_do_ruchu_npc[tmp].miny = i - zasiegnpc;
					dane_do_ruchu_npc[tmp].maxy = i + zasiegnpc;
					dane_do_ruchu_npc[tmp].npc_id = mapadane[i][j].npc - 1;
					dane_do_ruchu_npc[tmp].czas = clock();
					dane_do_ruchu_npc[tmp].ostatni_kierunek = 1;
					dane_do_ruchu_npc[tmp].kierunek = 5;
					dane_do_ruchu_npc[tmp].skok_miedzy_zmianami = 0;
				}
				if (mapadane[i][j].przeciwnik == 0) {
					mapadane[i][j].przeciwnik_id = 0;
				}
				else {
					mapadane[i][j].przeciwnik_id = y;
					y++;
					tmp = y - 2;
					dane_przeciwnikow_wartosci[tmp].nr_przeciwnika = mapadane[i][j].przeciwnik;
					dane_przeciwnikow_wartosci[tmp].poz_x = j*misjednostka*-1;
					dane_przeciwnikow_wartosci[tmp].poz_y = i*misjednostka*-1;
					dane_przeciwnikow_wartosci[tmp].poz_docelowa_x = dane_przeciwnikow_wartosci[tmp].poz_x;
					dane_przeciwnikow_wartosci[tmp].poz_docelowa_y = dane_przeciwnikow_wartosci[tmp].poz_y;
					dane_przeciwnikow_wartosci[tmp].ostatni_kierunek = 1;
					dane_przeciwnikow_wartosci[tmp].kierunek = 5;
					dane_przeciwnikow_wartosci[tmp].zdrowie = 100;
					dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami = 0;
					dane_przeciwnikow_wartosci[tmp].czas_ataku = clock();
				}

			}
		}
		mapadane[6][6].przedmiot = 3;
		rozmowa = 0;
		mapa = al_load_bitmap(nazwy_plikow[numerbitmapy][0]);
		if (numerbitmapy == 0) {
			mapadane[12][36].przejscie_mapy = 2;
		}
		al_start_timer(timer);
		srand(time(NULL));
		while (!zmianamapy) {
			ALLEGRO_EVENT zdarzenie;
			al_wait_for_event(kolejka, &zdarzenie);
			aktualna_pozycjax = (mapax - bohaterx)*-1 / misjednostka;
			aktualna_pozycjay = (mapay - bohatery)*-1 / misjednostka;
				if (zdarzenie.type == ALLEGRO_EVENT_KEY_DOWN)
				{
						switch (zdarzenie.keyboard.keycode)
						{
						case ALLEGRO_KEY_UP:
							zerowanie_klawiszy_ruchu(przyciski);
							przyciski[0] = true;
							break;
						case ALLEGRO_KEY_DOWN:
							zerowanie_klawiszy_ruchu(przyciski);
							przyciski[1] = true;
							break;
						case ALLEGRO_KEY_LEFT:
							zerowanie_klawiszy_ruchu(przyciski);
							przyciski[2] = true;
							break;
						case ALLEGRO_KEY_RIGHT:
							zerowanie_klawiszy_ruchu(przyciski);
							przyciski[3] = true;
							break;
						}
				}
				if (zdarzenie.type == ALLEGRO_EVENT_KEY_UP)
				{
					switch (zdarzenie.keyboard.keycode)
					{
					case ALLEGRO_KEY_UP:
						przyciski[0] = false;
						break;
					case ALLEGRO_KEY_DOWN:
						przyciski[1] = false;
						break;
					case ALLEGRO_KEY_LEFT:
						przyciski[2] = false;
						break;
					case ALLEGRO_KEY_RIGHT:
						przyciski[3] = false;
						break;
					case ALLEGRO_KEY_ESCAPE:
						zakonczenie = true;
						zmianamapy = true;
						break;
					case ALLEGRO_KEY_E:
						if (czy_ekwipunek == false) {
							czy_ekwipunek = true;
							ruch = false;
						}
						else
						{
							czy_ekwipunek = false;
							ruch = true;
						}
						break;
					case ALLEGRO_KEY_T:
						rozmowa=sprawdzenie_do_rozmowy(mapadane, aktualna_pozycjax, aktualna_pozycjay, wysokosc, szerokosc, dane_do_ruchu_npc);
						if (rozmowa != 0) {
							ktory_rozmawia = rozmowa-1;
							dane_do_ruchu_npc[ktory_rozmawia].rozmawia = true;
							nr_odp = 0;
							do_wyboru_dialogu = 1;
						}
						break;
					case ALLEGRO_KEY_SPACE:
						atak(mapadane, ostatni_kier_bohatera, aktualna_pozycjax, aktualna_pozycjay, dane_przeciwnikow_wartosci, przedmioty_zalozone, statystyki_przec, opisy_przedmiotow);
						break;
					case ALLEGRO_KEY_ENTER:
						if (mapadane[aktualna_pozycjay][aktualna_pozycjax].przedmiot != 0) {
							dodaj_przedmiot_do_ekwipunku(przedmioty_w_ekwipunku, mapadane[aktualna_pozycjay][aktualna_pozycjax].przedmiot);
						}
						mapadane[aktualna_pozycjay][aktualna_pozycjax].przedmiot = 0;
						break;
					case ALLEGRO_KEY_R:
						eventy(mapadane[aktualna_pozycjay][aktualna_pozycjax].zdarzenia, aktualna_pozycjax, aktualna_pozycjay, przedmioty_zalozone, przedmioty_w_ekwipunku);
						break;
					}
				}

			if (ruch && mapay==ydocelowe  && mapax==xdocelowe) {
				kierunek = 5;
				if (ydocelowe == mapay && przyciski[0] == true) {
					if (!poza_mapa(wysokosc, szerokosc, aktualna_pozycjax, aktualna_pozycjay - 1)) {
						if(!kolizja(aktualna_pozycjax, aktualna_pozycjay - 1, mapadane)) {
							ydocelowe += misjednostka;
							ostatni_kier_bohatera = 0;
							kierunek = 0;
							skok_zamiany_bitmapy_w_ruchu = 0;
						}
					}
				}
				if (ydocelowe == mapay && przyciski[1] == true) {
					if (!poza_mapa(wysokosc, szerokosc, aktualna_pozycjax, aktualna_pozycjay + 1)) {
						if (!kolizja(aktualna_pozycjax, aktualna_pozycjay + 1, mapadane)) {
							ydocelowe -= misjednostka;
							ostatni_kier_bohatera = 1;
							kierunek = 1;
							skok_zamiany_bitmapy_w_ruchu = 0;
						}
					}
				}
				if (xdocelowe == mapax && przyciski[2] == true) {
					if (!poza_mapa(wysokosc, szerokosc, aktualna_pozycjax - 1, aktualna_pozycjay)) {
						if (!kolizja(aktualna_pozycjax - 1, aktualna_pozycjay, mapadane)) {
							xdocelowe += misjednostka;
							ostatni_kier_bohatera = 2;
							kierunek = 2;
							skok_zamiany_bitmapy_w_ruchu = 0;
						}
					}
				}
				if (xdocelowe == mapax && przyciski[3] == true) {
					if (!poza_mapa(wysokosc, szerokosc, aktualna_pozycjax + 1, aktualna_pozycjay)) {
						if (!kolizja(aktualna_pozycjax + 1, aktualna_pozycjay, mapadane)) {
							xdocelowe -= misjednostka;
							ostatni_kier_bohatera = 3;
							kierunek = 3;
							skok_zamiany_bitmapy_w_ruchu = 0;
						}
					}
				}
			}
			printf_s("x  %i       y %i   \n",aktualna_pozycjax, aktualna_pozycjay);
			if (mapadane[aktualna_pozycjay][aktualna_pozycjax].przejscie_mapy !=0) {
				zmianamapy = true;
				if (numerbitmapy == 0) {
					xstarejpoz = aktualna_pozycjax-7;
					ystarejpoz = aktualna_pozycjay-7 + 1;
				}
				numerbitmapy = mapadane[aktualna_pozycjay][aktualna_pozycjax].przejscie_mapy-1;
				
				for (i = 0; i < 4; i++) {
					przyciski[i] = false;
				}
				
			}
			if (zdarzenie.type == ALLEGRO_EVENT_TIMER) {
				if (mapay < ydocelowe) {
					mapay += predkosc_bohatera;
					skok_zamiany_bitmapy_w_ruchu += predkosc_bohatera;
					skok_zamiany_bitmapy_w_ruchu=skok_zamiany_bitmapy_w_ruchu % 30;
				}
				if (mapay > ydocelowe) {
					mapay -= predkosc_bohatera;
					skok_zamiany_bitmapy_w_ruchu += predkosc_bohatera;
					skok_zamiany_bitmapy_w_ruchu = skok_zamiany_bitmapy_w_ruchu % 30;
				}
				if (mapax < xdocelowe) {
					mapax += predkosc_bohatera;
					skok_zamiany_bitmapy_w_ruchu += predkosc_bohatera;
					skok_zamiany_bitmapy_w_ruchu = skok_zamiany_bitmapy_w_ruchu % 30;
				}
				if (mapax > xdocelowe) {
					mapax -= predkosc_bohatera;
					skok_zamiany_bitmapy_w_ruchu += predkosc_bohatera;
					skok_zamiany_bitmapy_w_ruchu = skok_zamiany_bitmapy_w_ruchu % 30;
				}
			}
			al_draw_bitmap_region(mapa, -mapax, -mapay, 750, 750, 0, 0, 0);
			xszukania = (mapax / 50)*-1;
			yszukania = (mapay / 50)*-1;
			if (ilosc_rysowanych_npc> 0 || ilosc_rysowanych_wrogow>0) {
				for (i = yszukania; i < yszukania + szerszukania; i++) {
					for (j = xszukania; j < xszukania + szerszukania; j++) {
						if (!poza_mapa(wysokosc,szerokosc,i,j))
						{
							if (mapadane[i][j].przedmiot != 0) {
								al_draw_bitmap_region(przedmioty, (mapadane[i][j].przedmiot - 1)*misjednostka, 50, misjednostka, misjednostka, (j*misjednostka) - mapax*-1, (i*misjednostka) - mapay*-1, 0);
							}
							if (ilosc_rysowanych_npc > 0) {
								if (mapadane[i][j].npc_id != 0) {
									tmp = mapadane[i][j].npc_id - 1;
									rysowanie_postaci_w_ruchu(NPC[dane_do_ruchu_npc[tmp].npc_id], dane_do_ruchu_npc[tmp].kierunek, dane_do_ruchu_npc[tmp].skok_miedzy_zmianami, dane_do_ruchu_npc[tmp].pozx*-1 + mapax, dane_do_ruchu_npc[tmp].pozy*-1 + mapay, dane_do_ruchu_npc[tmp].ostatni_kierunek);
									if (dane_do_ruchu_npc[tmp].pozx == dane_do_ruchu_npc[tmp].poz_docelowa_x && dane_do_ruchu_npc[tmp].pozy == dane_do_ruchu_npc[tmp].poz_docelowa_y) {
										dane_do_ruchu_npc[tmp].kierunek = 4;
										if (!dane_do_ruchu_npc[tmp].rozmawia) {
											if (dane_do_ruchu_npc[tmp].pozx / misjednostka*-1 != j || dane_do_ruchu_npc[tmp].pozy / misjednostka*-1 != i) {
												x = mapadane[dane_do_ruchu_npc[tmp].pozy / misjednostka*-1][dane_do_ruchu_npc[tmp].pozx / misjednostka*-1].npc_id;
												mapadane[dane_do_ruchu_npc[tmp].pozy / misjednostka*-1][dane_do_ruchu_npc[tmp].pozx / misjednostka*-1].npc_id = mapadane[i][j].npc_id;
												mapadane[i][j].npc_id = x;
											}
											x = rand() % 4;
											aktualna_poz_npc_x = dane_do_ruchu_npc[tmp].pozx / misjednostka*-1;
											aktualna_poz_npc_y = dane_do_ruchu_npc[tmp].pozy / misjednostka*-1;
											if (dane_do_ruchu_npc[tmp].pozx == dane_do_ruchu_npc[tmp].poz_docelowa_x || dane_do_ruchu_npc[tmp].pozy == dane_do_ruchu_npc[tmp].poz_docelowa_y) {
												mapadane[dane_do_ruchu_npc[tmp].poz_docelowa_y / misjednostka*-1][dane_do_ruchu_npc[tmp].poz_docelowa_x / misjednostka*-1].przyszle_poz_npc = false;
											}
											switch (x) {
												//ruch npc w lewo
											case 0:
												if (((float)clock() - dane_do_ruchu_npc[tmp].czas) / CLOCKS_PER_SEC > przerwa_miedzy_przejsciami_npc) {
													if (aktualna_poz_npc_x > xspawnu && aktualna_poz_npc_x > dane_do_ruchu_npc[tmp].minx) {
														if (!kolizja(aktualna_poz_npc_x - 1, aktualna_poz_npc_y, mapadane) && (aktualna_pozycjax != aktualna_poz_npc_x - 1 || aktualna_pozycjay != aktualna_poz_npc_y)) {
															dane_do_ruchu_npc[tmp].poz_docelowa_x += misjednostka;
															dane_do_ruchu_npc[tmp].czas = clock();
															dane_do_ruchu_npc[tmp].ostatni_kierunek = 2;
															dane_do_ruchu_npc[tmp].kierunek = 2;
														}
													}
												}
												break;
												//ruch npc w prawo
											case 1:
												if (((float)clock() - dane_do_ruchu_npc[tmp].czas) / CLOCKS_PER_SEC > przerwa_miedzy_przejsciami_npc) {
													if (aktualna_poz_npc_x < szerokosc && aktualna_poz_npc_x < dane_do_ruchu_npc[tmp].maxx) {
														if (!kolizja(aktualna_poz_npc_x + 1, aktualna_poz_npc_y, mapadane) && (aktualna_pozycjax != aktualna_poz_npc_x + 1 || aktualna_pozycjay != aktualna_poz_npc_y)) {
															dane_do_ruchu_npc[tmp].poz_docelowa_x -= misjednostka;
															dane_do_ruchu_npc[tmp].czas = clock();
															dane_do_ruchu_npc[tmp].ostatni_kierunek = 3;
															dane_do_ruchu_npc[tmp].kierunek = 3;
														}
													}
												}
												break;
												//ruch npc w gore
											case 2:
												if (((float)clock() - dane_do_ruchu_npc[tmp].czas) / CLOCKS_PER_SEC > przerwa_miedzy_przejsciami_npc) {
													if (aktualna_poz_npc_y > yspawnu&& aktualna_poz_npc_y > dane_do_ruchu_npc[tmp].miny) {
														if (!kolizja(aktualna_poz_npc_x, aktualna_poz_npc_y - 1, mapadane) && (aktualna_pozycjax != aktualna_poz_npc_x || aktualna_pozycjay != aktualna_poz_npc_y - 1)) {
															dane_do_ruchu_npc[tmp].poz_docelowa_y += misjednostka;
															dane_do_ruchu_npc[tmp].czas = clock();
															dane_do_ruchu_npc[tmp].ostatni_kierunek = 0;
															dane_do_ruchu_npc[tmp].kierunek = 0;
														}
													}
												}
												break;
												//ruch npc w dol
											case 3:
												if (((float)clock() - dane_do_ruchu_npc[tmp].czas) / CLOCKS_PER_SEC > przerwa_miedzy_przejsciami_npc) {
													if (aktualna_poz_npc_y < wysokosc && aktualna_poz_npc_y < dane_do_ruchu_npc[tmp].maxy) {
														if (!kolizja(aktualna_poz_npc_x, aktualna_poz_npc_y + 1, mapadane) && (aktualna_pozycjax != aktualna_poz_npc_x || aktualna_pozycjay != aktualna_poz_npc_y + 1)) {
															dane_do_ruchu_npc[tmp].poz_docelowa_y -= misjednostka;
															dane_do_ruchu_npc[tmp].czas = clock();
															dane_do_ruchu_npc[tmp].ostatni_kierunek = 1;
															dane_do_ruchu_npc[tmp].kierunek = 1;
														}
													}
												}
												break;

											}
										}
									}
									if (dane_do_ruchu_npc[tmp].pozx != dane_do_ruchu_npc[tmp].poz_docelowa_x || dane_do_ruchu_npc[tmp].pozy != dane_do_ruchu_npc[tmp].poz_docelowa_y) {
										mapadane[dane_do_ruchu_npc[tmp].poz_docelowa_y / misjednostka*-1][dane_do_ruchu_npc[tmp].poz_docelowa_x / misjednostka*-1].przyszle_poz_npc = true;
									}
									if (zdarzenie.type == ALLEGRO_EVENT_TIMER) {
										if (dane_do_ruchu_npc[tmp].pozx < dane_do_ruchu_npc[tmp].poz_docelowa_x) {
											dane_do_ruchu_npc[tmp].pozx += 1;
											dane_do_ruchu_npc[tmp].skok_miedzy_zmianami++;
											dane_do_ruchu_npc[tmp].skok_miedzy_zmianami = dane_do_ruchu_npc[tmp].skok_miedzy_zmianami % 30;

										}
										if (dane_do_ruchu_npc[tmp].pozx > dane_do_ruchu_npc[tmp].poz_docelowa_x) {
											dane_do_ruchu_npc[tmp].pozx -= 1;
											dane_do_ruchu_npc[tmp].skok_miedzy_zmianami++;
											dane_do_ruchu_npc[tmp].skok_miedzy_zmianami = dane_do_ruchu_npc[tmp].skok_miedzy_zmianami % 30;
										}
										if (dane_do_ruchu_npc[tmp].pozy < dane_do_ruchu_npc[tmp].poz_docelowa_y) {
											dane_do_ruchu_npc[tmp].pozy += 1;
											dane_do_ruchu_npc[tmp].skok_miedzy_zmianami++;
											dane_do_ruchu_npc[tmp].skok_miedzy_zmianami = dane_do_ruchu_npc[tmp].skok_miedzy_zmianami % 30;
										}
										if (dane_do_ruchu_npc[tmp].pozy > dane_do_ruchu_npc[tmp].poz_docelowa_y) {
											dane_do_ruchu_npc[tmp].pozy -= 1;
											dane_do_ruchu_npc[tmp].skok_miedzy_zmianami++;
											dane_do_ruchu_npc[tmp].skok_miedzy_zmianami = dane_do_ruchu_npc[tmp].skok_miedzy_zmianami % 30;
										}
									}
								}
							}
							if (ilosc_rysowanych_wrogow > 0) {
								if (mapadane[i][j].przeciwnik_id != 0) {
									tmp = mapadane[i][j].przeciwnik_id - 1;
									if (dane_przeciwnikow_wartosci[tmp].zdrowie > 0) {
										rysowanie_postaci_w_ruchu(wrogowie[dane_przeciwnikow_wartosci[tmp].nr_przeciwnika - 1], dane_przeciwnikow_wartosci[tmp].kierunek, dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami, dane_przeciwnikow_wartosci[tmp].poz_x*-1 + mapax, dane_przeciwnikow_wartosci[tmp].poz_y*-1 + mapay, dane_przeciwnikow_wartosci[tmp].ostatni_kierunek);
										if (dane_przeciwnikow_wartosci[tmp].poz_x == dane_przeciwnikow_wartosci[tmp].poz_docelowa_x&&dane_przeciwnikow_wartosci[tmp].poz_y == dane_przeciwnikow_wartosci[tmp].poz_docelowa_y) {
											dane_przeciwnikow_wartosci[tmp].kierunek = 4;
											aktualna_poz_przeciwnika.x = dane_przeciwnikow_wartosci[tmp].poz_x / misjednostka*-1;
											aktualna_poz_przeciwnika.y = dane_przeciwnikow_wartosci[tmp].poz_y / misjednostka*-1;
											poz_gracza.x = aktualna_pozycjax;
											poz_gracza.y = aktualna_pozycjay;
											if (dane_przeciwnikow_wartosci[tmp].poz_x / misjednostka*-1 != j || dane_przeciwnikow_wartosci[tmp].poz_y / misjednostka*-1 != i) {
												x = mapadane[dane_przeciwnikow_wartosci[tmp].poz_y / misjednostka*-1][dane_przeciwnikow_wartosci[tmp].poz_x / misjednostka*-1].przeciwnik_id;
												mapadane[dane_przeciwnikow_wartosci[tmp].poz_y / misjednostka*-1][dane_przeciwnikow_wartosci[tmp].poz_x / misjednostka*-1].przeciwnik_id = mapadane[i][j].przeciwnik_id;
												mapadane[i][j].przeciwnik_id = x;
											}
											if (!czy_jest_obok(mapadane, poz_gracza, wysokosc, szerokosc, tmp + 1)) {
												pomoc = wytyczenie_trasy_dla_przeciwnikow(mapadane, aktualna_poz_przeciwnika, szerokosc, wysokosc, poz_gracza);
												if (pomoc.x != 10000 && pomoc.y != 10000 && pomoc.x != xdocelowe&&pomoc.y != ydocelowe) {
													dane_przeciwnikow_wartosci[tmp].poz_docelowa_x = (pomoc.x*misjednostka*-1);
													dane_przeciwnikow_wartosci[tmp].poz_docelowa_y = (pomoc.y*misjednostka*-1);
													if (dane_przeciwnikow_wartosci[tmp].poz_x < dane_przeciwnikow_wartosci[tmp].poz_docelowa_x) {
														dane_przeciwnikow_wartosci[tmp].kierunek = 2;
														dane_przeciwnikow_wartosci[tmp].ostatni_kierunek = 2;
													}
													if (dane_przeciwnikow_wartosci[tmp].poz_x > dane_przeciwnikow_wartosci[tmp].poz_docelowa_x) {
														dane_przeciwnikow_wartosci[tmp].kierunek = 3;
														dane_przeciwnikow_wartosci[tmp].ostatni_kierunek = 3;
													}
													if (dane_przeciwnikow_wartosci[tmp].poz_y < dane_przeciwnikow_wartosci[tmp].poz_docelowa_y) {
														dane_przeciwnikow_wartosci[tmp].kierunek = 0;
														dane_przeciwnikow_wartosci[tmp].ostatni_kierunek = 0;
													}
													if (dane_przeciwnikow_wartosci[tmp].poz_y > dane_przeciwnikow_wartosci[tmp].poz_docelowa_y) {
														dane_przeciwnikow_wartosci[tmp].kierunek = 1;
														dane_przeciwnikow_wartosci[tmp].ostatni_kierunek = 1;
													}
												}

											}
											else {
												if (((float)clock() - dane_przeciwnikow_wartosci[tmp].czas_ataku) / CLOCKS_PER_SEC > 1.5) {
													dane_przeciwnikow_wartosci[tmp].czas_ataku = clock();
													zdrowie -= statystyki_przec[dane_przeciwnikow_wartosci[tmp].nr_przeciwnika-1].atak;
												}
											}
										}
										if (zdarzenie.type == ALLEGRO_EVENT_TIMER) {
											if (dane_przeciwnikow_wartosci[tmp].poz_x < dane_przeciwnikow_wartosci[tmp].poz_docelowa_x) {
												dane_przeciwnikow_wartosci[tmp].poz_x += statystyki_przec[dane_przeciwnikow_wartosci[tmp].nr_przeciwnika-1].predkosc;
												dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami += statystyki_przec[dane_przeciwnikow_wartosci[tmp].nr_przeciwnika-1].predkosc;
												dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami = dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami % 30;
											}
											if (dane_przeciwnikow_wartosci[tmp].poz_x > dane_przeciwnikow_wartosci[tmp].poz_docelowa_x) {
												dane_przeciwnikow_wartosci[tmp].poz_x -= statystyki_przec[dane_przeciwnikow_wartosci[tmp].nr_przeciwnika-1].predkosc;
												dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami += statystyki_przec[dane_przeciwnikow_wartosci[tmp].nr_przeciwnika-1].predkosc;
												dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami = dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami % 30;
											}
											if (dane_przeciwnikow_wartosci[tmp].poz_y < dane_przeciwnikow_wartosci[tmp].poz_docelowa_y) {
												dane_przeciwnikow_wartosci[tmp].poz_y += statystyki_przec[dane_przeciwnikow_wartosci[tmp].nr_przeciwnika-1].predkosc;
												dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami += statystyki_przec[dane_przeciwnikow_wartosci[tmp].nr_przeciwnika-1].predkosc;
												dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami = dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami % 30;
											}
											if (dane_przeciwnikow_wartosci[tmp].poz_y > dane_przeciwnikow_wartosci[tmp].poz_docelowa_y) {
												dane_przeciwnikow_wartosci[tmp].poz_y -= statystyki_przec[dane_przeciwnikow_wartosci[tmp].nr_przeciwnika-1].predkosc;
												dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami += statystyki_przec[dane_przeciwnikow_wartosci[tmp].nr_przeciwnika-1].predkosc;
												dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami = dane_przeciwnikow_wartosci[tmp].skok_miedzy_zmianami % 30;
											}
										}
									}
									else {
										mapadane[i][j].przeciwnik_id = 0;
									}
								}
							}
						}
					}
				}
			}
			rysowanie_postaci_w_ruchu(bohater,kierunek,skok_zamiany_bitmapy_w_ruchu,bohaterx,bohatery,ostatni_kier_bohatera);
			//przemieszczanie podswietlenia w ekwipunku
			if (zdarzenie.type == ALLEGRO_EVENT_KEY_DOWN && czy_ekwipunek)
			{
				switch (zdarzenie.keyboard.keycode)
				{
				case ALLEGRO_KEY_UP:
					if (y_ramki > 0){
						y_ramki--;
					}
					break;
				case ALLEGRO_KEY_DOWN:
					if (y_ramki < 4){
						y_ramki++;
					}
					break;
				case ALLEGRO_KEY_LEFT:
					if (x_ramki > 0){
						x_ramki--;
					}
					break;
				case ALLEGRO_KEY_RIGHT:
					if (x_ramki < 2){
						x_ramki++;
					}
					break;
				case ALLEGRO_KEY_ENTER:
					zalozenie_przedmiotu(przedmioty_zalozone, przedmioty_w_ekwipunku, opisy_przedmiotow, x_ramki, y_ramki);
				}
			}
			if (czy_ekwipunek == true) {
				al_draw_bitmap(ekwipunek, 200, 150, 0);
				al_draw_bitmap(ramka_wyboru, 550+x_ramki*misjednostka, 200+y_ramki*misjednostka, 0);
				for (i = 0; i < 5; i++) {
					for (j = 0; j < 3; j++) {
						if (przedmioty_w_ekwipunku[i][j] != 0) {
							al_draw_bitmap_region(przedmioty, (przedmioty_w_ekwipunku[i][j] - 1)*misjednostka, misjednostka, misjednostka, misjednostka, j*misjednostka + 550, i*misjednostka + 200,0);
						}
					}
				}
				if (przedmioty_w_ekwipunku[y_ramki][x_ramki] != 0) {
					al_draw_text(czcionka_18, al_map_rgb(0, 0, 0), 260, 460, 0, opisy_przedmiotow[przedmioty_w_ekwipunku[y_ramki][x_ramki]-1].nazwa);
					al_draw_text(czcionka_18_cienka, al_map_rgb(0, 0, 0), 260, 480, 0,typy_przedmiotow_tab[opisy_przedmiotow[przedmioty_w_ekwipunku[y_ramki][x_ramki] - 1].typ-1].nazwa);
					al_draw_text(czcionka_18_cienka, al_map_rgb(0, 0, 0), 260, 500,0, opisy_przedmiotow[przedmioty_w_ekwipunku[y_ramki][x_ramki] - 1].opis);
				}
				for (i = 0; i < 6; i++) {
					if (przedmioty_zalozone[i] != 0) {
						al_draw_bitmap_region(przedmioty, (przedmioty_zalozone[i] - 1)*misjednostka, misjednostka, misjednostka, misjednostka, typy_przedmiotow_tab[opisy_przedmiotow[przedmioty_zalozone[i]-1].typ-1].x, typy_przedmiotow_tab[opisy_przedmiotow[przedmioty_zalozone[i]-1].typ-1].y, 0);
					}
				}
			}
			if (rozmowa != 0) {
				ruch = false;
				id = dane_do_ruchu_npc[rozmowa-1].npc_id+1;
				al_draw_bitmap(ramka_do_dialogow, 0, 0, 0);
				if (wybor) {
					al_draw_bitmap_region(glowy, 0, 0, 150, 150, 50, 550, 0);
						al_draw_bitmap(ramka_wyboru_do_dialogow, 190, 520 + do_wyboru_dialogu * 30, 0);
					if (zdarzenie.type == ALLEGRO_EVENT_KEY_UP) {
						switch (zdarzenie.keyboard.keycode)
						{
						case ALLEGRO_KEY_UP:
							if (do_wyboru_dialogu >1) {
								do_wyboru_dialogu--;
							}
							break;
						case ALLEGRO_KEY_DOWN:
							if (do_wyboru_dialogu <ilosc_opcji) {
								do_wyboru_dialogu++;
							}
							break;
						case ALLEGRO_KEY_ENTER:
							if (drugi_wybor == 0 && nr_odp != 0) {
								drugi_wybor = do_wyboru_dialogu;
							}
							if (nr_odp == 0) {
								nr_odp = do_wyboru_dialogu;
							}
							wziecie_czasu = true;
							wybor = false;
						}
					}
				}
				if (wziecie_czasu) {
					czas_startowy = clock();
					wziecie_czasu = false;
				}
				if (!wybor) {
					al_draw_bitmap_region(glowy, id * 150, 0, 150, 150, 50, 550, 0);
				}
				czas_aktualny = ((float)clock()- czas_startowy) / CLOCKS_PER_SEC;
				dialogi(czcionka_24,id, &nr_odp, &rozmowa,& nr_zad,czas_aktualny,&wziecie_czasu,&wybor,&ilosc_opcji,&drugi_wybor,numerbitmapy,przedmioty_zalozone,przedmioty_w_ekwipunku);
				if (rozmowa == 0) {
					wziecie_czasu = true;
					dane_do_ruchu_npc[ktory_rozmawia].rozmawia = false;
					ruch = true;
					do_wyboru_dialogu = 1;
				}
			}
			al_draw_bitmap(serce, 0, 0, 0);
			al_draw_textf(czcionka_48, al_map_rgb(189, 29, 29),50,0,0,"%i",zdrowie);
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
		fclose(dane_do_mapy);
		free(dane_do_ruchu_npc);
		for (i = 0; i < wysokosc; i++) {
			free(mapadane[i]);
		}
		free(mapadane);
		al_destroy_bitmap(mapa);
		free(dane_przeciwnikow_wartosci);
	}
	if (ilosc_npc != 0) {
		for (i = 0; i < ilosc_npc; i++) {
			al_destroy_bitmap(NPC[i]);
		}
		free(NPC);
	}
	if (ilosc_wrogow != 0) {
		for (i = 0; i < ilosc_wrogow; i++) {
			al_destroy_bitmap(wrogowie[i]);
		}
		free(wrogowie);
	}
	if (ilosc_map != 0) {
		for (i = 0; i < ilosc_map; i++) {
			free(nazwy_plikow[i]);
		}
		free(nazwy_plikow);
	}
	
	free(nazwy_npc);
	free(nazwy_przeciwnikow);
	al_destroy_bitmap(ekwipunek);
	al_destroy_bitmap(bohater);
	al_destroy_bitmap(przedmioty);
	al_destroy_display(okno);
	al_destroy_event_queue(kolejka);
	al_destroy_bitmap(serce);
	al_destroy_font(czcionka_48);
	al_destroy_font(czcionka_18);
	al_destroy_font(czcionka_24);
	al_destroy_bitmap(ramka_wyboru);
	al_destroy_font(czcionka_18_cienka);
	al_destroy_bitmap(glowy);
	al_destroy_bitmap(ramka_do_dialogow);
	al_destroy_bitmap(ramka_wyboru_do_dialogow);

	return 0;
}
/*typy
1 -bron
2 -helm
3 -zbroja
4 -uzytkowe
5 -pieniadze
6 -jedzenie
*/
void ladowanie_typow_przedmiotow(typy_przedmiotow_t *tablica) {
	strcpy_s(tablica[0].nazwa, 20, "typ: bron");
	tablica[0].x = 300;
	tablica[0].y = 350;
	strcpy_s(tablica[1].nazwa, 20, "typ: helm");
	tablica[1].x = 250;
	tablica[1].y = 450;
	strcpy_s(tablica[2].nazwa, 20, "typ: zbroja");
	tablica[2].x = 250;
	tablica[2].y = 450;
	strcpy_s(tablica[3].nazwa, 20, "typ: uzytkowe");
	tablica[3].x = 450;
	tablica[3].y = 300;
	strcpy_s(tablica[4].nazwa, 20, "typ: pieniadze");
	tablica[4].x = 250;
	tablica[4].y = 450;
	strcpy_s(tablica[5].nazwa, 20, "typ: jedzenie");
	tablica[5].x = 250;
	tablica[5].y = 450;
}
void ladowanie_opisow_przedmiotow(przedmiot_t *tablica) {
	//wiadro z woda
	strcpy_s(tablica[0].nazwa,20,"wiadro z woda");
	tablica[0].typ = 4;
	tablica[0].atak = 0;
	strcpy_s(tablica[0].opis, 50, "wiadro pelne wody");
	//miecz
	strcpy_s(tablica[1].nazwa, 20, "miecz");
	tablica[1].typ = 1;
	strcpy_s(tablica[1].opis, 50, "bardzo ostry miecz");
	tablica[1].atak = 100;
	//puste wiadro
	strcpy_s(tablica[2].nazwa, 20, "puste wiadro");
	tablica[2].typ = 4;
	strcpy_s(tablica[2].opis, 50, "puste wiadro");
	tablica[2].atak = 0;
	//noz
	strcpy_s(tablica[3].nazwa, 20, "noz");
	tablica[3].typ = 1;
	strcpy_s(tablica[3].opis, 50, "ostry noz");
	tablica[3].atak = 2;
	//widelec
	strcpy_s(tablica[4].nazwa, 20, "widelec");
	tablica[4].typ = 1;
	strcpy_s(tablica[4].opis, 50, "zazwyczaj sluzy do jedzenia");
	tablica[4].atak = 1;
	//moneta
	strcpy_s(tablica[5].nazwa, 20,"mismoneta");
	tablica[5].typ = 5;
	strcpy_s(tablica[5].opis, 50, "srodek platniczy");
	tablica[5].atak = 0;
	//jedzenie
	strcpy_s(tablica[6].nazwa, 20, "miodek");
	tablica[6].typ = 6;
	strcpy_s(tablica[6].opis, 50, "bardzo smaczny miodek");
	tablica[6].atak = 0;
}
void rysowanie_postaci_w_ruchu(ALLEGRO_BITMAP *obraz, int kierunek, int pix, int poz_x, int poz_y, int ostatni_kierunek) {
	int granica_skoku = 15;
	if (kierunek != 0 && kierunek != 1 && kierunek != 2 && kierunek != 3) {
		if (ostatni_kierunek == 0) {
			al_draw_bitmap_region(obraz, 0, 100, 50, 50, poz_x, poz_y, 0);
		}
		if (ostatni_kierunek == 1) {
			al_draw_bitmap_region(obraz, 50, 100, 50, 50, poz_x, poz_y, 0);
		}
		if (ostatni_kierunek == 2) {
			al_draw_bitmap_region(obraz, 100, 100, 50, 50, poz_x, poz_y, 0);
		}
		if (ostatni_kierunek == 3) {
			al_draw_bitmap_region(obraz, 150, 100, 50, 50, poz_x, poz_y, 0);
		}
	}
	if (kierunek == 0) {
		if (pix <= granica_skoku) {
			al_draw_bitmap_region(obraz, 0, 0, 50, 50, poz_x, poz_y, 0);
		}
		if (pix >granica_skoku) {
			al_draw_bitmap_region(obraz, 0, 50, 50, 50, poz_x, poz_y, 0);
		}
	}
	if (kierunek == 1) {
		if (pix <= granica_skoku) {
			al_draw_bitmap_region(obraz, 50, 0, 50, 50, poz_x, poz_y, 0);
		}
		if (pix >granica_skoku) {
			al_draw_bitmap_region(obraz, 50, 50, 50, 50, poz_x, poz_y, 0);
		}
	}
	if (kierunek == 2) {
		if (pix <= granica_skoku) {
			al_draw_bitmap_region(obraz, 100, 0, 50, 50, poz_x, poz_y, 0);
		}
		if (pix >granica_skoku) {
			al_draw_bitmap_region(obraz, 100, 50, 50, 50, poz_x, poz_y, 0);
		}
	}
	if (kierunek == 3) {
		if (pix <= granica_skoku) {
			al_draw_bitmap_region(obraz, 150, 0, 50, 50, poz_x, poz_y, 0);
		}
		if (pix >granica_skoku) {
			al_draw_bitmap_region(obraz, 150, 50, 50, 50, poz_x, poz_y, 0);
		}
	}
}
void wczytanie_z_pliku(FILE *wyjscie, wlasciwosci_pola_t **dane, int wysokosc, int szerokosc) {
	int i, j;
	int pomoc;
	for (j = 0; j < wysokosc; j++) {
		for (i = 0; i < szerokosc; i++) {
			fscanf_s(wyjscie, "%i", &pomoc);
			dane[j][i].przeszkoda = (bool)pomoc;
			fscanf_s(wyjscie, "%i", &dane[j][i].npc);
			fscanf_s(wyjscie, "%i", &dane[j][i].przejscie_mapy);
			fscanf_s(wyjscie, "%i", &dane[j][i].przeciwnik);
			fscanf_s(wyjscie, "%i", &dane[j][i].przedmiot);
			fscanf_s(wyjscie, "%i", &dane[j][i].zdarzenia);
		}
	}
}
void czytanie_nazw(char ***nazwy) {

	nazwy[0][0] = "data/mapy/miasto.png";
	nazwy[0][1] = "data/mapy/miasto.leafe";

	nazwy[1][0] = "data/mapy/wnetrze_dom_starca.png";
	nazwy[1][1] = "data/mapy/wnetrze_dom_starca.leafe";

	nazwy[2][0] = "data/mapy/domAlbrehta.png";
	nazwy[2][1] = "data/mapy/domAlbrehta.leafe";

	nazwy[3][0] = "data/mapy/karczma.png";
	nazwy[3][1] = "data/mapy/karczma.leafe";

	nazwy[4][0] = "data/mapy/piwnica_szczury.png";
	nazwy[4][1] = "data/mapy/piwnica_szczury.leafe";

	nazwy[5][0] = "data/mapy/wnetrze_dom_mieszkalny1.png";
	nazwy[5][1] = "data/mapy/mieszkanie1.leafe";

	nazwy[6][0] = "data/mapy/wnetrze_dom_mieszkalny2.png";
	nazwy[6][1] = "data/mapy/mieszkanie2.leafe";

	nazwy[7][0] = "data/mapy/wnetrze_dom_mieszkalny3.png";
	nazwy[7][1] = "data/mapy/mieszkanie3.leafe";

	nazwy[8][0] = "data/mapy/wnetrze_dom_mieszkalny1.png";
	nazwy[8][1] = "data/mapy/mieszkanie4.leafe";

	nazwy[9][0] = "data/mapy/wnetrze_dom_mieszkalny2.png";
	nazwy[9][1] = "data/mapy/mieszkanie5.leafe";

	nazwy[10][0] = "data/mapy/wnetrze_dom_mieszkalny3.png";
	nazwy[10][1] = "data/mapy/mieszkanie6.leafe";

	nazwy[11][0] = "data/mapy/wnetrze_dom_mieszkalny1.png";
	nazwy[11][1] = "data/mapy/mieszkanie7.leafe";

	nazwy[12][0] = "data/mapy/wnetrze_dom_mieszkalny2.png";
	nazwy[12][1] = "data/mapy/mieszkanie8.leafe";

	nazwy[13][0] = "data/mapy/wnetrze_dom_mieszkalny3.png";
	nazwy[13][1] = "data/mapy/mieszkanie9.leafe";

	nazwy[14][0] = "data/mapy/wnetrze_dom_mieszkalny1.png";
	nazwy[14][1] = "data/mapy/mieszkanie10.leafe";

	nazwy[15][0] = "data/mapy/wnetrze_dom_mieszkalny2.png";
	nazwy[15][1] = "data/mapy/mieszkanie11.leafe";

	nazwy[16][0] = "data/mapy/wnetrze_dom_mieszkalny3.png";
	nazwy[16][1] = "data/mapy/mieszkanie12.leafe";

	nazwy[17][0] = "data/mapy/wnetrze_dom_mieszkalny1.png";
	nazwy[17][1] = "data/mapy/mieszkanie13.leafe";

	nazwy[18][0] = "data/mapy/wnetrze_dom_mieszkalny2.png";
	nazwy[18][1] = "data/mapy/mieszkanie14.leafe";

	nazwy[19][0] = "data/mapy/wnetrze_dom_mieszkalny3.png";
	nazwy[19][1] = "data/mapy/mieszkanie15.leafe";

	nazwy[20][0] = "data/mapy/wnetrze_dom_mieszkalny1.png";
	nazwy[20][1] = "data/mapy/mieszkanie16.leafe";

	nazwy[21][0] = "data/mapy/wnetrze_dom_mieszkalny2.png";
	nazwy[21][1] = "data/mapy/mieszkanie17.leafe";

	nazwy[22][0] = "data/mapy/wnetrze_dom_mieszkalny3.png";
	nazwy[22][1] = "data/mapy/mieszkanie18.leafe";

	nazwy[23][0] = "data/mapy/wnetrze_dom_mieszkalny1.png";
	nazwy[23][1] = "data/mapy/mieszkanie19.leafe";

	nazwy[24][0] = "data/mapy/wnetrze_dom_mieszkalny2.png";
	nazwy[24][1] = "data/mapy/mieszkanie20.leafe";

	nazwy[25][0] = "data/mapy/wnetrze_dom_mieszkalny3.png";
	nazwy[25][1] = "data/mapy/mieszkanie21.leafe";

	nazwy[26][0] = "data/mapy/wnetrze_dom_mieszkalny1.png";
	nazwy[26][1] = "data/mapy/mieszkanie22.leafe";

	nazwy[27][0] = "data/mapy/wnetrze_dom_mieszkalny2.png";
	nazwy[27][1] = "data/mapy/mieszkanie23.leafe";

	nazwy[28][0] = "data/mapy/wnetrze_dom_mieszkalny3.png";
	nazwy[28][1] = "data/mapy/mieszkanie24.leafe";

}
void czytanie_nazw_przeciwnikow(char **nazwy) {
	nazwy[0] = "data/postacie/przeciwnicy/wilk.png";
	nazwy[1] = "data/postacie/przeciwnicy/szczur.png";
	nazwy[2] = "data/postacie/przeciwnicy/zywiolak.png";
}
void czytanie_nazw_npc(char **nazwy) {
	nazwy[0] = "data/postacie/npc/mieszkaniec.png";
	nazwy[1] = "data/postacie/npc/starzec.png";
	nazwy[2] = "data/postacie/npc/mieszkaniec2.png";
	nazwy[3] = "data/postacie/npc/laska1.png";
	nazwy[4] = "data/postacie/npc/laska2.png";
	nazwy[5] = "data/postacie/npc/albreht.png";
	nazwy[6] = "data/postacie/npc/straz1.png";
	nazwy[7] = "data/postacie/npc/straz2.png";
	nazwy[8] = "data/postacie/npc/straz3.png";
	nazwy[9] = "data/postacie/npc/bibliotekarz.png";
	nazwy[10] = "data/postacie/npc/najemnik1.png";
	nazwy[11] = "data/postacie/npc/najemnik2.png";
	nazwy[12] = "data/postacie/npc/sprzedawca1.png";
	nazwy[13] = "data/postacie/npc/sprzedawca2.png";

}
bool dodaj_przedmiot_do_ekwipunku(int ekwipunek[][3], int id_przedmiotu) {
	int i, j;
	for (i = 0; i<5; i++) {
		for (j = 0; j < 3; j++) {
			if (ekwipunek[i][j] == 0) {
				ekwipunek[i][j] = id_przedmiotu;
				return true;
			}
		}
	}
	return false;
}
bool kolizja(int poz_x, int poz_y, wlasciwosci_pola_t **struktura_mapy) {
	if (struktura_mapy[poz_y][poz_x].przeszkoda == true || struktura_mapy[poz_y][poz_x].npc_id != 0 || struktura_mapy[poz_y][poz_x].przyszle_poz_npc == true||struktura_mapy[poz_y][poz_x].przeciwnik_id!=0 || struktura_mapy[poz_y][poz_x].przyszle_poz_przeciwnikow == true) {
		return true;
	}
	return false;
}
int sprawdzenie_do_rozmowy(wlasciwosci_pola_t **dane, int poz_x, int poz_y, int wysokosc_mapy, int szerokosc_mapy, dane_npc_t *dane_do_npc) {
	int max_x = poz_x + 1;
	int min_x = poz_x - 1;
	int max_y = poz_y + 1;
	int min_y = poz_y - 1;
	int npc_id;
	int max_kratek_x = szerokosc_mapy;
	int max_kratek_y = wysokosc_mapy;
	int i, j;
	for (i = min_y; i <= max_y; i++) {
		for (j = min_x; j <= max_x; j++) {
			if (i >= 0 && j >= 0 && i <= max_kratek_y&& j <= max_kratek_x) {
				if (dane[i][j].npc_id != 0) {
					npc_id = dane[i][j].npc_id - 1;
					if (dane_do_npc[npc_id].pozx == dane_do_npc[npc_id].poz_docelowa_x && dane_do_npc[npc_id].pozy == dane_do_npc[npc_id].poz_docelowa_y) {
						return npc_id+1;
					}
				}
			}
		}
	}
	return 0;
}
void zalozenie_przedmiotu(int* zalozone_przedmioty, int przedmioty_w_ekwipunku[][3], przedmiot_t *tablica, int pozx, int pozy) {
	int tmp;
	if (przedmioty_w_ekwipunku[pozy][pozx] != 0) {
		if (zalozone_przedmioty[tablica[przedmioty_w_ekwipunku[pozy][pozx] - 1].typ - 1] == 0) {
			zalozone_przedmioty[tablica[przedmioty_w_ekwipunku[pozy][pozx] - 1].typ - 1] = przedmioty_w_ekwipunku[pozy][pozx];
			przedmioty_w_ekwipunku[pozy][pozx] = 0;
		}
		else
		{
			tmp = zalozone_przedmioty[tablica[przedmioty_w_ekwipunku[pozy][pozx] - 1].typ - 1];
			zalozone_przedmioty[tablica[przedmioty_w_ekwipunku[pozy][pozx] - 1].typ - 1] = przedmioty_w_ekwipunku[pozy][pozx];
			przedmioty_w_ekwipunku[pozy][pozx] = tmp;
		}
	}
}
void zerowanie_klawiszy_ruchu(bool*klawisze) {
	int i;
	for (i = 0; i < 4; i++) {
		klawisze[i] = false;
	}
}
void wyczyszczenie_listy_do_alg_A(s_lista**adres_pierwszy, s_lista**adres_ostatni)
{
	s_lista*pomoc;
	while (*adres_pierwszy != NULL)
	{
		pomoc = *adres_pierwszy;
		*adres_pierwszy = pomoc->adres;
		free(pomoc);
	}
	*adres_ostatni = NULL;
}
s_wspolrzedne_pola pobierz_najmniejszy(s_lista **adres_pierwszy, s_lista **adres_ostatniego, dane_do_A **dane)
{

	s_lista *adres_najmniejszego;
	int wartosc_najmniejszego;
	s_lista *adres_przeszukiwany;
	s_lista *pomoc, *pomoc2;
	s_wspolrzedne_pola wynik;
	if (*adres_ostatniego &&*adres_pierwszy)
	{
		adres_najmniejszego = *adres_pierwszy;
		wartosc_najmniejszego = dane[adres_najmniejszego->y][adres_najmniejszego->x].heu + dane[adres_najmniejszego->y][adres_najmniejszego->x].koszt;
		adres_przeszukiwany = adres_najmniejszego;
		while (adres_przeszukiwany)
		{
			if (wartosc_najmniejszego>dane[adres_przeszukiwany->y][adres_przeszukiwany->x].heu + dane[adres_przeszukiwany->y][adres_przeszukiwany->x].koszt)
			{
				adres_najmniejszego = adres_przeszukiwany;
				wartosc_najmniejszego = dane[adres_przeszukiwany->y][adres_przeszukiwany->x].heu + dane[adres_przeszukiwany->y][adres_przeszukiwany->x].koszt;
			}
			adres_przeszukiwany = adres_przeszukiwany->adres;
		}
		if (*adres_pierwszy == *adres_ostatniego) {
			*adres_pierwszy = NULL;
			*adres_ostatniego = NULL;
		}
		else {
			if (!adres_najmniejszego->adres_poprzedniego)
			{
				*adres_pierwszy = adres_najmniejszego->adres;
				pomoc = *adres_pierwszy;
				pomoc->adres_poprzedniego = NULL;
			}
			else if (!adres_najmniejszego->adres)
			{
				*adres_ostatniego = adres_najmniejszego->adres_poprzedniego;
				pomoc = *adres_ostatniego;
				pomoc->adres = NULL;
			}
			else
			{
				pomoc2 = adres_najmniejszego->adres;
				pomoc = adres_najmniejszego->adres_poprzedniego;
				pomoc2->adres_poprzedniego = pomoc;
				pomoc->adres = adres_najmniejszego->adres;
			}
		}
		wynik.x = adres_najmniejszego->x;
		wynik.y = adres_najmniejszego->y;
		free(adres_najmniejszego);
		return wynik;
	}
	else
	{
		wynik.x = 10000;
		wynik.y = 10000;
		return wynik;
	}
}
void dodaj_do_listy(s_lista **adres_pierwszy, s_lista**adres_ostatni, int x, int y)
{
	s_lista *ostatni_element = *adres_ostatni;
	s_lista *nowy;
	nowy = (s_lista*)malloc(sizeof(s_lista));
	*adres_ostatni = nowy;
	if (!*adres_pierwszy)
	{
		*adres_pierwszy = nowy;
		nowy->adres_poprzedniego = NULL;
	}
	if (ostatni_element)
	{
		ostatni_element->adres = nowy;
		nowy->adres_poprzedniego = ostatni_element;
	}
	nowy->x = x;
	nowy->y = y;
	nowy->adres = NULL;
}
s_wspolrzedne_pola wytyczenie_trasy_dla_przeciwnikow(wlasciwosci_pola_t **mapadane, s_wspolrzedne_pola pozycja, int szerokosc, int wysokosc, s_wspolrzedne_pola poz_gracza) {
	s_lista *adres_pierwszy = NULL;
	s_lista *adres_ostatni = NULL;
	dane_do_A **dane_do_alg;
	s_wspolrzedne_pola rozpatrywane_pole,wynik;
	int i, j;
	int badane_x, badane_y;
	dane_do_alg = (dane_do_A **)malloc(wysokosc * sizeof(dane_do_A*));
	for (i = 0; i < wysokosc; i++) {
		dane_do_alg[i] = (dane_do_A*)malloc(szerokosc * sizeof(dane_do_A));
	}
	for (i = 0; i < wysokosc; i++)
	{
		for (j = 0; j < szerokosc; j++)
		{
			dane_do_alg[i][j].czy_rozpatrzono = false;
			dane_do_alg[i][j].jest_na_liscie = false;
		}
	}
	dodaj_do_listy(&adres_pierwszy, &adres_ostatni, pozycja.x, pozycja.y);
	rozpatrywane_pole = pobierz_najmniejszy(&adres_pierwszy, &adres_ostatni, dane_do_alg);
	dane_do_alg[rozpatrywane_pole.y][rozpatrywane_pole.x].koszt = 0;
	dane_do_alg[rozpatrywane_pole.y][rozpatrywane_pole.x].czy_rozpatrzono = true;
	do
	{
		for (i = -1; i <= 1; i++)
		{
			for (j = -1; j <= 1; j++)
			{
				badane_x = rozpatrywane_pole.x + i;
				badane_y = rozpatrywane_pole.y + j;
				if (badane_x == rozpatrywane_pole.x || badane_y == rozpatrywane_pole.y)
				{
					if (!poza_mapa(wysokosc, szerokosc, badane_x, badane_y))
					{
						if (!kolizja(badane_x, badane_y, mapadane))
						{

							if (!dane_do_alg[badane_y][badane_x].czy_rozpatrzono)
							{
								if (!dane_do_alg[badane_y][badane_x].jest_na_liscie)
								{
									dane_do_alg[badane_y][badane_x].xmatki = rozpatrywane_pole.x;
									dane_do_alg[badane_y][badane_x].ymatki = rozpatrywane_pole.y;
									dane_do_alg[badane_y][badane_x].koszt = dane_do_alg[rozpatrywane_pole.y][rozpatrywane_pole.x].koszt + 1;
									dane_do_alg[badane_y][badane_x].heu = (unsigned int)badane_x - poz_gracza.x + (unsigned int)badane_y - poz_gracza.y;
									dodaj_do_listy(&adres_pierwszy, &adres_ostatni, badane_x, badane_y);
									dane_do_alg[badane_y][badane_x].jest_na_liscie = true;
								}
								else
								{
									if (dane_do_alg[badane_y][badane_x].koszt + dane_do_alg[badane_y][badane_x].heu > dane_do_alg[rozpatrywane_pole.y][rozpatrywane_pole.x].koszt + 1 + (unsigned int)badane_x - poz_gracza.x + (unsigned int)badane_y - poz_gracza.y)
									{
										dane_do_alg[badane_y][badane_x].xmatki = rozpatrywane_pole.x;
										dane_do_alg[badane_y][badane_x].ymatki = rozpatrywane_pole.y;
										dane_do_alg[badane_y][badane_x].koszt = dane_do_alg[rozpatrywane_pole.y][rozpatrywane_pole.x].koszt + 1;
										dane_do_alg[badane_y][badane_x].heu = (unsigned int)badane_x - poz_gracza.x + (unsigned int)badane_y - poz_gracza.y;
									}
								}
							}
						}
						dane_do_alg[rozpatrywane_pole.y][rozpatrywane_pole.x].czy_rozpatrzono = true;
						dane_do_alg[rozpatrywane_pole.y][rozpatrywane_pole.x].jest_na_liscie = false;
					}
				}
			}
		}
		rozpatrywane_pole = pobierz_najmniejszy(&adres_pierwszy, &adres_ostatni, dane_do_alg);
	} while ((rozpatrywane_pole.x != poz_gracza.x || rozpatrywane_pole.y != poz_gracza.y)&&adres_pierwszy);
	if (rozpatrywane_pole.x == poz_gracza.x && rozpatrywane_pole.y == poz_gracza.y) {
		badane_x = dane_do_alg[poz_gracza.y][poz_gracza.x].xmatki;
		badane_y = dane_do_alg[poz_gracza.y][poz_gracza.x].ymatki;
		while (badane_x != pozycja.x ||badane_y != pozycja.y) {
			wynik.x = badane_x;
			wynik.y = badane_y;
			badane_x = dane_do_alg[wynik.y][wynik.x].xmatki;
			badane_y = dane_do_alg[wynik.y][wynik.x].ymatki;
		}
		for (i = 0; i < wysokosc; i++) {
			free(dane_do_alg[i]);
		}
		free(dane_do_alg);
		wyczyszczenie_listy_do_alg_A(&adres_pierwszy, &adres_ostatni);
		return wynik;
	}
	else {
		wyczyszczenie_listy_do_alg_A(&adres_pierwszy, &adres_ostatni);
		for (i = 0; i < wysokosc; i++) {
			free(dane_do_alg[i]);
		}
		free(dane_do_alg);
		wynik.x = 10000;
		wynik.y = 10000;
		return wynik;
	}
}
bool poza_mapa(int wysokosc, int szerokosc, int poz_x, int poz_y) {
	if (poz_x<0 || poz_x>=szerokosc || poz_y<0 || poz_y>=wysokosc) {
		return true;
	}
	return false;
}
bool czy_jest_obok(wlasciwosci_pola_t **mapa,s_wspolrzedne_pola poz_gracza,int wysokosc,int szerokosc,int nr_przeciwnika) {
	int i, j,badane_x,badane_y;
	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			badane_x = poz_gracza.x + j;
			badane_y = poz_gracza.y + i;
			if (!poza_mapa(wysokosc, szerokosc, badane_x, badane_y)) {
				if (mapa[badane_y][badane_x].przeciwnik_id == nr_przeciwnika) {
					return true;
				}
			}
		}
	}
	return false;
}
void ladowanie_statystyk_przeciwnikow(przeciwnicy_statystyki_t*statystyki) {
	//wilk
	statystyki[0].atak = 2;
	statystyki[0].obrona = 2;
	statystyki[0].predkosc = 2;
	//szczur
	statystyki[1].atak = 1;
	statystyki[1].obrona = 1;
	statystyki[1].predkosc = 1;
	//zywiolak
	statystyki[2].atak = 10000;
	statystyki[2].obrona = 10000;
	statystyki[2].predkosc = 1;
}
void atak(wlasciwosci_pola_t**mapadane,int ostatni_kierunek,int poz_x,int poz_y,dane_przeciwnikow_t *przeciwnicy,int *ekwipunek,przeciwnicy_statystyki_t *statystyki_przeciwnikow,przedmiot_t *stat_przedmiotow){
	int atak_bohatera;
	if (ekwipunek[0] == 0) {
		atak_bohatera = 0;
	}
	else {
		atak_bohatera = stat_przedmiotow[ekwipunek[0] - 1].atak;
	}
	switch (ostatni_kierunek) {
	case 0:
		if (mapadane[poz_y-1][poz_x].przeciwnik_id!=0) {
			if (przeciwnicy[mapadane[poz_y - 1][poz_x].przeciwnik_id - 1].zdrowie > 0) {
				if (atak_bohatera - statystyki_przeciwnikow[przeciwnicy[mapadane[poz_y - 1][poz_x].przeciwnik_id - 1].nr_przeciwnika - 1].obrona > 0) {
					przeciwnicy[mapadane[poz_y - 1][poz_x].przeciwnik_id - 1].zdrowie -= atak_bohatera + statystyki_przeciwnikow[przeciwnicy[mapadane[poz_y - 1][poz_x].przeciwnik_id - 1].nr_przeciwnika - 1].obrona;
				}
			}
		}
		break;
	case 1:
		if (mapadane[poz_y + 1][poz_x].przeciwnik_id != 0) {
			if (przeciwnicy[mapadane[poz_y + 1][poz_x].przeciwnik_id - 1].zdrowie > 0) {
				if (atak_bohatera - statystyki_przeciwnikow[przeciwnicy[mapadane[poz_y + 1][poz_x].przeciwnik_id - 1].nr_przeciwnika - 1].obrona > 0) {
					przeciwnicy[mapadane[poz_y + 1][poz_x].przeciwnik_id - 1].zdrowie -= atak_bohatera + statystyki_przeciwnikow[przeciwnicy[mapadane[poz_y + 1][poz_x].przeciwnik_id - 1].nr_przeciwnika - 1].obrona;
					
				}
			}
		}
		break;
	case 2:
		if (mapadane[poz_y][poz_x - 1].przeciwnik_id != 0) {
			if (przeciwnicy[mapadane[poz_y][poz_x - 1].przeciwnik_id - 1].zdrowie > 0) {
				if (atak_bohatera - statystyki_przeciwnikow[przeciwnicy[mapadane[poz_y][poz_x - 1].przeciwnik_id - 1].nr_przeciwnika - 1].obrona > 0) {
					przeciwnicy[mapadane[poz_y][poz_x - 1].przeciwnik_id - 1].zdrowie -= atak_bohatera + statystyki_przeciwnikow[przeciwnicy[mapadane[poz_y][poz_x - 1].przeciwnik_id - 1].nr_przeciwnika - 1].obrona;
					
				}
			}
		}
		break;
	case 3:
		if (mapadane[poz_y][poz_x + 1].przeciwnik_id != 0) {
			if (przeciwnicy[mapadane[poz_y][poz_x + 1].przeciwnik_id - 1].zdrowie > 0) {
				if (atak_bohatera - statystyki_przeciwnikow[przeciwnicy[mapadane[poz_y][poz_x + 1].przeciwnik_id - 1].nr_przeciwnika - 1].obrona > 0) {
					przeciwnicy[mapadane[poz_y][poz_x + 1].przeciwnik_id - 1].zdrowie -= atak_bohatera + statystyki_przeciwnikow[przeciwnicy[mapadane[poz_y][poz_x + 1].przeciwnik_id - 1].nr_przeciwnika - 1].obrona;
					
				}
			}
		}
		break;

	}
}

