#include  "Cell.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"
#include  "Box.h"
#include  "Term.h"
#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include  <QLine>
#include  "CellWidget.h"



namespace Netlist {

  using namespace std;


  ostream& operator<< ( ostream& o, const QRect& rect )
  {
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
  }


  ostream& operator<< ( ostream& o, const QPoint& p )
  { o << "<QRect x:" << p.x() << " y:" << p.y() << ">"; return o; }



  CellWidget::CellWidget ( QWidget* parent )
            : QWidget(parent)
            , cell_  (NULL)
            , viewport_ ( Box (0 ,0 ,500 ,500))
  {
      setAttribute    ( Qt::WA_OpaquePaintEvent );
      setAttribute    ( Qt::WA_NoSystemBackground );
      setAttribute    ( Qt::WA_StaticContents );
      setSizePolicy   ( QSizePolicy::Expanding, QSizePolicy::Expanding ); // direction de x et y
      setFocusPolicy  ( Qt::StrongFocus );
      setMouseTracking( true );
  }
  CellWidget::~CellWidget ()
  { }


  void CellWidget::setCell ( Cell* cell )
  {
    cell_ = cell;
    repaint();
  }


  QSize  CellWidget::minimumSizeHint () const
  { return QSize(500,500); }

  void  CellWidget::resizeEvent ( QResizeEvent* event )
  { 
    const QSize &size = event->size();
    viewport_.setX2( viewport_.getX1() + size.width() );
    viewport_.setY1( viewport_.getY2() - size.height() );
    cerr << " CellWidget :: resizeEvent () viewport_ : " << viewport_ << endl ;
    repaint(); 
  }

  void CellWidget :: keyPressEvent ( QKeyEvent * event ) {
    event->ignore();
    if ( event->modifiers() & ( Qt::ControlModifier | Qt::ShiftModifier )) return ;
    switch ( event->key()) {
      case Qt :: Key_Up     : goUp    (); break ;
      case Qt :: Key_Down   : goDown  (); break ;
      case Qt :: Key_Left   : goLeft  (); break ;
      case Qt :: Key_Right  : goRight (); break ;
      default : return ;
    }
    event->accept();
  }

   void CellWidget :: goRight(){
    viewport_.translate( Point (20 ,0) );
    repaint();
  }

  void CellWidget :: goLeft(){
    viewport_.translate( Point (-20 ,0) );
    repaint();
  } 
  void CellWidget :: goUp() {
    viewport_.translate( Point (0 ,20) );
    repaint();
  }

  void CellWidget :: goDown() {
    viewport_.translate( Point (0 ,-20) );
    repaint();
  }


  void  CellWidget::paintEvent ( QPaintEvent* event )
  {
    QPainter painter(this);
    painter.setBackground( QBrush( Qt::black ) );
    painter.eraseRect    ( QRect( QPoint(0,0), size() ) );
    
    painter.setPen ( QPen ( Qt :: darkGreen , 3 ) );
    QRect rect1 = boxToScreenRect ( Box() );
    painter.drawRect ( rect1 );
    painter.setPen ( QPen ( Qt :: gray , 0 ) );
    painter.setBrush ( QBrush ( Qt :: gray ) );
    QRect rect2 ( (size().width()-100)/2, (size().height()-100)/2, 100, 100);
    //painter.drawRect(rect2);
    painter.drawText( rect2, Qt::AlignCenter, cell_->getName().c_str());

    painter.setPen ( QPen ( Qt :: red , 0 ) );
    painter.setBrush ( QBrush ( Qt :: red ) );
    query(1,painter);
    //painter.drawRect(rect2);
    
  }

  void CellWidget::query ( unsigned int flags , QPainter & painter ) {

    if (( not cell_ ) or ( not flags )) return ;
    const vector <Instance*>& instances = cell_->getInstances();
    if(instances.size() ) { 
      for ( size_t i = 0 ; i < instances.size() ; ++i ) {             
        Point           instPos = instances [ i ]->getPosition();
        const Symbol * symbol = instances [ i ]->getMasterCell()->getSymbol();
        if ( not symbol ) continue ;

        const vector <Shape*>& shapes = symbol->getShapes();
        for ( size_t j = 0 ; j < shapes.size() ; ++j ) {
          //BoxShape
          BoxShape*     boxShape      = dynamic_cast<BoxShape*>( shapes[ j ]);
          if ( boxShape ) {
            Box box = boxShape->getBoundingBox();
            QRect rect = boxToScreenRect( box.translate( instPos ));
            painter.setPen ( QPen ( Qt::darkGreen , 3 ) );
            painter.drawRect( rect );
          }

          //LineShape
          LineShape*    lineShape     = dynamic_cast<LineShape*>( shapes[ j ]);
          if ( lineShape ) {
            Point point1 = Point(lineShape->getX1(),lineShape->getY1()) ;
            Point point2 = Point(lineShape->getX2(),lineShape->getY2()) ;
            point1 = point1.translate(instPos) ;
            point2 = point2.translate(instPos) ;
            QLine line = QLine(xToScreenX(point1.getX()),yToScreenY(point1.getY()),xToScreenX(point2.getX()),yToScreenY(point2.getY()));
            painter.setPen ( QPen ( Qt::darkGreen , 3 ) );
            painter.drawLine( line );
          }
          //EllipseShape
          EllipseShape* ellipseShape  = dynamic_cast<EllipseShape*>( shapes[ j ]);
          if ( ellipseShape ) {
            Box box = ellipseShape->getBoundingBox();
            QRect ellipse = boxToScreenRect( box.translate( instPos ));
            painter.setPen ( QPen ( Qt::darkGreen , 3 ) );
            painter.setBrush ( QBrush ( Qt :: darkGreen ) );
            painter.drawEllipse( ellipse );
          }

          //ArcShape
          ArcShape*     arcShape      = dynamic_cast<ArcShape*>( shapes[ j ]);
          if ( arcShape ) {
            Box box = arcShape->getBoundingBox();
            QRect arc = boxToScreenRect( box.translate( instPos ));
            painter.setPen ( QPen ( Qt::darkGreen , 3 ) );
            painter.setBrush ( QBrush ( Qt :: darkGreen ) );
            painter.drawArc( arc , (arcShape->getStart())*16, (arcShape->getSpan())*16);
          }

          //TermShape
          TermShape*    termshape     = dynamic_cast<TermShape*>( shapes[ j ]);
          if ( termshape ) {
            Box box = termshape->getBoundingBox();
            QRect term = boxToScreenRect((box.translate( instPos )).inflate(4));
            painter.setPen ( QPen ( Qt :: red , 0 ) );
            painter.setBrush ( QBrush ( Qt :: red ) );
            painter.drawRect( term );
            QString nodeterm = termshape->getTerm()->getName().c_str();
            painter.drawText(xToScreenX(box.translate(-10,10).getX1()), yToScreenY(box.translate(-10,10).getY1()), nodeterm);
            QString inst = instances[i]->getName().c_str();
            painter.setPen ( QPen ( Qt :: darkGreen , 3 ) );
            painter.drawText(xToScreenX(instances[i]->getPosition().translate(0,-20).getX()), yToScreenY(instances[i]->getPosition().translate(0,-20).getY()), inst);
           
          }

          //Term
          for (vector<Term*>::const_iterator it = cell_->getTerms().begin() ; it != cell_->getTerms().end() ; ++it ) {
            string direction = Term::toString((*it)->getDirection());
            int x = (*it)-> getPosition().getX();
            int y = (*it)-> getPosition().getY();
            Point p1,p2,p3,p4,p5;
            if ((*it) != 0 && direction == "In"){
              p1.setX(0 + x);
              p1.setY(0 + y);
              p2.setX(-10 + x);
              p2.setY(10 + y);
              p3.setX(-20 + x);
              p3.setY(10 + y);
              p4.setX(-20 + x);
              p4.setY(-10 + y);
              p5.setX(-10 + x);
              p5.setY(-10 + y);      
            }
            if ((*it) != 0 && direction == "Out") {
              p1.setX(0 + x);
              p1.setY(0 + y);
              p2.setX(10 + x);
              p2.setY(10 + y);
              p3.setX(20 + x);
              p3.setY(10 + y);
              p4.setX(20 + x);
              p4.setY(-10 + y);
              p5.setX(10 + x);
              p5.setY(-10 + y);
            }
            //POlygon Term
            QPolygon polygon;
            polygon << pointToScreenPoint(p1) << pointToScreenPoint(p2) << pointToScreenPoint(p3) << pointToScreenPoint(p4) << pointToScreenPoint(p5) << pointToScreenPoint(p1);
            painter.setPen ( QPen ( Qt :: red , 0 ) );
            painter.setBrush ( QBrush ( Qt :: red ) );
            painter.drawPolygon(polygon);
            //Text
            QString term = (*it)->getName().c_str();
            painter.drawText(xToScreenX((*it)->getPosition().translate(10,15).getX()), yToScreenY((*it)->getPosition().translate(10,15).getY()), term);
          }
        }
      }
      
    }
    else { 
      const vector <Shape*>& shapes = cell_->getSymbol()->getShapes();
      for ( size_t j = 0 ; j < shapes.size() ; ++j ) {
        //LineShape
        LineShape*    lineShape     = dynamic_cast<LineShape*>( shapes[ j ]);
        if ( lineShape ) {
          Point point1 = Point(lineShape->getX1(),lineShape->getY1()) ;
          Point point2 = Point(lineShape->getX2(),lineShape->getY2()) ;           
          QLine line = QLine(xToScreenX(point1.getX()),yToScreenY(point1.getY()),xToScreenX(point2.getX()),yToScreenY(point2.getY()));
          painter.setPen ( QPen ( Qt::darkGreen , 3 ) );
          painter.drawLine( line );
        }


        //BoxShape
        BoxShape*     boxShape      = dynamic_cast<BoxShape*>( shapes[ j ]);
        if ( boxShape ) {
          Box box = boxShape->getBoundingBox();
          QRect rect = boxToScreenRect( (boxShape->getBoundingBox()));
          painter.setPen ( QPen ( Qt::darkGreen , 3 ) );
          painter.drawRect( rect );         
        }

        //EllipseShape
        EllipseShape* ellipseShape  = dynamic_cast<EllipseShape*>( shapes[ j ]);
        if ( ellipseShape ) {
          Box box = ellipseShape->getBoundingBox();
          QRect ellipse = boxToScreenRect( (ellipseShape->getBoundingBox()));
          painter.setPen ( QPen ( Qt::darkGreen , 3 ) );
          painter.setBrush ( QBrush ( Qt :: darkGreen ) );
          painter.drawEllipse( ellipse );
        }

        //ArcShape
        ArcShape*     arcShape      = dynamic_cast<ArcShape*>( shapes[ j ]);
        if ( arcShape ) {
          Box box = arcShape->getBoundingBox();
          QRect arc = boxToScreenRect( (arcShape->getBoundingBox()));
          painter.setPen ( QPen ( Qt::darkGreen , 3 ) );
          painter.setBrush ( QBrush ( Qt :: darkGreen ) );
          painter.drawArc( arc , (arcShape->getStart())*16 , (arcShape->getSpan())*16);
        }

        //TermShape
        TermShape*    termshape     = dynamic_cast<TermShape*>( shapes[ j ]);
        if ( termshape ) {
          Box box = termshape->getBoundingBox();
          QRect term = boxToScreenRect( (termshape->getBoundingBox().inflate(4)) );
          painter.setPen ( QPen ( Qt :: red , 0 ) );
          painter.setBrush ( QBrush ( Qt :: red ) );
          painter.drawRect( term );
        }
      }
      //
      
    }

        //Line
    for (vector<Net*>::const_iterator inet = cell_->getNets().begin() ; inet != cell_->getNets().end() ; ++inet ) {
      
      for (vector<Line*>::const_iterator iline = (*inet)->getLines().begin() ; iline != (*inet)->getLines().end() ; ++iline ){
        
        Point ps = (*iline)->getSourcePosition() ; 
        Point pt = (*iline)->getTargetPosition() ; 
                        
        QLine line1 = QLine(xToScreenX(ps.getX()),yToScreenY(ps.getY()),xToScreenX(pt.getX()),yToScreenY(pt.getY()));
        painter.setPen(QPen(Qt :: cyan , 1)) ;
        painter.drawLine(line1) ;     
      }

      for (vector<Node*>::const_iterator inode = (*inet)->getNodes().begin() ; inode != (*inet)->getNodes().end() ; ++inode ){
        
        NodePoint* nodePoint = dynamic_cast<NodePoint*>(*inode) ; 

        if(nodePoint)
        {
          int nbr = 0 ;
          for (vector<Line*>::const_iterator iline = (*inet)->getLines().begin() ; iline != (*inet)->getLines().end() ; ++iline ){
            if(nodePoint == dynamic_cast<NodePoint*>((*iline)->getSource()) || nodePoint == dynamic_cast<NodePoint*>((*iline)->getTarget()))
            {
              nbr++ ;
            }
            if(nbr > 2)
            {
              QPoint point = QPoint(xToScreenX(nodePoint->getPosition().getX()) , yToScreenY(nodePoint->getPosition().getY())) ;
              painter.setPen(QPen(Qt :: cyan , 1)) ;
              painter.setBrush ( QBrush ( Qt :: cyan ) );
              painter.drawEllipse(point,3 ,3 ) ;
            }
          }
        }
        else continue;
      } 
    } //end Line
  }
}  // Netlist namespace.
