#ifndef METHODEDECRYPTAGE_H
#define METHODEDECRYPTAGE_H

#include <vector>
#include <string>
#include "image.h"
#include "curseur.h"

std::vector<bool> uintToBits(unsigned int number);
unsigned int bitsToUint(const std::vector<bool> &bits);
std::vector<bool> stringToBits(std::string const& string);
bool bitsToString(std::vector<bool> const& bytes, std::string &out, unsigned int nombreDeBitsAignorer = 0);
unsigned char setBitPoidsFaible(unsigned char variable, bool valeur);

class MethodeDeCryptage
{
    //typedef std::vector< std::vector< std::vector<unsigned char> > > Image;

    //enum Methode{BOOLEEN_BITS_FAIBLES_VERTS};

    //const unsigned int taillecaractere = 8*sizeof(char);

public:
    virtual bool crypter(std::vector<Image> &images, std::string const& texte, std::string password = "", void *parametre1 = NULL, void *parametre2 = NULL) = 0;
    virtual bool decrypter(std::vector<Image> &images, std::string &texte, std::string password = "", void *parametre1 = NULL, void *parametre2 = NULL) = 0;
    virtual unsigned int size(std::vector<Image> const &images) = 0;//indique le nombre de lettres que peut contenir le lot d'images
    virtual bool passwordRequired() {return false;}
    virtual int id() = 0;
    virtual std::string name() = 0;
    //static void header(int methode, unsigned int nombreDeBits, std::vector<bool> &resultat);
    static bool writeHeader(std::vector<Image> &images, int methode, unsigned int nombreDeBits, Curseur *curseurFinHeader = NULL);
    static bool readHeader(std::vector<Image> &images, int &methode, unsigned int &nombreDeBits, Curseur *curseurFinHeader = NULL);
};

std::vector<MethodeDeCryptage*> getMethodes();//éditer cette méthode pour qu'elle retourne une instance de chaque classe héritant MethodeDeCryptage

class MethodeBitsPoidsFaiblesVerts : public MethodeDeCryptage
{
public:
    bool crypter(std::vector<Image> &images, std::string const& texte, std::string password = "", void *parametre1 = NULL, void *parametre2 = NULL);
    bool decrypter(std::vector<Image> &images, std::string &texte, std::string password = "", void *parametre1 = NULL, void *parametre2 = NULL);
    unsigned int size(std::vector<Image> const &images);
    int id() {return 0;}
    std::string name() {return "Cryptage simple : un bit par pixel.";}
};

class MethodeAvecPassword : public MethodeDeCryptage
{
public:
    virtual bool passwordRequired() {return true;}
};

class MethodePasswordRVB : public MethodeAvecPassword
{
public:
    bool crypter(std::vector<Image> &images, std::string const& texte, std::string password = "", void *parametre1 = NULL, void *parametre2 = NULL);
    bool decrypter(std::vector<Image> &images, std::string &texte, std::string password = "", void *parametre1 = NULL, void *parametre2 = NULL);
    unsigned int size(std::vector<Image> const &images);
    int id() {return 1;}
    std::string name() {return "Cryptage par mot de passe : trois bits par pixel.";}
};

#endif // METHODEDECRYPTAGE_H
