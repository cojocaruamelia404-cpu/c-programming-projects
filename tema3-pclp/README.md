
L-system:
  In biblioteca lsystem.h mi-am definit toate functiile pe care le voi
folosi la acest task. Structura rule defineste o regula din fisier si are 2
componente: un simbol si succesorul sau (regula), iar lsystem defineste un
lsystem, cu componenetele : axioma, numarul de reguli, un vector de structuri
rules si variabila stare in care retin daca exista un lsystem incarcat sau nu.
Structura lsys_arhiva este alcatuita din un vector de structuri lsystem in 
care voi retine fiecare lsystem incarcat intr-o arhiva.Functia copy_string face
o copie a unui sir, iar copy_lsystem face o copie a unui lsystem cu toate 
elementele sale. Alloc_lsystem initializeaza o structura lsystem goala, iar
free_lsystem goleste memoria ocupata de acesta. In find_rule caut in regulile
unui lsystem pentru un simbol a dat care este succesorul sau, regula si o
returnez. Functia incarcare_lsystem citeste din fisierul dat axioma, numarul de
reguli, iar pentru fiecare , un simbol si un succesor. Eliberez vechiul lsystem
din memorie si il incarc pe cel nou. Daca operatia decurge bine functia
returneaza 1. In functia derive_lsystem , pentru fiecare derivare din cele n
pe care trebuie sa le fac calculez lungimea mesajului ce urmeaza a se forma.
In variabila curr voi retine sirul pe care trebuie sa il transform.
Astfel, adun la lungimea totala nr de caractere al succesorului fiecarui
element din curr. Aloc un sir nou new_string de aceasta dimensiune in care
formez mesajul, caracter cu caracter, pe baza regulilor. La final, curr va
deveni egal cu new_string. Funcia alloc_arhiva initializeaza o arhiva, iar
free_arhiva elibereaza memoria ocupata de una. In save_arhiva, elimin toate
starile de redo existente. Parcurg toate elementele aflate după pozița curenta
(curr_poz) și le eliberez memoria, deoarece acele versiuni ale lsystemului nu
mai sunt accesibile dupa un undo. Actualizez contorul arhivei, verific daca am
ajuns la limita maxima a acesteia si daca da, ii dublez dimensiunea prin
functia grow_arhiva. Copiez lsystemul in arhiva. Functia undo_arhiva verifica,
la inceput, daca suntem la inceputul arhivei si daca da, returneaza 0. Altfel,
mut cursorul pe pozitia precedenta din istoric. Daca sunt pe o pozitie valida,
copiez datele lsystemului din arhiva in lsystemul activ.Daca decurge bine,
functia returneaza 1. Asemanator, functia redo_arhiva verifica daca sunt la
ultima stare disponibila. Daca nu, avansez cu o pozitie in arhiva si copiez
lsystemul de pe acea pozitie in lsystemul activ.
    In runic.c, definesc tipurile de comenzi undoable care pot aparea, dar si o
structura istoric_undo, care are ca parametru tip_cmd, tipul comezii introduse.
Functia add_cmd adauga o comanda tip.cmd intr-o stiva. Get_cmd salveaza la
adresa pointerului *tip_cmd tipul ultimelei comenzi introduse, iar remove_cmd
scade contorul stivei, eliminand astfel ultima comanda. In main definesc un
lsystem lsys si o arhiva de tip lsys_arhiva, pe care le aloc cu functiile
descrise anterior. Initializez si 2 stive de tip istoric_undo, numite
stiva_undo si stiva_redo in care voi retine comezile introduse, cu scopul
anularii sau reefectuarii uneia. Citesc, pe rand, comenzile. Daca primesc EXIT,
atunci se opreste bucla. Daca comanda e LSYSTEM, atunci apelez functia
run_lsystem. Aceasta verifica daca a putut fi incarcat in sistem un lsystem.
Daca da, il salvez in arhiva prin functia save_arhiva, adaug comanda in stiva
de undo prin functia add_cmd, si afisez mesajul corespunzator. Daca nu, afisez
mesajul de esec.Daca comanda e DERIVE, apelez functia run_derive. Aceasta
retine in pointerul result rezultatul derivarii de n ori a lsystemului, prin
functia derive_lsystem. Afisez mesajul specific, tinand cont daca rezultatul e
null sau nu.

Turle:
    În biblioteca turtle.h mi-am definit toate funcțiile pe care le voi folosi
la acest task. Structura pixel definește culorile unui punct din imagine prin
cele trei componente: r, g și b. Structura image definește o imagine, având ca
si componente: lățimea (width), înălțimea (height) și o matrice de structuri
de tip pixel numită data. Structura image_arhiva este alcătuită dintr-un vector
de structuri image, numit stari_turtle, în care voi reține fiecare imagine
într-o arhivă. Funcția copy_image creează o copie a unei imagini cu toate
elementele sale.Funcția ppm_free golește memoria ocupată de datele unei
imagini. În funcția ppm_load, citesc dintr-un fișier binar datele specifice 
formatului PPM (magic number, dimensiuni, valoare maximă), eliberez imaginea
anterioară din memorie și încarc noile date. Dacă decurge bine, funcția
returnează 1. În ppm_save, salvez imaginea curentă într-un fișier PPM,
scriind header-ul necesar și apoi datele pixelilor din matrice. Funcția
ppm_draw implementează algoritmul lui Bresenham pentru a trasa o linie între
două puncte date (x0, y0) și (x1, y1). Aceasta verifică dacă fiecare pixel se
află în interiorul limitelor imaginii si dupa ii atribuie culoarea primită
ca parametru. Funcția alloc_image_arhiva inițializează o arhivă goală pentru
istoricul imaginilor, setând contorul la zero și cursorul poziției curente la
-1. Funcția free_image_arhiva eliberează memoria ocupată de imaginile salvate
în arhiva. În save_image_arhiva, elimin toate stările de redo existente
prin parcurgerea elementelor de după poziția curentă și eliberarea
acestora, deoarece devin inaccesibile după o modificare undo. Actualizez
contorul arhivei, verific dacă am ajuns la limita capacității și, dacă este
nevoie, dublez dimensiunea vectorului de stări prin realloc. La final, copiez
imaginea curentă în arhivă. Funcția undo_image verifică dacă mai există stări
anterioare în istoric. Dacă da, mută cursorul pe poziția precedentă, eliberează
imaginea activă și o copiază în aceasta pe cea din arhivă. Dacă s-a ajuns la
începutul istoricului, imaginea devine nulă. Asemănător, funcția redo_image
verifică dacă există stări anulate anterior prin undo. Dacă da, avansează
cursorul cu o poziție și copiaza imaginea de pe acea pozitie în imaginea activă.
    În funcția main, initializez o structura de tip image, numita img și arhiva
acesteia, img_arhiva. Dacă primesc comanda LOAD, apelez funcția ppm_load. Dacă
imaginea este încărcată cu succes, o salvez în arhivă, prin save_image_arhiva,
adaug comanda pe stiva de undo cu eticheta comanda_image_turtle și resetez
contorul stivei de redo la 0. Pentru comanda SAVE, apelez functia
run_save_image, care scrie datele matricei în format binar PPM prin functia
ppm_save si afiseaza mesajul de succes sau esec. Dacă este introdusa comanda
TURTLE, apelez funcția run_turtle. Aceasta verifică existența unei imagini și
a unui L-system si afiseaza mesajele corespunzatoare daca nu exista. Altfel,
extrag parametrii de desenare (poziție, pas, unghi, culori). Formez șirul de
instrucțiuni prin derive_lsystem și îl parcurg caracter cu caracter.'F' pentru
desenarea unei linii folosind ppm_draw și actualizarea coordonatelor prin
funcțiiile sin si cos , '+' și '-' pentru schimbarea unghiului, iar parantezele
pătrate pentru salvarea sau restaurarea stării cursorului(coordonate si unghi).
Realizez asta prin functia add_state care salveaza starea turtle pe stiva.
Functia get_state afla care este ultima stare aflata pe stiva, iar remove_state
scade contorul stivei, eliminand astfel ultima stare. La finalul desenării,
noua stare a imaginii este salvată în arhivă prin save_image_arhiva si adaug
comanda pe stiva de undo prin add_cmd.

Font :
    În biblioteca font.h mi-am definit toate funcțiile necesare pentru
procesarea fonturilor de tip bitmap. Structura glyph definește un singur
caracter, reținând codul său (encoding), deplasarea cursorului la desenare
(dwx, dwy), dimensiunile matricei de pixeli (bb_w, bb_h, bb_xoff, bb_yoff)
și matricea de biți numită bitmap. Structura font este alcatuita dintr-un
vector de structuri glyph, numarul de caractere, dar si numele fontului.
Structura font_arhiva este alcătuită din vectorul de structuri font , numit
stari_font, în care voi retine fonturile intr-o arhiva. Funcția char_to_int
convertește un caracter într-un numar. Funcția alloc_font inițializează o
structură de tip font, cu toate elementele sale, iar funcția free_font,
elibereaza memoria ocupata de un font. Funcția copy_font realizează o copie a
unui font, alocând memorie nouă pentru nume și pentru fiecare glyph în parte.
Funcția load_font citește linie cu linie și identifică secțiunile cheie:
FONT pentru nume, CHARS pentru numărul de caractere și STARTCHAR pentru 
nceputul fiecărei glife. Când întâlnește eticheta BITMAP, apelează funcția
citire_bitmap, care prelucrează datele și le transformă în biți (0 sau 1) 
în matricea glifei. Sterge din memorie vechiul font si il introduce pe acesta.
Daca operatia decurge bine, functia returneaza 1. Funcția find_glyph caută în
vectorul de glife indexul corespunzător unui caracter dat, c, prin compararea
codului său ASCII cu câmpul encoding. Funcția alloc_font_arhiva initializeaza
o arhiva de fonturi, iar free_font_arhiva curăță memoria acesteia. În
save_font_arhiva, elimin stările de redo viitoare care devin invalide după o
nouă încărcare. Actualizez contorul arhivei, verific dacă am ajuns la limita
capacității și, dacă este nevoie, dublez dimensiunea vectorului de stări prin
realloc.La final, adaug fontul in istoric. Funcția undo_font verifică dacă
mai există fonturi anterioare în istoric. Dacă da, mută cursorul pe poziția
anterioara, sterge imaginea activă și o copiază în aceasta pe cea din arhivă.
Asemănător, funcția redo_font verifică dacă există stări anulate anterior prin
undo. Dacă da, avansează cursorul cu o poziție și copiaza fontul de pe acea
pozitie în fontul activ.
    În funcția main, am declarat un pointer către structura font, inițializat
cu NULL, și o arhivă de tip font_arhiva numită fnt_arhiva, pe care o aloc cu
funcția specifică. Dacă primesc comanda FONT, extrag numele fișierului și
apelez funcția load_font. Dacă acesta este încărcat cu succes, îl salvez în
arhivă prin save_font_arhiva, adaug comanda pe stiva de undo cu eticheta
comanda_font, resetez contorul stivei de redo la 0 si afisez mesajul de succes.
Altfel, afisez mesajul de esec.Dacă este introdusă comanda TYPE, apelez funcția
run_type. Aceasta verifică existența unei imagini și a unui font încărcat,
afișând mesaje de eroare în caz contrar. Altfel, extrag textul cuprins între
ghilimele, coordonatele de start și culorile r, g, b. Parcurg textul caracter
cu caracter și folosesc funcția find_glyph pentru a găsi indexul fiecărei 
itere în fontul activ.Apoi, desenez fiecare literă pe imagine folosind funcția
draw_glyph. Aceasta parcurge matricea de biți a glifei, iar daca bitul din
bitmap e 1 colorează pixelii corespunzători, actualizând poziția cursorului
pe orizontală după fiecare caracter scris. La finalul scrierii, noua stare a
imaginii este salvată în arhivă prin save_image_arhiva si adaug comanda pe 
stiva de undo cu eticheta comanda_image_type, prin add_cmd.

Bitcheck:
    În funcția main, daca primesc comanda BITCHECK, apelez funcția 
run_bitcheck. Aceasta parcurge imaginea curentă pentru a identifica eventuale
secvente de biți care ar putea fi interpretate greșit. Funcția parcurge
matricea de pixeli a imaginii, iar pentru fiecare punct, apelează funcția
bitcheck_error_print, avand ca parametri componentele de culoare și poziția
pixelului. În interiorul funcției bitcheck_error_print, extrag biții fiecărei
culori (r, g, b) și îi introduc într-o fereastră glisantă de 4 biți, numită
bit_final. Această fereastră o gestionez cu ajutorul unui contor, bit_ct,
care asigură că am mereu ultimii 4 biți procesați din datele imaginii.
Pentru fiecare bit nou adăugat, verific dacă secvența formată
corespunde sirurilor: 0010 sau 1101. Dacă gasesc o astfel de secvență,
fortez al treilea bit din secvență sa fie egal cu primul bit. Afișez mesajul
specific care indică coordonatele pixelului și valorile r, g, b pe care
acesta le-ar putea avea în urma unei interpretări eronate.

Undo si Redo :
    În main, daca primesc comanda UNDO, apelez funcția run_undo. Aceasta
verifica daca exista vreo comanda salvata pe stiva de undo prin get_cmd. Daca
nu exista, afiseaza mesajul specific. Dacă stiva nu este goală, extrag tipul
ultimei comenzi și o elimin de pe stivă prin remove_cmd. În funcție de eticheta
identificată (comanda_lsys, comanda_image_turtle, comanda_font sau
comanda_image_type), apelez funcția de undo specifică fiecăruia. Spre exemplu,
apelez undo_arhiva pentru lsystem sau undo_image pentru imagini. Dacă operația
de undo reușește, adaug tipul comenzii pe stiva_redo folosind add_cmd,
pregătind-o pentru o eventuală restaurare.
    În main, daca primesc comanda REDO, apelez funcția run_redo. Aceasta verifica
prin get_cmd dacă există acțiuni anulate anterior pe stiva_redo. Dacă există,
elimin comanda de pe această stivă și o reintroduc pe stiva_undo. În funcție de
tipul comenzii, apelez funcția de restaurare corespunzătoare, precum
redo_arhiva sau redo_image, care recuperează starea din arhivă și o încarcă în
structura activă.


