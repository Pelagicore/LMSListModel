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

#ifndef LMSLISTMODEL_H
#define LMSLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QStringList>

#include "database.h"


class LMSListModel : public QAbstractListModel
{

    Q_OBJECT
    Q_ENUMS(QueryType)
    Q_PROPERTY (QueryType queryType READ queryType WRITE setQueryType)
    Q_PROPERTY (QString artist READ whereArtist WRITE setWhereArtist)
    Q_PROPERTY (QString track READ whereTrack WRITE setWhereTrack)
    Q_PROPERTY (QString database READ database WRITE setDatabase)
public:
    typedef enum {
        Artists,
        Albums,
        Tracks
    } QueryType;

    LMSListModel(QObject *parent = 0);

     /*!
      * \brief Set the type of the query. This will flush the current list of results and thus
      * trigger a re-fetch from the QML side
      */
     void setQueryType (QueryType);

     /*!
      * \brief Get the current query type
      */
     QueryType queryType();

     /*!
      * \brief Filter on artist name. This will perform a LIKE %% query against the database.
      * Changing this triggers a flush of the current results, and triggers a re-fetch from QML
      */
     void setWhereArtist(QString);

     /*!
      * \brief Get the current artist filter
      */
     QString whereArtist();

     /*!
      * \brief Filter on track title. This will perform a LIKE %% query against the database.
      * Changing this triggers a flush of the current results, and triggers a re-fetch from QML
      */
     void setWhereTrack(QString);
     QString whereTrack();

     /*!
      * \brief Set the database file to use. The database is automatically opened when calling this,
      * and the current results are flushed, which also causes the QML to re-fetch results
      */
     void setDatabase(QString);

     /*!
      * \brief Get the path to the current database
      */
     QString database();

    /*!
     * \brief Names of the columns available in this list
     *
     * For a list of audio tracks, this could be [(0, "Title"), (1, "Artist")] for instance. The
     * QByteArray parameter is used from QML to identify the field, so pick something descriptive
     * here.
     *
     * \return Hash table of column names, keyed on a unique integer ID for each column name
     */
    QHash<int, QByteArray> roleNames() const;

    /*!
     * \brief Returns the number of elements available in this list.
     *
     * This should indicate the number of elements available in the model-internal storage, i.e
     * elements which have already been fetched from the database. Whether or not more elements
     * can be fetched from the database is controlled by canFetchMore() and fetchMore()
     *
     * \return The number of rows available in the list
     */
    int rowCount() const {
        return rowCount (QModelIndex());
    }

    /*!
     * \brief Fetch data for a row
     * \param index The row to fetch data for
     * \param role The field in the row to fetch data for
     *
     * Think of this like fetching data from a matrix, where the \a index specifies the Y axis and
     * the \a role specifies the Y axis. In order for this function to be efficient, some kind of
     * backing storage which allows efficient lookups based on row and column needs to be used.
     *
     * As an example, when doing a list of audio tracks, the index could be 1, and the role could
     * be "artist", which would return the artist field for the row with index 1.
     *
     * \return A cell from the LMSListModel
     */
    QVariant data(const QModelIndex &index, int role) const;

    /*!
     * \brief Fetch the header for the specified role, depending on orientation
     * \param section For horizontal lists, this is the row, for vertical lists, this is the column
     * \param orientation
     * \param role The same role as used by roleNames()
     *
     * Think of this like asking the listmodel what the title of a specific cell is. This is a way
     * to go from the integer \a role type to a (for instance) textual description of the cell
     *
     * \return A description suitable for describing the cell specified by the section and role
     *         parameters, given the specified orientation
     */
    QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    /*!
     * \brief Return any flags available for the specified index
     * \param index
     *
     * Currently unused, but will be used to indicate that an item is currently playing, etc
     *
     * \return An integer which can be masked to discover the flags of the index
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /*!
     * \brief Indicate whether there is more data available
     * \param parent
     *
     * This is a good place to compare the data available in the model to the data available
     * in the database
     *
     * \return true if there is more data, false otherwise
     */
    bool canFetchMore(const QModelIndex & parent) const;

    /*!
     * \brief Fetch more data from the LMS database
     * \param parent
     *
     * This function should modify the internal cache of the database, which should reside in the
     * model implementing this interface
     */
    void fetchMore(const QModelIndex & parent);
protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Database m_database;

private:
    QString m_allItemsQuery;
    QString m_allItemsCountQuery;
    QList<QHash<int, QString> > m_itemsList;
    QHash<QString, QString> m_whereQueries;

};


#endif // LMSLISTMODEL_H
