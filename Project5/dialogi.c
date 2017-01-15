#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "dialogi.h"
void dialogi(ALLEGRO_FONT *czcionka, int rodzaj_npc, int* nr_odp, int *rozmowa, int *zad, float czas, bool *wziecie_czasu, bool *wybor, int *ilosc_opcji, int *drugi_wybor, int numer_mapy,int *przedmioty_zalozone,int przedmioty_w_ekwipunku[][3]) {
	al_init();
	al_init_font_addon();
	rodzaj_npc--;
	//zadanie 1;
	if (*zad == 1) {
		if (numer_mapy == 0) {
			if (rodzaj_npc == 0) {
				al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Nie znam Cie,zostaw mnie w spokoju");
				if (czas > 3) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 1) {
				if (*nr_odp == 0 && czas < 3) {
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Przepraszam, zgubiles sie?");
				}
				if (czas > 3 && *nr_odp == 0) {
					*wybor = true;
					*ilosc_opcji = 3;
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Jestem tu nowy");
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 590, 0, "Moglbys mi pomoc?");
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 620, 0, "Co to za miejsce?");
				}
				if (*nr_odp == 3 && czas > 3) {
					*nr_odp = 0;
				}
				if (*nr_odp == 2 || *nr_odp == 1) {
					if (czas < 4 && *drugi_wybor == 0) {
						al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Oczywiscie jestes pewnie glodny, ale nie ma nic za ");
						al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 590, 0, "darmo. Gdzies w miescie znajduje sie studnia, moglbys ");
						al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 620, 0, "byc uprzejmym misiem i pomogl starcowi przyniesc");
						al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 650, 0, "wode?");
					}
					if (czas > 4 && *drugi_wybor == 0) {
						*wybor = true;
						*ilosc_opcji = 2;
						al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Tak");
						al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 590, 0, "Nie, dziekuje");
					}
					if (czas < 3 && *drugi_wybor == 1) {
						al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Zatem ruszaj, ja w tym czasie udam sie do domu");
						al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 590, 0, "latwo go znajdziesz, jest wyjatkowy");
					}
					if (czas < 3 && *drugi_wybor == 2) {
						al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Wiec radz sobie sam");
					}
					if (czas > 3 && (*drugi_wybor == 2 || *drugi_wybor == 1)) {
						*drugi_wybor = 0;
						*nr_odp = 0;
						*rozmowa = 0;
						*zad = 2;
					}
				}
				if (*nr_odp == 3 && czas < 3) {
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Jest to Kraina Polaris, Jestesmy w miescie Podlesno");
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 590, 0, "wolne lozka");
				}
			}
			if (rodzaj_npc == 2) {
				al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Przepraszam, nie mam czasu");
				if (czas > 3) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 3) {
				al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Czemu mnie zaczepiasz? Straz! Pomocy!!");
				if (czas > 3) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 4) {
				al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Nie jestes w mojej lidze. Splywaj");
				if (czas > 3) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 10) {
				al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "HA! HA! HA!, Odwal sie!!");
				if (czas > 3) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 11) {
				al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Splywaj!");
				if (czas > 3) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 12) {
				if (czas < 3) {
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Warzywa! Warzywa!");
				}
				if (czas > 3 && czas < 6) {
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Kupujesz czy sie gapisz?");
				}
				if (czas > 6) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 13) {
				if (czas < 3) {
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Ryby! swieze ryby");
				}
				if (czas > 3 && czas < 6) {
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "To nie wystawa, straszysz mi klientow");
				}
				if (czas > 6) {
					*rozmowa = 0;
				}
			}
		}
	}
	if (*zad == 2) {
		if (numer_mapy == 0) {
			if (rodzaj_npc == 0) {
				al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Nie znam Cie,zostaw mnie w spokoju");
				if (czas > 3) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 1) {
				if (czas < 3 && *nr_odp == 0) {
					*wybor = false;
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Witaj, masz juz wiadro wody?");
				}
				if (czas > 3 && *nr_odp==0) {
					*wybor = true;
					*ilosc_opcji = 2;
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Tak");
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 590, 0, "Nie");
				}
				if (czas < 3 && *nr_odp==1) {
					if (sprawdzenie_czy_przedmiot_jest_w_ekwipunku(przedmioty_zalozone,przedmioty_w_ekwipunku,1,3)) {
						al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "ok");
					}
					else {
						al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Przykro mi ale nie masz wiadra wody");
					}
				}
				if (czas > 3 && *nr_odp == 1) {
					if (sprawdzenie_czy_przedmiot_jest_w_ekwipunku(przedmioty_zalozone, przedmioty_w_ekwipunku, 1, 3)) {
						*zad = 3;
						*rozmowa = 0;
						zabranie_przedmioty_z_ekwipunku(przedmioty_zalozone, przedmioty_w_ekwipunku, 1, 3);
					}
					else
					{
						*rozmowa = 0;
					}
				}
				if (*nr_odp == 2 && czas<3) {
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Zatem czekam");
				}
				if (*nr_odp == 2 && czas > 3) {
					*rozmowa = false;
				}
			}
			if (rodzaj_npc == 2) {
				al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Przepraszam, nie mam czasu");
				if (czas > 3) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 3) {
				al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Czemu mnie zaczepiasz? Straz! Pomocy!!");
				if (czas > 3) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 4) {
				al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Nie jestes w mojej lidze. Splywaj");
				if (czas > 3) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 10) {
				al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "HA! HA! HA!, Odwal sie!!");
				if (czas > 3) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 11) {
				al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Splywaj!");
				if (czas > 3) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 12) {
				if (czas < 3) {
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Warzywa! Warzywa!");
				}
				if (czas > 3 && czas < 6) {
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Kupujesz czy sie gapisz?");
				}
				if (czas > 6) {
					*rozmowa = 0;
				}
			}
			if (rodzaj_npc == 13) {
				if (czas < 3) {
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Ryby! swieze ryby");
				}
				if (czas > 3 && czas < 6) {
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "To nie wystawa, straszysz mi klientow");
				}
				if (czas > 6) {
					*rozmowa = 0;
				}
			}
		}
	}
}
void zamiana_itemu_w_ekwipunku(int *przedmioty_zaolozone, int przedmioty_w_ekwipunku[][3],int z_czego,int na_co,int typ_itemu) {
	bool zamieniono = false;
	int i, j;
	if (przedmioty_zaolozone[typ_itemu] == z_czego) {
		przedmioty_zaolozone[typ_itemu] = na_co;
	}
	else {
		for (i = 0; i < 5; i++) {
			for (j = 0; j < 3; j++) {
				if (!zamieniono && przedmioty_w_ekwipunku[i][j] == z_czego) {
					zamieniono = true;
					przedmioty_w_ekwipunku[i][j] = na_co;
				}
			}
		}
	}

}
void eventy(int ktory_event,int poz_gracza_x, int poz_gracza_y, int *przedmioty_zaolozone, int przedmioty_w_ekwipunku[][3]) {
	switch (ktory_event)
	{
	case 1:
		zamiana_itemu_w_ekwipunku(przedmioty_zaolozone, przedmioty_w_ekwipunku, 3, 1,3);
		break;
	}
}
bool sprawdzenie_czy_przedmiot_jest_w_ekwipunku(int*przedmioty_zalozone, int przedmioty_w_ekwipunku[][3], int czego_szukamy,int typ_itemu) {
	int i, j;
	if (przedmioty_zalozone[typ_itemu] == czego_szukamy) {
		return true;
	}
	else {
		for (i = 0; i < 5; i++) {
			for (j = 0; j < 3; j++) {
				if (przedmioty_w_ekwipunku[i][j] == czego_szukamy) {
					return true;
				}
			}
		}
	}
	return false;
}
void zabranie_przedmioty_z_ekwipunku(int*przedmioty_zalozone, int przedmioty_w_ekwipunku[][3], int czego_szukamy, int typ_itemu) {
	bool zabrano = false;
	int i, j;
	if (przedmioty_zalozone[typ_itemu] == czego_szukamy) {
		przedmioty_zalozone[typ_itemu] = 0;
	}
	else {
		for (i = 0; i < 5; i++) {
			for (j = 0; j < 3; j++) {
				if (przedmioty_w_ekwipunku[i][j] == czego_szukamy) {
					przedmioty_w_ekwipunku[i][j] = 0;
					zabrano = true;
				}
			}
		}
	}
	return false;
}
bool sprawdzenie_czy_sa_przeciwnicy_na_aktualnej_mapie() {

}