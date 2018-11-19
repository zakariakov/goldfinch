/***************************************************************************
 *      Project created by QtCreator 2018-06-01T17:15:24                   *
 *                                                                         *
 *    goldfinch Copyright (C) 2014 AbouZakaria <yahiaui@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "propertiesfile.h"
#include "ui_propertiesfile.h"
#include "database.h"

#ifdef USE_LIB_TAG
#include "tagid.h"
#else
#include "database.h"
#endif
#include <QDebug>
PropertiesFile::PropertiesFile(const QString &file, bool readOnly, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PropertiesFile)
{

    ui->setupUi(this);

#ifdef USE_LIB_TAG
    if(readOnly){

        QVariantMap map=TagId::mediaTags(file);
        ui->lineEditTitle->setText(map.value(COL_S_TITLE).toString());
        ui->comboBoxAlbum->addItem(map.value(COL_S_ALBUM).toString());
        ui->comboBoxArtist->addItem(map.value(COL_S_ARTIST).toString());
        ui->comboBoxGenre->addItem(map.value(COL_S_GENRE).toString());
        ui->lineEditPath->setText(map.value(COL_S_PATH).toString());
        ui->labelBitrate->setText(tr("Bitrate:  ")+map.value(COL_S_BITRATE).toString());
        ui->labelLanght->setText(tr("Lenght:  ")+map.value(COL_S_TIME).toString());
        ui->labelSampleRate->setText(tr("SampleRate:  ")+map.value(COL_S_SAMPLE_RATE).toString());

    }else{
        ui->buttonBox->addButton(QDialogButtonBox::Save);

        ui->lineEditTitle->setReadOnly(false);
        ui->comboBoxAlbum->setEditable(true);
        ui->comboBoxArtist->setEditable(true);
        ui->comboBoxGenre->setEditable(true);

        ui->comboBoxAlbum->addItems(DataBase::chargeRoot(COL_I_ALBUM));
        ui->comboBoxArtist->addItems(DataBase::chargeRoot(COL_I_ARTIST));
        ui->comboBoxGenre->addItems(DataBase::chargeRoot(COL_I_GENRE));

        ui->comboBoxAlbum->setCurrentIndex(-1);
        ui->comboBoxArtist->setCurrentIndex(-1);
        ui->comboBoxGenre->setCurrentIndex(-1);

        QVariantMap map=TagId::mediaTags(file);
        ui->lineEditTitle->setText(map.value(COL_S_TITLE).toString());
        ui->comboBoxAlbum->setCurrentText(map.value(COL_S_ALBUM).toString());
        ui->comboBoxArtist->setCurrentText(map.value(COL_S_ARTIST).toString());
        ui->comboBoxGenre->setCurrentText(map.value(COL_S_GENRE).toString());
        ui->lineEditPath->setText(map.value(COL_S_PATH).toString());
        ui->labelBitrate->setText(tr("Bitrate:  ")+map.value(COL_S_BITRATE).toString());
        ui->labelLanght->setText(tr("Lenght:  ")+map.value(COL_S_TIME).toString());
        ui->labelSampleRate->setText(tr("SampleRate:  ")+map.value(COL_S_SAMPLE_RATE).toString());

        albumText= ui->comboBoxAlbum->currentText();
        genreText= ui->comboBoxGenre->currentText();
        artistText= ui->comboBoxArtist->currentText();

    }


#else
    QVariantMap map=DataBase::filePropery(file);
    ui->lineEditTitle->setText(map.value(COL_S_TITLE).toString());
    ui->lineEditAlbum->setText(map.value(COL_S_ALBUM).toString());
    ui->lineEditArtist->setText(map.value(COL_S_ARTIST).toString());
    ui->lineEditPath->setText(map.value(COL_S_PATH).toString());
    ui->lineEditLanght->setText(map.value(COL_S_TIME).toString());
    ui->lineEditGenre->setText(map.value(COL_S_GENRE).toString());
#endif
}

PropertiesFile::~PropertiesFile()
{
    delete ui;
}

void PropertiesFile::on_buttonBox_clicked(QAbstractButton *button)
{

    if(ui->buttonBox->buttonRole(button)==QDialogButtonBox::AcceptRole){

        QVariantMap map;
        bool modied=false;

        if(ui->lineEditTitle->isModified()){
            map.insert(COL_S_TITLE,ui->lineEditTitle->text());
            modied=true;
        }

        if(albumText!=ui->comboBoxAlbum->currentText()){
            map.insert(COL_S_ALBUM,ui->comboBoxAlbum->currentText());
            modied=true;
        }

        if(artistText!=ui->comboBoxArtist->currentText()){
            map.insert(COL_S_ARTIST,ui->comboBoxArtist->currentText());
            modied=true;
        }

         if(genreText!=ui->comboBoxGenre->currentText()){
            map.insert(COL_S_GENRE,ui->comboBoxGenre->currentText());
            modied=true;
        }

        if(modied){
            map.insert(COL_S_PATH,ui->lineEditPath->text());
            TagId::setMediaTags(map);
        }
    }

}

