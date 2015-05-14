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

#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QtSql>

class Database
{
public:
    static const int TitleRole;
    static const int ArtistRole;
    static const int AlbumArtRole;
    static const int AlbumRole;
    static const int PathRole;
    static const int IDRole;

    Database();
    ~Database();

    /*!
     * \brief Set the SQLite database file created by LMS
     * \param db absolute path to the LMS database file
     */
    void setDatabase(QString db);

    /*!
     * \brief Number of tracks available
     * \return Number of tracks available
     */
    int allTracksCount() const;

    /*!
     * \brief Execute an arbitraty SQL query
     * \param queryStr
     * \return Query result
     */
    QSqlQuery executeQuery(QString queryStr) const;

    /*!
     * \brief Count the number of items, given a SQL query
     * \param query SQL COUNT() query to qount items for
     * \return number of items returned by COUNT()
     */
    int allItemsCount(QString query) const;

    /*!
     * \brief Return a list of media items
     * \param query SQL query of items to select
     * \param whereQuery WHERE part of the query, excluding 'WHERE', for example: "artist_id == 1"
     * \param start first index to include
     * \param end last index to include
     * \return list of items matching query
     */
    QList<QHash<int, QString> > allItems(QString query, QString whereQuery, uint start, uint end) const;

    /*!
     * \brief Get the current database file
     * \return Path to current database file
     */
    QString database();


private:
    QSqlDatabase m_database;
};

#endif // DATABASE_H
