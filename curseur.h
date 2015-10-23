#ifndef CURSEUR_H
#define CURSEUR_H

#include "image.h"

class Curseur
{
public:
    //enum ModeDeDeplacement{ParPixel, ParCouleur};

    Curseur(Image &image);
    bool seek(unsigned int x, unsigned int y, Couleur couleur);
    void getPosition(unsigned int *x, unsigned int *y, Couleur *couleur = NULL) const;
    inline unsigned int getX() const {return m_x;}
    inline unsigned int getY() const {return m_y;}
    inline Couleur getCouleur() const {return m_couleur;}
    inline unsigned char read() {return m_image.get(m_x, m_y, m_couleur);}
    void readPixel(unsigned char &rouge, unsigned char &vert, unsigned char &bleu, unsigned char &alpha);
    inline void set(unsigned char valeur) {m_image.set(m_x, m_y, m_couleur, valeur);}
    void setPixel(unsigned char rouge, unsigned char vert, unsigned char bleu, unsigned char alpha);
    bool atEnd() const;
    bool exist(unsigned int x, unsigned int y) const;
    bool moveToNextPixel(Couleur couleur = ROUGE);
    bool moveToNextColor(bool considererAlpha = false);
    static Couleur nextColor(Couleur c, bool considererAlpha = false);

private:
    Image &m_image;
    unsigned int m_x, m_y;
    Couleur m_couleur;
    //ModeDeDeplacement m_mode;
};

//class CurseurPixel
//{z
//public:

//};

#endif // CURSEUR_H
