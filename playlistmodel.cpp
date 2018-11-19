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

#include "playlistmodel.h"
#include "tumb.h"
#include <QFileInfo>
#include <QUrl>
#include <QMediaPlaylist>
#include <QIcon>
PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

PlaylistModel::~PlaylistModel()
{
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return m_playlist && !parent.isValid() ? m_playlist->mediaCount() : 0;
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    return !parent.isValid() ? ColumnCount : 0;
}

QModelIndex PlaylistModel::index(int row, int column, const QModelIndex &parent) const
{
    return m_playlist && !parent.isValid()
            && row >= 0 && row < m_playlist->mediaCount()
            && column >= 0 && column < ColumnCount
            ? createIndex(row, column)
            : QModelIndex();
}

QModelIndex PlaylistModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);

    return QModelIndex();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())  return QVariant();

    if (role == Qt::DisplayRole && index.column() == Title) {
        QVariant value = m_data[index];
        if (!value.isValid()) {
            QUrl location = m_playlist->media(index.row()).canonicalUrl();
            QString value = m_dataTitle[location];

            if(value.isEmpty())
                return QFileInfo(location.path()).fileName();

            return value;
            //
        }

        return value;
    }

    if ( role == Qt::ToolTipRole && index.column() == Title) {

        QUrl location = m_playlist->media(index.row()).canonicalUrl();
        QString value = m_dataTitle[location];

        return QFileInfo(location.path()).fileName();

    }

    if ( role == Qt::DecorationRole && index.column() == Title) {

        if(m_playlist.data()->currentIndex()==index.row())
            return  Tumb::icon(I_PLAY);
        //       return QIcon(":/icons/play");
        else
            return QIcon();

    }

    if ( role == Qt::UserRole && index.column() == Title) {

        QUrl location = m_playlist->media(index.row()).canonicalUrl();
        return location.toLocalFile();
    }
    return QVariant();
}

QMediaPlaylist *PlaylistModel::playlist() const
{
    return m_playlist.data();
}

void PlaylistModel::setPlaylist(QMediaPlaylist *playlist)
{
    if (m_playlist) {
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeInserted, this, &PlaylistModel::beginInsertItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaInserted, this, &PlaylistModel::endInsertItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeRemoved, this, &PlaylistModel::beginRemoveItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaRemoved, this, &PlaylistModel::endRemoveItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaChanged, this, &PlaylistModel::changeItems);
    }

    beginResetModel();
    m_playlist.reset(playlist);

    if (m_playlist) {
        connect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeInserted, this, &PlaylistModel::beginInsertItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaInserted, this, &PlaylistModel::endInsertItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeRemoved, this, &PlaylistModel::beginRemoveItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaRemoved, this, &PlaylistModel::endRemoveItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaChanged, this, &PlaylistModel::changeItems);

    }

    endResetModel();
}

bool PlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{


    Q_UNUSED(role);

    m_data[index] = value;

    emit dataChanged(index, index);
    return true;
}

bool PlaylistModel::setTitle(const QModelIndex &index,const QVariant &key, const QString &value)
{

    m_dataTitle[key] = value;

    emit dataChanged(index, index);
    return true;
}



void PlaylistModel::beginInsertItems(int start, int end)
{
    m_data.clear();
    //  qDebug()<<"beginInsertItems"<<start<<end;
    beginInsertRows(QModelIndex(), start, end);

}

void PlaylistModel::endInsertItems()
{
    endInsertRows();

}

void PlaylistModel::beginRemoveItems(int start, int end)
{
    m_data.clear();
    QUrl location = m_playlist->media(start).canonicalUrl();
    beginRemoveRows(QModelIndex(), start, end);
}

void PlaylistModel::endRemoveItems()
{
    endInsertRows();
}

void PlaylistModel::changeItems(int start, int end)
{
    m_data.clear();
    //  qDebug()<<"changeItems"<<start<<end;
    emit dataChanged(index(start,0), index(end,ColumnCount));
}

QString PlaylistModel::mediaTitle(QUrl url)
{

    return  m_dataTitle[url];

}
