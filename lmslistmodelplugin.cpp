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

#include "lmslistmodelplugin.h"

#include <QQmlExtensionPlugin>
#include <QtQml>
#include <QDebug>


void LMSListModelPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("LMSListModel"));
    qmlRegisterType<LMSListModel>(uri, 1, 0, "LMSListModel");
}
