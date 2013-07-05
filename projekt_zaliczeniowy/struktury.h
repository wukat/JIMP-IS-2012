#ifndef STRUKTURY_H
#define STRUKTURY_H

struct art {
      long int ID;
      char * nazwa;
};
typedef struct art artykul;

struct dat {
      int dzien;
      int miesiac;
      int rok;
};
typedef struct dat data;

struct w_hurtowni {
      long int ID;
      char * nazwa;
      int ilosc;
      float cena;
      data waznosc;
};
typedef struct w_hurtowni sklad;

struct hurt {
      char * miasto;
      int numer_miasta;
      sklad * produkty;
      int ilosc_pozycji;
};
typedef struct hurt hurtownia;

struct produkt_zamawiany {
      int ilosc;
      char * nazwa;
      long int ID;
};
typedef struct produkt_zamawiany pozycja;

struct zam {
      data termin;
      int numer_zl;
      char * miasto;
      int numer_miasta;
      pozycja * pozycje;
      int ilosc_pozycji;
};
typedef struct zam zamowienie;

#endif

