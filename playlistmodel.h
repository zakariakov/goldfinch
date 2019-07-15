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

#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractItemModel>
#include <QScopedPointer>
#include <QMimeData>
QT_BEGIN_NAMESPACE
class QMediaPlaylist;
QT_END_NAMESPACE

class PlaylistModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Column
    {
        Title = 0,
        ColumnCount
    };


    explicit PlaylistModel(QObject *parent = nullptr);
   ~PlaylistModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QMediaPlaylist *playlist() const;
    void setPlaylist(QMediaPlaylist *playlist);

   bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;
bool setTitle(const QModelIndex &index,const QVariant &key, const QString &value);
void clear(){m_dataTitle.clear();m_data.clear();}
QString mediaTitle(QUrl url);
private slots:
    void beginInsertItems(int start, int end);
    void endInsertItems();
    void beginRemoveItems(int start, int end);
    void endRemoveItems();
    void changeItems(int start, int end);

protected:
 //!    عند عملية السحب والافلات


//    virtual QStringList mimeTypes() const;
//    virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
//    virtual bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
//                                 int row, int column, const QModelIndex &parent) const;
//    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action,
//                              int row, int column, const QModelIndex &parent);


    bool dropMimeData(const QMimeData * data,
                     Qt::DropAction action,
                     int row,
                     int column,
                     const QModelIndex & parent ) override;

   QStringList mimeTypes() const override;

private:


    QScopedPointer<QMediaPlaylist> m_playlist;
    QMap<QModelIndex, QVariant> m_data;
    QMap<QVariant, QString> m_dataTitle;
};

#endif // PLAYLISTMODEL_H
