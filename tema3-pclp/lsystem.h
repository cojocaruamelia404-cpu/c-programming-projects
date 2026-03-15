#ifndef LSYSTEM_H
#define LSYSTEM_H

typedef struct {
	char simbol;
	char *succesor;
} rule;

typedef struct {
	char *axioma;
	int nr_rules;
	rule *rules;
	int stare;
} lsystem;

typedef struct {
	lsystem *history;
	int curr_poz;
	int contor;
	int limit;
} lsys_arhiva;

void alloc_lsystem(lsystem *lsys);
void free_lsystem(lsystem *lsys);
int incarcare_lsystem(lsystem *lsys, char *filename);
char *derive_lsystem(lsystem *lsys, int n);

void alloc_arhiva(lsys_arhiva *arhiva);
void free_arhiva(lsys_arhiva *arhiva);
void save_arhiva(lsys_arhiva *arhiva, lsystem *lsys);
int undo_arhiva(lsys_arhiva *arhiva, lsystem *lsys);
int redo_arhiva(lsys_arhiva *arhiva, lsystem *lsys);

#endif
