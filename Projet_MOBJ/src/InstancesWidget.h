#ifndef INSTANCES_WIDGET_H
#define INSTANCES_WIDGET_H

#include "CellViewer.h"
#include "InstancesModel.h"
#include <QMainWindow>
#include <QTableView>
#include <QPushButton>

namespace Netlist
{
    class CellViewer;

    class InstancesWidget : public QWidget {
        Q_OBJECT;
        public:
            InstancesWidget ( QWidget* parent = NULL );
            void  setCellViewer   ( CellViewer* );
            int   getSelectedRow  () const;
            inline  void  setCell ( Cell* c){baseModel_->setCell(c);};
        public slots:
            void  load();
        private:
            CellViewer*     cellViewer_;
            InstancesModel* baseModel_;
            QTableView*     view_;
            QPushButton*    load_;
    };
    
}

#endif