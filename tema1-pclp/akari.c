#include<stdio.h>
int n, m;
char grid[51][51], grid_luminat[51][51];
void light(int i, int j)
{
	for (int k = i - 1; k >= 0; k--) {
		if (grid[k][j] == '#' || (grid[k][j] <= '4' && grid[k][j] >= '0'))
			break;
		if (grid_luminat[k][j] == '-')
			grid_luminat[k][j] = 'x';
	}

	for (int k = i + 1; k < n; k++) {
		if (grid[k][j] == '#' || (grid[k][j] <= '4' && grid[k][j] >= '0'))
			break;
		if (grid_luminat[k][j] == '-')
			grid_luminat[k][j] = 'x';
	}

	for (int k = j - 1; k >= 0; k--) {
		if (grid[i][k] == '#' || (grid[i][k] <= '4' && grid[i][k] >= '0'))
			break;
		if (grid_luminat[i][k] == '-')
			grid_luminat[i][k] = 'x';
	}

	for (int k = j + 1; k < m; k++) {
		if (grid[i][k] == '#' || (grid[i][k] <= '4' && grid[i][k] >= '0'))
			break;
		if (grid_luminat[i][k] == '-')
			grid_luminat[i][k] = 'x';
	}
}

void check_black_cells(void)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (grid[i][j] == '0' || grid[i][j] == '1' ||
				grid[i][j] == '2' || grid[i][j] == '3' ||
				grid[i][j] == '4') {
				//functia verifica pentru fiecare celula neagra
				// numerotata becurile adiacente
				int bec = 0;
				int value_grid = grid[i][j] - '0';
				if (i > 0 && grid[i - 1][j] == 'L')
					bec++;
				if (i < n - 1 && grid[i + 1][j] == 'L')
					bec++;
				if (j > 0 && grid[i][j - 1] == 'L')
					bec++;
				if (j < m - 1 && grid[i][j + 1] == 'L')
					bec++;
				if (bec == value_grid) {
					//daca numarul de pe celula neagra coinicide
					//cu nr becurilor adiacente atunci
					//voi marca casutele albe adiacente cu 'x'
					if (i > 0 && grid_luminat[i - 1][j] == '-')
						grid_luminat[i - 1][j] = 'x';
					if (i < n - 1 && grid_luminat[i + 1][j] == '-')
						grid_luminat[i + 1][j] = 'x';
					if (j > 0 && grid_luminat[i][j - 1] == '-')
						grid_luminat[i][j - 1] = 'x';
					if (j < m - 1 && grid_luminat[i][j + 1] == '-')
						grid_luminat[i][j + 1] = 'x';
					}
			}
		}
	}
}

int check_rule1(void)
{

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (grid[i][j] == 'L') {
				for (int k = j + 1; k < m; k++) {
					//parcurg tot gridul si verific pentru fiecare bec gasit
					//daca mai este vreunul pe aceeasi linie sau acceasi coloana
					if (grid[i][k] == '#' ||
						(grid[i][k] >= '0' && grid[i][k] <= '4'))
						break;
					if (grid[i][k] == 'L') {
						return 0;
					}
				}
				for (int k = i + 1; k < n; k++) {
					if (grid[k][j] == '#' ||
						(grid[k][j] >= '0' && grid[k][j] <= '4'))
						break;
					if (grid[k][j] == 'L') {
						return 0;
					}
				}
			}
		}
	}
	return 1;
}

int is_illumin(int i, int j)
{
	// pentru o celula cu coordonatele i si j verific
	// daca are alaturi vreun bec care sa o lumineze
	for (int k = i - 1; k >= 0; k--) {
		if (grid[k][j] == '#' || (grid[k][j] <= '4' && grid[k][j] >= '0'))
			break;
		if (grid[k][j] == 'L')
			return 1;
	}

	for (int k = i + 1; k < n; k++) {
		if (grid[k][j] == '#' || (grid[k][j] <= '4' && grid[k][j] >= '0'))
			break;
		if (grid[k][j] == 'L')
			return 1;
	}

	for (int k = j - 1; k >= 0; k--) {
		if (grid[i][k] == '#' || (grid[i][k] <= '4' && grid[i][k] >= '0'))
			break;
		if (grid[i][k] == 'L')
			return 1;
	}

	for (int k = j + 1; k < m; k++) {
		if (grid[i][k] == '#' || (grid[i][k] <= '4' && grid[i][k] >= '0'))
			break;
		if (grid[i][k] == 'L')
			return 1;
	}
	return 0;
}

int check_rule2(void)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (grid[i][j] == '0' || grid[i][j] == '1' ||
				grid[i][j] == '2' || grid[i][j] == '3' ||
				grid[i][j] == '4') {
				// pentru fiecare casuta neagra numerotata verific cate becuri
				// adiacente are si cate casute adiacente neiluminate are
				int bec = 0;
				int value_grid = grid[i][j] - '0';
				int possible = 0;
				if (i > 0) {
					if (grid[i - 1][j] == 'L')
						bec++;
					else if (grid[i - 1][j] == '-' && is_illumin(i - 1, j) == 0)
						possible++;
				}
				if (i < n - 1) {
					if (grid[i + 1][j] == 'L')
						bec++;
					else if (grid[i + 1][j] == '-' && is_illumin(i + 1, j) == 0)
						possible++;
				}
				if (j > 0) {
					if (grid[i][j - 1] == 'L')
						bec++;
					else if (grid[i][j - 1] == '-' && is_illumin(i, j - 1) == 0)
						possible++;
				}
				if (j < m - 1) {
					if (grid[i][j + 1] == 'L')
						bec++;
					else if (grid[i][j + 1] == '-' && is_illumin(i, j + 1) == 0)
						possible++;
				}
				// daca numarul de pe celula neagra este
				// mai mic decat nr becurilor adiacente
				// atunci se incalca regula
				if (value_grid < bec)
					return 0;
				// daca numarul de casute ce pot fi iluminate
				// este mai mic decat becurile
				// care ar mai trebui puse in jurul casutei negre
				// atunci se incalca regula
				if (possible < (value_grid - bec))
					return 0;
			}
		}
	}
	return 1;
}

int main(void)
{
	int o;
	scanf("%d", &o);
	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			scanf(" %c", &grid[i][j]);
			grid_luminat[i][j] = grid[i][j];
		}
	}

	if (o == 1) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (grid[i][j] == 'L') {
					// de fiecare data cand gasesc un bec in grid
					// luminez toate elementele de pe linia
					// si coloana respectiva
					 // pana intalnesc o celula neagra
					light(i, j);
				}
			}
		}
		check_black_cells();
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				printf(" %c", grid_luminat[i][j]);
			}
			printf("\n");
		}
	} else {
		if (check_rule1() && check_rule2()) {
			printf("ichi");
		} else {
			printf("zero");
		}
	}
	return 0;
}
