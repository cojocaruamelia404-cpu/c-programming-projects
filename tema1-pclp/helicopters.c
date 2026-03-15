#include<stdio.h>
int n, m, k, a[1001][1001], r1[1001], r2[1001], c1[1001], c2[1001], s[1001];
int nrzero, nrunu;
int abs(int a)
{
	if (a < 0) {
		return -a;
	} else {
		return a;
	}
}

void calcul_s_poz(int i)
{
	nrzero = 0;
	nrunu = 0;
	for (int j = r2[i]; j >= r1[i]; j--) {
		// parcurg triunghiul de jos in sus
		int v = r2[i] - j + 1;
		// calculez nivelul la care sunt in triunghi
		// dar si nr de elemente de pe linia j
		for (int u = 0; u < v; u++) {
			int col;
			if (c1[i] > c2[i]) {
				// calculez coloana prin suma/diferenta
				// in functie de cum creste triunghiul
				col = c2[i] + u;
			} else {
				col = c2[i] - u;
			}
			if (a[j][col] == 0) {
				nrzero++;
			} else {
				nrunu++;
			}
		}
	}
}

void calcul_s_neg(int i)
{
	nrzero = 0;
	nrunu = 0;
	for (int j = r1[i]; j <= r2[i] ; j++) {
		// parcurg triunghiul de sus in jos
		int v = j - r1[i] + 1;
		// calculez nivelul la care sunt in triunghi
		// dar si nr de elemente de pe linia j
		for (int u = 0; u < v; u++) {
			int col;
			if (c1[i] < c2[i]) {
				// calculez coloana prin suma/diferenta
				// in functie de cum creste triunghiul
				col = c1[i] + u;
			} else {
				col = c1[i] - u;
			}
			if (a[j][col] == 0) {
				nrzero++;
			} else {
				nrunu++;
			}
		}
	}
}

int main(void)
{
	int helic_invalide[1001], aux = 0;
	int nr_helic_ok = 0, nr_helic_poz_gresita = 0, helic_poz_gresita[1001];

	scanf("%d  %d", &n, &m);

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			scanf(" %d", &a[i][j]);
		}
	}
	scanf("%d", &k);
	for (int i = 1; i <= k; i++) {
		scanf(" %d %d %d %d %d", &r1[i], &c1[i], &r2[i], &c2[i], &s[i]);
	}
	for (int i = 1; i <= k; i++) {
		if (r1[i] == r2[i] || c1[i] == c2[i] ||
			(abs(r1[i] - r2[i]) != abs(c1[i] - c2[i]))) {
			//verific elicopterele pozitionate incorect
			// care nu pot forma un triunghi dreptunghic isoscel
			helic_invalide[aux] = i;
			aux++;
		} else {
			int l = abs(r1[i] - r2[i]) + 1;
			int nr_patratele = (l * (l + 1)) / 2;
			if (r2[i] < r1[i]) {
				//verific coordonatele capetelor ipotenuzei
				//pentru a avea mereu r1[i] < r2[i] si le inversez daca e cazul
				int c = r1[i];
				r1[i] = r2[i];
				r2[i] = c;
				c = c1[i];
				c1[i] = c2[i];
				c2[i] = c;
			}
			if (s[i] == 1) {
				calcul_s_poz(i);
			} else {
				calcul_s_neg(i);
			}
			if (nrunu == nr_patratele) {
				nr_helic_ok++;
			}
			if (nrzero > (nr_patratele / 2)) {
				helic_poz_gresita[nr_helic_poz_gresita] = i;
				nr_helic_poz_gresita++;
			}
		}
	}
	for (int i = 0; i < aux; i++) {
		printf("Elicopterul %d este pozitionat necorespunzator!\n",
			   helic_invalide[i]);
	}
	printf("%d\n", nr_helic_ok);
	printf("%d\n", nr_helic_poz_gresita);
	for (int i = 0; i < nr_helic_poz_gresita; i++) {
		printf("%d ", helic_poz_gresita[i]);
	}
	printf("\n");
	return 0;
}
