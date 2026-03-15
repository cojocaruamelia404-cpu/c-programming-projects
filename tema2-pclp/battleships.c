#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	int lg;
	int x_start, y_start;
	int x_end, y_end;
} ship;

typedef struct {
	int total;
	int hit;
} hits;

int  **alloc_board(int n, int m)
{
	// functia aloca dinamic matricea ce reprez
	// tabla jucatorilor
	int **a = malloc((n + 1) * sizeof(int *));
	for (int i = 0; i <= n; i++) {
		a[i] = calloc(m + 1, sizeof(int));
	}
	return a;
}

void free_board(int **a, int n)
{
	for (int i = 0; i <= n; i++)
		free(a[i]);
	free(a);
}

int **alloc_hill_m(int m)
{
	int **a = malloc(m * sizeof(int *));
	for (int i = 0; i < m; i++) {
		a[i] = calloc(m, sizeof(int));
	}
	return a;
}

void free_hill_m(int **a, int m)
{
	for (int i = 0; i < m; i++)
		free(a[i]);
	free(a);
}

const char *nume_nava(char tip)
{
	if (tip == 'A')
		return "Albacore";
	else if (tip == 'L')
		return "Laffey";
	else if (tip == 'B')
		return "Belfast";
	else if (tip == 'Y')
		return "Yamato";
	else
		return "Shinano";
	return "";
}

const char *nume_nava_lg(int lg)
{
	if (lg == 1)
		return "Albacore";
	else if (lg == 2)
		return "Laffey";
	else if (lg == 3)
		return "Belfast";
	else if (lg == 4)
		return "Yamato";
	else
		return "Shinano";
	return "";
}

const char *orientare(char orient)
{
	if (orient == 'V')
		return "vertical";
	else
		return "orizontal";
	return 0;
}

int lg_nava(char tip)
{
	// functia returneaza lungimea in fctie de nava
	if (tip == 'A')
		return 1;
	if (tip == 'L')
		return 2;
	if (tip == 'B')
		return 3;
	if (tip == 'Y')
		return 4;
	if (tip == 'S')
		return 5;
	return 0;
}

void afisare(int **tabla, int n, int m)
{
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			printf("%d ", tabla[i][j]);
		}
		printf("\n");
	}
}

int verif_dimensiune(int x, int y, int lg, char orient, int n, int m)
{
	// verific daca nava incape in tabla
	if (x < 1 || x > n || y < 1 || y > m)
		return 0;
	if (orient == 'V') {
		if (x - lg + 1 < 1)
			return 0;
	} else {
		if (y + lg - 1 > m)
			return 0;
	}
	return 1;
}

int verif_suprapunere(int x, int y, int lg, char orient, int **tabla)
{
	if (orient == 'V') {
		for (int i = x; i >= x - lg + 1; i--) {
			if (tabla[i][y] != 0)
				return 0;
		}
	} else {
		for (int i = y; i <= y + lg - 1; i++) {
			if (tabla[x][i] != 0)
				return 0;
		}
	}
	return 1;
}

void complete_ship(int x, int y, int lg, char orient, int **tabla)
{
	if (orient == 'V') {
		for (int i = x; i >= x - lg + 1; i--) {
			tabla[i][y] = lg;
		}
	} else {
		for (int i = y; i <= y + lg - 1; i++) {
			tabla[x][i] = lg;
		}
	}
}

int total_n(int n, int m)
{
	int nave_necesare[6];
	nave_necesare[1] = n * m / 20; // nava A
	nave_necesare[2] = n * m / 30; // nava L
	nave_necesare[3] = n * m / 40; // nava B
	nave_necesare[4] = n * m / 55; // nava Y
	nave_necesare[5] = n * m / 70; // nava S
	int total_nave = 0;
	for (int i = 1; i <= 5; i++) {
		total_nave = total_nave + nave_necesare[i];
	}
	return total_nave;
}

void plasare_nave(int total_nave, int n, int m, int **tabla1,
				  int **tabla2, ship *ship1, ship *ship2)
{
	int nr_nave = 0;
	int turn = 1;
	int aux1 = 0, aux2 = 0;
	while (nr_nave < total_nave) {
		char tip, orient;
		int x, y;
		scanf(" %c %c %d %d", &tip, &orient, &x, &y);
		int lg = lg_nava(tip);
		int **tabla;
		ship *s;
		// verific al cui jucator e randul
		if (turn % 2 == 1) {
			tabla = tabla1;
			s = &ship1[aux1];
		} else {
			tabla = tabla2;
			s = &ship2[aux2];
		}
		if (verif_dimensiune(x, y, lg, orient, n, m) == 0 ||
			verif_suprapunere(x, y, lg, orient, tabla) == 0) {
			printf("Nava %s nu poate fi amplasata %s la"
				   " coordonatele (%d, %d).\n", nume_nava(tip),
				   orientare(orient), x, y);
		} else {
			complete_ship(x, y, lg, orient, tabla);
			s->lg = lg;
			s->x_start = x;
			s->y_start = y;
			if (orient == 'V') {
				s->x_end = x - lg + 1;
				s->y_end = y;
			} else {
				s->x_end = x;
				s->y_end = y + lg - 1;
			}
			if (turn % 2 == 1)
				aux1++;
			else
				aux2++;
			nr_nave++;
			turn++;
		}
	}
}

void marcheaza_distrusa(int **tabla, ship s)
{
	if (s.x_start == s.x_end) {
		for (int j = s.y_start; j <= s.y_end; j++)
			if (tabla[s.x_start][j] != -1)
				tabla[s.x_start][j] = 0;
	} else {
		for (int i = s.x_end; i <= s.x_start; i++)
			if (tabla[i][s.y_start] != -1)
				tabla[i][s.y_start] = 0;
	}
}

int ship_contains(ship s, int x, int y)
{
	if (s.x_start == s.x_end) {
		if (x == s.x_start && y >= s.y_start && y <= s.y_end)
			return 1;
		else
			return 0;
	}
	if (y == s.y_start && x >= s.x_end && x <= s.x_start)
		return 1;
	return 0;
}

int lovitura_nava(int **tabla, ship *nava, int ship_count,
				  int tinta, int x, int y, int juc,
				  int *nr_nave_ramase1, int *nr_nave_ramase2)
{
	tabla[x][y] = -1; // marchez o lovitura a navei
	int found = -1;
	for (int i = 0; i < ship_count; i++) {
		if (nava[i].lg == tinta &&
			ship_contains(nava[i], x, y) == 1) {
			found = i;
			break;
		}
	}
	ship *s = &nava[found];
	if (x == s->x_start && y == s->y_start) {
		marcheaza_distrusa(tabla, *s);
		printf("Jucatorul %d a distrus o nava %s plasata intre coordonatele"
			   " (%d, %d) si (%d, %d).\n", juc, nume_nava_lg(s->lg),
			   s->x_start, s->y_start, s->x_end, s->y_end);
		if (juc == 1) {
			(*nr_nave_ramase2)--;
			if (*nr_nave_ramase2 == 0) {
				printf("Jucatorul 1 a castigat.\n");
				return 1;
			}
		} else {
			(*nr_nave_ramase1)--;
			if (*nr_nave_ramase1 == 0) {
				printf("Jucatorul 2 a castigat.\n");
				return 1;
			}
		}
	} else {
		printf("Jucatorul %d a lovit o nava %s la coordonatele (%d, %d).\n",
			   juc, nume_nava_lg(s->lg), x, y);
	}
	return 0;
}

void procesare_atacuri(int n, int m, int total_nave, int **tabla1, int **tabla2,
					   ship *ship1, ship *ship2, hits *hit1, hits *hit2)
{
		int nr_nave_ramase1 = total_nave / 2;
		int nr_nave_ramase2 = total_nave / 2;
		int turn = 1;
		int x, y;
		while (scanf("%d %d", &x, &y) == 2) {
			int **tabla;
			int juc;
			ship *nava;
			hits *lovitura;
			int ship_count = total_nave / 2;
			if (turn % 2 == 1) {
				tabla = tabla2;
				juc = 1;
				nava = ship2;
				lovitura = hit1;
			} else {
				tabla = tabla1;
				juc = 2;
				nava = ship1;
				lovitura = hit2;
			}
			lovitura->total++;
			if (x < 1 || x > n || y < 1 || y > m) {
				printf("Jucatorul %d a ratat o lovitura la coordonatele "
					   "(%d, %d).\n", juc, x, y);
				turn++;
			} else {
				int tinta = tabla[x][y];
				if (tinta == -1) {
					// verific daca a fost o celula atacata deja
					printf("Coordonatele (%d, %d) au fost deja atacate de"
						   " jucatorul %d.\n", x, y, juc);
					turn++;
				} else if (tinta == 0) {
					// verific daca e lovitura ratata
					tabla[x][y] = -1; // marchez o celula atacata
					printf("Jucatorul %d a ratat o lovitura la coordonatele "
						   "(%d, %d).\n", juc, x, y);
					turn++;
				} else {
					lovitura->hit++;
					if (lovitura_nava(tabla, nava, ship_count, tinta, x, y, juc,
									  &nr_nave_ramase1, &nr_nave_ramase2) == 1)
						break;
					turn++;
				}
			}
		}
}

void procesare_meciuri(int j, int *acc1, int *acc2, int *k, hits *h1, hits *h2)
{
	for (int a = 1; a <= j; a++) {
		int n, m;
		scanf("%d %d", &n, &m);
		int **tabla1 = alloc_board(n, m);
		int **tabla2 = alloc_board(n, m);
		hits hit1, hit2;
		// calculez cate nave de fiecare tip am nevoie
		int total_nave = total_n(n, m);
		k[a] = total_nave;
		// fac cate un vector de struct pentru navele fiecarui jucator
		ship *ship1 = malloc(total_nave * sizeof(ship));
		ship *ship2 = malloc(total_nave * sizeof(ship));
		hit1.total = 0;
		hit1.hit = 0;
		hit2.total = 0;
		hit2.hit = 0;
		total_nave = total_nave * 2;
		plasare_nave(total_nave, n, m, tabla1, tabla2, ship1, ship2);
		afisare(tabla1, n, m);
		printf("\n");
		afisare(tabla2, n, m);
		procesare_atacuri(n, m, total_nave, tabla1, tabla2, ship1,
						  ship2, &hit1, &hit2);
		h1[a] = hit1;
		h2[a] = hit2;
		free(ship1);
		free(ship2);
		free_board(tabla1, n);
		free_board(tabla2, n);
		if (hit1.total > 0)
			acc1[a] = hit1.hit * 10000 / hit1.total;
		else
			acc1[a] = 0;
		if (hit2.total > 0)
			acc2[a] = hit2.hit * 10000 / hit2.total;
		else
			acc2[a] = 0;
	}
}

int statistica2(int juc, double accuracy,
				hits hit1, hits hit2)
{
	int total, hit;
	if (juc == 1) {
		total = hit1.total;
		hit = hit1.hit;
	} else {
		total = hit2.total;
		hit = hit2.hit;
	}
	double target = accuracy / 100.0;
	int i;
	for (i = hit; ; i++) {
		if ((double)i / total >= target)
			break;
	}
	return i - hit;
}

int statistica4(int juc, double accuracy,
				hits hit1, hits hit2)
{
	int total, hit;
	if (juc == 1) {
		total = hit1.total;
		hit = hit1.hit;
	} else {
		total = hit2.total;
		hit = hit2.hit;
	}
	double target = accuracy / 100.0;
	int i;
	for (i = hit; i >= 0; i--) {
		if ((double)i / total < target)
			break;
	}
	if (hit - i == 0)
		return hit - i;
	i++;
	return hit - i;
}

void calc_sum(int juc, int *acc1, int *acc2, int *k, int j,
			  double *sum, double *sum_k)
{
    // calculez suma navelor din fiecare meci in sum_k
	// calculez in sum  suma acuratetilor fiecarui meci
	// inmultita cu nr de nave din fiecare
	*sum = 0;
	*sum_k = 0;
	for (int i = 1; i <= j; i++) {
		double acc;
		if (juc == 1) {
			acc = acc1[i] / 10000.0;
		} else {
			acc = acc2[i] / 10000.0;
		}
		*sum  = *sum + acc * k[i];
		*sum_k = *sum_k + k[i];
	}
}

int statistica1(int juc, int j, double accuracy, int *k, int *acc1, int *acc2)
{
	double target = accuracy / 100.0;
	double sum, sum_k;
	calc_sum(juc, acc1, acc2, k, j, &sum, &sum_k);
	double acc;
	double castig[j + 1];
	for (int i = 1; i <= j; i++) {
		if (juc == 1) {
			acc = acc1[i] / 10000.0;
		} else {
			acc = acc2[i] / 10000.0;
		}
		castig[i] = (1.0 - acc) * k[i];
	}
	// sortez meciurile descrescator dupa castig
	// ca sa vad de unde obtin cel mai mare profit
	for (int i = 1; i < j; i++) {
		for (int s = i + 1; s <= j; s++) {
			if (castig[s] > castig[i]) {
				double aux = castig[i];
				castig[i] = castig[s];
				castig[s] = aux;
			}
		}
	}
	int aux = 0;
	for (int i = 1; i <= j; i++) {
		if (sum / sum_k >= target)
			return aux;
		sum = sum + castig[i];
		aux++;
	}
	return j;
}

int statistica3(int juc, int j, double accuracy, int *k, int *acc1, int *acc2)
{
	double target = accuracy / 100.0;
	double sum, sum_k;
	calc_sum(juc, acc1, acc2, k, j, &sum, &sum_k);
	double acc;
	double castig[j + 1];
	for (int i = 1; i <= j; i++) {
		if (juc == 1) {
			acc = acc1[i] / 10000.0;
		} else {
			acc = acc2[i] / 10000.0;
		}
		castig[i] = acc * k[i];
	}
	for (int i = 1; i < j; i++) {
		for (int s = i + 1; s <= j; s++) {
			if (castig[s] < castig[i]) {
				double aux = castig[i];
				castig[i] = castig[s];
				castig[s] = aux;
			}
		}
	}
	int aux = 0;
	for (int i = 1; i <= j; i++) {
		double new_acc = (sum - castig[i]) / sum_k;
		if (new_acc >= target) {
			sum = sum - castig[i];
			aux++;
		} else {
			break;
		}
	}
	return aux;
}

int mod_37(long long a)
{
	int rez = a % 37;
	if (rez < 0)
		rez = rez + 37;
	return rez;
}

int transf_litera_numar(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'A' && c <= 'Z')
		return c - 'A' + 10;
	else if (c == '.')
		return 36;
	return 0;
}

int transf_numar_litera(int a)
{
	if (a >= 0 && a <= 9)
		return a + '0';
	else if (a >= 10 && a <= 35)
		return 'A' + (a - 10);
	else if (a == 36)
		return '.';
	return 0;
}

void transf_cheie(char *cheie, int m, int **key)
{
	// imi formez matricea cheie
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			char c = cheie[i * m + j];
			key[i][j] = transf_litera_numar(c);
		}
	}
}

int calcul_determinant(int **key, int m)
{
	long long det = 0;
	if (m == 1)
		return mod_37(key[0][0]);
	if (m == 2) {
		det = (long long)key[0][0] * key[1][1] -
			  (long long)key[0][1] * key[1][0];
		return mod_37(det);
	}
	det = det + (long long)key[0][0] * key[1][1] * key[2][2];
	det = det + (long long)key[0][1] * key[1][2] * key[2][0];
	det = det + (long long)key[1][0] * key[2][1] * key[0][2];
	det = det - (long long)key[0][2] * key[1][1] * key[2][0];
	det = det - (long long)key[1][0] * key[0][1] * key[2][2];
	det = det - (long long)key[2][1] * key[1][2] * key[0][0];
	return mod_37(det);
}

int calcul_factor(int a)
{
	// calculez inversul multiplicativ al unui nr a
	int x_invers;
	for (int i = 1; i <= 36; i++) {
		if (mod_37(a * i) == 1) {
			x_invers = i;
			break;
		}
	}
	return x_invers;
}

void calcul_adjuncta(int **key, int **adjuncta, int m)
{
	if (m == 1) {
		adjuncta[0][0] = 1;
		return;
	} else if (m == 2) {
		adjuncta[0][0] = mod_37(key[1][1]);
		adjuncta[0][1] = mod_37(-key[0][1]);
		adjuncta[1][0] = mod_37(-key[1][0]);
		adjuncta[1][1] = mod_37(key[0][0]);
	} else {
		int a = key[0][0];
		int b = key[0][1];
		int c = key[0][2];
		int d = key[1][0];
		int e = key[1][1];
		int f = key[1][2];
		int g = key[2][0];
		int h = key[2][1];
		int i = key[2][2];
		adjuncta[0][0] = mod_37(e * i - f * h);
		adjuncta[0][1] = mod_37(-(b * i - c * h));
		adjuncta[0][2] = mod_37(b * f - e * c);
		adjuncta[1][0] = mod_37(-(d * i - f * g));
		adjuncta[1][1] = mod_37(a * i - c * g);
		adjuncta[1][2] = mod_37(-(a * f - c * d));
		adjuncta[2][0] = mod_37(d * h - g * e);
		adjuncta[2][1] = mod_37(-(a * h - b * g));
		adjuncta[2][2] = mod_37(a * e - b * d);
	}
}

void calcul_inversa(int **adjuncta, int x_invers, int **inversa, int m)
{
	if (m == 1) {
		inversa[0][0] = mod_37(adjuncta[0][0] * x_invers);
		return;
	}
	for (int i = 0; i < m ; i++) {
		for (int j = 0; j < m; j++) {
			inversa[i][j] = mod_37((long long)x_invers * adjuncta[i][j]);
		}
	}
}

void inmultire(int m, int **a, int *v, int *out)
{
	for (int i = 0; i < m; i++) {
		long long s = 0;
		for (int j = 0; j < m; j++) {
			s = s + (long long)a[i][j] * v[j];
		}
		out[i] = mod_37(s);
	}
}

void decriptare(char *cheie, char *comanda, char *rez)
{
	int lg_cheie = strlen(cheie);
	// calculez dimensiunea matricei
	int m = sqrt(lg_cheie);
	// impart comanda in blocuri
	int lung = strlen(comanda);
	int ct = 0;
	for (int i = 0; i < lung; ) {
		int lung_ramasa = lung - i;
		int dim;
		if (lung_ramasa >= m)
			dim = m;
		else
			dim = lung_ramasa;
		char key2[32];
		for (int s = 0; s < dim * dim; s++)
			key2[s] = cheie[s];
		key2[dim * dim] = '\0';
		int **key = alloc_hill_m(dim);
		// transform cheia in matrice
		transf_cheie(key2, dim, key);
		// calculez dterminantul
		int det = calcul_determinant(key, dim);
		// calculez inversul multiplicativ al determinantului
		int x_invers = calcul_factor(det);
		int **adjuncta = alloc_hill_m(dim);
		// calculez adjuncta
		calcul_adjuncta(key, adjuncta, dim);
		int **inversa = alloc_hill_m(dim);
		// calculez inversa
		calcul_inversa(adjuncta, x_invers, inversa, dim);
		int *v = calloc(dim, sizeof(int));
		int *out = calloc(dim, sizeof(int));
		int j = 0;
		while (j < dim) {
			char nr = comanda[i + j];
			v[j] = transf_litera_numar(nr);
			j++;
		}
		// decodific fiecare bloc
		inmultire(dim, inversa, v, out);
		for (j = 0; j < dim; j++) {
			rez[ct] = transf_numar_litera(out[j]);
			ct++;
		}
		free(v);
		free(out);
		free_hill_m(key, dim);
		free_hill_m(adjuncta, dim);
		free_hill_m(inversa, dim);
		i = i + dim;
	}
	rez[ct] = '\0';
}

void criptare(char *cheie, char *comanda, char *rez)
{
	int lg_cheie = strlen(cheie);
	int m = sqrt(lg_cheie);
	int lung = strlen(comanda);
	int ct = 0;
	for (int i = 0; i < lung; ) {
		int lung_ramasa = lung - i;
		int dim;
		if (lung_ramasa >= m)
			dim = m;
		else
			dim = lung_ramasa;
		char key2[32];
		for (int s = 0; s < dim * dim; s++)
			key2[s] = cheie[s];
		key2[dim * dim] = '\0';
		int **key = alloc_hill_m(dim);
		transf_cheie(key2, dim, key);
		int *v = calloc(dim, sizeof(int));
		int *out = calloc(dim, sizeof(int));
		int j = 0;
		while (j < dim) {
			char nr = comanda[i + j];
			v[j] = transf_litera_numar(nr);
			j++;
		}
		// criptez fiecare bloc
		inmultire(dim, key, v, out);
		for (j = 0; j < dim; j++) {
			rez[ct] = transf_numar_litera(out[j]);
			ct++;
		}
		free(v);
		free(out);
		free_hill_m(key, dim);
		i = i + dim;
	}
	rez[ct] = '\0';
}

void calcul_rezultat(int cur_acc, int rez, char *rezultat)
{
	int a = cur_acc / 100;
	int b = cur_acc % 100;
	int ct = 0;
	rezultat[ct] = (a / 100) + '0';
	ct++;
	rezultat[ct] = ((a / 10) % 10) + '0';
	ct++;
	rezultat[ct] = (a % 10) + '0';
	ct++;
	rezultat[ct] = '.';
	ct++;
	rezultat[ct] = (b / 10) + '0';
	ct++;
	rezultat[ct] = (b % 10) + '0';
	ct++;
	rezultat[ct] = '.';
	ct++;
	if (rez == 0) {
		rezultat[ct] = '0';
		ct++;
	} else if (rez > 0) {
		//extrag cifrele din rez
		int c_rez = rez;
		int cifre[5], aux = 0;
		while (c_rez != 0) {
			cifre[aux] = c_rez % 10;
			aux++;
			c_rez = c_rez / 10;
		}
		aux--;
		for (int i = aux; i >= 0; i--) {
			rezultat[ct] = cifre[i] + '0';
			ct++;
		}
	}
	rezultat[ct] = '\0';
}

int main(void)
{
	int j;
	scanf("%d", &j);
	int *acc1 = malloc((j + 1) * sizeof(int));
	int *acc2 = malloc((j + 1) * sizeof(int));
	int *k = malloc((j + 1) * sizeof(int));
	hits *h1 = malloc((j + 1) * sizeof(hits));
	hits *h2 = malloc((j + 1) * sizeof(hits));
	procesare_meciuri(j, acc1, acc2, k, h1, h2);
	char cheie[32];
	scanf(" %s", cheie);
	char comanda[32];
	while (scanf(" %s", comanda) == 1 && strcmp(comanda, "Q") != 0) {
		char com_decript[32];
		if (strcmp(cheie, "-") != 0)
			decriptare(cheie, comanda, com_decript);
		else
			strcpy(com_decript, comanda);
		int juc;
		if (com_decript[0] == 'O')
			juc = 1;
		else
			juc = 2;
		int tip1 = (com_decript[1] == 'U' ? 0 : 1);
		int nr_meci = (com_decript[2] - '0') * 100 + (com_decript[3] - '0')
					   * 10 + (com_decript[4] - '0');
		char buf[8];
		strncpy(buf, com_decript + 5, 6);
		buf[6] = '\0';
		double accuracy = atof(buf);
		int tip2;
		if (com_decript[11] == 'M' && com_decript[12] == 'I' &&
			com_decript[13] == 'N')
			tip2 = 0; // pentru min
		else
			tip2 = 1; // pentru max
		if (nr_meci < 1 || nr_meci > j || h1[nr_meci].total == 0) {
			printf("Nu exista date pentru meciul cerut.\n");
		} else {
			int cur_acc, rez;
			if (tip1 == 0 && tip2 == 0) {
				rez = statistica2(juc, accuracy, h1[nr_meci], h2[nr_meci]);
				if (juc == 1)
					cur_acc = acc1[nr_meci];
				else
					cur_acc = acc2[nr_meci];
			} else if (tip1 == 0 && tip2 == 1) {
				rez = statistica4(juc, accuracy, h1[nr_meci], h2[nr_meci]);
				if (juc == 1)
					cur_acc = acc1[nr_meci];
				else
					cur_acc = acc2[nr_meci];
			} else if (tip1  == 1 && tip2 == 0) {
				rez = statistica1(juc, j, accuracy, k, acc1, acc2);
				double sum, sum_k;
				calc_sum(juc, acc1, acc2, k, j, &sum, &sum_k);
				cur_acc = (int)(((sum / sum_k) + 0.00005) * 10000.0);
			} else {
				rez = statistica3(juc, j, accuracy, k, acc1, acc2);
				double sum, sum_k;
				calc_sum(juc, acc1, acc2, k, j, &sum, &sum_k);
				cur_acc = (int)(((sum / sum_k) + 0.00005) * 10000.0);
			}
			char rezultat[32];
			calcul_rezultat(cur_acc, rez, rezultat);
			char output[32];
			if (strcmp(cheie, "-") != 0)
				criptare(cheie, rezultat, output);
			else
				strcpy(output, rezultat);
			printf("%s\n", output);
		}
	}
	free(acc1);
	free(acc2);
	free(h1);
	free(h2);
	free(k);
	return 0;
}
