// -*- explicit-buffer-name: "Main.cpp<M1-MOBJ/8-10>" -*-

#include <QApplication>
#include <QtWidgets>
#include "CellViewer.h"
#include "Cell.h"

using namespace std;
using namespace Netlist;

  int main( int argc, char* argv[]) {
    cout << "Chargement des modeles..." << endl;
    Cell::load( "vdd" );
    Cell::load( "gnd" );
    Cell::load( "TransistorN" );
    Cell::load( "TransistorP" );
    Cell::load( "or2" );
    Cell::load( "and2" );
    Cell::load( "xor2" );
    Cell* halfadder = Cell::load( "halfadder" );
    Cell::load( "fulladder" );

    QApplication* qa = new QApplication ( argc , argv );
    CellViewer* viewer = new CellViewer();
    viewer->setCell( halfadder );
    viewer->show();
    int rvalue = qa->exec();
    delete qa;
    return rvalue;
 }
