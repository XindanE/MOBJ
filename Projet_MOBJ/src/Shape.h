// -*- explicit-buffer-name: "Shape.h<M1-MOBJ/7>" -*-

#ifndef NETLIST_SHAPE_H
#define NETLIST_SHAPE_H

#include "Indentation.h"
#include "Term.h"
#include "Net.h"                                     
#include "Instance.h"                                    
#include "Point.h"                                   
#include "Node.h"                                    
#include "Cell.h"                                    
#include "XmlUtil.h"                                     
#include "Symbol.h"                                    
#include "Box.h"                                    
#include "Line.h"   
#include <vector>

namespace Netlist {

    class Cell;
    class Term;
    class Symbol;

    class Shape {
        public : 
                                Shape                   ( Symbol* );
            virtual            ~Shape                   ();
            inline      Symbol* getSymbol               () const;
            virtual     Box     getBoundingBox          () const = 0;
            virtual     void    toXml                   ( std::ostream& stream)const = 0;
            static      Shape*  fromXml                 ( Symbol* owner, xmlTextReaderPtr reader );
        private :
                        Symbol* owner_;

    };

    class BoxShape : public Shape{
        public:
                                        BoxShape            ( Symbol*, const Box&);
                                        BoxShape            (Symbol* owner,int x1, int y1,int x2,int y2);
                                        ~BoxShape           ();
            virtual     Box             getBoundingBox      () const;
            virtual     void            toXml               ( std::ostream& )const;
            static      BoxShape*       fromXml             ( Symbol* owner, xmlTextReaderPtr reader );
        private:
                        Box     box_;
    };
    
    class LineShape : public Shape{
        public:
                                    LineShape               ( Symbol*, int x1, int y1, int x2, int y2);
                                    ~LineShape              ();
            inline      int 	    getX1                   () const ;
            inline      int 	    getY1                   () const ;
            inline      int 	    getX2                   () const ;
            inline      int 	    getY2                   () const ;
            inline      Symbol*     getSymbol               () const;
            virtual     Box         getBoundingBox          () const;
            virtual     void        toXml                   ( std::ostream& )const;
            static      LineShape*  fromXml                 ( Symbol* owner, xmlTextReaderPtr reader );
        private:
                        int     x1_,y1_,x2_,y2_;
    };
    inline int LineShape::getX1 () const { return x1_; }
    inline int LineShape::getY1 () const { return y1_; }
    inline int LineShape::getX2 () const { return x2_; }
    inline int LineShape::getY2 () const { return y2_; }

    
    
    class EllipseShape : public Shape{
        public:
                                        EllipseShape        ( Symbol*, const Box&);
                                        EllipseShape (Symbol*, int x1, int y1, int x2, int y2);
                                        ~EllipseShape       ();
            virtual     Box             getBoundingBox      () const;
            virtual     void            toXml               ( std::ostream& )const;
            static      EllipseShape*   fromXml             ( Symbol* owner, xmlTextReaderPtr reader );
        private:
                        Box     box_;
    };

    class ArcShape : public Shape
    {    
    public:
                    ArcShape (Symbol*, int start_, int span_, const Box&);
                    ArcShape (Symbol*, int start_, int span_, int x1, int y1, int x2, int y2);
                    ~ArcShape ();
        virtual Box getBoundingBox () const;

        inline int getStart()const;
        inline int getSpan()const;
        virtual void  toXml   (std::ostream&) const;
        static ArcShape* fromXml (Symbol* owner, xmlTextReaderPtr reader);
    private:
        int start_;
        int span_;
        Box box_;
    };

    inline int ArcShape::getStart()const{return start_;}
    inline int ArcShape::getSpan()const{return span_;}
    
    class TermShape : public Shape{
        public:
            enum        NameAlign       { TopLeft, TopRight, BottomLeft, BottomRight };
        public:
                                        TermShape           ( Symbol* owner,std::string name,int x1, int y1 ,NameAlign align);
                                        ~TermShape          ();
            inline      Symbol*         getSymbol           () const;
            virtual     Box             getBoundingBox      () const;
                        Term *          getTerm () const    {return term_;};
            inline      int             getX                () const;           //récupérer la valeur x dans termshape
            inline      int             getY                () const;           //récupérer la valeur y dans termshape
            static      std::string     Aligntostr(NameAlign al) ;
            static      NameAlign       Strtoalign(std::string str);
            virtual     void            toXml               ( std::ostream& )const;
            static      TermShape*      fromXml             ( Symbol* owner, xmlTextReaderPtr reader );
        private:
                        Term* term_;
                        int x1_;
                        int y1_;
                        NameAlign align_;   
    };
    inline int TermShape::getX() const{return x1_;}
    inline int TermShape::getY() const{return y1_;}
}  // Netlist namespace.

#endif  // NETLIST_POINT_H