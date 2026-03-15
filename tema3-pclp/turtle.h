#ifndef TURTLE_H
#define TURTLE_H

typedef struct {
	unsigned char r, g, b;
} pixel;

typedef struct {
	int width, height;
	pixel **data;
} image;

typedef struct {
	double x, y, unghi;
} turtle_arhiva;

typedef struct {
	image *stari_turtle;
	int contor;
	int curr_poz;
	int capacitate;
} image_arhiva;

int ppm_load(image *img, const char *filename);
int ppm_save(image *img, const char *filename);
void ppm_draw(image *img, int x0, int y0, int x1, int y1, int r, int g, int b);
void ppm_free(image *img);
void alloc_image_arhiva(image_arhiva *arhiva);
int undo_image(image_arhiva *arhiva, image *img);
int redo_image(image_arhiva *arhiva, image *img);
void save_image_arhiva(image_arhiva *arhiva, image *img);
void free_image_arhiva(image_arhiva *arhiva);
#endif
