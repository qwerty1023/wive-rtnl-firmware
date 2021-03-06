/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*
 * wpa_gui - EventHistory class
 * Copyright (c) 2005-2006, Jouni Malinen <jkmaline@cc.hut.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef EVENTHISTORY_H
#define EVENTHISTORY_H

#include <QObject>
#include "ui_eventhistory.h"


class EventListModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	EventListModel(QObject *parent = 0)
		: QAbstractTableModel(parent) {}

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const;
	void addEvent(QString time, QString msg);

private:
	QStringList timeList;
	QStringList msgList;
};


class EventHistory : public QDialog, public Ui::EventHistory
{
	Q_OBJECT

public:
	EventHistory(QWidget *parent = 0, const char *name = 0,
		     bool modal = false, Qt::WFlags fl = 0);
	~EventHistory();

public slots:
	virtual void addEvents(WpaMsgList msgs);
	virtual void addEvent(WpaMsg msg);

protected slots:
	virtual void languageChange();

private:
	EventListModel *elm;
};

#endif /* EVENTHISTORY_H */
