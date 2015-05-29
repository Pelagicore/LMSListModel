/*
   Copyright (C) 2015 Pelagicore AB <jonatan.palsson@pelagicore.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation; either version 2.1 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDebug>

#include "lmslistmodel.h"

LMSListModel::LMSListModel(QObject *parent)
    : QAbstractListModel(parent) {}

void LMSListModel::setDatabase(QString db) {
    beginResetModel();
    m_itemsList.clear();
    m_database.setDatabase(db);
    endResetModel();
}

QString LMSListModel::database() {
    return m_database.database();
}

void LMSListModel::setWhereArtist(QString q) {
    m_whereQueries.insert("artist_name", q);

    beginResetModel();
    qDebug() << "Clearing model in " << __FUNCTION__;
    m_itemsList.clear();
    endResetModel();
}

QString LMSListModel::whereArtist() {
    if (m_whereQueries.contains("artist_name")) {
        return m_whereQueries["artist_name"];
    }

    return QString();
}

void LMSListModel::setWhereTrack(QString q) {
    m_whereQueries.insert("title", q);

    beginResetModel();
    qDebug() << "Clearing model in " << __FUNCTION__;
    m_itemsList.clear();
    endResetModel();
}

QString LMSListModel::whereTrack() {
    if (m_whereQueries.contains("title")) {
        return m_whereQueries["title"];
    }

    return QString();
}

void LMSListModel::setQueryType(QueryType type){
    qDebug() << "Setting query type to " << type;
    switch (type) {
    case Tracks: {
        m_allItemsQuery = "SELECT audios.id as id, audios.title as title, "
                                  "audio_artists.name as artist_name, "
                                  "audio_albums.name as album_name, "
                                  "audio_albums.album_art_url as album_art_url, "
                                  "files.path as path FROM audios "
                            "LEFT JOIN audio_artists ON audios.artist_id = audio_artists.id "
                            "LEFT JOIN audio_albums ON audio_artists.id = audio_albums.artist_id "
                            "LEFT JOIN files ON audios.id = files.id";
        m_allItemsCountQuery = "SELECT COUNT(id) FROM files;"; //Shouldn't this be audios?
        break;
    }
    case Albums: {
        m_allItemsQuery =  "SELECT audio_albums.id, audio_albums.name as title, "
                                "audio_artists.name as artist_name, "
                                "audio_albums.album_art_url as album_art_url FROM audio_albums "
                            "LEFT JOIN audio_artists ON audio_albums.artist_id = audio_artists.id ";

        m_allItemsCountQuery = "SELECT COUNT(id) FROM audio_albums;";
        break;
    }
    case Artists: {
        m_allItemsQuery = "SELECT audio_artists.id as id, "
                                 "audio_artists.name as title FROM audio_artists ";
        m_allItemsCountQuery = "SELECT COUNT(id) FROM audio_artists;";
        break;
    }
    case Videos: {
        m_allItemsQuery = "SELECT videos.id as id, videos.title as title, "
                                 "videos.artist as artist, files.path as path "
                                 "FROM videos LEFT JOIN files ON videos.id = files.id";
        m_allItemsCountQuery = "SELECT COUNT(id) FROM videos;";
        break;
    }
    default: {
        qDebug() << "Unhandled case in" << __FUNCTION__;
    }
    }
    beginResetModel();
    m_itemsList.clear();
    endResetModel();
}

QHash<int, QByteArray> LMSListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles.insert(Database::TitleRole, QByteArray("titleColumn"));
    roles.insert(Database::ArtistRole, QByteArray("artistColumn"));
    roles.insert(Database::AlbumRole, QByteArray("albumColumn"));
    roles.insert(Database::AlbumArtRole, QByteArray("albumArtColumn"));
    roles.insert(Database::PathRole, QByteArray("pathColumn"));
    return roles;
}

bool LMSListModel::canFetchMore(const QModelIndex &parent) const {
    Q_UNUSED (parent);

    bool result  = m_database.allItemsCount(m_allItemsCountQuery) > m_itemsList.size();
    qDebug() << "Checking for more data: " << result;
    return result;
}

void LMSListModel::fetchMore(const QModelIndex &parent) {
    qDebug() << "Populating more data";
    QStringList whereQueryList;
    QString whereQuery;

    for (int i = 0; i < m_whereQueries.keys().length(); i++) {
        QString key = m_whereQueries.keys()[i];
        whereQueryList <<  (" " + key + " LIKE '%" + m_whereQueries[key] + "%' ");
    }

    whereQuery = whereQueryList.join (" AND ");
    /* TODO: Make this a property */
    int chunkSize = 25;

    int beginTrack = m_itemsList.size();
    QList<QHash<int, QString> > items = m_database.allItems(m_allItemsQuery,
                                                            whereQuery,
                                                            beginTrack,
                                                            chunkSize);

    beginInsertRows(parent, beginTrack, beginTrack + items.length() - 1);
    m_itemsList.append(items);
    endInsertRows();
}

int LMSListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED (parent);
    return m_itemsList.length();
}

QVariant LMSListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        qDebug() << "Requested index " << index.row() << " is invalid";
        return QVariant();
    }

    if (index.row() >= m_itemsList.size()) {
        qDebug() << "Requested index " << index.row() << " is not cached";
        return QVariant();
    }

    QHash<int, QString> track = m_itemsList.at(index.row());

    QString retval = track.value(role);
    return retval;
}

QVariant LMSListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}

Qt::ItemFlags LMSListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
