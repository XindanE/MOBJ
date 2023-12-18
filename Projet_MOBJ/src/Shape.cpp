#include "Shape.h"
#include "Symbol.h"
#include "Term.h"
#include "Cell.h"
#include "Indentation.h"
#include "Net.h"                                     
#include "Instance.h"                                    
#include "Point.h"                                   
#include "Node.h"
#include "XmlUtil.h"                                                                        
#include "Box.h"                                    
#include "Line.h"   
using namespace std;
namespace Netlist {
        Shape::Shape(Symbol* s):owner_(s) {owner_->add(this);}
        Shape::~Shape() {owner_->remove(this);}
        Box Shape::getBoundingBox()const {return Box();}

        Shape* Shape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
        {
        // Factory-like method.
        const xmlChar* boxTag
            = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
        const xmlChar* ellipseTag
            = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
        const xmlChar* arcTag
            = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
        const xmlChar* lineTag
            = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
        const xmlChar* termTag
            = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
        const xmlChar* nodeName
            = xmlTextReaderConstLocalName( reader );

        Shape* shape = NULL;
        if (boxTag == nodeName)
            shape = BoxShape::fromXml( owner, reader );
        if (ellipseTag == nodeName)
            shape = EllipseShape::fromXml( owner, reader );
        if (arcTag == nodeName)
            shape = ArcShape::fromXml( owner, reader );
        if (lineTag == nodeName)
            shape = LineShape::fromXml( owner, reader );
        if (termTag == nodeName)
            shape = TermShape::fromXml( owner, reader );

        if (shape == NULL)
            cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
                << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;

        return shape;
	}

    //BoxShape
 	BoxShape :: BoxShape ( Symbol * owner , const Box & box ): Shape ( owner ) , box_ ( box ){}
    BoxShape :: BoxShape ( Symbol * owner , int x1, int y1, int x2, int y2 ): Shape ( owner ) , box_ (x1,y1,x2,y2){}
   
    BoxShape ::~ BoxShape (){}
    Box BoxShape :: getBoundingBox () const{
        return box_ ;
    }

    void  BoxShape::toXml (std::ostream& stream ) const
    {
    stream << indent << "<box x1=\"" << box_.getX1()
                     << "\" y1=\"" << box_.getY1() 
                     << "\" x2=\"" << box_.getX2()
                     << "\" y2=\"" << box_.getY2() << "\"/>\n";
    }
    
    BoxShape* BoxShape::fromXml (Symbol* owner, xmlTextReaderPtr reader) 
    {
    std::string x1_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
    std::string y1_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
    std::string x2_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
    std::string y2_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
    const xmlChar* nodeTag  = xmlTextReaderConstString                  ( reader, (const xmlChar*)"box" );
    const xmlChar* nodeName = xmlTextReaderConstLocalName               ( reader );
    if (nodeTag == nodeName){
      if(x1_str.empty() || y1_str.empty() || x2_str.empty() || y2_str.empty()) return NULL;
      else{
        int x1 = atoi(x1_str.c_str());
        int y1 = atoi(y1_str.c_str());
        int x2 = atoi(x2_str.c_str());
        int y2 = atoi(y2_str.c_str());
        BoxShape* Nbox = new BoxShape(owner,x1,y1,x2,y2);
        return Nbox;
      }
    }
     return NULL; 
    }

    //LineShape
	  LineShape :: LineShape (Symbol* owner, int x1, int y1, int x2, int y2)
            : Shape ( owner ) , x1_ ( x1 ), y1_ ( y1 ), x2_ ( x2 ), y2_ ( y2 ){}
    
    LineShape ::~LineShape (){}

    Box LineShape :: getBoundingBox () const{
        return Box(min(x1_,x2_),min(y1_,y2_),max(x1_,x2_),max(y1_,y2_)) ;
    }
   
    void  LineShape::toXml (std::ostream& stream ) const
    {
    stream << indent << "<line x1=\"" << getBoundingBox().getX1()
                     <<    "\" y1=\"" << getBoundingBox().getY1() 
                     <<    "\" x2=\"" << getBoundingBox().getX2()
                     <<    "\" y2=\"" << getBoundingBox().getY2() << "\"/>" << std::endl;
    }
    LineShape* LineShape::fromXml (Symbol* owner, xmlTextReaderPtr reader) 
    {
    std::string x1_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
    std::string y1_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
    std::string x2_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
    std::string y2_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
    const xmlChar* nodeTag  = xmlTextReaderConstString                  ( reader, (const xmlChar*)"line" );
    const xmlChar* nodeName = xmlTextReaderConstLocalName               ( reader );
    if(nodeTag == nodeName){
      if(x1_str.empty() or y1_str.empty() or x2_str.empty() or y2_str.empty()) return NULL;
      else{
        int x1 = atoi(x1_str.c_str());
        int y1 = atoi(y1_str.c_str());
        int x2 = atoi(x2_str.c_str());
        int y2 = atoi(y2_str.c_str());
        LineShape* Nline = new LineShape(owner,x1,y1,x2,y2);
        return Nline;
      }
    }
     return NULL; 
    }
    
    
    //EllipseShape
    EllipseShape::EllipseShape (Symbol * owner, const Box & box): Shape(owner), box_(box){}
    EllipseShape::EllipseShape (Symbol * owner, int x1, int y1, int x2, int y2): Shape(owner), box_(x1,y1,x2,y2){}
    EllipseShape::~EllipseShape (){}
    Box EllipseShape::getBoundingBox () const{
        return box_;
    }
    void EllipseShape::toXml (std::ostream& stream ) const
    {
    stream << indent << "<ellipse x1=\"" << box_.getX1()
                     <<    "\" y1=\"" << box_.getY1() 
                     <<    "\" x2=\"" << box_.getX2()
                     <<    "\" y2=\"" << box_.getY2() << "\"/>" << std::endl;
    }
    EllipseShape* EllipseShape::fromXml (Symbol* owner, xmlTextReaderPtr reader) 
    {
        const xmlChar* nodeTag  = xmlTextReaderConstString                  ( reader, (const xmlChar*)"ellipse" );
        const xmlChar* nodeName = xmlTextReaderConstLocalName               ( reader );
        std::string x1_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
        std::string y1_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
        std::string x2_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
        std::string y2_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
        if (nodeTag == nodeName){
        if(x1_str.empty() or y1_str.empty() or x2_str.empty() or y2_str.empty()) return NULL;
        else{
          int x1 = atoi(x1_str.c_str());
          int y1 = atoi(y1_str.c_str());
          int x2 = atoi(x2_str.c_str());
          int y2 = atoi(y2_str.c_str());
          EllipseShape* Nell = new EllipseShape(owner,x1,y1,x2,y2);
          return Nell;
        }
        }
         return NULL; 
    }
    //ArcShape
    ArcShape::ArcShape (Symbol* owner, int start, int span, const Box& box): Shape(owner),start_(start),span_(span), box_(box){

    }
    ArcShape::ArcShape (Symbol * owner, int start_, int span_, int x1, int y1, int x2, int y2): Shape(owner), start_(start_), span_(span_), box_(x1,y1,x2,y2){

    }
    ArcShape::~ArcShape (){

    }
    Box ArcShape::getBoundingBox () const{
        return box_;
    }
    void  ArcShape::toXml (std::ostream& stream ) const
    {
    stream << indent << "<arc x1=\"" << box_.getX1()
                     <<    "\" y1=\"" << box_.getY1() 
                     <<    "\" x2=\"" << box_.getX2()
                     <<    "\" y2=\"" << box_.getY2()
                     <<    "\" start=\"" << getStart() 
                     <<    "\" span=\"" << getSpan() << "\"/>" << std::endl;
    }
    ArcShape* ArcShape::fromXml (Symbol* owner, xmlTextReaderPtr reader) 
    {
      std::string x1_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
      std::string y1_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
      std::string x2_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
      std::string y2_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
      std::string start_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"start"));
      std::string span_str = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"span"));
      const xmlChar* nodeTag  = xmlTextReaderConstString                  ( reader, (const xmlChar*)"arc" );
      const xmlChar* nodeName = xmlTextReaderConstLocalName               ( reader );
      if (nodeTag == nodeName){
          if(x1_str.empty() or y1_str.empty() or x2_str.empty() or y2_str.empty() or start_str.empty() or span_str.empty()) return NULL;
          else{
            int x1 = atoi(x1_str.c_str());
            int y1 = atoi(y1_str.c_str());
            int x2 = atoi(x2_str.c_str());
            int y2 = atoi(y2_str.c_str());
            int start = atoi(start_str.c_str());
            int span = atoi(span_str.c_str());
            ArcShape* Narc = new ArcShape(owner,start,span,x1,y1,x2,y2);
            return Narc;
          }
      }  
       return NULL;   
    }
    // TermShape
    TermShape::TermShape( Symbol* owner,  string name, int x1, int y1, NameAlign align)
        :Shape(owner)
        ,term_(NULL)
        ,x1_(x1)
        ,y1_(y1)
        ,align_(align)
        {
            term_ = owner->getCell()->getTerm(name);
    }                                                                                                                                                                                   
    
    TermShape::~TermShape(){}

    
    inline Box TermShape::getBoundingBox() const{
        return Box(x1_,y1_,x1_,y1_);
    }
    std::string TermShape::Aligntostr(NameAlign al){
	    if      (al == TopLeft)       return "TopLeft";
        else if (al == TopRight)      return "TopRight";
        else if (al == BottomLeft)    return "BottomLeft";
        else if (al == BottomRight )  return "BottomRight ";
        else {
            cerr << "toString erreur" <<endl;
            exit(1);
        } 
	}

	TermShape::NameAlign TermShape::Strtoalign(std::string str){
	    if      (str == "top_left")        return TopLeft;
        else if (str == "top_right")       return TopRight;
        else if (str == "bottom_left")     return BottomLeft;
        else if (str == "bottom_right")    return BottomRight;
        else {
            cerr << "toAlign erreur" <<endl;
            exit(1);
        } 
	}
    void  TermShape::toXml (std::ostream& stream ) const
	{
	    stream << indent << "<term name=\"" << getTerm()->getName()
		             <<    "\" x1=\"" << x1_
		             <<    "\" y1=\"" << y1_
		             << "\" align=\"" << Aligntostr(align_) << "\"/>" << std::endl;
	}
    TermShape* TermShape::fromXml( Symbol* owner, xmlTextReaderPtr reader ){
        string name             = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name"    ) );
        string x_str               = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1"      ) );
        string y_str               = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1"      ) );
        string align_str            = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"align"   ) );
        const xmlChar* nodeTag  = xmlTextReaderConstString                  ( reader, (const xmlChar*)"term" );
        const xmlChar* nodeName = xmlTextReaderConstLocalName               ( reader );
        if (nodeTag == nodeName){
         if(name.empty() || align_str.empty() || x_str.empty() || y_str.empty()) return NULL;
         else{
            NameAlign align = Strtoalign( align_str );
            int x = atoi(x_str.c_str());
            int y = atoi(y_str.c_str());
            TermShape* Ntshape = new TermShape(owner, name, x, y, align);
            return Ntshape;
         }
      }	 
      return NULL; 
    }
}// Netlist