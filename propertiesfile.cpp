#include "propertiesfile.h"
#include "ui_propertiesfile.h"

PropertiesFile::PropertiesFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PropertiesFile)
{
    ui->setupUi(this);
}

PropertiesFile::~PropertiesFile()
{
    delete ui;
}

void PropertiesFile::setInformations(const QVariantMap &map)
{
   ui->lineEditTitle->setText(map.value("Title").toString());
   ui->lineEditAlbum->setText(map.value("Album").toString());
   ui->lineEditArtist->setText(map.value("Artist").toString());
   ui->lineEditPath->setText(map.value("Path").toString());


}
