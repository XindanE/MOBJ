// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
class QPainter;
#include "Box.h"

namespace Netlist {

  class Cell;
  class NodeTerm;

  class CellWidget : public QWidget {
      Q_OBJECT;
    public:
                      CellWidget          ( QWidget* parent=NULL );
      virtual        ~CellWidget          ();
              void    setCell             ( Cell* );
      inline  Cell*   getCell             () const;
      inline  QRect   boxToScreenRect     ( const Box& ) const;
      inline  QPoint  pointToScreenPoint  ( const Point& ) const;
      inline  Box     screenRectToBox     ( const QRect& ) const;
      inline  Point   screenPointToPoint  ( const QPoint& ) const;
      inline  int     xToScreenX          ( int x ) const;
      inline  int     yToScreenY          ( int y ) const;
      inline  int     screenXTox          ( int x ) const;
      inline  int     screenYToy          ( int y ) const;
      virtual QSize   minimumSizeHint     () const;
      virtual void    resizeEvent         ( QResizeEvent* );
              void    query               ( unsigned int flags , QPainter & painter );
    protected:
      virtual void    paintEvent          ( QPaintEvent* );
      virtual void    keyPressEvent       ( QKeyEvent* );
    public slots:
              void    goUp                ();
              void    goDown              ();
              void    goLeft              ();
              void    goRight             ();
    private:
      Cell* cell_;
      Box   viewport_;
  };


  inline  Cell*   CellWidget::getCell             () const { return cell_; }
  inline  int     CellWidget::xToScreenX          ( int x ) const{ return x - viewport_.getX1();}
  inline  int     CellWidget::yToScreenY          ( int y ) const{ return viewport_.getY2() - y;}
  inline  int     CellWidget::screenXTox          ( int x ) const{ return x + viewport_.getX1();}
  inline  int     CellWidget::screenYToy          ( int y ) const{ return viewport_.getY2() - y;}
  inline  QRect   CellWidget::boxToScreenRect     ( const Box& Box) const{ return QRect(xToScreenX(Box.getX1()),yToScreenY(Box.getY2()),Box.getWidth(),Box.getHeight()); }
  inline  QPoint  CellWidget::pointToScreenPoint  ( const Point& Point) const{ return QPoint(xToScreenX(Point.getX()),yToScreenY(Point.getY()));}
  inline  Box     CellWidget::screenRectToBox     ( const QRect& QRect) const{ return Box(screenXTox(QRect.x()),screenXTox(QRect.x() + QRect.width()),screenYToy(QRect.y() - QRect.height()),screenYToy(QRect.y()));}
  inline  Point   CellWidget::screenPointToPoint  ( const QPoint& QPoint) const{ return Point(screenXTox(QPoint.x()),screenYToy(QPoint.y()));}


}  // Netlist namespace.

#endif  // NETLIST_CELL_WIDGET_H
