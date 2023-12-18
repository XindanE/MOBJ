#include "InstancesWidget.h"
#include "InstancesModel.h"
#include "Cell.h"
#include "Instance.h"
#include <QMenuBar>
#include <QResizeEvent>
#include <QFont>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "SaveCellDialog.h"
#include "OpenCellDialog.h"
#include <QWidget>
#include <QApplication>
#include <QHeaderView>
#include "CellWidget.h"
#include "CellsLib.h"
#include "CellViewer.h"





using namespace std;

namespace Netlist
{
    InstancesWidget::InstancesWidget(QWidget *parent)
    :QWidget(parent), cellViewer_( NULL ), baseModel_(new InstancesModel(this)), view_(new QTableView(this)), load_(new QPushButton(this))
    {
        setAttribute(Qt::WA_QuitOnClose , false );
        setAttribute(Qt::WA_DeleteOnClose , false );
        setContextMenuPolicy(Qt::ActionsContextMenu);
        view_->setShowGrid(false);
        view_->setAlternatingRowColors(true);
        view_->setSelectionBehavior(QAbstractItemView::SelectRows);
        view_->setSelectionMode(QAbstractItemView::SingleSelection);
        view_->setSortingEnabled(true);
        view_->setModel(baseModel_);
        QHeaderView *horizontalHeader = view_->horizontalHeader();
        horizontalHeader->setDefaultAlignment(Qt::AlignHCenter);
        horizontalHeader->setMinimumSectionSize(300);
        horizontalHeader->setStretchLastSection(true);
        QHeaderView * verticalHeader = view_->verticalHeader();
        verticalHeader->setVisible( false );
        load_->setText("Load");
        connect (load_, SIGNAL(clicked()), this, SLOT(load()));

        QHBoxLayout * hLayout = new QHBoxLayout();
        hLayout->addStretch();
        hLayout->addWidget( load_ );
        hLayout->addStretch();
        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->addWidget( view_ );
        vLayout->addLayout( hLayout );
        setLayout( vLayout );
    }

    void InstancesWidget::setCellViewer(CellViewer* cv)
    {
        cellViewer_ = cv;
    }

    int InstancesWidget::getSelectedRow() const
    {
        QModelIndexList selecteds = view_->selectionModel()->selection().indexes();
        if (selecteds.empty())return -1;
        return selecteds.first().row();
    }

    void InstancesWidget::load()
    {
        int selectedRow = getSelectedRow();
        if ( selectedRow < 0) return;
        cellViewer_->setCell(baseModel_->getModel(selectedRow));
    }
}