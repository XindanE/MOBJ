#include "CellsModel.h"
#include "Cell.h"
#include "CellWidget.h"
#include "CellViewer.h"
#include "InstancesWidget.h"
# include "SaveCellDialog.h"
# include "OpenCellDialog.h"

#include <QMenuBar>
#include <QResizeEvent>
#include <QFont>
# include <QLabel>
# include <QPushButton>
# include <QHBoxLayout>
# include <QVBoxLayout>
#include <QWidget>
#include <QApplication>



using namespace std;

namespace Netlist
{
    CellsModel::CellsModel(QObject *parent):QAbstractTableModel(parent), cell_( NULL ){}

    CellsModel::~CellsModel()
    { }

    void CellsModel::setCell(Cell* cell) 
    {
        emit layoutAboutToBeChanged();
        cell_ = cell;
        emit layoutChanged();
    }

    Cell * CellsModel::getModel( int row )
    {
        if ( not cell_ ) return NULL ;
        if ( row >= ( int )cell_->getAllCells().size())return NULL ;
        return cell_->getAllCells()[row];
    }

    int CellsModel::rowCount( const QModelIndex & parent ) const
    { return ( cell_ ) ? cell_->getAllCells().size():0;}

    int CellsModel::columnCount(const QModelIndex &parent ) const
    { return 1; }

    QVariant CellsModel::data(const QModelIndex &index, int role ) const
    {
        if ( not cell_ or not index.isValid ()) return QVariant ();
        if ( role == Qt::DisplayRole ) 
        {
            int row = index.row();
            switch(index.column())
            {
                case 0:return cell_->getAllCells()[row]->getName().c_str();
            }
        }
        return QVariant();
    }

    QVariant CellsModel::headerData ( int section, Qt::Orientation orientation, int role ) const
    {
        if ( orientation == Qt::Vertical ) return QVariant ();
        if ( role != Qt::DisplayRole ) return QVariant ();
        switch ( section ) 
        {
            case 0: return "Cell";
        }
        return QVariant();
    }

    void CellsModel::updateDatas()
    {
        emit layoutAboutToBeChanged();
        cell_ = Cell::getAllCells()[rowCount()];
        emit layoutChanged();
    }

}//End Netlist