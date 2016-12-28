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
	bool przyszle_poz_npc;
};
struct dane_npc {
	bool rozmawia;
	int npc_id;
	int nr_npc;
	int pozx;
	int pozy;
	int poz_docelowa_x;
	int poz_docelowa_y;
	int maxx;
	int maxy;
	int minx;
	int miny;
	clock_t czas;
};
struct przedmiot {
	int typ;
	char nazwa[20];
	char opis[50];
};
struct typy_przedmiotow {
	char nazwa[20];
	int x;
	int y;
};
typedef struct wlasciwosci_pola wlasciwosci_pola_t;
typedef struct dane_npc dane_npc_t;
typedef struct przedmiot przedmiot_t;
typedef struct typy_przedmiotow typy_przedmiotow_t;
void wczytanie_z_pliku(FILE *wyjscie, wlasciwosci_pola_t **dane, int wysokosc, int szerokosc) {
	int i, j;
	int pomoc[3];
	for (j = 0; j < wysokosc; j++) {
		for (i = 0; i < szerokosc; i++) {
			fscanf_s(wyjscie, "%i", &pomoc[0]);
			fscanf_s(wyjscie, "%i", &pomoc[1]);
			fscanf_s(wyjscie, "%i", &pomoc[2]);
			dane[j][i].przeszkoda = (bool)pomoc[0];
			dane[j][i].npc = pomoc[1];
			dane[j][i].przejscie_mapy = pomoc[2];
		}
	}
}
void czytanie_nazw(char *nazwy[][2]) {

	nazwy[0][0] = "data/mapy/miasto.png";
	nazwy[0][1] = "data/mapy/mapa.leafe";
	nazwy[1][0] = "data/mapy/wnetrze.png";
	nazwy[1][1] = "data/mapy/wnetrze.leafe";

}
void czytanie_nazw_przeciwnikow(char **nazwy) {
	nazwy[0] = "data/postacie/wilk.png";
}
void ladowanie_opisow_przedmiotow(przedmiot_t *tablica);
void ladowanie_typow_przedmiotow(typy_przedmiotow_t *tablica);
void czytanie_nazw_npc(char **nazwy) {
	nazwy[0] = "data/postacie/mieszkaniec.png";
	nazwy[1] = "data/postacie/starzec.png";
}
bool dodaj_przedmiot_do_ekwipunku(int ekwipunek[][3], int id_przedmiotu) {
	int i, j;
	for(i=0;i<5;i++){
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
	if (struktura_mapy[poz_y][poz_x].przeszkoda == true || struktura_mapy[poz_y][poz_x].npc_id != 0 || struktura_mapy[poz_y][poz_x].przyszle_poz_npc == true) {
		return true;
	}
	return false;
}
int sprawdzenie_do_rozmowy(wlasciwosci_pola_t **dane, int poz_x, int poz_y,int wysokosc_mapy,int szerokosc_mapy,dane_npc_t *dane_do_npc) {
	int max_x=poz_x+1;
	int min_x=poz_x-1;
	int max_y=poz_y+1;
	int min_y=poz_y-1;
	int npc_id;
	int max_kratek_x = szerokosc_mapy;
	int max_kratek_y = wysokosc_mapy;
	int i, j;
	for (i = min_y; i <=max_y; i++) {
		for (j = min_x; j <= max_x; j++) {
			if (i >= 0 && j >= 0 && i <= max_kratek_y&& j <= max_kratek_x) {
				if (dane[i][j].npc_id != 0) {
					npc_id = dane[i][j].npc_id-1;
					if (dane_do_npc[npc_id].pozx == dane_do_npc[npc_id].poz_docelowa_x && dane_do_npc[npc_id].pozy == dane_do_npc[npc_id].poz_docelowa_y) {
						return npc_id;
					}
				}
			}
		}
	}
	return 0;
}
void zalozenie_przedmiotu(int* zalozone_przedmioty, int przedmioty_w_ekwipunku[][3],przedmiot_t *tablica,int pozx,int pozy) {
	int tmp;
	if (przedmioty_w_ekwipunku[pozy][pozx]!= 0) {
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
int main() {
	int mapax = 0;
	int mapay = 0;
	int tmp;
	int przerwa_miedzy_przejsciami_npc=3;
	int zasiegnpc = 3;
	int bohaterx = 350;
	int bohatery = 350;
	int x_ramki = 0, y_ramki = 0;
	int misjednostka = 50;
	int pozdocelowa = 350;
	int xdocelowe = mapax;
	int ydocelowe = mapay;
	int wysokosc, szerokosc,i,j,x;
	int aktualna_pozycjax;
	int aktualna_pozycjay;
	int przedmioty_w_ekwipunku[5][3];
	int przedmioty_zalozone[6];
	int zdrowie = 100;
	int nr_zad = 1,nr_odp;
	int ilosc_npc,ilosc_rysowanych_npc,ilosc_wrogow=1;
	int aktualna_poz_npc_x, aktualna_poz_npc_y;
	int numerbitmapy=0;
	int xspawnu, yspawnu,xstarejpoz=0,ystarejpoz=0;
	int do_wyboru_dialogu;
	char *nazwy_plikow[2][2];
	char *nazwy_npc[2];
	char *nazwy_przeciwnikow[1];
	int rozmowa,id,ktory_rozmawia;
	float czas_aktualny;
	clock_t czas_startowy;
	int xszukania, yszukania, szerszukania=16;
	czytanie_nazw(nazwy_plikow);
	czytanie_nazw_npc(nazwy_npc);
	czytanie_nazw_przeciwnikow(nazwy_przeciwnikow);
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
	FILE *dane_do_mapy;
	ALLEGRO_FONT *czcionka_48;
	ALLEGRO_FONT *czcionka_18;
	ALLEGRO_FONT *czcionka_18_cienka;
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
	dodaj_przedmiot_do_ekwipunku(przedmioty_w_ekwipunku, 2);
	dodaj_przedmiot_do_ekwipunku(przedmioty_w_ekwipunku, 4);
	dodaj_przedmiot_do_ekwipunku(przedmioty_w_ekwipunku, 1);
	dodaj_przedmiot_do_ekwipunku(przedmioty_w_ekwipunku, 1);
	while (!zakonczenie) {
		ALLEGRO_BITMAP **NPC = NULL;
		ALLEGRO_BITMAP *mapa = NULL;
		ALLEGRO_BITMAP **wrogowie = NULL;
		zmianamapy = false;
		fopen_s(&dane_do_mapy, nazwy_plikow[numerbitmapy][1], "r");
		fscanf_s(dane_do_mapy, "%i", &wysokosc);
		fscanf_s(dane_do_mapy, "%i", &szerokosc);
		fscanf_s(dane_do_mapy, "%i", &ilosc_npc);
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
		mapadane[10][12].przeciwnik = 1;
		mapadane[14][8].przeciwnik = 1;
		if (ilosc_npc != 0) {
			NPC = (ALLEGRO_BITMAP**)malloc(ilosc_npc * sizeof(ALLEGRO_BITMAP*));
		}
		if (ilosc_wrogow != 0) {
			wrogowie= (ALLEGRO_BITMAP**)malloc(ilosc_wrogow * sizeof(ALLEGRO_BITMAP*));
		}
		wczytanie_z_pliku(dane_do_mapy, mapadane, wysokosc, szerokosc);
		ilosc_rysowanych_npc = 0;
		//zliczenie ilosci rysowanych npc
		for (i = 0; i < wysokosc; i++) {
			for (j = 0; j < szerokosc; j++) {
				if (mapadane[i][j].npc != 0) {
					ilosc_rysowanych_npc++;
				}
			}
		}
		//deklaracja pamieci na zmienne portzebne do przemieszczanie npc
		dane_do_ruchu_npc = (dane_npc_t*)malloc(ilosc_rysowanych_npc * sizeof(dane_npc_t));
		//zerowanie npc.id na mapie i wyliczenie danych potrzebnych do ruchu
		x = 1;
		for (i = 0; i < wysokosc; i++) {
			for (j = 0; j < szerokosc; j++) {
				mapadane[i][j].przyszle_poz_npc = false; //zerowanie przyszlych poz npc potrzebnych do kolizji z npc;
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
					dane_do_ruchu_npc[tmp].nr_npc = mapadane[i][j].npc;
					dane_do_ruchu_npc[tmp].maxx = j+zasiegnpc;
					dane_do_ruchu_npc[tmp].minx = j-zasiegnpc;
					dane_do_ruchu_npc[tmp].miny = i-zasiegnpc;
					dane_do_ruchu_npc[tmp].maxy = i+zasiegnpc;
					dane_do_ruchu_npc[tmp].npc_id = mapadane[i][j].npc - 1;
					dane_do_ruchu_npc[tmp].czas = clock();
				}
			}
		}
		rozmowa = 0;
		mapa = al_load_bitmap(nazwy_plikow[numerbitmapy][0]);
		for (i = 0; i < ilosc_npc; i++) {
			NPC[i] = al_load_bitmap(nazwy_npc[i]);
		}
		for (i = 0; i < ilosc_wrogow; i++) {
			wrogowie[i] = al_load_bitmap(nazwy_przeciwnikow[i]);
		}
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
							dane_do_ruchu_npc[rozmowa].rozmawia = true;
							ktory_rozmawia = rozmowa;
							nr_odp = 0;
							do_wyboru_dialogu = 1;
						}
						break;
					}
				}

			if (ruch && mapay==ydocelowe  && mapax==xdocelowe) {
				if (ydocelowe == mapay && przyciski[0] == true && aktualna_pozycjay > yspawnu) {
					if (!kolizja(aktualna_pozycjax,aktualna_pozycjay-1,mapadane)) {
						ydocelowe += misjednostka;
					}
				}
				if (ydocelowe == mapay && przyciski[1] == true && aktualna_pozycjay < wysokosc) {
					if (!kolizja(aktualna_pozycjax, aktualna_pozycjay + 1, mapadane)) {
						ydocelowe -= misjednostka;
					}
				}
				if (xdocelowe == mapax && przyciski[2] == true && aktualna_pozycjax > xspawnu) {
					if (!kolizja(aktualna_pozycjax-1, aktualna_pozycjay, mapadane)) {
						xdocelowe += misjednostka;
					}
				}
				if (xdocelowe == mapax && przyciski[3] == true && aktualna_pozycjax < szerokosc) {
					if (!kolizja(aktualna_pozycjax+1, aktualna_pozycjay, mapadane)) {
						xdocelowe -= misjednostka;
					}
				}
			}
			//printf_s("x  %i       y %i   \n",aktualna_pozycjax, aktualna_pozycjay);
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
					mapay += 2;
				}
				if (mapay > ydocelowe) {
					mapay -= 2;
				}
				if (mapax < xdocelowe) {
					mapax += 2;
				}
				if (mapax > xdocelowe) {
					mapax -= 2;
				}
			}
			al_draw_bitmap(mapa, mapax, mapay, 0);
			xszukania = (mapax / 50)*-1;
			yszukania = (mapay / 50)*-1;
			if (ilosc_npc > 0) {
				for (i = yszukania; i < yszukania + szerszukania; i++) {
					for (j = xszukania; j < xszukania + szerszukania; j++) {
						if (i >= yspawnu && j >= xspawnu && j < szerokosc &&i < wysokosc) {
							if (mapadane[i][j].npc_id != 0) {
								tmp = mapadane[i][j].npc_id - 1;
								al_draw_bitmap(NPC[dane_do_ruchu_npc[tmp].npc_id], dane_do_ruchu_npc[tmp].pozx*-1 + mapax, dane_do_ruchu_npc[tmp].pozy*-1 + mapay, 0);
								if (dane_do_ruchu_npc[tmp].pozx == dane_do_ruchu_npc[tmp].poz_docelowa_x && dane_do_ruchu_npc[tmp].pozy == dane_do_ruchu_npc[tmp].poz_docelowa_y) {
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
									}
									if (dane_do_ruchu_npc[tmp].pozx > dane_do_ruchu_npc[tmp].poz_docelowa_x) {
										dane_do_ruchu_npc[tmp].pozx -= 1;
									}
									if (dane_do_ruchu_npc[tmp].pozy < dane_do_ruchu_npc[tmp].poz_docelowa_y) {
										dane_do_ruchu_npc[tmp].pozy += 1;
									}
									if (dane_do_ruchu_npc[tmp].pozy > dane_do_ruchu_npc[tmp].poz_docelowa_y) {
										dane_do_ruchu_npc[tmp].pozy -= 1;
									}
								}
							}
							if (mapadane[i][j].przeciwnik != 0) {
								al_draw_bitmap(wrogowie[mapadane[i][j].przeciwnik-1], j*misjednostka - mapax*-1, i*misjednostka - mapay*-1, 0);
							}
						}
					}
				}
			}
			al_draw_bitmap(bohater, bohaterx, bohatery, 0);
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
				id = dane_do_ruchu_npc[rozmowa].nr_npc;
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
							if (do_wyboru_dialogu <4) {
								do_wyboru_dialogu++;
							}
							break;
						case ALLEGRO_KEY_ENTER:
							nr_odp = do_wyboru_dialogu;
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
				dialogi(id, &nr_odp, &rozmowa, nr_zad,czas_aktualny,&wziecie_czasu,&wybor);
				if (rozmowa == 0) {
					wziecie_czasu = true;
					dane_do_ruchu_npc[ktory_rozmawia].rozmawia = false;
					ruch = true;
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
		al_destroy_bitmap(mapa);
	}
	al_destroy_bitmap(ekwipunek);
	al_destroy_bitmap(bohater);
	al_destroy_bitmap(przedmioty);
	al_destroy_display(okno);
	al_destroy_event_queue(kolejka);
	al_destroy_bitmap(serce);
	al_destroy_font(czcionka_48);
	al_destroy_font(czcionka_18);
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
	strcpy_s(tablica[0].opis, 50, "wiadro pelne wody");
	//miecz
	strcpy_s(tablica[1].nazwa, 20, "miecz");
	tablica[1].typ = 1;
	strcpy_s(tablica[1].opis, 50, "bardzo ostry miecz");
	//puste wiadro
	strcpy_s(tablica[2].nazwa, 20, "puste wiadro");
	tablica[2].typ = 4;
	strcpy_s(tablica[2].opis, 50, "puste wiadro");
	//noz
	strcpy_s(tablica[3].nazwa, 20, "noz");
	tablica[3].typ = 1;
	strcpy_s(tablica[3].opis, 50, "ostry noz");
	//widelec
	strcpy_s(tablica[4].nazwa, 20, "widelec");
	tablica[4].typ = 1;
	strcpy_s(tablica[4].opis, 50, "zazwyczaj sluzy do jedzenia");
	//moneta
	strcpy_s(tablica[5].nazwa, 20,"mismoneta");
	tablica[5].typ = 5;
	strcpy_s(tablica[5].opis, 50, "srodek platniczy");
	//jedzenie
	strcpy_s(tablica[6].nazwa, 20, "miodek");
	tablica[6].typ = 6;
	strcpy_s(tablica[6].opis, 50, "bardzo smaczny miodek");
}
