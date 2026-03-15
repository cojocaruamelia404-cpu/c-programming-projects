#ifndef FONT_H
#define FONT_H

typedef struct {
	int encoding;
	int dwx, dwy;
	int bb_w, bb_h;
	int bb_xoff, bb_yoff;
	unsigned char **bitmap;
} glyph;

typedef struct {
	char *nume;
	int nr_glyphs;
	glyph *glyphs;
} font;

typedef struct {
	font **stari_font;
	char **fisier_font;
	int contor;
	int curr_poz;
	int capacitate;
} font_arhiva;

int char_to_int(char c);
void alloc_font(font *font);
void free_font(font *font);
int load_font(font **f, const char *filename);
int find_glyph(font *font, char c);

void alloc_font_arhiva(font_arhiva *arhiva);
void free_font_arhiva(font_arhiva *arhiva);
void save_font_arhiva(font_arhiva *arhiva, font *font, char *file);
int undo_font(font_arhiva *arhiva, font **f);
int redo_font(font_arhiva *arhiva, font **f);
void copy_font(font *copie_f, font *font);
#endif
