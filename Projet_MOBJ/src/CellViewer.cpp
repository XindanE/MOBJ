#include "Cell.h"
#include <QMenuBar>
#include <QResizeEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QApplication>
#include "CellsLib.h" 
#include "CellViewer.h" 
#include "OpenCellDialog.h" 
#include "SaveCellDialog.h"
#include "InstancesWidget.h"
#include "CellWidget.h" 
using namespace std;

namespace Netlist {
   
    CellViewer::CellViewer(QWidget *parent)
                :QMainWindow(parent)
                ,cellWidget_(NULL)
                ,saveCellDialog_(NULL)
    {
        cellWidget_ = new CellWidget();
        saveCellDialog_ = new SaveCellDialog(this);
        setCentralWidget(cellWidget_);
        QMenu *fileMenu = menuBar()->addMenu("&File");
        
        QAction *action = new QAction("&Save As", this); //save
        action->setStatusTip("Save to disk(rename) the Cell");
        action->setShortcut(QKeySequence("CTRL+S"));
        action->setVisible(true);
        fileMenu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(saveCell()));
        
        action = new QAction("&Quit", this); //quit
        action->setStatusTip("Exit the Netlist Viewer");
        action->setShortcut(QKeySequence("CTRL+Q"));
        action->setVisible(true);
        fileMenu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(close()));
        
        action = new QAction("&Open Cell", this); //Open Cell
        action->setStatusTip("Open a Cell Netlist Viewer");
        action->setShortcut(QKeySequence("CTRL+O"));
        action->setVisible(true);
        fileMenu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(openCell()));
        
        instancesWidget_ = new InstancesWidget();
        instancesWidget_->setCellViewer(this);
        action = new QAction("&Open Instance List", this); //Open instance list
        action->setStatusTip("Open the Instance List of the Cell");
        action->setShortcut(QKeySequence("CTRL+I"));
        action->setVisible(true);
        fileMenu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(showInstancesWidget()));
        
        cellsLib_ = new CellsLib();
        cellsLib_->setCellViewer(this);
        action = new QAction("&Open Cell List", this); //Open Cell list
        action->setStatusTip("Open the CellsLib");
        action->setShortcut(QKeySequence("CTRL+C"));
        action->setVisible(true);
        fileMenu->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(showCellsLib()));
        emit cellLoaded();
    }

    CellViewer::~CellViewer(){}

    Cell* CellViewer::getCell()const{
        return cellWidget_->getCell();
    }

    CellsLib* CellViewer::getCellsLib(){
        return cellsLib_;
    }

    void CellViewer::setCell( Cell* c)
    {
        cellWidget_->setCell( c );
        instancesWidget_->setCell( c );
        cellsLib_->getBaseModel()->setCell(c);
    }

    void CellViewer::saveCell()
    {
        Cell* cell = getCell ();
        if ( cell == NULL ) return ;
        QString cellName = cell->getName().c_str();
        if ( saveCellDialog_->run( cellName )) {
            cell->setName ( cellName.toStdString() );
            cell->save ( cellName.toStdString() );
        }
    }

    void CellViewer::openCell()
    {
        Cell* cell = getCell ();
        if ( cell == NULL )
            return ;
        QString cellName = cell->getName ().c_str ();
        if ( OpenCellDialog::run( cellName )) {
            cell =Cell::find(cellName.toStdString());
            if (!(cell))
                cell->load ( cellName.toStdString () );
           }
        setCell(cell);
    }

    void CellViewer::showCellsLib()
    {
        cellsLib_->show();
    }

    void CellViewer::showInstancesWidget()
    {
        instancesWidget_->show();
    }

}// Netlist