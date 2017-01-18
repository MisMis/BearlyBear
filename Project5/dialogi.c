#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "dialogi.h"
void dialogi(wlasciwosci_pola_t **dane, ALLEGRO_FONT *czcionka, int rodzaj_npc, int* nr_odp, int *rozmowa, int *zad, float czas, bool *wziecie_czasu, bool *wybor, int *ilosc_opcji, int *drugi_wybor, int numer_mapy, int *przedmioty_zalozone, int przedmioty_w_ekwipunku[][3], bool *zmianamapy, int*stary_numer_bitmapy, char***nazwy_plikow,bool *stara_pozycja) {
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
					if (czas > 3 && *drugi_wybor == 1) {
						*zmianamapy = true;
						*stary_numer_bitmapy = 0;
						dodanie_npc_do_innej_mapy(nazwy_plikow, 11, 11, 2, 1);
						dane[27][42].npc = 0;
						*stara_pozycja = true;
						*drugi_wybor = 0;
						*nr_odp = 0;
						*rozmowa = 0;
						*zad = 2;
					}
					if(czas>3 && *drugi_wybor == 2){
						*drugi_wybor = 0;
						*nr_odp = 0;
						*rozmowa = 0;
					}
				}
				if (*nr_odp == 3 && czas < 3) {
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Jest to Kraina Polaris, Jestesmy w miescie Podlesno");
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
		if (numer_mapy == 1) {
			if (rodzaj_npc == 1) {
				if (czas < 3 && *nr_odp == 0) {
					*wybor = false;
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Witaj, masz juz wiadro wody?");
				}
				if (czas > 3 && *nr_odp == 0) {
					*wybor = true;
					*ilosc_opcji = 2;
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Tak");
					al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 590, 0, "Nie");
				}
				if (czas < 3 && *nr_odp == 1) {
					if (sprawdzenie_czy_przedmiot_jest_w_ekwipunku(przedmioty_zalozone, przedmioty_w_ekwipunku, 1, 3)) {
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
						zapis_pliku_leafe_z_pliku_do_pliku("data/mapy/karczma.leafe", "wynik.leafe");
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
				if (!zabrano && przedmioty_w_ekwipunku[i][j] == czego_szukamy) {
					przedmioty_w_ekwipunku[i][j] = 0;
					zabrano = true;
				}
			}
		}
	}
}
bool sprawdzenie_czy_sa_przeciwnicy_na_aktualnej_mapie(wlasciwosci_pola_t** mapa,int wysokosc,int szerokosc,int ktorego_wroga_szukamy) {
	int i, j;
	for (i = 0; i < wysokosc; i++) {
		for (j = 0; j < szerokosc; j++) {
			if (mapa[i][j].przeciwnik == ktorego_wroga_szukamy) {
				return true;
			}
		}
	}
	return false;
}
bool sprawdzenie_czy_sa_przeciwnicy_na_innej_mapie(char* nazwa_mapy,int ktorego_wroga_szukamy) {
	FILE *plik;
	int wysokosc, szerokosc, xspawnu, yspawnu;
	int i, j;
	int pomoc,to_co_trzeba;
	fopen_s(&plik,nazwa_mapy, "r");
	fscanf_s(plik, "%i", &wysokosc);
	fscanf_s(plik, "%i", &szerokosc);
	fscanf_s(plik, "%i", &xspawnu);
	fscanf_s(plik, "%i", &yspawnu);
	for (i = 0; i < wysokosc; i++) {
		for (j = 0; j < szerokosc; j++) {
			fscanf_s(plik, "%i", &pomoc);
			fscanf_s(plik, "%i", &pomoc);
			fscanf_s(plik, "%i", &pomoc);
			fscanf_s(plik, "%i", &to_co_trzeba);
			fscanf_s(plik, "%i", &pomoc);
			fscanf_s(plik, "%i", &pomoc);
			if (to_co_trzeba == ktorego_wroga_szukamy) {
				return true;
			}
		}
	}
	fclose(plik);
	return false;
}
void dodanie_npc_do_innej_mapy(char*** nazwy_plikow,int na_poz_x,int na_poz_y,int ktory_npc,int na_ktora_mape) {
	FILE *plik;
	fopen_s(&plik, nazwy_plikow[na_ktora_mape][3], "r");
	int wysokosc, szerokosc, xspawnu, yspawnu,i;
	fscanf_s(plik, "%i", &wysokosc);
	fscanf_s(plik, "%i", &szerokosc);
	fscanf_s(plik, "%i", &xspawnu);
	fscanf_s(plik, "%i", &yspawnu);
	wlasciwosci_pola_t **dane;
	dane = (wlasciwosci_pola_t **)malloc(wysokosc * sizeof(wlasciwosci_pola_t*));
	for (i = 0; i < wysokosc; i++) {
		dane[i] = (wlasciwosci_pola_t*)malloc(szerokosc * sizeof(wlasciwosci_pola_t));
	}
	wczytanie_z_pliku(plik, dane, wysokosc, szerokosc);
	dane[na_poz_y][na_poz_x].npc = ktory_npc;
	fclose(plik);
	fopen_s(&plik, nazwy_plikow[na_ktora_mape][3], "w");
	zapis_do_pliku(plik,dane,wysokosc,szerokosc,xspawnu,yspawnu);
	fclose(plik);
	for (i = 0; i < wysokosc; i++) {
		free(dane[i]);
	}
	free(dane);
}
void zapis_pliku_leafe_z_pliku_do_pliku(char*nazwa_wejscia, char*nazwa_wyjscia) {
	FILE *wejscie;
	FILE *wyjscie;
	int wysokosc, szerokosc, xspawnu, yspawnu,i,j,k;
	int tmp;
	fopen_s(&wejscie, nazwa_wejscia, "r");
	fopen_s(&wyjscie, nazwa_wyjscia, "w");
	fscanf_s(wejscie, "%i", &wysokosc);
	fscanf_s(wejscie, "%i", &szerokosc);
	fscanf_s(wejscie, "%i", &xspawnu);
	fscanf_s(wejscie, "%i", &yspawnu);
	fprintf(wyjscie, "%i", wysokosc);
	fprintf(wyjscie, "\n");
	fprintf(wyjscie, "%i", szerokosc);
	fprintf(wyjscie, "\n");
	fprintf(wyjscie, "%i", xspawnu);
	fprintf(wyjscie, "\n");
	fprintf(wyjscie, "%i", yspawnu);
	fprintf(wyjscie, "\n");
	for (i = 0; i < wysokosc; i++) {
		for (j = 0; j < szerokosc; j++) {
			for (k = 0; k < 6; k++) {
				fscanf_s(wejscie, "%i", &tmp);
				fprintf(wyjscie, "%i ", tmp);
			}
		}
		fprintf(wyjscie, "\n");
	}
	fclose(wyjscie);
	fclose(wejscie);
}
void zapis_wszystkich_plikow(char*** nazwy_plikow,int ilosc_plikow,int pliki_wejscia,int pliki_wyjscia) 
{
	int i;
	for (i = 0; i < ilosc_plikow; i++) {
		zapis_pliku_leafe_z_pliku_do_pliku(nazwy_plikow[i][pliki_wejscia], nazwy_plikow[i][pliki_wyjscia]);
	}
}
void menu(ALLEGRO_FONT *czczionka,ALLEGRO_COLOR *kolory,int wybor,ALLEGRO_BITMAP*grafika,bool jest_gra) {
	int i;
	for (i = 0; i < 5; i++) {
		kolory[i] = al_map_rgb(0, 0, 0);
	}
	if (!jest_gra) {
		kolory[0] = al_map_rgb(156, 156, 156);
		kolory[3] = al_map_rgb(156, 156, 156);
	}
	kolory[wybor] = al_map_rgb(0, 205, 70);
	al_draw_bitmap(grafika, 0, 0, 0);
	al_draw_text(czczionka, kolory[0], 470, 420, 0, "kontynuuj");
	al_draw_text(czczionka, kolory[1], 470, 470, 0, "nowa Gra");
	al_draw_text(czczionka, kolory[2], 470, 520, 0, "wczytaj");
	al_draw_text(czczionka, kolory[3], 470, 570, 0, "zapisz");
	al_draw_text(czczionka, kolory[4], 470, 620, 0, "wyjscie");
	al_flip_display();
}
void wybor_z_menu(int pole_wybrane, bool *zakonczenie,bool *gra,char*** nazwy_plikow,int ilosc_plikow,bool *jest_gra,bool *jest_pozycja, int przedmioty_w_ekwipunku[][3], int *przedmioty_zalozone, int *zdrowie, int *nr_zad, int *numerbitmapy, int *stary_numer_bitmapy, int *pozycja_kontunulowaniax, int *pozycja_kontunulowaniay) {
	FILE *plik;
	switch (pole_wybrane)
	{
	case 0:
		if (*jest_gra) {
			*gra = true;
			*jest_pozycja = true;
		}
		break;
	case 1:
		zapis_wszystkich_plikow(nazwy_plikow,ilosc_plikow,1,3);
		*gra = true;
		*jest_gra = true;
		break;
	case 2:
		fopen_s(&plik,"save/mapy/miastosav.leafe","r");
			if (plik) {
				fclose(plik);
				zapis_wszystkich_plikow(nazwy_plikow, ilosc_plikow, 2, 3);
				*gra = true;
				*jest_gra = true;
				*jest_pozycja = true;
				wczytanie_danych_poczatkowych(przedmioty_w_ekwipunku, przedmioty_zalozone, zdrowie, nr_zad, numerbitmapy, stary_numer_bitmapy, pozycja_kontunulowaniax, pozycja_kontunulowaniay);
			}
		break;
	case 3:
		zapis_wszystkich_plikow(nazwy_plikow, ilosc_plikow, 3, 2);
		zapis_danych_poczatkowych(przedmioty_w_ekwipunku, przedmioty_zalozone, *zdrowie, *nr_zad, *numerbitmapy, *stary_numer_bitmapy, *pozycja_kontunulowaniax, *pozycja_kontunulowaniay, jest_pozycja);
		break;
	case 4:
		*zakonczenie = true;
		*gra = false;
		break;
	}
}
void zapis_do_pliku(FILE *wyjscie, wlasciwosci_pola_t **dane, int wysokosc, int szerokosc, int xspawnu, int yspawnu) {
	int i, j;
	fprintf(wyjscie, "%i", wysokosc);
	fprintf(wyjscie, "\n");
	fprintf(wyjscie, "%i", szerokosc);
	fprintf(wyjscie, "\n");
	fprintf(wyjscie, "%i", xspawnu);
	fprintf(wyjscie, "\n");
	fprintf(wyjscie, "%i", yspawnu);
	fprintf(wyjscie, "\n");
	for (j = 0; j < wysokosc; j++) {
		for (i = 0; i < szerokosc; i++) {
			fprintf(wyjscie, "%i ", dane[j][i].przeszkoda);
			fprintf(wyjscie, "%i ", dane[j][i].npc);
			fprintf(wyjscie, "%i ", dane[j][i].przejscie_mapy);
			fprintf(wyjscie, "%i ", dane[j][i].przeciwnik);
			fprintf(wyjscie, "%i ", dane[j][i].przedmiot);
			fprintf(wyjscie, "%i ", dane[j][i].zdarzenia);
		}
		fprintf(wyjscie, "\n");

	}
}
void zapis_danych_poczatkowych(int przedmioty_w_ekwipunku[][3],int *przedmioty_zalozone,int zdrowie,int nr_zad,int numerbitmapy,int stary_numer_bitmapy,int pozycja_kontunulowaniax,int pozycja_kontunulowaniay,bool *jest_pozycja) {
	FILE*plik;
	int i, j;
	fopen_s(&plik, "save/dane.sav","w");
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 3; j++) {
			fprintf_s(plik, "%i", przedmioty_w_ekwipunku[i][j]);
			fprintf_s(plik, "\n");
		}
	}
	fprintf_s(plik, "\n");
	for (i = 0; i < 6; i++)
	{
		fprintf_s(plik, "%i", przedmioty_zalozone[i]);
		fprintf_s(plik, "\n");
	}
	fprintf_s(plik, "\n");
	fprintf_s(plik, "%i", zdrowie);
	fprintf_s(plik, "\n");
	fprintf_s(plik, "%i", nr_zad);
	fprintf_s(plik, "\n");
	fprintf_s(plik, "%i", numerbitmapy);
	fprintf_s(plik, "\n");
	fprintf_s(plik, "%i", stary_numer_bitmapy);
	fprintf_s(plik, "\n");
	fprintf_s(plik, "%i", pozycja_kontunulowaniax);
	fprintf_s(plik, "\n");
	fprintf_s(plik, "%i", pozycja_kontunulowaniay);
	*jest_pozycja = true;
	fclose(plik);
}
void wczytanie_danych_poczatkowych(int przedmioty_w_ekwipunku[][3], int *przedmioty_zalozone, int *zdrowie, int *nr_zad, int *numerbitmapy, int *stary_numer_bitmapy, int *pozycja_kontunulowaniax, int *pozycja_kontunulowaniay) {
	FILE *plik;
	int i, j;
	fopen_s(&plik, "save/dane.sav", "r");
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 3; j++) {
			fscanf_s(plik, "%i", &przedmioty_w_ekwipunku[i][j]);
		}
	}
	for (i = 0; i < 6; i++) {
		fscanf_s(plik, "%i", &przedmioty_zalozone[i]);
	}
	fscanf_s(plik, "%i", zdrowie);
	fscanf_s(plik, "%i", nr_zad);
	fscanf_s(plik, "%i", numerbitmapy);
	fscanf_s(plik, "%i", stary_numer_bitmapy);
	fscanf_s(plik, "%i", pozycja_kontunulowaniax);
	fscanf_s(plik, "%i", pozycja_kontunulowaniay);
	fclose(plik);
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