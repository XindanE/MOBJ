
#ifndef CELLS_LIB_H
#define CELLS_LIB_H

#include "CellsModel.h"
#include "CellViewer.h"

#include <QMainWindow>
#include "InstancesModel.h"
#include <QTableView>
#include <QPushButton>

namespace Netlist
{
  class CellsLib : public QWidget {
    Q_OBJECT;
  public:
                       CellsLib       ( QWidget* parent=NULL );
           void        setCellViewer  ( CellViewer* );
           int         getSelectedRow () const;
    inline CellsModel* getBaseModel   ();

  public slots:
           void        load           ();
  private:
          CellViewer*  cellViewer_;
          CellsModel*  baseModel_;
          QTableView*  view_;
          QPushButton* load_;
  };

  inline CellsModel* CellsLib::getBaseModel(){return baseModel_;}
}
#endif