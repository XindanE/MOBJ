// -*- explicit-buffer-name: "Instance.h<M1-MOBJ/4-5>" -*-

#ifndef  NETLIST_INSTANCE_H
#define  NETLIST_INSTANCE_H
 
#include <string>
#include <iostream>
#include <vector>
#include "Point.h"
#include <fstream>
#include "XmlUtil.h"
#include <libxml/xmlreader.h>

namespace Netlist {
    class Term;
    class Net;
    class Node;
    //class Instance;
    class Cell;
    class Indentation;
    
    class Instance{ 
        private: 
            Cell*               owner_;
            Cell*               masterCell_;
            std::string         name_;
            std::vector<Term*>  terms_; // Duplicata du modele Point position_;
            Point               position_;
        public: 
                    Instance            ( Cell* owner, Cell* model, const std::string& s);
                    ~Instance           ();
            inline  const std::string&          getName       () const;
            inline  Cell*                       getMasterCell () const;
            inline  Cell*                       getCell       () const;
            inline  const std::vector<Term*>&   getTerms      () const;
                    Term*                       getTerm       ( const std::string& ) const;
            inline  Point                       getPosition   () const;
                    bool                        connect       ( const std::string& name, Net* n);
                    void                        add           ( Term* );
                    void                        remove        ( Term* );
                    void                        setPosition   ( const Point& );
                    void                        setPosition   ( int x, int y );
                    void                        toXml         ( std::ostream& )const;
            static  Instance*                   fromXml       (Cell*, xmlTextReaderPtr);
    };
    inline  const std::string&        Instance::getName       () const{return name_;}
    inline  Cell*                     Instance::getMasterCell () const{return masterCell_;}
    inline  Cell*                     Instance::getCell       () const{return owner_;}
    inline  const std::vector<Term*>& Instance::getTerms      () const{return terms_;}
    inline  Point                     Instance::getPosition   () const{return position_;}

}  // Netlist namespace.

#endif //  NETLIST_INSTANCE_H