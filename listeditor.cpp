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

#include <QtWidgets>

#include "tumb.h"
#include "listeditor.h"
//#include "starrating.h"
#include "QToolButton"
//#include "QDebug"
#include "QLabel"
void Iditoring::paint(QPainter *painter, const QRect &rect,
                      const QPalette &palette, EditMode mode) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);

    if (mode == Editable) {
        painter->setBrush(palette.highlight());
    } else {
        painter->setBrush(palette.foreground());
    }


    // painter->translate(rect.x(), rect.y() );
    if (mode == Editable) {
        //   qDebug()<<"idetoryng"<<rect;
        painter->setOpacity(0.5);
        painter->fillRect(rect,palette.highlight());

    }/*else{
        painter->drawPixmap(rect.x(),rect.y(),16,16,QPixmap(":/icons/undo"));
        qDebug()<<"idetoryng"<<rect.x()<<rect.y();
        painter->translate(1.0, 0.0);
    }*/

    painter->restore();
}
//! [2]


//! [0]
ListEditor::ListEditor(const QModelIndex &index, QWidget *parent)
    : QWidget(parent),mIndex(index)
{
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    setMaximumHeight(30);

    QString style= QString(" QToolButton {"
                           /* " border: 1px solid #8f8f91;"*/
                           "border-radius: 10px;"
                           "background-color:/*transparent*/palette(window);;"
                           "width: 20px;"
                           " height:20px;"
                           "max-width: 20px;"
                           " max-height:20px;"
                           "}"
                           " QToolButton:hover {"
                           " border: 1px solid palette(window-text);"
                           // " border: 1px solid palette(highlighted-text );"
                           //                           "background-color:palette(highlight);;"
                           "}"
                           );

    setStyleSheet(style);

    QToolButton *btnPlay=new QToolButton;

    QToolButton *btnAdd=new QToolButton;

    btnFavorit=new QToolButton;

    QToolButton *btnImage=new QToolButton;

    btnPlay->setIcon(Tumb::icon(I_PLAY));
    btnPlay->setIconSize(QSize(12,12));

    btnAdd->setIcon(Tumb::icon(I_ADD));
    btnAdd->setIconSize(QSize(12,12));

    btnFavorit->setIcon(Tumb::icon(I_START));
    btnFavorit->setIconSize(QSize(12,12));
    btnFavorit->setCheckable(true);
    btnImage->setIcon(Tumb::icon(I_IMG));
    btnImage->setIconSize(QSize(12,12));

    connect(btnFavorit,SIGNAL(toggled(bool)),this,SLOT(favoritToggled(bool)));
btnFavorit->setChecked(index.data(USER_RATED).toBool());
    QHBoxLayout *hLayout = new QHBoxLayout;

    hLayout->setMargin(0);

    hLayout->addWidget(btnPlay);
    hLayout->addWidget(btnAdd);

    int id=index.data(USER_ID).toInt();
    if(id==COL_I_ALBUM /*||id==CAT_ALBUMRATED*/){
        hLayout->addWidget(btnFavorit);
        bool checked=index.data(USER_RATED).toBool();
        btnFavorit->setChecked(checked);
    }

    hLayout->addWidget(btnImage);

    connect(btnPlay,SIGNAL(clicked()),this,SLOT(playAlbumClicked()));
    connect(btnAdd,SIGNAL(clicked()),this,SLOT(addAlbumClicked()));
    connect(btnFavorit,SIGNAL(clicked()),this,SLOT(favoritAlbumClicked()));
    connect(btnImage,SIGNAL(clicked()),this,SLOT(imageAlbumClicked()));

    setLayout(hLayout);

    setMouseTracking(true);
    //  setAutoFillBackground(true);
}

ListEditor::~ListEditor()
{
    // qDebug()<<"delet wwww";
}

void ListEditor::favoritToggled(bool cheked)
{
    if(cheked)
        btnFavorit->setIcon(Tumb::icon(I_STARTED));
    else
        btnFavorit->setIcon(Tumb::icon(I_START));

}

void ListEditor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    myStarRating.paint(&painter, rect(), this->palette(),
                       Iditoring::Editable);
}

//------------------------------------------------Album
void ListEditor::setEditingAcion()
{
    QToolButton *btn = qobject_cast<QToolButton *>(sender());

    if(btn)
    {
      ///  int data=btn->defaultAction()->data().toInt();
      //  qDebug()<<"setEditingAcion data"<<data <<mIndex.row();
        //  emit editingChanged(mIndex.row(),data,btnFavorit->isChecked());

    }
}
//------------------------------------------------Album
void ListEditor::addAlbumClicked()
{ emit addAlbum(mIndex.row());}

void ListEditor::playAlbumClicked()
{ emit playAlbum(mIndex.row());}

void ListEditor::imageAlbumClicked()
{emit changeImageAlbum(mIndex.row());}

void ListEditor::favoritAlbumClicked( )
{emit favoritAlbum(mIndex.row(),btnFavorit->isChecked());}

