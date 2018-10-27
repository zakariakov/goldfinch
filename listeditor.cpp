/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "tumb.h"
#include "listeditor.h"
//#include "starrating.h"
#include "QToolButton"
#include "QDebug"
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
    if(id==CAT_ALBUM /*||id==CAT_ALBUMRATED*/){
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
        int data=btn->defaultAction()->data().toInt();
        qDebug()<<"setEditingAcion data"<<data <<mIndex.row();
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

