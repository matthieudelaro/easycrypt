#include "curseur.h"

Curseur::Curseur(Image &image) : m_image(image), m_x(0), m_y(0), m_couleur(ROUGE)
{
}

bool Curseur::seek(unsigned int x, unsigned int y, Couleur couleur)
{
    if(exist(x, y))
    {
        m_x = x;
        m_y = y;
        m_couleur = couleur;
        return true;
    }
    else
    {
        return false;
    }
}

void Curseur::getPosition(unsigned int *x, unsigned int *y, Couleur *couleur) const
{
    *x = m_x;
    *y = m_y;
    if(couleur)
        *couleur = m_couleur;
}

void Curseur::readPixel(unsigned char &rouge, unsigned char &vert, unsigned char &bleu, unsigned char &alpha)
{
    rouge = m_image.get(m_x, m_y, ROUGE);
    vert = m_image.get(m_x, m_y, VERT);
    bleu = m_image.get(m_x, m_y, BLEU);
    alpha = m_image.get(m_x, m_y, ALPHA);
}

void Curseur::setPixel(unsigned char rouge, unsigned char vert, unsigned char bleu, unsigned char alpha)
{
    m_image.set(m_x, m_y, ROUGE, rouge);
    m_image.set(m_x, m_y, VERT, vert);
    m_image.set(m_x, m_y, BLEU, bleu);
    m_image.set(m_x, m_y, ALPHA, alpha);
}

bool Curseur::atEnd() const
{
    return (m_x == m_image.getWidth() && m_y == m_image.getHeight() && m_couleur == BLEU);
}

bool Curseur::exist(unsigned int x, unsigned int y) const
{
    return (x < m_image.getWidth() && y < m_image.getHeight());
}

bool Curseur::moveToNextPixel(Couleur couleur)
{
    unsigned int nextX = m_x, nextY = m_y;

    if(m_x+1 < m_image.getWidth())
    {
        nextX = m_x+1;
    }
    else
    {
        if(m_y+1 < m_image.getHeight())
        {
            nextY = m_y+1;
            nextX = 0;
        }
        else
        {
            return false;
        }
    }

    if(exist(nextX, nextY))
    {
        m_y = nextY;
        m_x = nextX;
        m_couleur = couleur;
        return true;
    }
    else
    {
        return false;
    }
}

bool Curseur::moveToNextColor(bool considererAlpha)
{
    unsigned int nextX = m_x, nextY = m_y;
    Couleur prochaineCouleur = nextColor(m_couleur, considererAlpha);

    if( int(prochaineCouleur) < int(m_couleur) )
    {
        if(m_x+1 < m_image.getWidth())
        {
            nextX = m_x+1;
        }
        else
        {
            if(m_y+1 < m_image.getHeight())
            {
                nextY = m_y+1;
                nextX = 0;
            }
            else
            {
                return false;
            }
        }
    }

    /*Couleur prochaineCouleur;

    if( (considererAlpha && m_couleur < ALPHA) || ( !considererAlpha && m_couleur < BLEU))
    {
        prochaineCouleur = nextColor(m_couleur);
    }
    else
    {
        if(m_x+1 < m_image.getWidth())
        {
            nextX = m_x+1;
        }
        else
        {
            if(m_y+1 < m_image.getHeight())
            {
                nextY = m_y+1;
                nextX = 0;
            }
            else
            {
                return false;
            }
        }
    }*/


    if(exist(nextX, nextY))
    {
        m_y = nextY;
        m_x = nextX;
        m_couleur = prochaineCouleur;
        return true;
    }
    else
    {
        return false;
    }
}

Couleur Curseur::nextColor(Couleur c, bool considererAlpha)
{
    switch(c)
    {
    case ROUGE : c = VERT; break;
    case VERT : c = BLEU; break;
    case BLEU :
        if(considererAlpha)
            c = ALPHA;
        else
            c = ROUGE;
        break;
    case ALPHA : c = ROUGE; break;
    }
    return c;
}


