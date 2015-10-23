#ifndef INTERFACEIMAGES_H
#define INTERFACEIMAGES_H

#include <QMainWindow>
#include <QtGui>
#include <QFile>
//#include "cryptage.h"
#include "curseur.h"
#include "methodedecryptage.h"

class InterfaceImages : public QMainWindow
{
    Q_OBJECT
public:
    explicit InterfaceImages(QWidget *parent = 0);
    ~InterfaceImages();
    
signals:
    
public slots:
    void nouveau();
    void ouvrir();
    void enregistrer();
    void enregistrerSous();
    void changerLaMethodeDeCryptage();
    void changerPassword();
    void redimensionnerImage();
    void resizeEvent( QResizeEvent * event );
    void updateNombreCaracteres();

private:
    static void qimageToImage(const QImage &qimage, Image &image);
    static void imageToQImage(Image &image, QImage &qimage);
    void enregistrerDansLeFichier(QString const& fichier);
    void changerImage(QString const& fichier);
    inline MethodeDeCryptage* methode() {return m_methodesDeCryptage[m_indexMethodeDeCryptage];}

    QMenu *m_menuFichier;
    QAction *m_actionQuitter;
    QAction *m_actionEnregistrer;
    QAction *m_actionEnregistrerSous;
    QAction *m_actionOuvrir;
    QAction *m_actionNouveau;
    QMenu *m_menuMethodeCryptage;
    QAction *m_actionChangerMethodeCryptage;
    QAction *m_actionChangerPassword;

    QFile *m_fichierImage;
    bool m_nouvelleImage;

    QTextEdit *m_textEdit;

    QHBoxLayout *m_layoutPrincipal;
    QVBoxLayout *m_layoutImage;
    QVBoxLayout *m_layoutTexte;

    QLabel *m_labelDimensionsImage;
    QLabel *m_labelPlaceImage;
    QLabel *m_vueImage;
    QLabel *m_textePasDimageEncours;
    QLabel *m_nombreDeCaracteresTexte;

    QWidget *m_zoneCentrale;
    QFrame *m_frameImage;

    QPixmap m_image;
    QImage m_imageCryptage;

    std::vector<MethodeDeCryptage*> m_methodesDeCryptage;
    unsigned int m_indexMethodeDeCryptage;

    std::string m_password;

    bool m_imageEnCours;
};

#endif // INTERFACEIMAGES_H
