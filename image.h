#ifndef IMAGE_H
#define IMAGE_H

#include <vector>

enum Couleur{ROUGE = 0, VERT = 1, BLEU = 2, ALPHA = 3};

class Image
{
public:
    Image(unsigned int width, unsigned int height);
    inline unsigned int getWidth() const {if(pixels.empty()) return 0; else return pixels[0].size();}
    inline unsigned int getHeight() const {return pixels.size();}
    inline void set(unsigned int x, unsigned int y, Couleur couleur, unsigned char valeur) {pixels[y][x][couleur] = valeur;}
    unsigned char get(unsigned int x, unsigned int y, Couleur couleur) const {return pixels[y][x][couleur];}

private:
    std::vector< std::vector< std::vector<unsigned char> > > pixels;
};

#endif // IMAGE_H
