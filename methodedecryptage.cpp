#include "methodedecryptage.h"
#include <cmath>
#include <iostream>

unsigned int bitsToUint(std::vector<bool> const& bits)
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

unsigned char setBitPoidsFaible(unsigned char variable, bool valeur)
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
    return variable;
}

std::vector<bool> uintToBits(unsigned int number)
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

std::vector<bool> stringToBits(std::string const& string)
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
        for(unsigned int j = 0; j < (8*sizeof(char)); j++)
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


bool bitsToString(std::vector<bool> const& bytes, std::string &out, unsigned int nombreDeBitsAignorer)
{
    if((bytes.size() - nombreDeBitsAignorer)%(8*sizeof(char)) != 0)
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
        for(unsigned int j = 0; j < (8*sizeof(char)); j++)
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

std::vector<MethodeDeCryptage*> getMethodes()
{
    std::vector<MethodeDeCryptage*> methodesDeCryptage;

    methodesDeCryptage.push_back(new MethodeBitsPoidsFaiblesVerts);
    methodesDeCryptage.push_back(new MethodePasswordRVB);

    return methodesDeCryptage;
}

/*void MethodeDeCryptage::header(int methode, unsigned int nombreDeBits, std::vector<bool> &resultat)
{
    std::vector<bool> bytesSecret = stringToBits("secret");

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
}*/

bool MethodeDeCryptage::writeHeader(std::vector<Image> &images, int methode, unsigned int nombreDeBits, Curseur *curseurFinHeader)
{
    if(images.empty())
        return false;

    Curseur curseur(images[0]);

    std::vector<bool> bitsSecrets = stringToBits("secret");
    std::vector<bool> bitsMethode = uintToBits(methode);
    std::vector<bool> bitsTailleTexte = uintToBits(nombreDeBits);

    std::vector<bool> header;
    header.reserve(bitsSecrets.size() + bitsMethode.size() + bitsTailleTexte.size());
    header.insert(header.end(), bitsSecrets.begin(), bitsSecrets.end());
    header.insert(header.end(), bitsMethode.begin(), bitsMethode.end());
    header.insert(header.end(), bitsTailleTexte.begin(), bitsTailleTexte.end());

    unsigned int bitsEcrits = 0;
    //std::cerr << "\nEcriture du header :" << std::endl;
    while(bitsEcrits < header.size())
    {
        //std::cerr << header[bitsEcrits] << " à " << bitsEcrits << std::endl;
        curseur.set(setBitPoidsFaible(curseur.read(), header[bitsEcrits]));
        curseur.moveToNextPixel();
        bitsEcrits++;
    }

    /*//1 on marque l'image comme étant cryptée : on écrit de façon brute les bytes correspondants à "secret"
    std::vector<bool> bitsSecrets = stringToBits("secret");
    unsigned int bitsEcrits = 0;
    while(bitsEcrits < bitsSecrets.size())
    {
        curseur.set(setBitPoidsFaible(curseur.read(), bitsSecrets[bitsEcrits]));
        curseur.moveToNextPixel();
        bitsEcrits++;
    }

    //2 on marque la méthode de cryptage
    std::vector<bool> bitsMethode = uintToBits(methode);
    bitsEcrits = 0;
    while(bitsEcrits < bitsMethode.size())
    {
        curseur.set(setBitPoidsFaible(curseur.read(), bitsMethode[bitsEcrits]));
        curseur.moveToNextPixel();
        bitsEcrits++;
    }

    //3 on marque le nombre de bits à lire pour obtenir le texte
    std::vector<bool> bitsTailleTexte = uintToBits(nombreDeBits);
    bitsEcrits = 0;
    while(bitsEcrits < bitsTailleTexte.size())
    {
        curseur.set(setBitPoidsFaible(curseur.read(), bitsTailleTexte[bitsEcrits]));
        curseur.moveToNextPixel();
        bitsEcrits++;
    }*/

    if(curseurFinHeader)
        curseurFinHeader->seek(curseur.getX(), curseur.getY(), curseur.getCouleur());
    return true;
}

bool MethodeDeCryptage::readHeader(std::vector<Image> &images, int &methode, unsigned int &nombreDeBits, Curseur *curseurFinHeader)
{
    if(images.empty())
        return false;

    Curseur curseur(images[0]);

    //std::cerr << "\nLecture du header :" << std::endl;
    //std::vector<bool> header;
    std::vector<bool> bitsSecret;
    std::vector<bool> bitsMethode;
    std::vector<bool> bitsTailleTexte;
    for(unsigned int bitsLus = 0; bitsLus < 112; bitsLus++)
    {
        //header.push_back(curseur.read()%2);
        //std::cerr << header[bitsLus] << " à " << bitsLus << std::endl;

        if(bitsLus < 48)
            bitsSecret.push_back(curseur.read()%2);
        else if(bitsLus < 80)
            bitsMethode.push_back(curseur.read()%2);
        else
            bitsTailleTexte.push_back(curseur.read()%2);

        curseur.moveToNextPixel();
    }

    std::string texteSecret;
    bitsToString(bitsSecret, texteSecret);
    if((texteSecret == "secret")==false)//si l'image n'est pas cryptée, alors on arrête
        return false;

    methode = bitsToUint(bitsMethode);

    nombreDeBits = bitsToUint(bitsTailleTexte);
    /*//On lit l'en-tête
    //1 on vérifie que l'image est cryptée
    //on lit 48 bits sur les bits de poids faibles rouges des 48 premiers pixels,
    //en espérant trouver le texte "secret" (8bits * 6lettres = 48)
    std::vector<bool> bitsSecret;
    for(unsigned int bitsLus = 0; bitsLus < 48; bitsLus++)
    {
        bitsSecret.push_back(curseur.read()%2);
        curseur.moveToNextPixel();
    }

    std::string texteSecret;
    bitsToString(bitsSecret, texteSecret);
    if((texteSecret == "secret")==false)//si l'image n'est pas cryptée, alors on arrête
        return false;

    //2 on lit la méthode de cryptage
    std::vector<bool> bitsMethode;
    for(unsigned int bitsLus = 0; bitsLus < 32; bitsLus++)
    {
        bitsMethode.push_back(curseur.read()%2);
        curseur.moveToNextPixel();
    }
    methode = bitsToUint(bitsMethode);

    //3 on lit le nombre de pixels à lire pour obtenir le texte
    std::vector<bool> bitsTailleTexte;
    for(unsigned int bitsLus = 0; bitsLus < 32; bitsLus++)
    {
        bitsTailleTexte.push_back(curseur.read()%2);
        curseur.moveToNextPixel();
    }
    nombreDeBits = bitsToUint(bitsTailleTexte);

    */if(curseurFinHeader)
        curseurFinHeader->seek(curseur.getX(), curseur.getY(), curseur.getCouleur());
    return true;
}

bool MethodeBitsPoidsFaiblesVerts::crypter(std::vector<Image> &images, std::string const& texte, std::string password, void *parametre1, void *parametre2)
{
    if(texte.size()*8 + 112 > size(images) || images.empty())//112 correspond à la taille du header
        return false;

    Curseur curseur(images[0]);

    std::vector<bool> bitsTexte = stringToBits(texte);
    if(!writeHeader(images, id(), bitsTexte.size(), &curseur))
        return false;

    if(!curseur.seek(curseur.getX(), curseur.getY(), VERT))
        return false;

    for(unsigned int i = 0; i < bitsTexte.size(); i++)
    {
        curseur.set(setBitPoidsFaible(curseur.read(), bitsTexte[i]));
        curseur.moveToNextPixel(VERT);
    }
    return true;
}

bool MethodeBitsPoidsFaiblesVerts::decrypter(std::vector<Image> &images, std::string &texte, std::string password, void *parametre1, void *parametre2)
{
    if(images.empty())
        return false;

    Curseur curseur(images[0]);
    int methode;
    unsigned int nombreDeBits;

    if(!readHeader(images, methode, nombreDeBits, &curseur))//si on n'arrive pas à lire le header
        return false;

    if(methode != id())//si ce n'est pas la bonne méthode de cryptage
        return false;

    if(!curseur.seek(curseur.getX(), curseur.getY(), VERT))
        return false;

    std::vector<bool> bitsTexte;
    for(unsigned int i = 0; i < nombreDeBits; i++)
    {
        bitsTexte.push_back(curseur.read()%2);
        curseur.moveToNextPixel(VERT);
    }
    bitsToString(bitsTexte, texte);
    return true;
}

unsigned int MethodeBitsPoidsFaiblesVerts::size(std::vector<Image> const &images)
{
    if(!images.empty())
    {
        if(images[0].getWidth() * images[0].getHeight() <= 112)
            return 0;
        else
            return images[0].getWidth() * images[0].getHeight() - 112;//le header fait 112 bits !
    }
    else
    {
        return 0;
    }
}

unsigned int MethodePasswordRVB::size(std::vector<Image> const &images)
{
    if(!images.empty())
    {
        if(images[0].getWidth() * images[0].getHeight() * 3 <= 112)
            return 0;
        else
            return images[0].getWidth() * images[0].getHeight() * 3 - 112;//le header fait 112 bits !
    }
    else
    {
        return 0;
    }
}

bool MethodePasswordRVB::crypter(std::vector<Image> &images, std::string const& texte, std::string password, void *parametre1, void *parametre2)
{
    if(texte.size()*8 + 112 > size(images) || images.empty() || password.empty())//112 correspond à la taille du header
        return false;

    Curseur curseur(images[0]);

    std::vector<bool> bitsTexte = stringToBits(texte);
    std::vector<bool> bitsPassword = stringToBits(password);
    if(!writeHeader(images, id(), bitsTexte.size(), &curseur))
        return false;

    unsigned int positionDansPassword = 0;
    for(unsigned int i = 0; i < bitsTexte.size(); i++)
    {
        bool valeur;
        valeur = bitsPassword[positionDansPassword]^bitsTexte[i];// ^ est l'opérateur XOR (OU Exclusif)
        curseur.set(setBitPoidsFaible(curseur.read(), valeur));

        curseur.moveToNextColor();
        positionDansPassword++;
        if(positionDansPassword == bitsPassword.size())
            positionDansPassword = 0;
    }
    return true;
}

bool MethodePasswordRVB::decrypter(std::vector<Image> &images, std::string &texte, std::string password, void *parametre1, void *parametre2)
{
    if(images.empty() || password.empty())
        return false;

    Curseur curseur(images[0]);
    int methode;
    unsigned int nombreDeBits;

    if(!readHeader(images, methode, nombreDeBits, &curseur))//si on n'arrive pas à lire le header
        return false;

    if(methode != id())//si ce n'est pas la bonne méthode de cryptage
        return false;

    unsigned int positionDansPassword = 0;
    std::vector<bool> bitsPassword = stringToBits(password);
    std::vector<bool> bitsTexte;
    for(unsigned int i = 0; i < nombreDeBits; i++)
    {
        bool valeur = (curseur.read()%2)^bitsPassword[positionDansPassword];
        bitsTexte.push_back(valeur);

        curseur.moveToNextColor();
        positionDansPassword++;
        if(positionDansPassword == bitsPassword.size())
            positionDansPassword = 0;
    }
    bitsToString(bitsTexte, texte);
    return true;
}
