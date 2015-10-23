#include "interfaceimages.h"
#include <iostream>

InterfaceImages::InterfaceImages(QWidget *parent) :
    QMainWindow(parent)
{
    //initialisation des méthodes de cryptage
    m_methodesDeCryptage = getMethodes();
    m_indexMethodeDeCryptage = 0;
    m_password = "";

    //initialisation des membres de l'objet
    m_fichierImage = NULL;
    m_vueImage = NULL;
    m_labelDimensionsImage = NULL;
    m_labelPlaceImage = NULL;
    m_nouvelleImage = true;
    m_imageEnCours = false;

    //initialisation des menus
    m_menuFichier = menuBar()->addMenu("&Fichier");
    m_actionNouveau = m_menuFichier->addAction("&Nouveau");
    m_actionNouveau->setShortcut(QKeySequence("Ctrl+n"));

    m_actionOuvrir = m_menuFichier->addAction("&Ouvrir");
    m_actionOuvrir->setShortcut(QKeySequence("Ctrl+O"));

    m_actionEnregistrer = m_menuFichier->addAction("&Enregistrer");
    m_actionEnregistrer->setShortcut(QKeySequence("Ctrl+s"));

    m_actionEnregistrerSous = m_menuFichier->addAction("&Enregistrer Sous");
    m_actionEnregistrerSous->setShortcut(QKeySequence("Ctrl+Alt+s"));

    QList<QKeySequence> raccourcisQuitter;
    raccourcisQuitter.push_back(QKeySequence("Ctrl+Q"));
    raccourcisQuitter.push_back(QKeySequence("Escape"));
    m_actionQuitter = m_menuFichier->addAction("&Quitter");
    m_actionQuitter->setShortcuts(raccourcisQuitter);

    m_menuMethodeCryptage = menuBar()->addMenu("&Méthode de cryptage");
    m_actionChangerMethodeCryptage = m_menuMethodeCryptage->addAction("&Changer de méthode");
    m_actionChangerMethodeCryptage->setShortcut(QKeySequence("Ctrl+c"));
    m_actionChangerPassword = m_menuMethodeCryptage->addAction("&Changer de mot de passe");
    m_actionChangerPassword->setShortcut(QKeySequence("Ctrl+p"));

    //initialisation de la zone centrale
    m_textePasDimageEncours = new QLabel("Sélectionnez une image.", this);
    m_textePasDimageEncours->setAlignment(Qt::AlignCenter);

    m_textEdit = new QTextEdit("", this);
    m_nombreDeCaracteresTexte = new QLabel(this);
    updateNombreCaracteres();
    m_layoutTexte = new QVBoxLayout(this);
    m_layoutTexte->addWidget(m_textEdit);
    m_layoutTexte->addWidget(m_nombreDeCaracteresTexte);
//    std::vector<bool> in = Cryptage::stringToBits("Abc déàî");
//    std::vector<bool> in = Cryptage::uintToBytes(9);
//    std::string texte;
//    for(unsigned int i = 0; i < in.size(); i++)
//    {
//        std::cerr << "coucou";
//        texte += char('0'+in[i]);
//    }
//    Cryptage::bitsToString(in, texte);
//    m_textEdit->setText(QString(texte.c_str()));
//      m_textEdit->setText(QString(texte.c_str()));

//      std::cerr << "test : " << Cryptage::bitsToUint(Cryptage::uintToBits(504)) << std::endl;


    m_vueImage = new QLabel(this);
    m_vueImage->setPixmap(m_image);
    m_labelDimensionsImage = new QLabel(this);
    m_labelPlaceImage = new QLabel(this);

    m_layoutImage = new QVBoxLayout;
    m_layoutImage->addWidget(m_vueImage);
    m_layoutImage->addWidget(m_labelDimensionsImage);
    m_layoutImage->addWidget(m_labelPlaceImage);

    m_frameImage = new QFrame(this);
    m_frameImage->setFrameShape(QFrame::StyledPanel);
    m_frameImage->setLayout(m_layoutImage);
    redimensionnerImage();

    m_layoutPrincipal = new QHBoxLayout;
    m_layoutPrincipal->addLayout(m_layoutTexte);
    m_layoutPrincipal->addWidget(m_frameImage);

    m_zoneCentrale = new QWidget;
    m_zoneCentrale->setLayout(m_layoutPrincipal);
    setCentralWidget(m_textePasDimageEncours);

    //initialisation des signaux et des slots
    connect(m_actionQuitter, SIGNAL(triggered ()), qApp, SLOT(quit()));
    connect(m_actionNouveau, SIGNAL(triggered ()), this, SLOT(nouveau()));
    connect(m_actionOuvrir, SIGNAL(triggered ()), this, SLOT(ouvrir()));
    connect(m_actionEnregistrerSous, SIGNAL(triggered ()), this, SLOT(enregistrerSous()));
    connect(m_actionEnregistrer, SIGNAL(triggered ()), this, SLOT(enregistrer()));
    connect(m_actionChangerMethodeCryptage, SIGNAL(triggered ()), this, SLOT(changerLaMethodeDeCryptage()));
    connect(m_actionChangerPassword, SIGNAL(triggered ()), this, SLOT(changerPassword()));
    connect(m_textEdit, SIGNAL(textChanged ()), this, SLOT(updateNombreCaracteres()));
}

InterfaceImages::~InterfaceImages()
{
    for(unsigned int i = 0; i < m_methodesDeCryptage.size(); i++)
        delete m_methodesDeCryptage[i];
}

void InterfaceImages::nouveau()
{
    QString fichier = QFileDialog::getOpenFileName(this, "Selectionner une image qui servira à stocker votre texte.", qApp->applicationDirPath());

    if(fichier.isNull())//si l'utilisateur n'a pas sélectionné de fichier
        return;

    m_nouvelleImage = true;

    m_imageCryptage.load(fichier);
    m_imageCryptage = m_imageCryptage.convertToFormat(QImage::Format_ARGB32);

    m_textEdit->setPlainText("");

    changerImage(fichier);

    setCentralWidget(m_zoneCentrale);
}

void InterfaceImages::ouvrir()
{
    QString fichier = QFileDialog::getOpenFileName(this, "Selectionner une image qui contient du texte.", qApp->applicationDirPath());

    /*while(fichier.split(".").last().toLower() != QString("bmp"))
    {
        QMessageBox::information(this, "Erreur", "Ce fichier n'est pas une image valide pour la cryptographie. Merci de sélectionner une image bmp.");
        fichier = QFileDialog::getOpenFileName(this, "Selectionner une image qui contient du texte.", qApp->applicationDirPath(), "*.bmp");

        if(fichier.isNull())//si l'utilisateur n'a pas sélectionné de fichier
            return;
    }*/



    QImage imageOuverte;
    if(imageOuverte.load(fichier, "bmp"))
    {
        std::string texte;
        Image image(imageOuverte.width(), imageOuverte.height());
        qimageToImage(imageOuverte, image);

        std::vector<Image> images;
        images.push_back(image);
        int methode;
        unsigned int nombreDeBits;
        if(MethodeDeCryptage::readHeader(images, methode, nombreDeBits))
        {
            bool methodeTrouvee = false;
            unsigned int indexMethode = 0;
            while(!methodeTrouvee && indexMethode < m_methodesDeCryptage.size())
            {
                if(m_methodesDeCryptage[indexMethode]->id() == methode)
                    methodeTrouvee = true;
                else
                    indexMethode++;
            }

            if(methodeTrouvee)
            {
                std::string texte;
                if(m_methodesDeCryptage[indexMethode]->passwordRequired())
                    changerPassword();
                if(m_methodesDeCryptage[indexMethode]->decrypter(images, texte, m_password))
                {
                    m_imageCryptage.load(fichier, "bmp");
                    m_imageCryptage = m_imageCryptage.convertToFormat(QImage::Format_ARGB32);
                    changerImage(fichier);

                    m_textEdit->setPlainText(texte.c_str());

                    m_nouvelleImage = false;

                    setCentralWidget(m_zoneCentrale);

                    m_indexMethodeDeCryptage = indexMethode;
                }
                else
                {
                    QMessageBox::information(this, "Erreur", "Erreur lors du décryptage.");
                }
            }
            else
            {
                QMessageBox::information(this, "Erreur", "Méthode de cryptographie inconnue.");
            }
        }
        else
        {
            QMessageBox::information(this, "Erreur", "Aucune zone cryptée n'a été trouvée dans ce fichier. Si vous voulez utiliser ce fichier pour crypter des informations, utilisez la fonction Fichier->Nouveau.");
        }
        /*if(Cryptage::decrypter(image, texte))
        {
            m_imageCryptage.load(fichier);
            m_imageCryptage = m_imageCryptage.convertToFormat(QImage::Format_ARGB32);
            changerImage(fichier);

            m_textEdit->setPlainText(texte.c_str());

            m_nouvelleImage = false;

            setCentralWidget(m_zoneCentrale);
        }
        else
        {
            QMessageBox::information(this, "Erreur", "Aucune zone cryptée n'a été trouvée dans ce fichier. Si vous voulez utiliser ce fichier pour crypter des informations, utilisez la fonction Fichier->Nouveau.");
        }*/
    }
    else
    {
        QMessageBox::information(this, "Erreur", "Aucune zone cryptée n'a été trouvée dans ce fichier. Si vous voulez utiliser ce fichier pour crypter des informations, utilisez la fonction Fichier->Nouveau.");
    }
}

void InterfaceImages::enregistrer()
{
    if(m_nouvelleImage)
    {
        int reponse = QMessageBox::question(this, "Enregistrer", "Vous êtes sur le point de supprimer l'image originale. Etes-vous sûr de vouloir continuer ? (Vous pourriez aussi enregistrer sous un autre nom.)", QMessageBox::Yes | QMessageBox::No);
        if (reponse == QMessageBox::Yes)
        {
            enregistrerDansLeFichier(m_fichierImage->fileName());
        }
        else if (reponse == QMessageBox::No)
        {
            reponse = QMessageBox::question(this, "Enregistrer", "Voulez-vous enregistrer sous un autre nom ?", QMessageBox::Yes | QMessageBox::No);
            if (reponse == QMessageBox::Yes)
            {
                enregistrerSous();
            }
        }
    }
    else
    {
        enregistrerDansLeFichier(m_fichierImage->fileName());
    }
}

void InterfaceImages::enregistrerSous()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Selectionner un fichier pour sauvegarder", qApp->applicationDirPath(), "");

    if(fichier.isNull())//si l'utilisateur n'a pas sélectionné de fichier
        return;

    enregistrerDansLeFichier(fichier);
}

void InterfaceImages::changerLaMethodeDeCryptage()
{
    Image imagePourCrypter(m_imageCryptage.width(), m_imageCryptage.height());
    std::vector<Image> images;
    images.push_back(imagePourCrypter);

    QStringList methodes;
    for(unsigned int i = 0; i < m_methodesDeCryptage.size(); i++)
        methodes.append( QString("%1 (%2 caractères)").arg(m_methodesDeCryptage[i]->name().c_str()).arg(m_methodesDeCryptage[i]->size(images)));
    bool ok;
    QString selection = QInputDialog::getItem(this, "Méthode de cryptage", "Quelle méthode souhaitez-vous utiliser pour crypter vos données ?", methodes, m_indexMethodeDeCryptage, false, &ok);

    if(ok)
    {
        bool selectionTrouvee = false;
        int i = 0;
        while(!selectionTrouvee && i < methodes.size())
        {
            if(selection == methodes[i])
               selectionTrouvee = true;
            else
                i++;
        }

        m_indexMethodeDeCryptage = i;
        if(methode()->passwordRequired())
            changerPassword();
    }
}

void InterfaceImages::changerPassword()
{
    m_password = QInputDialog::getText(this, "Methode de cryptage", "Mot de passe ?", QLineEdit::Password).toStdString();
}

void InterfaceImages::redimensionnerImage()
{
    //m_frameImage->setMaximumWidth(this->height()/3);
    //m_frameImage->resize(this->width()/3, m_frameImage->height());
    //m_frameImage->resize(100, 300);
    //m_textEdit->resize(400, 400);
    m_image = m_image.scaledToWidth(300, Qt::SmoothTransformation);
}

void InterfaceImages::resizeEvent(QResizeEvent *event)
{
    redimensionnerImage();
}

void InterfaceImages::updateNombreCaracteres()
{
    unsigned int nombre = m_textEdit->toPlainText().size();
    if(nombre == 0)
        m_nombreDeCaracteresTexte->setText(QString("Le texte ne contient aucun caractère."));
    else if(nombre == 1)
        m_nombreDeCaracteresTexte->setText(QString("Le texte contient un caractère."));
    else
        m_nombreDeCaracteresTexte->setText(QString("Le texte contient %1 caractères.").arg(nombre));
}

void InterfaceImages::qimageToImage(const QImage &qimage, Image &image)
{
    if((unsigned int)qimage.width() == image.getWidth() && (unsigned int)qimage.height() == image.getHeight())
    {
        Curseur curseur(image);
        do
        {
            QColor color(qimage.pixel(curseur.getX(), curseur.getY()));
            curseur.setPixel(color.red(), color.green(), color.blue(), color.alpha());
        } while(curseur.moveToNextPixel());

        /*for(unsigned int y = 0; y < (unsigned int)qimage.height(); y++)
        {
            for(unsigned int x = 0; x < (unsigned int)qimage.width(); x++)
            {
                QColor color(qimage.pixel(x, y));
                image[y][x][Cryptage::ROUGE] = color.red();
                image[y][x][Cryptage::VERT] = color.green();
                image[y][x][Cryptage::BLEU] = color.blue();
                image[y][x][Cryptage::ALPHA] = color.alpha();
            }
        }*/
    }
    else
    {
        std::cerr << "L'image et la QImage n'ont pas les même dimensions." << std::endl;
    }
}

void InterfaceImages::imageToQImage(Image &image, QImage &qimage)
{
    qimage.scaled(QSize(image.getWidth(), image.getHeight()));

    Curseur curseur(image);
    unsigned char rouge, vert, bleu, alpha;
    do
    {
        curseur.readPixel(rouge, vert, bleu, alpha);
        QColor color(rouge, vert, bleu, alpha);
        qimage.setPixel(int(curseur.getX()), int(curseur.getY()), color.rgba());
    } while(curseur.moveToNextPixel());
    /*for(unsigned int y = 0; y < image.getHeight(); y++)
    {
        for(unsigned int x = 0; x < image.getWidth(); x++)
        {
            //QColor color(image[y][x][Cryptage::ROUGE], image[y][x][Cryptage::VERT], image[y][x][Cryptage::BLEU], image[y][x][Cryptage::ALPHA]);
            QColor color(image[y][x][Cryptage::ROUGE], image[y][x][Cryptage::VERT], image[y][x][Cryptage::BLEU], image[y][x][Cryptage::ALPHA]);
            //QColor color(254, 255, 255, 255);

            qimage.setPixel(int(x), int(y), color.rgba());
        }
    }*/
}

void InterfaceImages::enregistrerDansLeFichier(const QString &fichier)
{
    Image imagePourCrypter(m_imageCryptage.width(), m_imageCryptage.height());
    qimageToImage(m_imageCryptage, imagePourCrypter);

    //Cryptage::crypter(imagePourCrypter, m_textEdit->toPlainText().toStdString());
    std::vector<Image> images;
    images.push_back(imagePourCrypter);
    if(!methode()->crypter(images, m_textEdit->toPlainText().toStdString(), m_password))
        QMessageBox::information(this, "Erreur", "Sauvegarde impossible.");

    //imageToQImage(imagePourCrypter, m_imageCryptage);
    imageToQImage(images[0], m_imageCryptage);

    m_imageCryptage.save(fichier, "bmp");
    changerImage(fichier);

    if(m_fichierImage == NULL)
    {
        m_fichierImage = new QFile(fichier);
    }
    else if(fichier != m_fichierImage->fileName())
    {
        delete m_fichierImage;
        m_fichierImage = new QFile(fichier);
    }

    m_nouvelleImage = false;
}

void InterfaceImages::changerImage(const QString &fichier)
{
    if(!m_image.load(fichier))
        m_image.load(fichier, "bmp");
    m_labelDimensionsImage->setText(QString("Taille de l'image : %1x%2").arg(m_image.width()).arg(m_image.height()));
    m_labelPlaceImage->setText(QString("L'image contient %1 pixels.").arg(m_image.width()*m_image.height()));
    redimensionnerImage();
    m_vueImage->setPixmap(m_image);


    if(m_fichierImage == NULL)
    {
        m_fichierImage = new QFile(fichier);
    }
    else if(fichier != m_fichierImage->fileName())
    {
        delete m_fichierImage;
        m_fichierImage = new QFile(fichier);
    }
}
