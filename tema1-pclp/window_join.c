#include <stdio.h>

typedef unsigned long long llu;

llu f_cmmdc(llu x1, llu x2)
{
	while (x2 != 0) {
		llu r = x1 % x2;
		x1 = x2;
		x2 = r;
	}
	return x1;
}

llu f_cmmmc(llu x1, llu x2)
{
	return (x1 / f_cmmdc(x1, x2)) * x2;
}

int main(void)
{
	llu window;
	llu t[100001], x[100001];
	llu sir_cmmdc[100001], sir_cmmmc[100001];
	llu t1[100001], t2[100001];

	int nr_perechi = 0;
	int aux = 0;

	scanf("%llu", &window);

	while (1) {
		llu t_k, x_k;
		scanf("%llu %llu", &t_k, &x_k);

		if (t_k == 0 && x_k == 0)
			break;

		t[aux] = t_k;
		x[aux] = x_k;

		for (int i = aux - 1; i >= 0; i--) {
			if (t_k - t[i] > window)
				break;
			llu cmmdc = f_cmmdc(x_k, x[i]);
			llu cmmmc = f_cmmmc(x_k, x[i]);
			t1[nr_perechi] = t[i];
			t2[nr_perechi] = t_k;
			sir_cmmdc[nr_perechi] = cmmdc;
			sir_cmmmc[nr_perechi] = cmmmc;
			nr_perechi++;
		}
		aux++;
	}

	for (int i = 0; i <= nr_perechi - 2; i++) {
		for (int j = i + 1; j <= nr_perechi - 1; j++) {
			if (t1[i] > t1[j] || (t1[i] == t1[j] && t2[i] > t2[j])) {
				llu a;

				a = t1[i];
				t1[i] = t1[j];
				t1[j] = a;

				a = t2[i];
				t2[i] = t2[j];
				t2[j] = a;

				a = sir_cmmmc[i];
				sir_cmmmc[i] = sir_cmmmc[j];
				sir_cmmmc[j] = a;

				a = sir_cmmdc[i];
				sir_cmmdc[i] = sir_cmmdc[j];
				sir_cmmdc[j] = a;
			}
		}
	}
	for (int i = 0; i <= nr_perechi - 1; i++) {
		printf("%llu %llu\n", sir_cmmmc[i], sir_cmmdc[i]);
	}
	return 0;
}
