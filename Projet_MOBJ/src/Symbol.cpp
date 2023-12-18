#include "Indentation.h"
#include "Term.h"
#include "Net.h"                                     
#include "Instance.h"                                    
#include "Point.h"                                   
#include "Node.h"                                    
#include "Cell.h"                                    
#include "XmlUtil.h"                                     
#include "Symbol.h"
#include "Shape.h"                                
#include "Box.h"                                    
#include "Line.h"                                   
using namespace std;

namespace Netlist {
    Symbol::Symbol(Cell* cell):owner_(cell),shapes_(){}
  Symbol::~Symbol(){}

  Cell*     Symbol::getCell         () const{return owner_;}
  
  Box       Symbol::getBoundingBox  () const{
    Box bb;
    for(vector<Shape*>::const_iterator it=shapes_.begin();it!=shapes_.end();++it){
        bb.merge((*it)->getBoundingBox());
    }
    return bb;
  }

    Point Symbol::getTermPosition (Term* term) const
  {
    return term->getPosition();
    /*TermShape* tshape = getTermShape(term);
    if(tshape)
      return Point(tshape->getX(), tshape->getY());
    return Point();*/
  }
  
  TermShape* Symbol::getTermShape (Term* term) const
  {
    for (vector<Shape*>::const_iterator shape = shapes_.begin(); shape != shapes_.end(); ++shape){
            TermShape* p = dynamic_cast<TermShape*>(*shape);    //pour sélectionner les termshape, pour les autres shape on met en NULL
            if(p){
                if(term->getName() == p->getTerm()->getName()){
                    return p;
                }
            }
        }
        return NULL;
  }
  
    void Symbol::add (Shape* s)
  {
    shapes_.push_back(s);
  }

  void Symbol::remove (Shape* s)
  {
    for ( vector<Shape*>::iterator il = shapes_.begin(); il != shapes_.end() ; ++il ) {
                if (*il == s) {
                    shapes_.erase( il );
                }
            }
  }

    void Symbol::toXml (std::ostream& stream) const
  {
    stream << indent++ << "<symbol>\n";
    for(size_t i = 0; i < shapes_.size(); ++i)
    {
      shapes_[i]->toXml(stream);
    }
    stream << --indent << "</symbol>\n";
  }

  Symbol* Symbol::fromXml (Cell* c, xmlTextReaderPtr reader)
  {
    Symbol* s = c->getSymbol();
    if(s == NULL) return NULL;

    const xmlChar* SymbolTag = xmlTextReaderConstString( reader, (const xmlChar*)"symbol" );
    
    while(1)
    {
      int status = xmlTextReaderRead(reader);
      if (status != 1) {
      if (status != 0) {
        std::cerr << "[ERROR] Symbol::fromXml(): Unexpected termination of the XML parser." << std::endl;
      }
      break;
      }
      
      switch ( xmlTextReaderNodeType(reader) ) {
      case XML_READER_TYPE_COMMENT:
      case XML_READER_TYPE_WHITESPACE:
      case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
	continue;
      }
      
      const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );
      if((nodeName == SymbolTag) && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT))
	    return s;
      else if(Shape::fromXml(s, reader)) continue;

      std::cerr << "[ERROR] Symbol::fromXml(): Unknown or misplaced tag <" << nodeName
		  << "> (line: " << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
      break;
    }
        
    return s;
  }

    
}  // Netlist namespace.