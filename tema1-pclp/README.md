
PROBLEMA window_join:

  Am creat initial 2 functii f_cmmdc si f_cmmmc care calculeaza cmmdc si cmmmc intre 2 numere. Citesc de la tastatura variabila window rezprezentand
durata de timp . In vectorii t si x voi retine timpii cititi si valorile citite,iar in vectorii sir_cmmdc si sir_cmmmc cel mai mare divizor comun si
cel mai mic multimpul comun pentru fiecare pereche valida.Variabila nr_perechi reprezinta nr de perechi valide ce vor fi afisate la final. Citesc de
la tastatura perechi t_k si x_k pe care le retin in vectorii t si x. Ma voi opri din citit atunci cand gasesc perechea 0 0 .Pentru fiecare pereche
nou citita  verific daca diferenta dintre timpul t_k introdus ultimul si cei anteriori este mai mare decat fereastra window, cu ajutorul contorului i.
Daca este mai mare , ma opresc , pentru ca timpii sunt in ordine crescatoare.(timpii din spate vor fi si mai vechi). Altfel , inseamna ca am gasit o
pereche valida si voi retine in sir_cmmdc si in sir_cmmmc valorile functiilor apelate: f_cmmdc si f_cmmmc intre valoarea x_k citita ultima si cea
anterioara, x[i]. Totodata vectorii t1 si t2 retin momentele de timp aferente perechii. In t1 voi retine momentul corespondent valorii t[i],iar in
t2 momentul t_k.Dupa ce verific toate perechile gasite le voi sorta in functie de t1 crescator sau daca t1 dintre 2 elemente este egal , atunci dupa
t2 crescator, schimband atat timpii t1 si t2 , cat si valorile corespunzatoare din sir_cmmdc si sir_cmmmc.La final afisez pentru fiecare pereche valorile
cmmmc si cmmdc.

PROBLEMA akari:

  Citesc de la tastatura variabila o reprezentand cerinta pe care trebuie sa o rezolv, apoi gridul de n linii si m coloane.In acelasi timp fac o copie
a gridului, numita grid_luminat in care voi forma tabloul ce trebuie afisat la optiunea 1. Daca o este 1, atunci parcurg fiecare element din grid si
atunci cand gasesc un bec apelez functia light cu parametrii i si j,reprezentand coordonatele becului gasit.Functia light verifica elementele de pe
linia i si pe cele de pe coloana j si marcheaza cu 'x' toate celule albe intalnite pana la prima casuta neagra(numerotata sau nu). Dupa ce aplic functia
pentru toate becurile gasite, apelez functia check_black_cells.Aceasta functie parcurge fiecare element din grid. Pentru fiecare celula neagra numerotata
iau variabila bec in care retin numarul de becuri adiacente.Daca nr becurilor adiacente este egal cu valoarea inscriptionata pe celula neagra atunci voi
marca cu 'x' celulele albe adiacente .Ultimul pas este afisarea gridului luminat. Daca o este 2, atunci aplic functia check_rule1 si check_rule_2.Functia
check_rule_1 parcurge intreg gridul si pentru fiecare bec gasit verifica daca mai este vreunul in dreapta pe aceeasi linie sau in jos pe aceeasi coloana. 
Daca gasesc alt bec atunci functia returneaza 0, altfel returneaza 1. Functia is_illumin are ca parametrii i si j ,reprezentand coordonatele celulei pe 
are o analizez.Functia verifica daca o celula are alaturat un bec care sa o lumineze si returneaza 1 daca e luminata si 0, altfel. Functia check_rule_2
retine pentru fiecare casuta neagra numerotata din grid nr becurilor adiacente in variabila bec si nr casutelor albe adiacente neiluminate in variabila
possible.(ajutandu-ma de functia is_illumin). Daca nr becurilor adiacente este mai mare decat valoarea de pe celula neagra atunci regula este incalcata
si functia returneaza 0.Daca nr de casute ce pot fi iluminate este mai mic decat nr de becuri ce ar mai trebui puse in jurul casutei negre pentru a
ajunge la nr inscriptionat atunci regula este incalcata si functia returneaza 0. Altfel, returneaza 1. Daca ambele functii check_rule_1 si check_rule_2
sunt indeplinite si returneaza 1 atunci gridul este valid si afisez "ichi", altfel afisez "zero".

PROBLEMA helicopters:

  Citesc de la tastatura tabloul a de n linii si m coloane. Citesc k, reprezentand nr elicopterelor si pentru fiecare dintre ele coordonatele capetelor
ipotenuzei , pe care le retin in vectorii r1, c1, r2, c2. Vectorul helic_invalide retine elicopterele invalide,cele care nu se pot forma conform cerintei,
iar vectorul helic_poz_gresita retine elicopterele care fac umbra la mai mult de jumatate de patratele.Variabilele nr_helic_ok si nr_heli_poz_gresita retin
nr elicopterelor care au toata suprafata de 1 ,respectiv nr celor care fac umbra la mai mult de jumatate de suprafata. Pentru  fiecare elicopter verific
daca respecta cerinta . Daca capetele ipotenuzei se afla pe aceeasi linie sau coloana sau catetele nu sunt egale atunci elicopterul este invalid si il
retin in vectorul helic_invalide.Altfel, in l voi retine lungimea catetei ,iar in nr_patratele numarul patratelelor acoperite de elicopter.Daca r2>r1 atunci
interschimb capetele ipotenuzei pentru a avea indicii r1 si r2 in ordine crescatoare. Daca s este 1,adica triunghiul are varful deasupra ipotenuzei, atunci
apelez functia calcul_s_pozitiv.Variabilele nrunu si nrzero retin nr de unu si de zerouri din fiecare suprafata. Pracurg triunghiul de jos in sus de la randul
r2 la r1 cu ajutorul lui j.In variabila v calculez nivelul la care sunt in triunghi, dar si nr de elemente de pe linia j. Cu u voi parcurge v elemente de pe
linia j. Variabila col reprezinta coloana pe care ma aflu. Daca c1 > c2 atunci triunghiul se extinde in dreapta si col va fi c2+u,altfel triunghiul se extinde
in stanga si col va fi c2-u. Pentru fiecare element din triunghi verific daca este 0 sau 1 cu ajutorul variabilelor nrunu nrzero. Daca s este -1,adica triunghiul
are varful sub ipotenuza, atunci apelez functia calcul_s_neg.Variabilele nrunu si nrzero retin nr de unu si de zerouri din fiecare suprafata. Pracurg triunghiul
de sus in jos de la randul r1 la r2 cu ajutorul lui j.In variabila v calculez nivelul la care sunt in triunghi, dar si nr de elemente de pe linia j. Cu u voi
parcurge v elemente de pe linia j. Variabila col reprezinta coloana pe care ma aflu. Daca c1 > c2 atunci triunghiul se extinde in stanga si col va fi c1+u, 
altfel triunghiul se extinde in dreapta  si col va fi c1-u. Pentru fiecare element din triunghi verific daca este 0 sau 1 cu ajutorul variabilelor nrunu nrzero.
Daca nrunu este egal cu nr de patratele atunci elicopterul este ok. Daca nrzero este mai mare de jumatate din nr de patratele atunci retin in vectorul helic_poz_gresita
nr elicopterului. Afisez intai elicopterele pozitionate gresit din vectorul helic_invalide. Dupa afisez nr_helic_ok. Afisez nr_helic_poz_gresita, dar si care sunt
acele elicoptere.
