#include "Cell.h"
#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Node.h"
#include "Point.h"
#include "Indentation.h"
#include "Symbol.h"                                    
#include "Box.h"                                    
#include "Line.h"   
#include "Shape.h"
using namespace std;
namespace Netlist {
using namespace std;
    Instance::Instance    ( Cell* owner, Cell* model, const std::string& name)
                :owner_(owner)
                ,masterCell_(model)
                ,name_(name)
                ,terms_()
                ,position_()
                {
                     for ( std::vector<Term*>::const_iterator iterm=model->getTerms().begin() ; iterm != model->getTerms().end() ; ++iterm){ //getTerms const
                        new Term(this, *iterm);
                     }
                     owner->add(this);
                }


    Instance::~Instance   (){

        while ( not terms_.empty() ) delete terms_[0];
    }

    Term*   Instance::getTerm   ( const std::string& name) const{
        for ( std::vector<Term*>::const_iterator iterm=terms_.begin() ; iterm != terms_.end() ; ++iterm) {
            if ((*iterm)->getName() == name)  return *iterm;
        }
        return NULL;
    }

    bool    Instance::connect    (const std::string& name, Net* net){
        Term* term = getTerm( name );
        if (term == NULL) return false;
        term->setNet( net );
        return true;
    }

    void  Instance::add ( Term* term){
        size_t i = 0;
        for (vector<Term*>::const_iterator iterm = getTerms().begin(); iterm != getTerms().end(); ++iterm){
            if ((*iterm) == NULL) {
                terms_[i] = term;
                return;
            }
            else i++;
        }
        terms_.push_back( term );
    }

    void  Instance::remove  ( Term* term){
        for ( std::vector<Term*>::iterator iterm=terms_.begin() ; iterm != terms_.end() ; ++iterm ) {
            if (*iterm == term) {
                //terms_.erase( iterm );
            }
        }
    }

    void  Instance::toXml ( ostream& stream )const{
        stream  << indent << "<instance name=\"" << name_ << "\" "
            <<"mastercell=\"" << getMasterCell()->getName() << "\" "
            <<"x=\"" << getPosition().getX() << "\" "
            <<"y=\"" << getPosition().getY() << "\"/>\n";
    }
    
    void Instance::setPosition(const Point& P){
         setPosition(Point(P.getX(),P.getY()));
    }

    void Instance::setPosition(int x,int y){   
        size_t i;
        int x1,y1;
        for(i = 0; i < getTerms().size(); i++){         
            Shape* s = this->getMasterCell()->getSymbol()->getTermShape(terms_[i]);
            TermShape* ts = dynamic_cast<TermShape*>(s);
            if(ts != NULL) {
                x1 = ts->getX();                        
                y1 = ts->getY();                        
            }
            terms_[i]->setPosition( x + x1, y + y1);    
        }
        position_.setX(x);                              
        position_.setY(y);
        }

       

    Instance* Instance::fromXml(Cell* c, xmlTextReaderPtr reader){
        string name = xmlCharToString(xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
        string masterCell = xmlCharToString(xmlTextReaderGetAttribute( reader, (const xmlChar*)"mastercell" ) );
        string x = xmlCharToString(xmlTextReaderGetAttribute( reader, (const xmlChar*)"x" ) );
        string y = xmlCharToString(xmlTextReaderGetAttribute( reader, (const xmlChar*)"y" ) );
        Instance* inst = new Instance(c, Cell::find(masterCell), name);   
        inst->setPosition(atoi(x.c_str()),atoi(y.c_str()));          
        return inst;
    }

}
