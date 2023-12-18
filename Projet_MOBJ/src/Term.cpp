#include "Cell.h"
#include "Term.h"
#include "Net.h"
#include "Instance.h"
//#include "Indentation.h"
//#include "Node.h"
//#include "Point.h"

using namespace std;

namespace Netlist{
    //CTOR de Term
    Term::Term ( Cell* c, const string& name, Direction d)
        :owner_(c)
        ,name_(name)
        ,direction_(d)
        ,type_(External),
        net_(NULL),
        node_(this,Node::noid){
            c->add(this);
    }
    Term::Term( Instance* i, const Term* modelTerm )
        :owner_(i)
        ,name_(modelTerm->name_)
        ,direction_(modelTerm->direction_)
        ,type_(Internal)
        ,net_(NULL)
        ,node_(this,Node::noid){
           i->add(this);
    }
    //DTOR de Term
    Term::~Term(){
        if(this->isExternal()) getCell()->remove(this);
        if(this->isInternal()) getInstance()->remove(this);
    }
    Cell* Term::getOwnerCell() const{
        if   (isExternal())  return static_cast<Cell*>(owner_);//->Cell
        else if (isInternal()) return (static_cast<Instance*>(owner_)->getCell());//->Instance->Cell
        else exit( 1 );
    }
    void Term::setPosition( const Point& P){
        node_.setPosition(P);
        //getNode()->setPosition(P); 
    }

    void Term::setPosition( int x, int y ){
        node_.setPosition(x,y);
    }

    void Term::setNet(Net* n){
        if(n){
            if(n->getCell() != getOwnerCell()){
                cerr<<"ERROR"<<endl;
                return ;
            }
            net_ = n;
            net_->add(&node_);
        }
    } 
    void Term::setNet(const string& name){
        /*
        Cell *c = getOwnerCell();                       
        net_ = c->getNet(name);                         //mettre le nom dans le Net
        if(net_ != NULL){                               //condtion net est different de NULL    
            net_->add(&node_);                          //rajouter le node à Net
        }*/
        setNet(getOwnerCell()->getNet(name));
    }
    string Term::toString (Type t){                     
        if (t == 1)                 return "Internal";
        else if (t == 2)            return "External";
        else exit( 1 );
    }
    string Term::toString (Direction d){
        if (d == 1)                 return "In";
        else if (d == 2)            return "Out";
        else if (d == 3)            return "Inout";
        else if (d == 4)            return "Tristate";
        else if (d == 5)            return "Transcv";
        else if (d == 6)            return "Unknown";
        else exit( 1 );
    }
    Term::Direction Term::toDirection (string s){
        if ( s == "In")             return In;
        else if ( s == "Out")       return Out;
        else if ( s == "Inout")     return Inout;
        else if ( s == "Tristate")  return Tristate;
        else if ( s == "Transcv")   return Transcv;
        else if ( s == "Unknown")   return Unknown;
        else exit( 1 );
    }
    Term::Type Term::toType (string s){
        if ( s == "Internal")       return Internal;
        else if ( s == "External")  return External;
        else exit( 1 );
    }
    void  Term::toXml ( ostream& stream )const{
        stream  << indent << "<term name=\"" 
                << name_ << "\" "<<"direction=\"" 
                << toString(direction_) << "\" "
                <<"x=\"" << getPosition().getX() << "\" "
                <<"y=\"" << getPosition().getY() << "\"/>\n";
    }
    Term* Term::fromXml(Cell* c, xmlTextReaderPtr reader){
        string name = xmlCharToString(xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
        string direction = xmlCharToString(xmlTextReaderGetAttribute( reader, (const xmlChar*)"direction" ) );
        string x = xmlCharToString(xmlTextReaderGetAttribute( reader, (const xmlChar*)"x" ) );
        string y = xmlCharToString(xmlTextReaderGetAttribute( reader, (const xmlChar*)"y" ) );
        Term* term = new Term(c,name,Term::toDirection(direction)); //créer Term avec name et direction en string qu'il faut tranfermer le type à direction
        term->setPosition(atoi(x.c_str()),atoi(y.c_str()));         //pour rajouter des coordonnées
        return term;
    }
}
