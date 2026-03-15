
Problema battleships:
    Initial , am alocat dinamic tablele celor 2 jucatori prin functia alloc_board si mi-am definit
o structura cu ajutorul careia retin pentru fiecare nava lungimea ei si coordonatele de inceput si de sfarsit.
Functiile nume_nava si nume_nava_lg imi returneaza numele complet al navei in functie de initiala, respectiv lungime.
Functia orientare returneaza ce fel de nava este plasata, iar lg_nava lungimea ei in functie de tipul ei. Aceste 
functii ma ajuta sa afisez mesajele ca in cerinta.
    In functia procesare_meciuri incep sa parcurg fiecare meci din cele j cu un for. Citesc n si m si aloc tablele celor 2 jucatori,
tabla1 si tabla2.Aloc si 2 vectori de strcturi ship1 si ship2 in care voi retine date despre navele plasate de fiecare jucator.
Cu ajutorul functiei total_n calculez numarul de nave pe care fiecare jucator trebuie sa le plaseze.Astfel in total_nave 
voi avea acest numar ori 2, adica pentru intreg meciul. Apelez functia plasare_nave. Imi iau un contor turn in care retin 
al cui jucator este randul si unul nr_nave prin care retin cate nave am pus deja.Cat timp mai am nave de asezat, citesc tipul,
orientarea si x si y, reprezentand coordonatele.Verific al cui este randul pentru a stii pe ce tabla voi pune navele.
De asemenea, in functie de turn voi alege si vectorul s, reprezentand vectorul de struct in care voi retine navele. 
Functia verif_dimensiune verifica daca pentru coord x si y, nava incape in tabla, in functie de orientare, vertical/orizontal.
Functia verif_suprapunere verifica daca a fost deja plasata o nava la acele coordonate. Daca una din aceste functii returneaza
0, atunci se afiseaza mesajul corespunzator. Altfel , voi completa tabla cu lungimea navei in fiecare casuta din nava, prin functia
complete_ship. Voi retine coordonatele de inceput si de sfarsit, dar si lungimea in vectorul s. Afisez cele 2 table.
    Apelez functia procesare_atacuri. In functie de al cui e randul , aleg tabla si vectorul de nave .Daca coordonatele introduse
nu se incadreaza in limitele tablei , afisez mesajul corespunzator. Altfel , in tinta voi retine valoarea din tabla de la acele
coordonate. Cu -1 voi marca toate loviturile introduse de la tastatura. Daca tinta e -1, atunci afisez mesajul pentru coordonate
deja atacate.Daca este 0 , atunci o marchez cu -1 si afisez mesajul pentru lovitura ratata.Daca tinta este un nr > 0, inseamna ca
loveste o nava. In functia lovitura_nava verific care dintre nave are lungimea egala cu tinta si care contine coordonatele x si y.
A doua parte o verific prin functia ship_contains. Dupa ce gasesc nava , verific daca a fost o lovitura la capul navei si afisez
mesajul corespunzator. Mai mult , prin functia marcheaza_distrusa voi pune in toata nava 0 , mai putin in cap, pentru a retine ca sunt 
lovituri ratate.Daca nu este lovitura la cap, atunci afisez mesajul pentru lovitura. Verific care dintre jucatori ajunge la 0 nave
ramase si afisez castigatorul.

Problema statistici :
    Mi-am definit o structura in care retin pentru fiecare meci loviturile nimerite si loviturile totale in variabilele hit si total.
Citesc cheia si apoi comenzile pana ajung la Q.Daca cheia este "-",inseamna ca mesajul nu e codat, deci voi copia in sirul com_decript comanda.
Voi afla jucatorul, daca trebuie o statistica pentru un meci sau pentru toate, numarul meciului acuratetea si daca trebuie un min sau un max.
Aloc dinamic un vector k in care retin navele din fiecare meci, 2 vectori h1, h2 in care retin pentru fiecare jucator loviturile nimerite si
totale per meci si 2 vectori acc1, acc2 in care retin acuratetea fiecarui meci pentru fiecare jucator. Voi creste totalul la fiecare meci,
iar hit doar la loviturile asupra unei nave.In functia procesare_meciuri, la sfarsitul fiecarui meci voi retine in acc1 si acc2 acuratetea
avuta de fiecare jucator la acel meci.
    Verific daca meciul respectiv nu exsita si afisez mesajul corespunzator .Altfel, imi iau 2 variabile cur_acc si rez in care retin acuratetea meciului
si rezultatul statisticii.Verific ce statistica trebuie sa aflu in functie de tip1 si tip2.Functia statistica2 verifica numarul maxim de lovituri 
nimerite pentru care acuratetea era mai mare decat target.Afiseaza diferenta dintre aceasta valoare si numarul de meciuri nimerite ,reprezentand,
de fapt, numarul minim de ratari.In mod asemanator, pentru statistica 4 apelez statistica4.In aceasta functie voi porni de la numarul de lovituri nimerite,
hit, si le scad pe rand, crescand astfel numarul de ratari. La fiecare pas verific daca am scazut acuratetea sub cea ceruta. Dupa ce ies din bucla, i 
devine primul numar de lovituri nimerite care nu ar indeplini cerinta, asa ca il cresc o data.Returnez numarul hit-i, adica numarul  maxim de ratari.
Pentru statisticile 1 si 3 ma folosesc de functia calc_sum in care calculez in variabila sum suma produselor dintre acuratetea fiecarui meci si numarul
de nave din fiecare, iar in sum_k suma navelor din meciuri.Pentru statistica 1 , imi voi crea un vector castig in care retin pentru fiecare meci ce
castig ar aduce la acuratete daca ar fi fost 100%, sortez acest vector descrescator , intrucat ma intereseaza numarul minim de meciuri cu castig 100%.
Adaug la sum printr o bucla castigul fiecarui meci sortat si verific daca acuratetea totala ar fi mai mare decat target.Pentru statistica 3 , imi voi 
crea un vector castig in care retin pentru fiecare meci ce castig aduce la acuratete si sortez acest vector crescator , intrucat ma intereseaza numarul 
maxim de meciuri ratate, adica cu aport 0. Astfel am nevoie de meciurile cu cel mai mic impact.Pentru fiecare meci, calculez acuratetea fara contributia
lui, verific daca a scazut sub target si daca da, ma opresc.Prin functia calcul_rezultat, retin in sirul rezultat ce trebuie sa afisez, in formatul cerut.
Daca cheia este "-", atunci afisez direct rezultat.

Problema Hill Chiper :
    Functia mod_37 calculeaza restul impartirii unui nr la 37. Daca este negativ, il transform in complementul sau, adunand 37.Functiile transf_numar_litera si 
transf_litera_numar transforma cifrele in litere si invers, asa cum cere problema. Prin transf_cheie imi alcatuiesc matricea patratica cu care voi lucra si
convertesc fiecare litera in numar. In functia calcul_determinant calculez determinantul unei matrice de tip 1x1, 2x2 sau 3x3 cu formulele obisnuite.
Aplic asupra rezultatului mod_37.Prin functia calcul_factor determin inversul multiplicativ al unui nr a. Parcurg elementele de la 1 la 36 si odata gasit i-ul 
pentru care restul impartirii lui a*i la 37 e 1, ma opresc si returnez i.Functia adjuncta calculeaza adjuncta unei matrici de tip 1x1, 2x2 sau 3x3 prin
calcul direct.Pentru fiecare element al matricei aplic mod_37. In functia calcul_inversa calculez matricea inversa, inmultind fiecare element al adjunctei cu
inversul multiplicativ determinat anterior. Functia inmultire inmulteste o matrice patratica cu un vector colana.
    In cazul in care cheia nu este "-", atunci trebuie intai sa aplic algoritmul de decriptare a comenzii, prin functia decriptare.Determin lungimea cheii si
aplic radical pentru aflarea dimensiunii matricii patratice, notata prin m. Incep sa parcurg comanda prin impartirea ei pe blocuri, cu ajutorul lui i. 
Daca lungimea comenzii ramase e mai mare decat m, atunci blocul pe care il decriptez e de lungime m. Altfel e de lungimea comenzii - i. Imi selectez in sirul key2 
cheia pe care o prelucrez in functie de dimensiune. Transform cheia in matrice si o retin in key. Calculez determinantul acestei matrici key si inversul 
multiplicativ al determinantului. Aflu adjuncta si apoi inversa. Toate aceste matrici le aloc dinamic prin functia alloc_hill_m, prin care aloc o matrice patratica.
Aloc 2 vectori v si out. In v retin elementele din comanda si le transform literele in numere. Aplic functia inmultire pentru acel bloc si in out retin rezultatul.
Fiecare numar din vector il transform din nou in litera. Eliberez memoria si trec la urmatoarul bloc.
    Dupa decriptarea comenzii, continui cu aflarea statisticii, exact ca in problema anterioara. La final, rezultatul obtinut in sirul rezultat il criptez prin 
functia criptare. Parcurg aceiasi pasi ca la decriptare, impartind rezultatul pe blocuri. Astfel, in vectorul v voi retine fiecare element al blocului parcurs,
iar in out retin vectorul rezultat dupa ce aplic functia inmultire intre blocul v si cheia key.Fiecare element din out il criptez, transformandu-l in litera.
Afisez ouputul criptat si eliberez memoria.

