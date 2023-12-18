// -*- explicit-buffer-name: "Term.h<M1-MOBJ/7>" -*-

#ifndef NETLIST_TERM_H
#define NETLIST_TERM_H

#include <iostream>
#include "Indentation.h"
#include "Node.h"
#include <fstream>
#include "XmlUtil.h"
#include <libxml/xmlreader.h>

namespace Netlist {

    //class Term;
    class Net;
    //class Node;
    class Instance;
    class Cell;
    class Point;
    class Indentation;
    class Term{ //Term::Internal
        public: 
            enum Type      { Internal=1, External=2};
            enum Direction { In=1, Out=2, Inout=3, Tristate=4, Transcv=5, Unknown=6 };
        public: 
            Term    ( Cell*    , const std::string& name, Direction );
            Term    ( Instance*, const Term* modelTerm );
            ~Term   ();
            inline  bool               isInternal   () const;
            inline  bool               isExternal   () const;
            inline  const std::string& getName      () const;
            inline  NodeTerm*          getNode      ();
            inline  Net*               getNet       () const;
            inline  Cell*              getCell      () const;
                    Cell*              getOwnerCell () const;
            inline  Instance*          getInstance  () const;
            inline  Direction          getDirection () const;
            inline  Point              getPosition  () const;
            inline  Type               getType      () const;
            inline  void    setDirection ( Direction );
                    void    setPosition  ( const Point& );
                    void    setPosition  ( int x, int y );
                    void    setNet(Net*); 
                    void    setNet(const std::string& name);// surcharge
                    static  std::string  toString    ( Type );
                    static  std::string  toString    ( Direction );
                    static  Direction    toDirection ( std::string );
                    static  Type         toType      ( std::string );
                    void    toXml ( std::ostream& )const;
            static  Term*   fromXml(Cell*, xmlTextReaderPtr);
        private: 
            void* owner_; 
            std::string name_; 
            Direction direction_; 
            Type type_; // Internal -> Instance, External -> Cell; 
            Net* net_;
            NodeTerm node_; //un objet Node"entier"    
    };
    inline bool               Term::isInternal   () const{return type_ == 1;}
    inline bool               Term::isExternal   () const{return type_ == 2;}
    inline const std::string& Term::getName      () const{return name_;}
    inline NodeTerm*          Term::getNode      () {return &(node_);}
    inline Net*               Term::getNet       () const{return net_;}
    inline Cell*              Term::getCell      () const{
        return (type_==External) ? static_cast<Cell*>(owner_):NULL;}            //si le type = 2 getCell(), return Cell et convertir le type en cell*
    inline Instance*          Term::getInstance  () const{
        return (type_==Internal) ? static_cast<Instance*>(owner_):NULL;}        //si le type = 1 getInstance(), return Instance et convertir le type en Instance*
    inline Term::Direction    Term::getDirection () const{return direction_;}
    inline Point              Term::getPosition  () const{return node_.getPosition();}
    inline Term::Type         Term::getType      () const{return type_;}
    inline void               Term::setDirection ( Direction d){direction_ = d;}
}
#endif  // NETLIST_TERM_H