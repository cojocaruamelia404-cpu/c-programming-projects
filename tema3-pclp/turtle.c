#include <stdio.h>
#include <stdlib.h>
#include "turtle.h"

void copy_image(image *copie_img, image *img)
{
	// creez o copie a unei imagini, cu toate elementele ei
	copie_img->width = img->width;
	copie_img->height = img->height;
	copie_img->data = malloc(img->height * sizeof(pixel *));
	for (int i = 0; i < img->height; i++) {
		copie_img->data[i] = malloc(img->width * sizeof(pixel));
		for (int j = 0; j < img->width; j++) {
			copie_img->data[i][j] = img->data[i][j];
		}
	}
}

void alloc_image_arhiva(image_arhiva *arhiva)
{
	// initializez arhiva pentru istoricul imaginilor
	arhiva->stari_turtle = NULL;
	arhiva->contor = 0;
	arhiva->curr_poz = -1;
	arhiva->capacitate = 0;
}

int ppm_load(image *img, const char *filename)
{
	FILE *file = fopen(filename, "rb");
	if (!file)
		return 0;
	char magic[3];
	// citesc toate elementele unei imagini
	fscanf(file, "%2s", magic);
	int width, height;
	fscanf(file, "%d %d", &width, &height);
	int val_max;
	fscanf(file, "%d", &val_max);
	fgetc(file);
	// eliberez imaginea anterioara din memorie
	ppm_free(img);
	// incarc noua imagine
	img->width = width;
	img->height = height;
	img->data = malloc(height * sizeof(pixel *));
	for (int i = 0 ; i < height; i++) {
		img->data[i] = malloc(width * sizeof(pixel));
		fread(img->data[i], sizeof(pixel), width, file);
	}
	fclose(file);
	return 1;
}

void ppm_free(image *img)
{
	// eliberez memoria unei imagini
	if (!img || !img->data)
		return;
	for (int i = 0; i < img->height; i++)
		free(img->data[i]);
	free(img->data);
	img->data = NULL;
	img->width = 0;
	img->height = 0;
}

int ppm_save(image *img, const char *filename)
{
	if (!img)
		return 0;
	FILE *file = fopen(filename, "wb");
	if (file == 0)
		return 0;
	// salvez imaginea intr-un fisier ppm
	// si scriu datele specifice fisierului
	fprintf(file, "P6\n%d %d\n255\n", img->width, img->height);
	for (int i = 0; i < img->height; i++) {
		fwrite(img->data[i], sizeof(pixel), img->width, file);
	}
	fclose(file);
	return 1;
}

void ppm_draw(image *img, int x0, int y0, int x1, int y1, int r, int g, int b)
{
	int dx = abs(x1 - x0);
	int dy = -abs(y1 - y0);
	int sx, sy;
	if (x0 < x1)
		sx = 1;
	else
		sx = -1;
	if (y0 < y1)
		sy = 1;
	else
		sy = -1;
	int err = dx + dy;
	while (1) {
		// verific daca pixelul se afla in interiorul imaginii
		if (x0 >= 0 && x0 < img->width &&
			y0 >= 0 && y0 < img->height) {
			img->data[y0][x0].r = (unsigned char)r;
			img->data[y0][x0].g = (unsigned char)g;
			img->data[y0][x0].b = (unsigned char)b;
		}
		if (x0 == x1 && y0 == y1)
			break;
		int e2 = 2 * err;
		if (e2 >= dy) {
			err = err + dy;
			x0 = x0 + sx;
		}
		if (e2 <= dx) {
			err = err + dx;
			y0 = y0 + sy;
		}
	}
}

void free_image_arhiva(image_arhiva *arhiva)
{
	// eliberez memoria alocata arhivei
	for (int i = 0; i < arhiva->contor; i++) {
		ppm_free(&arhiva->stari_turtle[i]);
	}
	free(arhiva->stari_turtle);
	arhiva->stari_turtle = NULL;
	arhiva->contor = 0;
	arhiva->curr_poz = -1;
	arhiva->capacitate = 0;
}

void save_image_arhiva(image_arhiva *arhiva, image *img)
{
	if (!img->data)
		return;
	// sterg starile de redo viitoare, daca exista
	for (int i = arhiva->curr_poz + 1; i < arhiva->contor; i++) {
		ppm_free(&arhiva->stari_turtle[i]);
	}
	arhiva->contor = arhiva->curr_poz + 1;
	// maresc capacitatea arhivei doar daca e nevoie
	if (arhiva->contor == arhiva->capacitate) {
		if (arhiva->capacitate == 0)
			arhiva->capacitate = 2;
		else
			arhiva->capacitate =  arhiva->capacitate * 2;
		arhiva->stari_turtle = realloc(arhiva->stari_turtle,
									   arhiva->capacitate * sizeof(image));
	}
	// copiez imaginea curenta in arhiva
	copy_image(&arhiva->stari_turtle[arhiva->contor], img);
	arhiva->curr_poz = arhiva->contor;
	arhiva->contor++;
}

int undo_image(image_arhiva *arhiva, image *img)
{
	if (arhiva->curr_poz < 0) {
		return 0;
	}
	// revin la starea anterioara din arhiva
	arhiva->curr_poz--;
	ppm_free(img);
	if (arhiva->curr_poz >= 0) {
		// copiez in imagine activa pe cea anterioara
		copy_image(img, &arhiva->stari_turtle[arhiva->curr_poz]);
	} else {
		img->data = NULL;
		img->width = 0;
		img->height = 0;
	}
	return 1;
}

int redo_image(image_arhiva *arhiva, image *img)
{
	if (arhiva->curr_poz + 1 >= arhiva->contor) {
		return 0;
	}
	// avansez cu o pozitie in arhiva
	arhiva->curr_poz++;
	ppm_free(img);
	// copiez in imaginea activa o imagine anulata inainte prin undo
	copy_image(img, &arhiva->stari_turtle[arhiva->curr_poz]);
	return 1;
}
