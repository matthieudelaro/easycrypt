#include "cryptage.h"
#include <cmath>
#include <iostream>

bool Cryptage::crypter(Image &image, std::string const& texte, unsigned int methode)
{
    //Définition de l'emplacement de lecture de l'image
    unsigned int x = 0, y = 0, couleur = ROUGE;

    //on écrit l'en-tête
        //1 on marque l'image comme étant cryptée : on écrit de façon brute les bytes correspondants à "secret", dans l'ordre suivant :
        //  de gauche à droite, de bas en haut, rouge puis vert puis bleu
        std::vector<bool> bytesSecret = stringToBits("secret");
        unsigned int bitsEcrits = 0;
        while(bitsEcrits < bytesSecret.size())
        {
//            image[y][x][couleur] = bytesSecret[bitsEcrits];
//            std::cerr << "Modif bit poids faible ("<< bytesSecret[bitsEcrits] << ") : " << (int)image[y][x][couleur] << " => ";
            setBitPoidsFaible(image[y][x][couleur], bytesSecret[bitsEcrits]);
//            std::cerr << (unsigned int)image[y][x][couleur] << std::endl;
            avancerPixel(image, x, y, couleur, ROUGE);
            bitsEcrits++;
        }

        //2 on marque la méthode de cryptage
        std::vector<bool> bytesMethode = uintToBits(methode);
        bitsEcrits = 0;
        while(bitsEcrits < bytesMethode.size())
        {
//            image[y][x][couleur] = bytesMethode[bitsEcrits];
            setBitPoidsFaible(image[y][x][couleur], bytesMethode[bitsEcrits]);
            avancerPixel(image, x, y, couleur, ROUGE);
            bitsEcrits++;
        }

        //3 on marque le nombre de bits à lire pour obtenir le texte
//        std::cerr << "Cryptage : texte = " << texte << std::endl;
        std::vector<bool> bytesTexte = stringToBits(texte);
        std::vector<bool> bytesTailleTexte = uintToBits(bytesTexte.size());
        bitsEcrits = 0;
//        std::cerr << "La taille du texte contient " << bytesTailleTexte.size() << "bits.\n";
        while(bitsEcrits < bytesTailleTexte.size())
        {
//            image[y][x][couleur] = bytesTailleTexte[bitsEcrits];
            setBitPoidsFaible(image[y][x][couleur], bytesTailleTexte[bitsEcrits]);
            avancerPixel(image, x, y, couleur, ROUGE);
            bitsEcrits++;
        }

    //on écrit le texte
    switch(methode)
    {
        case BOOLEEN_BITS_FAIBLES_VERTS :
            bitsEcrits = 0;
            couleur = VERT;
//            std::cerr << "Le texte contient " << bytesTexte.size() << "bits.\n";
            /*for(int bitsAecrire = bytesTexte.size()-1; bitsAecrire >= 0; bitsAecrire--)
            {
                setBitPoidsFaible(image[y][x][couleur], bytesTexte[bitsAecrire]);
                avancerPixel(image, x, y, couleur, VERT);
            }
            bitsEcrits = 0;*/
//            std::cerr << "Le texte contient " << bytesTexte.size() << "bits.\n";
            while(bitsEcrits < bytesTexte.size())
            {
    //                image[y][x][couleur] = bytesTexte[bitsEcrits];
                couleur = VERT;
                setBitPoidsFaible(image[y][x][couleur], bytesTexte[bitsEcrits]);
                avancerPixel(image, x, y, couleur, VERT);
                bitsEcrits++;
            }
            break;
        default :
            return false;
            break;
    }
    return true;
}

bool Cryptage::decrypter(Image const& image, std::string &texte)
{
    //Définition de l'emplacement de lecture de l'image
    unsigned int x = 0, y = 0, couleur = ROUGE;

    //On lit l'en-tête
        //1 on vérifie que l'image est cryptée
        //on lit 48 bits sur les bits de poids faibles rouges des 48 premiers pixels,
        //en espérant trouver le texte "secret" (8bits * 6lettres = 48)
        std::vector<bool> bitsSecret;
        for(unsigned int bitsLus = 0; bitsLus < 48; bitsLus++)
        {
            bitsSecret.push_back(image[y][x][couleur]%2);
            avancerPixel(image, x, y, couleur, ROUGE);
        }
        std::string texteSecret;
        bitsToString(bitsSecret, texteSecret);
//        std::cerr << "texteSecret = " << texteSecret << std::endl;
        if((texteSecret == "secret")==false)//si l'image n'est pas cryptée, alors on arrête
            return false;

        //2 on lit la méthode de cryptage
        std::vector<bool> bitsMethode;
        for(unsigned int bitsLus = 0; bitsLus < 32/*sizeof(unsigned int)*/; bitsLus++)
        {
            bitsMethode.push_back(image[y][x][couleur]%2);
            avancerPixel(image, x, y, couleur, ROUGE);
        }
        unsigned int methode = bitsToUint(bitsMethode);

        //3 on lit le nombre de pixels à lire pour obtenir le texte
        std::vector<bool> bitsTailleTexte;
        for(unsigned int bitsLus = 0; bitsLus < 32/*sizeof(unsigned int)*/; bitsLus++)
        {
            bitsTailleTexte.push_back(image[y][x][couleur]%2);
            avancerPixel(image, x, y, couleur, ROUGE);
        }
        unsigned int tailleTexte = bitsToUint(bitsTailleTexte);
//        std::cerr << "il y a " << tailleTexte << "bits à lire.\n";

    //on lit le texte
    std::vector<bool> bitsTexte;
    switch(methode)
    {
        case BOOLEEN_BITS_FAIBLES_VERTS :
            couleur = VERT;
            for(unsigned int bitsLus = 0; bitsLus < tailleTexte; bitsLus++)
            {
                bitsTexte.push_back(image[y][x][couleur]%2);
                avancerPixel(image, x, y, couleur, VERT);
            }
            bitsToString(bitsTexte, texte);
//            std::cerr << "texte = " << texte << std::endl;
            break;
        default :
            return false;
            break;
    }
    return true;

}

unsigned int Cryptage::bitsToUint(std::vector<bool> const& bits)
{
    unsigned int out = 0;
    unsigned int puissanceDe2 = 1;
    for(unsigned int i = 0; i < bits.size(); i++)
    {
        out += bits[i]*puissanceDe2;
        puissanceDe2 *= 2;
    }
    return out;
}

void Cryptage::setBitPoidsFaible(unsigned char &variable, bool valeur)
{
    if(variable%2 == 0)
    {
        //variable += valeur;
        if(valeur)
        {
            variable++;
        }
    }
    else if(!valeur)//on sait que le bit de poids faible vaut 1. Donc si on voulait 0, alors
    {
        variable--;
    }
}

std::vector<bool> Cryptage::uintToBits(unsigned int number)
{
    std::vector<bool> out;
//    std::cerr << "Nouvelle lettre : " << lettre << ", soit " << int(lettre) << "\n";
    for(unsigned int j = 0; j < 8*sizeof(number); j++)
    {
        //int power = 7-j;
//            int valeurBit = lettre%int(pow(2, j)+1);
        int valeurBit = number%2;
//        std::cerr << "lettre%2 = " << number%2 << " et lettre = " << number << "\n";
//        std::cerr << "On ajoute un bit : " << valeurBit << " grâce à " << pow(2, j) << "\n";
//            lettre -= valeurBit;
//            lettre >> 1;
        number = number >> 1;
        out.push_back((bool) valeurBit);
    }
//    for(unsigned int i = 0; i < out.size(); i++)
//    {
//        std::cerr << out[i];
//    }
    return out;
}

std::vector<bool> Cryptage::stringToBits(std::string const& string)
{
    std::vector<bool> out;
    //on va placer, dans out, les bits représentant les lettres.
    //on met les lettres une par une
    //et pour chaque lettre, on met les bits un par un
    //en commençant par le bit de poids faible
    for(unsigned int i = 0; i < string.size(); i++)
    {
        char lettre = string[i];
//        std::cerr << "Nouvelle lettre : " << lettre << ", soit " << int(lettre) << "\n";
        for(unsigned int j = 0; j < taillecaractere; j++)
        {
            //int power = 7-j;
//            int valeurBit = lettre%int(pow(2, j)+1);
            int valeurBit = lettre%2;
//            std::cerr << "lettre%2 = " << lettre%2 << " et lettre = " << lettre << "\n";
//            std::cerr << "On ajoute un bit : " << valeurBit << " grâce à " << pow(2, j) << "\n";
//            lettre -= valeurBit;
//            lettre >> 1;
            lettre = lettre >> 1;
            out.push_back((bool) valeurBit);
        }
    }
//    std::cerr << "Resultat : ";
//    for(unsigned int i = 0; i < out.size(); i++)
//    {
//        std::cerr << out[i];
//    }
//    std::cerr << "\n";
    return out;
}


bool Cryptage::bitsToString(std::vector<bool> const& bytes, std::string &out, unsigned int nombreDeBitsAignorer)
{
    if((bytes.size() - nombreDeBitsAignorer)%taillecaractere != 0)
        return false;//le nombre de bytes ne correspond pas à des lettres

    unsigned int i = nombreDeBitsAignorer;
//    std::cerr << "Décryptage\n";
    while(i < bytes.size())
    {
        char lettre = 0;
        /*std::cerr << "Nouvelle lettre : ";
        for(unsigned int j = 0; j < taillecaractere; j++)
        {
            std::cerr << bytes[i+j];
        }
        std::cerr << "\n"*/;
        for(unsigned int j = 0; j < taillecaractere; j++)
        {
            //int power = 7-j;
            lettre += bytes[i]*pow(2, j);
//            std::cerr << "On ajoute un bit : " << bytes[i]*pow(2, j) << " grâce à " << pow(2, j) << "\n";
            i++;
        }
//        std::cerr << "Lettre = " << lettre << " (" << int(lettre) << ")\n";
        out += lettre;
    }

    return true;
}

bool Cryptage::avancerCouleur(Image const& bytes, unsigned int &x, unsigned int &y, unsigned int &couleur)
{
    unsigned int nextX = x, nextY = y, nextCouleur = couleur;

    if(couleur == BLEU)
    {
        nextCouleur = ROUGE;
        if(x+1 < bytes[y].size())
        {
            nextX = x+1;
        }
        else
        {
            if(y+1 < bytes.size())
            {
                nextY = y+1;
                nextX = 0;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        nextCouleur = couleur+1;
    }

    if(nextY < bytes.size() && nextX < bytes[nextY].size())
    {
        y = nextY;
        x = nextX;
        couleur = nextCouleur;
        return true;
    }
    else
    {
        return false;
    }
}

bool Cryptage::avancerPixel(const Image &bytes, unsigned int &x, unsigned int &y, unsigned int &couleur, bool nextCouleur)
{
    unsigned int nextX = x, nextY = y;

    if(x+1 < bytes[y].size())
    {
        nextX = x+1;
    }
    else
    {
        if(y+1 < bytes.size())
        {
            nextY = y+1;
            nextX = 0;
        }
        else
        {
            return false;
        }
    }

    if(nextY < bytes.size() && nextX < bytes[nextY].size())
    {
        y = nextY;
        x = nextX;
        couleur = nextCouleur;
        return true;
    }
    else
    {
        return false;
    }
}
