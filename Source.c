#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct stanica {
	int br;
	char ime[256];
	double longituda;
	double latituda;
	int zona;
}Stanica;
typedef struct cvor {
	Stanica s;
	struct cvor* prethodni;
	struct cvor* sledeci;
}Cvor;
  double StepeniURadijane(double st) {
	  double rad = st * 3.14 / 180;
	return rad;
}
  double rastojanje(  double lat1,   double lon1,   double lat2,   double lon2) {
	  double t1 = pow(sin((lat1 - lat2) / 2), 2);
	  double t2 = pow(sin((lon1 - lon2) / 2), 2);
	  double d = 2 * 6371 * asin(sqrt(t1 + t2 * cos(lat1) * cos(lat2)));
	return d;
}

Cvor* napraviJedan(int broj, char* naziv,   double lat,   double lon, int zona) {
	Cvor* el = malloc(sizeof(Cvor));
	el->s.br = broj;
	strcpy(el->s.ime, naziv);
	el->s.latituda = lat;
	el->s.longituda = lon;
	el->s.zona = zona;
	el->sledeci = NULL;
	return el;
}

Cvor* napravi(char* linija) {
	FILE* f = fopen(linija, "r");
	if (!f) {
		printf("DAT_GRESKA");
		exit(0);
	}
	Cvor* glava = NULL, * posl = glava, *el;
	int br = 0, zona = 0;
	  double lat = 0, lon = 0;
	char naziv[256];
	while (fscanf(f, "%d!%[^!]!%lf!%lf!%d\n", &br, naziv, &lat, &lon, &zona) == 5) {
		el = napraviJedan(br, naziv, lat, lon, zona);
		if (!glava) {
			el->prethodni = NULL;
			glava = el;
		}
		else {
			posl->sledeci = el;
			el->prethodni = posl;
		}
		posl = el;
	}
	fclose(f);
	return glava;
}

void ispisListe(Cvor* glava) {
	Cvor* trenutni;
	for(trenutni = glava;trenutni!=NULL;trenutni = trenutni->sledeci)
		printf("%d %s %.7lf %.7lf %d->\n", trenutni->s.br, trenutni->s.ime, trenutni->s.latituda, trenutni->s.longituda, trenutni->s.zona);
	printf("NULL");
}

int brojElemenata(Cvor* glava) {
	int i = 0;
	Cvor* tmp = glava;
	while (tmp) {
		i++;
		tmp = tmp->sledeci;
	}
	return i;
}
int minInd(double* niz,int len) {
	double min = niz[0];
	int i;
	for (i = 0;i < len;i++) {
		if (niz[i] < min)
			min = niz[i];
	}
	for (i = 0;i < len;i++) {
		if (niz[i] == min)
			return i;
	}
}

Cvor* vratiPoslednji(Cvor* glava) {
	Cvor* tmp = glava;
	while (tmp->sledeci)
		tmp = tmp->sledeci;
	return tmp;
}

void oslobodi(Cvor* glava) {
	Cvor* tmp;
	while (glava) {
		tmp = glava;
		glava = glava->sledeci;
		free(tmp);
	}
}

Cvor* obrada(Cvor* glava1, Cvor* glava2, double lat1, double lon1, double lat2, double lon2, char *c) { //glava1 je dirA, glava2 je dirB
	Cvor* glava3 = NULL, *posl3 = glava3, *glava4 = NULL,*posl4 = glava4;
	double x1 = StepeniURadijane(lat1), y1 = StepeniURadijane(lon1), x2 = StepeniURadijane(lat2), y2 = StepeniURadijane(lon2);
	int len = brojElemenata(glava1);
	double* niz = malloc(len * sizeof(double));
	if (!niz) { printf("MEM_GRESKA"); exit(0); }
	Cvor* tmp = glava1;
	int i = 0;
	while (tmp) {
		double la = StepeniURadijane(tmp->s.latituda);
		double lo = StepeniURadijane(tmp->s.longituda);
		double d = rastojanje(x1, y1, la, lo);
		niz[i++] = d;
		tmp = tmp->sledeci;
	}
	int ind = minInd(niz, len);
	free(niz);
	tmp = glava1;
	i = 0;
	while (tmp) {
		if (i != ind)
			tmp = tmp->sledeci;
		else
			break;
		i++;
	}
	//sada smo na elementu koji bi mogao biti pocetna stanica. na osnovu promene rastojanja znacemo da li je smer dobar
	if (tmp->sledeci) { //ako je tmp poslednji element liste, jasno je da ovo ne pije vodu
		double la = StepeniURadijane(tmp->s.latituda);
		double lo = StepeniURadijane(tmp->s.longituda);
		double d1 = rastojanje(la, lo, x2, y2);
		double la1 = StepeniURadijane(tmp->sledeci->s.latituda);
		double lo1 = StepeniURadijane(tmp->sledeci->s.longituda);
		double d2 = rastojanje(la1, lo1, x2, y2);
		if ((d1 - d2) > 0) { //ovo znaci da se rastojanje smanjuje, dakle to nam treba
			double* niz1 = malloc((len - ind) * sizeof(double));
			if (!niz1) { printf("MEM_GRESKA"); exit(0); }
			Cvor* tmp2 = tmp;
			i = 0;
			while (tmp2) {
				double la = StepeniURadijane(tmp2->s.latituda);
				double lo = StepeniURadijane(tmp2->s.longituda);
				double d = rastojanje(la, lo, x2, y2);
				niz1[i++] = d;
				tmp2 = tmp2->sledeci;
			}
			int ind2 = minInd(niz1, len - ind) + 1;
			free(niz1);
			i = 0;
			while ((i != ind2) && tmp) {
				Cvor* el = malloc(sizeof(Cvor));
				*el = *tmp;
				el->sledeci = NULL;
				if (!glava3) {
					glava3 = el;
					el->prethodni;
				}
				else {
					posl3->sledeci = el;
					el->prethodni = posl3;
				}
				posl3 = el;
				tmp = tmp->sledeci;
				i++;
			}
			strcpy(c, "A");
		}
		else goto drugi;
	}
drugi: //za smer b
	len = brojElemenata(glava2);
	niz = malloc(len * sizeof(double));
	if (!niz) { printf("MEM_GRESKA"); exit(0); }
	tmp = glava2;
	i = 0;
	while (tmp) {
		double la = StepeniURadijane(tmp->s.latituda);
		double lo = StepeniURadijane(tmp->s.longituda);
		double d = rastojanje(x1, y1, la, lo);
		niz[i++] = d;
		tmp = tmp->sledeci;
	}
	ind = minInd(niz, len);
	free(niz);
	tmp = glava2;
	i = 0;
	while (tmp) {
		if (i != ind)
			tmp = tmp->sledeci;
		else
			break;
		i++;
	}
	//sada smo na elementu koji bi mogao biti pocetna stanica. na osnovu promene rastojanja znacemo da li je smer dobar
	if (tmp->sledeci) { //ako je tmp poslednji element liste, jasno je da ovo ne pije vodu
		double la = StepeniURadijane(tmp->s.latituda);
		double lo = StepeniURadijane(tmp->s.longituda);
		double d1 = rastojanje(la, lo, x2, y2);
		double la1 = StepeniURadijane(tmp->sledeci->s.latituda);
		double lo1 = StepeniURadijane(tmp->sledeci->s.longituda);
		double d2 = rastojanje(la1, lo1, x2, y2);
		if ((d1 - d2) > 0) { //ovo znaci da se rastojanje smanjuje, dakle to nam treba
			double* niz1 = malloc((len - ind) * sizeof(double));
			if (!niz1) { printf("MEM_GRESKA"); exit(0); }
			Cvor* tmp2 = tmp;
			i = 0;
			while (tmp2) {
				double la = StepeniURadijane(tmp2->s.latituda);
				double lo = StepeniURadijane(tmp2->s.longituda);
				double d = rastojanje(la, lo, x2, y2);
				niz1[i++] = d;
				tmp2 = tmp2->sledeci;
			}
			int ind2 = minInd(niz1, len - ind) + 1;
			free(niz1);
			i = 0;
			while ((i != ind2) && tmp) {
				Cvor* el = malloc(sizeof(Cvor));
				*el = *tmp;
				el->sledeci = NULL;
				if (!glava4) {
					glava4 = el;
					el->prethodni;
				}
				else {
					posl4->sledeci = el;
					el->prethodni = posl4;
				}
				posl4 = el;
				tmp = tmp->sledeci;
				i++;
			}
			strcpy(c, "B");
		}
	}
	if (!glava3) {
		if (!glava4) { //obe su prazne
			return NULL;
		}
		else { //samo smer b
			strcpy(c, "B");
			return glava4;
		}
	}
	else {
		if (!glava4) { //samo smer a
			strcpy(c, "A");
			return glava3;
		}
		else { //obe su pune, gledamo ciji je krajnji element blizi tacki x2,y2
			Cvor* posl3 = vratiPoslednji(glava3);
			Cvor* posl4 = vratiPoslednji(glava4);
			double d3 = rastojanje(StepeniURadijane(posl3->s.latituda), StepeniURadijane(posl3->s.longituda), x2, y2);
			double d4 = rastojanje(StepeniURadijane(posl4->s.latituda), StepeniURadijane(posl4->s.longituda), x2, y2);
			if (d3 < d4) {
				
				strcpy(c, "A");
				oslobodi(glava4);
				return glava3;
			}
			else {
				
				strcpy(c, "B");
				oslobodi(glava3);
				return glava4;
			}
		}
	}
}

void izlaz(Cvor *lista, char *ime,char *linija, char *c) {
	FILE* f = fopen(ime, "a");
	fprintf(f, "%s!%s\n", linija, c);
	Cvor* trenutni = lista;
	for (trenutni = lista;trenutni != NULL;trenutni = trenutni->sledeci)
		fprintf(f, "%d!%s!%.6lf!%.6lf!%d\n", trenutni->s.br, trenutni->s.ime, trenutni->s.latituda, trenutni->s.longituda, trenutni->s.zona);
	fclose(f);
}

int main(int argc, char **argv) {
	if (argc < 7) {
		printf("ARG_GRESKA - nisu uneti argumenti");
		return 0;
	}
	double x1 = atof(argv[2]),y1 = atof(argv[3]), x2 = atof(argv[4]), y2 = atof(argv[5]);
	if (x1 == 0. || y1 == 0. || x2 == 0. || y2 == 0.) {
		printf("ARG_GRESKA - nevalidni argumenti");
		return 0;
	}
	Cvor** lista;
	Cvor* glava1 = NULL, * glava2 = NULL;
	int i,size = 0;
	for (i = 6;i < argc;i++) {
		size++;
	}
	char linijaA[13], linijaB[13];
	lista = malloc(size * sizeof(Cvor*));
	for (i = 0;i < size;i++) {
		char c[2];
		strcpy(linijaA, argv[i + 6]);
		strcat(linijaA, "_dirA.txt");
		strcpy(linijaB, argv[i + 6]);
		strcat(linijaB, "_dirB.txt");
		glava1 = napravi(linijaA);
		glava2 = napravi(linijaB);
		lista[i] = obrada(glava1, glava2, x1, y1, x2, y2, c);
		izlaz(lista[i], argv[1],argv[i + 6], c);
		oslobodi(glava1);
		oslobodi(glava2);
	}
	for (i = 0;i < size;i++)
		oslobodi(lista[i]);
	free(lista);
	return 0;
}