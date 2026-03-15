#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lsystem.h"

char *copy_string(char *sir)
{
	char *p = malloc(strlen(sir) + 1);
	if (p)
		strcpy(p, sir);
	return p;
}

void alloc_lsystem(lsystem *lsys)
{
	// initializez o structura l-system goala
	lsys->axioma = NULL;
	lsys->nr_rules = 0;
	lsys->rules = NULL;
	lsys->stare = 0;
}

void free_lsystem(lsystem *lsys)
{
	// eliberez structura l-system
	if (lsys->stare == 0)
		return;
	free(lsys->axioma);
	for (int i = 0; i < lsys->nr_rules; i++)
		free(lsys->rules[i].succesor);
	free(lsys->rules);
	alloc_lsystem(lsys);
}

void copy_lsystem(lsystem *lsys, lsystem *copie_lsys)
{
	// creez o copie a unui l-system cu toate elementele lui
	alloc_lsystem(copie_lsys);
	copie_lsys->stare = lsys->stare;
	copie_lsys->nr_rules = lsys->nr_rules;
	copie_lsys->axioma = copy_string(lsys->axioma);
	copie_lsys->rules = malloc(lsys->nr_rules * sizeof(rule));
	for (int i = 0; i < lsys->nr_rules; i++) {
		copie_lsys->rules[i].simbol = lsys->rules[i].simbol;
		copie_lsys->rules[i].succesor = copy_string(lsys->rules[i].succesor);
	}
}

char *find_rule(lsystem *lsys, char a)
{
	// caut regula pentru un simbol a si returnez succesorul, daca exista
	for (int i = 0; i < lsys->nr_rules; i++) {
		if (lsys->rules[i].simbol == a)
			return lsys->rules[i].succesor;
	}
	return NULL;
}

int incarcare_lsystem(lsystem *lsys, char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == 0)
		return 0;
	char buff[256];
	fgets(buff, sizeof(buff), file);
	buff[strcspn(buff, "\n")] = 0;
	char *axioma = copy_string(buff);
	int nr_rules;
	// citesc numarul de reguli
	fscanf(file, "%d\n", &nr_rules);
	// aloc un vector de reguli
	rule *rules = malloc(nr_rules * sizeof(rule));
	for (int i = 0; i < nr_rules; i++) {
		char simbol;
		char succ[256];
		fscanf(file, " %c %255s\n", &simbol, succ);
		// retin fiecare simbol si succesorul fiecaruia
		rules[i].simbol = simbol;
		rules[i].succesor = copy_string(succ);
	}
	fclose(file);
	// eliberez vechiul l-system si il incarc in sistem pe cel nou
	free_lsystem(lsys);
	lsys->axioma = axioma;
	lsys->nr_rules = nr_rules;
	lsys->rules = rules;
	lsys->stare = 1;
	return 1;
}

char *derive_lsystem(lsystem *lsys, int n)
{
	if	(lsys->stare == 0) {
		return NULL;
	}
	char *curr = malloc(strlen(lsys->axioma) + 1);
	strcpy(curr, lsys->axioma);
	for (int i = 0; i < n ; i++) {
		// calculez lungimea mesajului ce urmeaza a fi afisat
		int lung = 0;
		for (int j = 0; curr[j] != '\0'; j++) {
			char *succ = find_rule(lsys, curr[j]);
			if (succ)
				lung = lung + strlen(succ);
			else
				lung++;
		}
		char *new_string = malloc(lung + 1);
		int poz = 0;
		// formez mesajul, caracater cu caracter, pe baza regulilor
		for (int j = 0; curr[j] != '\0'; j++) {
			char *succ = find_rule(lsys, curr[j]);
			if (succ) {
				strcpy(new_string + poz, succ);
				poz = poz + strlen(succ);
			} else {
				new_string[poz] = curr[j];
				poz++;
			}
		}
		new_string[poz] = '\0';
		free(curr);
		curr = new_string;
	}
	return curr;
}

void alloc_arhiva(lsys_arhiva *arhiva)
{
	// initializez arhiva pentru undo/redo
	arhiva->limit = 2;
	arhiva->curr_poz = -1;
	arhiva->contor = 0;
	arhiva->history = malloc(arhiva->limit * sizeof(lsystem));
}

void free_arhiva(lsys_arhiva *arhiva)
{
	// eliberez arhiva l-system
	for (int i = 0; i < arhiva->contor; i++)
		free_lsystem(&arhiva->history[i]);
	free(arhiva->history);
	arhiva->history = NULL;
	arhiva->contor = 0;
	arhiva->curr_poz = -1;
	arhiva->limit = 0;
}

void grow_arhiva(lsys_arhiva *arhiva)
{
	// dublez capacitatea arhivei
	arhiva->limit = arhiva->limit * 2;
	arhiva->history = realloc(arhiva->history,
							  arhiva->limit * sizeof(lsystem));
}

void save_arhiva(lsys_arhiva *arhiva, lsystem *lsys)
{
	if (lsys->stare == 0)
		return;
	// elimin starile de REDO, daca exista, incepand de la pozitia curenta
	for (int i = arhiva->curr_poz + 1; i < arhiva->contor; i++)
		free_lsystem(&arhiva->history[i]);
	// arhiva se termina acum la pozitia curenta;
	arhiva->contor = arhiva->curr_poz + 1;
	// daca e nevoie, maresc capacitatea arhivei
	if (arhiva->contor == arhiva->limit)
		grow_arhiva(arhiva);
	// copiez lsystem-ul in arhiva
	copy_lsystem(lsys, &arhiva->history[arhiva->contor]);
	arhiva->curr_poz = arhiva->contor;
	arhiva->contor++;
}

int undo_arhiva(lsys_arhiva *arhiva, lsystem *lsys)
{
	if (arhiva->curr_poz < 0) {
		return 0;
	}
	free_lsystem(lsys);
	// revin la starea anterioara din arhiva
	arhiva->curr_poz--;
	if (arhiva->curr_poz >= 0)
		// copiez intreaga structura l-system din arhiva in l-systemul activ
		copy_lsystem(&arhiva->history[arhiva->curr_poz], lsys);
	return 1;
}

int redo_arhiva(lsys_arhiva *arhiva, lsystem *lsys)
{
	// verific daca sunt la ultima stare disponibila
	if (arhiva->curr_poz + 1 >= arhiva->contor) {
		//printf("Nothing to redo\n");
		return 0;
	}
	// avansez in arhiva cu o pozitie
	arhiva->curr_poz++;
	free_lsystem(lsys);
	// copiez l-systemul de la noua pozitie in l-systemul activ
	copy_lsystem(&arhiva->history[arhiva->curr_poz], lsys);
	return 1;
}
