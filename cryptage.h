#ifndef CRYPTAGE_H
#define CRYPTAGE_H

#include <vector>
#include <string>

namespace Cryptage
{
    typedef std::vector< std::vector< std::vector<unsigned char> > > Image;
    enum Couleur{ROUGE = 0, VERT = 1, BLEU = 2, ALPHA = 3};
    enum Methode{BOOLEEN_BITS_FAIBLES_VERTS};

    const unsigned int taillecaractere = 8*sizeof(char);

    bool crypter(Image &image, std::string const& texte, unsigned int methode = BOOLEEN_BITS_FAIBLES_VERTS);
    bool decrypter(Image const& image, std::string &texte);
    std::vector<bool> uintToBits(unsigned int number);
    unsigned int bitsToUint(const std::vector<bool> &bits);
    std::vector<bool> stringToBits(std::string const& string);
    bool bitsToString(std::vector<bool> const& bytes, std::string &out, unsigned int nombreDeBitsAignorer = 0);
    bool avancerCouleur(Image const& bytes, unsigned int &x, unsigned int &y, unsigned int &couleur);
    bool avancerPixel(Image const& bytes, unsigned int &x, unsigned int &y, unsigned int &couleur, bool nextCouleur = ROUGE);
    void setBitPoidsFaible(unsigned char &variable, bool valeur);
}

#endif // CRYPTAGE_H
