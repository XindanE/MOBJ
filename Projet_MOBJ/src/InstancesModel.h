#ifndef INSTANCES_MODEL_H
#define INSTANCES_MODEL_H

#include <QtWidgets>
#include <QAbstractTableModel>
#include <QObject>
#include "Cell.h"

namespace Netlist
{
    class Cell;
    class InstancesModel : public QAbstractTableModel
    {
        Q_OBJECT ;
        public :
            InstancesModel(QObject *parent=NULL);
            ~InstancesModel();
            void setCell(Cell*);
            Cell *getModel(int row);
            int rowCount(const QModelIndex& parent=QModelIndex())const;
            int columnCount(const QModelIndex& parent=QModelIndex())const;
            QVariant data(const QModelIndex& index, int role=Qt::DisplayRole)const;
            QVariant headerData( int section, Qt::Orientation orientation, int role = Qt :: DisplayRole) const;
        private :
            Cell * cell_;
    };



}


#endif //INSTANCES_MODEL_H