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

#ifndef LISTEDETOR_H
#define LISTEDETOR_H

#include <QWidget>
#include <QModelIndex>
#include <QToolButton>

class Iditoring
{
public:
    enum EditMode { Editable, ReadOnly };

    explicit Iditoring(){}

    void paint(QPainter *painter, const QRect &rect,
               const QPalette &palette, EditMode mode) const;

private:
    //QSize mSize;
};
Q_DECLARE_METATYPE(Iditoring)
//! [0]
class ListEditor : public QWidget
{
    Q_OBJECT

public:
    ListEditor(const QModelIndex &index, QWidget *parent = nullptr);
    ~ListEditor() override;
    //    QSize sizeHint() const override;

    Iditoring iditoring() { return myStarRating; }

    void setSize(QSize size){mSize=size;}

signals:
    void addAlbum(int);
    void playAlbum(int);
    void favoritAlbum(int,bool);
    void changeImageAlbum(int);

    void editingChanged(int row,int action,bool favo=false);

protected:
    void paintEvent(QPaintEvent *event) override;


private:
    QToolButton *btnFavorit;
    QModelIndex mIndex;
    Iditoring myStarRating;
    QSize mSize;
private slots:
    void favoritToggled(bool cheked);
    //---------------------
    void addAlbumClicked();
    void playAlbumClicked();
    void favoritAlbumClicked();
    void imageAlbumClicked();


    //---------------------
    void  setEditingAcion();
};
//! [0]

#endif
