#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "font.h"

int char_to_int(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return 0;
}

void alloc_font(font *font)
{
	// initializez un font
	font->glyphs = NULL;
	font->nr_glyphs = 0;
	font->nume = NULL;
}

void free_font(font *font)
{
	// eliberez memoria ocupata de un font
	if (!font)
		return;
	for (int i = 0; i < font->nr_glyphs; i++) {
		for (int y = 0; y < font->glyphs[i].bb_h; y++) {
			free(font->glyphs[i].bitmap[y]);
		}
		free(font->glyphs[i].bitmap);
	}
	free(font->glyphs);
	free(font->nume);
	free(font);
}

char *copy_text(const char *text)
{
	int lung = strlen(text);
	char *copie_text = malloc(lung + 1);
	if (!copie_text)
		return NULL;
	for (int i = 0; i < lung; i++)
		copie_text[i] = text[i];
	copie_text[lung] = '\0';
	return copie_text;
}

void citire_bitmap(FILE *file, glyph *gly)
{
	char line[1024];
	for (int i = 0; i < gly->bb_h; i++) {
		if (!fgets(line, sizeof(line), file))
			break;
		int cursor_x = 0;
		for (int k = 0; line[k] && line[k] != '\n' && line[k] != '\r'; k++) {
			int val = char_to_int(line[k]);
			for (int bit = 3; bit >= 0; bit--) {
				if (cursor_x < gly->bb_w) {
					gly->bitmap[i][cursor_x] = (val >> bit) & 1;
					cursor_x++;
				}
			}
		}
	}
}

int load_font(font **f, const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file)
		return 0;
	// aloc un font nou
	font *new_font = malloc(sizeof(font));
	if (!new_font) {
		fclose(file);
		return 0;
	}
	alloc_font(new_font);
	char line[100000];
	// initializez un contor in care sa retin la ce litera sunt
	int glyph_ct = -1;
	while (fgets(line, sizeof(line), file)) {
		line[strcspn(line, "\n")] = 0;
		if (strncmp(line, "FONT ", 5) == 0) {
			// citesc numele fontului
			char *nume = line + 5;
			new_font->nume = malloc(strlen(nume) + 1);
			strcpy(new_font->nume, nume);
		} else if (strncmp(line, "CHARS ", 6) == 0) {
			// citesc nr de caractere care urmeaza
			// si aloc vectorul de glife
			int n;
			if (sscanf(line + 6, "%d", &n) == 1) {
				new_font->nr_glyphs = n;
				new_font->glyphs = malloc(n * sizeof(glyph));
			}
		} else if (strncmp(line, "STARTCHAR", 9) == 0) {
			// incepe o litera noua
			glyph_ct++;
			glyph *gly = &new_font->glyphs[glyph_ct];
			// initializez structura
			gly->encoding = 0;
			gly->dwx = 0;
			gly->dwy = 0;
			gly->bb_w = 0;
			gly->bb_h = 0;
			gly->bb_xoff = 0;
			gly->bb_yoff = 0;
			gly->bitmap = NULL;
		} else if (strncmp(line, "ENCODING ", 9) == 0) {
			glyph *gly = &new_font->glyphs[glyph_ct];
			if (sscanf(line + 9, "%d", &gly->encoding) != 1)
				return 0;
		} else if (strncmp(line, "DWIDTH ", 7) == 0) {
			glyph *gly = &new_font->glyphs[glyph_ct];
			if (sscanf(line + 7, "%d %d", &gly->dwx, &gly->dwy) != 2)
				return 0;
		} else if (strncmp(line, "BBX ", 4) == 0) {
			glyph *gly = &new_font->glyphs[glyph_ct];
			// citesc dimenisunile matricei de pixeli
			if (sscanf(line + 4, "%d %d %d %d", &gly->bb_w, &gly->bb_h,
					   &gly->bb_xoff, &gly->bb_yoff) == 4) {
				// aloc matricea pentru acest caracter
				gly->bitmap = malloc(gly->bb_h * sizeof(unsigned char *));
				for (int i = 0; i < gly->bb_h; i++)
					gly->bitmap[i] = malloc(gly->bb_w * sizeof(unsigned char));
			}
		} else if (strncmp(line, "BITMAP", 6) == 0) {
			citire_bitmap(file, &new_font->glyphs[glyph_ct]);
		}
	}
	fclose(file);
	if (*f)
		free_font(*f);
	*f = new_font;
	return 1;
}

int find_glyph(font *font, char c)
{
	// caur in font indexul glifei corespunzatoare caracterului c
	int cod = (unsigned char)c;
	for (int i = 0; i < font->nr_glyphs; i++) {
		if (font->glyphs[i].encoding == cod)
			return i;
	}
	return -1;
}

void alloc_font_arhiva(font_arhiva *arhiva)
{
	// initializez o arhiva de fonturi
	arhiva->stari_font = NULL;
	arhiva->fisier_font = NULL;
	arhiva->contor = 0;
	arhiva->curr_poz = -1;
	arhiva->capacitate = 0;
}

void free_font_arhiva(font_arhiva *arhiva)
{
	// eliberez memoria alocata unei arhive
	for (int i = 0; i < arhiva->contor; i++) {
		free_font(arhiva->stari_font[i]);
		free(arhiva->fisier_font[i]);
	}
	free(arhiva->stari_font);
	free(arhiva->fisier_font);
	arhiva->stari_font = NULL;
	arhiva->contor = 0;
	arhiva->curr_poz = -1;
	arhiva->capacitate = 0;
}

void copy_font(font *copie_f, font *font)
{
	// fac o copie a unui font cu toate elementele
	if (!font || !copie_f)
		return;
	alloc_font(copie_f);
	copie_f->nume = malloc(strlen(font->nume) + 1);
	for (int i = 0; font->nume[i] != '\0'; i++)
		copie_f->nume[i] = font->nume[i];
	copie_f->nume[strlen(font->nume)] = '\0';
	copie_f->nr_glyphs = font->nr_glyphs;
	copie_f->glyphs = malloc(copie_f->nr_glyphs * sizeof(glyph));
	for (int i = 0; i < copie_f->nr_glyphs; i++) {
		glyph *g_font = &font->glyphs[i];
		glyph *g_copie = &copie_f->glyphs[i];
		g_copie->encoding = g_font->encoding;
		g_copie->dwx = g_font->dwx;
		g_copie->dwy = g_font->dwy;
		g_copie->bb_w = g_font->bb_w;
		g_copie->bb_h = g_font->bb_h;
		g_copie->bb_xoff = g_font->bb_xoff;
		g_copie->bb_yoff = g_font->bb_yoff;
		g_copie->bitmap = malloc(g_copie->bb_h * sizeof(unsigned char *));
		for (int y = 0; y < g_copie->bb_h; y++) {
			g_copie->bitmap[y] = malloc(g_copie->bb_w * sizeof(unsigned char));
			for (int x = 0; x < g_copie->bb_w; x++) {
				g_copie->bitmap[y][x] = g_font->bitmap[y][x];
			}
		}
	}
}

void save_font_arhiva(font_arhiva *arhiva, font *f, char *file)
{
	if (!f)
		return;
	// sterg starile de redo
	for (int i = arhiva->curr_poz + 1; i < arhiva->contor; i++) {
		free_font(arhiva->stari_font[i]);
		free(arhiva->fisier_font[i]);
	}
	arhiva->contor = arhiva->curr_poz + 1;
	// maresc capacitatea arhivei, daca e nevoie
	if (arhiva->contor == arhiva->capacitate) {
		if (arhiva->capacitate == 0) {
			arhiva->capacitate = 2;
		} else {
			arhiva->capacitate = arhiva->capacitate * 2;
		}
		arhiva->stari_font = realloc(arhiva->stari_font,
									 arhiva->capacitate * sizeof(font *));
		arhiva->fisier_font = realloc(arhiva->fisier_font,
									  arhiva->capacitate * sizeof(char *));
	}
	arhiva->stari_font[arhiva->contor] = malloc(sizeof(font));
	copy_font(arhiva->stari_font[arhiva->contor], f);
	int lg = strlen(file);
	arhiva->fisier_font[arhiva->contor] = malloc(lg + 1);
	strcpy(arhiva->fisier_font[arhiva->contor], file);
	arhiva->curr_poz = arhiva->contor;
	arhiva->contor++;
}

int undo_font(font_arhiva *arhiva, font **f)
{
	if (arhiva->curr_poz < 0)
		return 0;
	free_font(*f);
	// revin la fontul anterior din istoric
	arhiva->curr_poz--;
	if (arhiva->curr_poz >= 0) {
		*f = malloc(sizeof(font));
		copy_font(*f, arhiva->stari_font[arhiva->curr_poz]);
	}
	return 1;
}

int redo_font(font_arhiva *arhiva, font **f)
{
	if (arhiva->curr_poz + 1 >= arhiva->contor)
		return 0;
	arhiva->curr_poz++;
	free_font(*f);
	*f = malloc(sizeof(font));
	copy_font(*f, arhiva->stari_font[arhiva->curr_poz]);
	return 1;
}
