#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M_PI 3.14159265358979323846
#include <math.h>
#include "lsystem.h"
#include "turtle.h"
#include "font.h"
#define comanda_lsys 1
#define comanda_image_turtle 2
#define comanda_font 3
#define comanda_image_type 4
typedef struct {
	int tip_cmd;
} istoric_undo;

void add_cmd(istoric_undo **stiva, int *nr_elem, int *max_elem, int tip_cmd)
{
	// adaug o comanda in stiva de istoric
	if (*nr_elem == *max_elem) {
		if (*max_elem == 0)
			*max_elem = 4;
		else
			*max_elem = *max_elem * 2;
		*stiva = realloc(*stiva, (*max_elem) * sizeof(istoric_undo));
	}
	(*stiva)[*nr_elem].tip_cmd = tip_cmd;
	(*nr_elem)++;
}

int get_cmd(istoric_undo *stiva, int nr_elem, int *tip_cmd)
{
	// returnez ultima comanda de pe stiva
	if (nr_elem == 0)
		return 0;
	*tip_cmd = stiva[nr_elem - 1].tip_cmd;
	return 1;
}

int remove_cmd(int *nr_elem)
{
	// elimin ultima comanda de pe stiva
	if (*nr_elem == 0)
		return 0;
	(*nr_elem)--;
	return 1;
}

void add_state(turtle_arhiva **stiva, int *nr_elem, int *max_elem,
			   double x, double y, double unghi)
{
	if (*nr_elem == *max_elem) {
		if (*max_elem == 0)
			*max_elem = 4;
		else
			*max_elem = (*max_elem) * 2;

		*stiva = realloc(*stiva, (*max_elem) * sizeof(turtle_arhiva));
	}
	// salvez starea turtle pe stiva(pozitia si unghiul)
	(*stiva)[*nr_elem].x = x;
	(*stiva)[*nr_elem].y = y;
	(*stiva)[*nr_elem].unghi = unghi;
	(*nr_elem)++;
}

int get_state(turtle_arhiva *stiva, int nr_elem, double *x, double *y,
			  double *unghi)
{
	// aflu care este ultima stare salvata pe stiva
	if (nr_elem == 0) {
		return 0;
	}
	*x = stiva[nr_elem - 1].x;
	*y = stiva[nr_elem - 1].y;
	*unghi = stiva[nr_elem - 1].unghi;
	return 1;
}

int remove_state(int *nr_elem)
{
	if (*nr_elem == 0)
		return 0;
	(*nr_elem)--;
	return 1;
}

void run_lsystem(lsystem *lsys, lsys_arhiva *arhiva, istoric_undo **stiva_undo,
				 int *nr_undo, int *max_undo, int *nr_redo,
				 char **last_filename, char *file)
{
	if (incarcare_lsystem(lsys, file) == 1) {
		// salvez in arhiva l-systemul
		save_arhiva(arhiva, lsys);
		// adaug comanda in stiva de undo
		add_cmd(stiva_undo, nr_undo, max_undo, comanda_lsys);
		*nr_redo = 0;
		free(*last_filename);
		*last_filename = malloc(strlen(file) + 1);
		strcpy(*last_filename, file);
		printf("Loaded %s (L-system with %d rules)\n",
			   file, lsys->nr_rules);
	} else {
		printf("Failed to load %s\n", file);
	}
}

void run_derive(lsystem *lsys, char *info)
{
	int n = atoi(info + 7);
	char *result = derive_lsystem(lsys, n);
	if (!result) {
		printf("No L-system loaded\n");
	} else {
		printf("%s\n", result);
		free(result);
	}
}

void run_save_image(image *img, char *file)
{
	// salvez imaginea in format ppm
	if (!img->data) {
		printf("No image loaded\n");
	} else {
		if (ppm_save(img, file) == 1)
			printf("Saved %s\n", file);
		else
			printf("Failed to save %s\n", file);
	}
}

void run_turtle(lsystem *lsys, image *img, image_arhiva *img_arhiva,
				istoric_undo **stiva_undo, int *nr_undo, int *max_undo,
				int *nr_redo, char *info)
{
	if (!img->data) {
		printf("No image loaded\n");
	} else if (lsys->stare == 0) {
		printf("No L-system loaded\n");
	} else {
		double x, y, pas, unghi, pas_unghiular;
		int n, r, g, b;
		if (sscanf(info + 7, "%lf %lf %lf %lf %lf %d %d %d %d",
				   &x, &y, &pas, &unghi, &pas_unghiular, &n,
				   &r, &g, &b) == 9) {
			char *command = derive_lsystem(lsys, n);
			if (!command) {
				printf("No L-system loaded\n");
				return;
			}
			double cx = x;
			double cy = y;
			double ung = unghi;
			turtle_arhiva *stiva = NULL;
			int nr_elem = 0, max_elem = 0;
			for (int i = 0; command[i] != '\0'; i++) {
				char c = command[i];
				if (c == 'F') {
					// desenez liniile pe imagine
					double rad = ung * M_PI / 180.0;
					double nx = cx + pas * cos(rad);
					double ny = cy + pas * sin(rad);
					ppm_draw(img, (int)round(cx),
							 (int)round(img->height - 1 - cy),
							 (int)round(nx),
							 (int)round(img->height - 1 - ny),
							 r, g, b);
					cx = nx;
					cy = ny;
				} else if (c == '+') {
					ung = ung + pas_unghiular;
				} else if (c == '-') {
					ung = ung - pas_unghiular;
				} else if (c == '[') {
					add_state(&stiva, &nr_elem, &max_elem, cx, cy, ung);
				} else if (c == ']') {
					double poz_x, poz_y, poz_unghi;
					if (get_state(stiva, nr_elem, &poz_x, &poz_y,
								  &poz_unghi) == 1) {
						remove_state(&nr_elem);
						cx = poz_x;
						cy = poz_y;
						ung = poz_unghi;
					}
				}
			}
			free(command);
			free(stiva);
			printf("Drawing done\n");
			// salvez imaginea in arhiva
			save_image_arhiva(img_arhiva, img);
			// adaug comanda pe stiva undo
			add_cmd(stiva_undo, nr_undo, max_undo, comanda_image_turtle);
			*nr_redo = 0;
		}
	}
}

void run_undo(lsystem *lsys, lsys_arhiva *arhiva,
			  image *img, image_arhiva *img_arhiva,
			  font **font, font_arhiva *fnt_arhiva,
			  istoric_undo **stiva_undo, int *nr_undo,
			  istoric_undo **stiva_redo, int *nr_redo, int *max_redo)
{
	int tip;
	if (get_cmd(*stiva_undo, *nr_undo, &tip) == 0) {
		printf("Nothing to undo\n");
	} else {
		// sterg ultima comanda si o adaug pe stiva de redo potrivita
		remove_cmd(nr_undo);
		if (tip == comanda_lsys && undo_arhiva(arhiva, lsys) == 1) {
			add_cmd(stiva_redo, nr_redo, max_redo, comanda_lsys);
		} else if ((tip == comanda_image_turtle ||
					tip == comanda_image_type) &&
					undo_image(img_arhiva, img) == 1) {
			add_cmd(stiva_redo, nr_redo, max_redo, tip);
		} else if (tip == comanda_font && undo_font(fnt_arhiva, font) == 1) {
			add_cmd(stiva_redo, nr_redo, max_redo, comanda_font);
		}
	}
}

void run_redo(lsystem *lsys, lsys_arhiva *arhiva, image *img,
			  image_arhiva *img_arhiva, istoric_undo **stiva_undo,
			  font **font, font_arhiva *fnt_arhiva,
			  int *nr_undo, int *max_undo, istoric_undo **stiva_redo,
			  int *nr_redo, char *last_filename)
{
	int tip;
	if (get_cmd(*stiva_redo, *nr_redo, &tip) == 0) {
		printf("Nothing to redo\n");
	} else {
		// sterg ultima comanda si o adaug pe stiva de undo specifica
		remove_cmd(nr_redo);
		if (tip == comanda_lsys && redo_arhiva(arhiva, lsys) == 1) {
			add_cmd(stiva_undo, nr_undo, max_undo, comanda_lsys);
			if (last_filename)
				printf("Loaded %s (L-system with %d rules)\n",
					   last_filename, lsys->nr_rules);
		} else if ((tip == comanda_image_turtle ||
					tip == comanda_image_type) &&
					redo_image(img_arhiva, img) == 1) {
			add_cmd(stiva_undo, nr_undo, max_undo, tip);
			if (tip == comanda_image_turtle) {
				printf("Drawing done\n");
			} else if (tip == comanda_image_type) {
				printf("Text written\n");
			}
		} else if (tip == comanda_font && redo_font(fnt_arhiva, font) == 1) {
			add_cmd(stiva_undo, nr_undo, max_undo, comanda_font);
			if (*font) {
				char *fisier = fnt_arhiva->fisier_font[fnt_arhiva->curr_poz];
				printf("Loaded %s (bitmap font %s)\n", fisier, (*font)->nume);
			}
		}
	}
}

void draw_glyph(image *img, glyph *gly, int cursor_x, int cursor_y,
				int r, int g, int b)
{
	for (int y = 0; y < gly->bb_h; y++) {
		for (int x = 0; x < gly->bb_w; x++) {
			// Verific dacă bitul din bitmap este 1
			if (gly->bitmap[y][x] != 1)
				continue;
			int px = cursor_x + x + gly->bb_xoff;
			int py = cursor_y + (gly->bb_h - 1 - y) + gly->bb_yoff;
			// Verific limitele imaginii
			if (px >= 0 && px < img->width && py >= 0 && py < img->height) {
				img->data[img->height - 1 - py][px].r = r;
				img->data[img->height - 1 - py][px].g = g;
				img->data[img->height - 1 - py][px].b = b;
			}
		}
	}
}

void run_type(image *img, image_arhiva *img_arhiva, font *font,
			  istoric_undo **stiva_undo, int *nr_undo, int *max_undo,
			  int *nr_redo, char *info)
{
	if (!img->data) {
		printf("No image loaded\n");
		return;
	}
	if (!font) {
		printf("No font loaded\n");
		return;
	}
	char *p = strchr(info, '"');
	if (!p)
		return;
	p++;
	char text[256];
	int idx = 0;
	while (*p && *p != '"') {
		text[idx++] = *p;
		p++;
	}
	text[idx] = '\0';
	int start_x, start_y, r, g, b;
	if (sscanf(p + 1, "%d %d %d %d %d", &start_x, &start_y, &r, &g, &b) == 5) {
		int cursor_x = start_x;
		int cursor_y = start_y;
		for (int i = 0; text[i] != '\0'; i++) {
			int index = find_glyph(font, text[i]);
			if (index == -1)
				continue;
			draw_glyph(img, &font->glyphs[index], cursor_x, cursor_y, r, g, b);
			cursor_x = cursor_x + font->glyphs[index].dwx;
		}
		printf("Text written\n");
		save_image_arhiva(img_arhiva, img);
		add_cmd(stiva_undo, nr_undo, max_undo, comanda_image_type);
		*nr_redo = 0;
	}
}

void bitcheck_error_print(image *img, int *bit_final, int *bit_px, int *bit_py,
						  int *bit_color, int *bit_pos, int *bit_ct, int j,
						  int i)
{
	unsigned char *color[3];
	color[0] = &img->data[i][j].r;
	color[1] = &img->data[i][j].g;
	color[2] = &img->data[i][j].b;
	for (int c = 0; c < 3; c++) {
		for (int b = 7; b >= 0; b--) {
			int bit = (*color[c] >> b) & 1;
			if (*bit_ct == 4) {
				for (int k = 0; k < 3; k++) {
					bit_final[k] = bit_final[k + 1];
					bit_px[k] = bit_px[k + 1];
					bit_py[k] = bit_py[k + 1];
					bit_color[k] = bit_color[k + 1];
					bit_pos[k] = bit_pos[k + 1];
				}
				*bit_ct = 3;
			}
			bit_final[*bit_ct] = bit;
			bit_px[*bit_ct] = j;
			bit_py[*bit_ct] = i;
			bit_color[*bit_ct] = c;
			bit_pos[*bit_ct] = b;
			(*bit_ct)++;
			if (*bit_ct < 4)
				continue;
			// Verific dacă fereastra de 4 biți conține 0010 sau 1101
			int cond1 = (bit_final[0] == 0 && bit_final[1] == 0 &&
						 bit_final[2] == 1 && bit_final[3] == 0);
			int cond2 = (bit_final[0] == 1 && bit_final[1] == 1 &&
						 bit_final[2] == 0 && bit_final[3] == 1);
			if (cond1 || cond2) {
				int px = bit_px[2];
				int py = bit_py[2];
				int ch = bit_color[2];
				int pos = bit_pos[2];
				// retin valorile pixelilor
				unsigned char r = img->data[py][px].r;
				unsigned char g = img->data[py][px].g;
				unsigned char bch = img->data[py][px].b;
				unsigned char new_r = r;
				unsigned char new_g = g;
				unsigned char new_b = bch;
				unsigned char *bit_dif;
				if (ch == 0) {
					bit_dif = &new_r;
				} else if (ch == 1) {
					bit_dif = &new_g;
				} else {
					bit_dif = &new_b;
				}
				// forțez bitul 3 să fie egal cu primul bit
				if (bit_final[0] == 1)
					*bit_dif |= (1 << pos);
				else
					*bit_dif &= ~(1 << pos);
				printf("Warning: pixel at (%d, %d) may be read as "
					   "(%d, %d, %d)\n", px, img->height - 1 - py,
					   new_r, new_g, new_b);
			}
		}
	}
}

void run_bitcheck(image *img)
{
	if (!img || !img->data) {
		printf("No image loaded\n");
		return;
	}
	int bit_final[4];
	int bit_ct = 0;
	int bit_px[4], bit_py[4], bit_color[4], bit_pos[4];
	// parcurg pixelii din memorie
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++) {
			bitcheck_error_print(img, bit_final, bit_px, bit_py, bit_color,
								 bit_pos, &bit_ct, j, i);
		}
	}
}

int main(void)
{
	lsystem lsys;
	lsys_arhiva arhiva;
	image img;
	image_arhiva img_arhiva;
	font *font = NULL;
	font_arhiva fnt_arhiva;
	istoric_undo *stiva_undo = NULL;
	int nr_undo = 0, max_undo = 0, nr_redo = 0, max_redo = 0;
	istoric_undo *stiva_redo = NULL;
	alloc_lsystem(&lsys);
	alloc_arhiva(&arhiva);
	alloc_image_arhiva(&img_arhiva);
	alloc_font_arhiva(&fnt_arhiva);
	img.data = NULL;
	img.width = 0;
	img.height = 0;
	char *last_filename = NULL;
	char info[100000];
	while (fgets(info, sizeof(info), stdin)) {
		info[strcspn(info, "\n")] = 0;
		if (strcmp(info, "EXIT") == 0)
			break;
		if (strncmp(info, "LSYSTEM ", 8) == 0) {
			char *file = info + 8;
			run_lsystem(&lsys, &arhiva, &stiva_undo, &nr_undo, &max_undo,
						&nr_redo, &last_filename, file);
		} else if (strncmp(info, "DERIVE ", 7) == 0) {
			run_derive(&lsys, info);
		} else if (strncmp(info, "LOAD ", 5) == 0) {
			char *file = info + 5;
			if (ppm_load(&img, file) == 1) {
				save_image_arhiva(&img_arhiva, &img);
				add_cmd(&stiva_undo, &nr_undo, &max_undo, comanda_image_turtle);
				nr_redo = 0;
				printf("Loaded %s (PPM image %dx%d)\n", file,
					   img.width, img.height);
			} else {
				printf("Failed to load %s\n", file);
			}
		} else if (strncmp(info, "SAVE ", 5) == 0) {
			char *file = info + 5;
			run_save_image(&img, file);
		} else if (strncmp(info, "TURTLE ", 7) == 0) {
			run_turtle(&lsys, &img, &img_arhiva, &stiva_undo, &nr_undo,
					   &max_undo, &nr_redo, info);
		} else if (strncmp(info, "FONT ", 5) == 0) {
			char *file = info + 5;
			if (load_font(&font, file) == 1) {
				save_font_arhiva(&fnt_arhiva, font, file);
				add_cmd(&stiva_undo, &nr_undo, &max_undo, comanda_font);
				nr_redo = 0;
				printf("Loaded %s (bitmap font %s)\n", file, font->nume);
			} else {
				printf("Failed to load font %s\n", file);
			}
		} else if (strncmp(info, "TYPE ", 5) == 0) {
			run_type(&img, &img_arhiva, font, &stiva_undo, &nr_undo,
					 &max_undo, &nr_redo, info);
		} else if (strcmp(info, "BITCHECK") == 0) {
			run_bitcheck(&img);
		} else if (strcmp(info, "UNDO") == 0) {
			run_undo(&lsys, &arhiva, &img, &img_arhiva, &font, &fnt_arhiva,
					 &stiva_undo, &nr_undo, &stiva_redo, &nr_redo, &max_redo);
		} else if (strcmp(info, "REDO") == 0) {
			run_redo(&lsys, &arhiva, &img, &img_arhiva, &stiva_undo, &font,
					 &fnt_arhiva, &nr_undo, &max_undo, &stiva_redo, &nr_redo,
					 last_filename);
		}
	}
	free_lsystem(&lsys);
	free_arhiva(&arhiva);
	free(last_filename);
	free_font(font);
	free_font_arhiva(&fnt_arhiva);
	free_image_arhiva(&img_arhiva);
	ppm_free(&img);
	free(stiva_undo);
	free(stiva_redo);
	return 0;
}
