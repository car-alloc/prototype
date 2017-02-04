#include <iostream>
#include <iomanip>

#include <csvstream.hpp>

//class toaster { public: toaster(const std::string & s) { } };

int main()
 {
  /*
  csvstream z("test.csv",std::ios::out);

  z
   << 3.15 << "" << 501 << "t\"oa\"\"ster" << 14 << std::endl
   << 12 << "" << 400 << "fl orb" << 12 << std::endl;

  csvstream y("test.csv",std::ios::in);

  float f;
  int i1,i2;
  std::string s1,s2;

  y
   >> f >> s1  >> i1 >> s2 >> i2 >> std::ws
   ;

  std::cout
   << "f=" << f
   << " s1=" << s1
   << " i1=" << i1
   << " s2=" << s2
   << " i2=" << i2
   << std::endl;

  y
   >> f >> s1 >> i1 >> s2 >> i2 >> std::ws
   ;

  std::cout
   << "f=" << f
   << " s1=" << s1
   << " i1=" << i1
   << " s2=" << s2
   << " i2=" << i2
   << std::endl;
  */

  /*
  csvstream books("books.csv"); // std::ios::in par defaut

  std::cout << std::setprecision(2);
  while (!books.eof())
   {
    //toaster test("");
    std::string titre,
                auteur1,auteur2,auteur3,auteur4,auteur5,
                serie,
                cote_congress,cote_dewey,
                sujet1,sujet2,sujet3,
                editeur,
                isbn,
                date_achat;
    int annee,tome,annee_1ere,no_serie,int_inconnu1,int_inconnu2;
    float prix;
    bool bool_inconnu1;

    books >> titre
          >> auteur1 >> auteur2 >> auteur3 >> auteur4 >> auteur5
          >> serie
          >> cote_congress >> cote_dewey
          >> sujet1 >> sujet2 >> sujet3
          >> annee >> tome >> annee_1ere
          >> editeur
          >> no_serie
          >> int_inconnu1
          >> isbn
          >> prix
          >> date_achat
          >> bool_inconnu1
          >> int_inconnu2
          >> std::ws;

    std::cout
     << "-----------------------------------------"
     << std::endl
     << std::endl
     << "line:\t" << books.where()
     << std::endl
     << '\t' << titre
             << '\t' << tome
     << '\t' << auteur1 << ','
             << auteur2 << ','
             << auteur3 << ','
             << auteur4 << ','
             << auteur5
             << std::endl
     << '\t' << serie << ','
             << no_serie << '\t'
             << annee << " (" << annee_1ere << ')'
             << std::endl
     << '\t' << editeur
             << std::endl
     << '\t' << isbn
             << std::endl
     << '\t' << prix << '\t' << date_achat
             << std::endl
     << std::endl;
   }
  */

  /*
  csvstream test("test-data/short3.csv"); // std::ios::in par defaut

  std::cout << std::setprecision(2);
  while (!test.eof())
   {
    int machin;
    std::string bidule,gugusse,cossin;
    float patente;

    test
     >> machin
     >> bidule
     >> gugusse
     >> patente
     >> cossin
     >> std::ws;

    std::cout
     << machin << std::endl
     << bidule << std::endl
     << gugusse << std::endl
     << patente << std::endl
     << cossin << std::endl
     << std::endl;
   }
  */

  csvstream test("test-data/empty-lines.csv");


  while (!test.eof())
   {
    int a,l;
    std::string b,c;
    float d;

    try
     {
      test >> l >> a >> b >> c >> d >> std::ws;

      std::cout
       << test.where()
       << '\t'
       << l << ','
       << a << ',' << b << ",[" << c << "]," << d
       << std::endl;
     }
    
    catch (std::exception & e)
     {
      std::cerr
       << "line: " << test.where()
       << " (at pos " << test.tellg() << ")\t"
       << e.what() << std::endl;
     }
   }
  
  return 0;
 }
