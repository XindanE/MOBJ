// -*- explicit-buffer-name: "Net.h<M1-MOBJ/7>" -*-

#ifndef NETLIST_NET_H
#define NETLIST_NET_H

#include <iostream>
#include <vector>
#include "Term.h"
#include <fstream>
#include "XmlUtil.h"
#include <libxml/xmlreader.h>

namespace Netlist {
    //class Net;
    class Node;
    class Cell;
    class Line;
    class Net{ 
        public:
                        Net( Cell*, const std::string&, Term::Type );
                        ~Net();
                inline  Cell*                       getCell () const;
                inline  const std::string&          getName () const;
                inline  unsigned int                getId   () const;
                inline  Term::Type                  getType () const;
                inline  const std::vector<Node*>&   getNodes() const;
                inline  Node*                       getNode ( size_t ) const;
                inline  const std::vector<Line*>&   getLines() const;
                        size_t                      getFreeNodeId ()const;// premier index libre dans le tableau 
                        void                        add ( Line* line );
                        void                        add    ( Node* );
                        bool                        remove ( Line* line );
                        bool                        remove ( Node* );
                        void                        toXml ( std::ostream& )const;
                static  Net*                        fromXml(Cell*, xmlTextReaderPtr);

        private: 
            Cell*               owner_;
            std::string         name_;
            unsigned int        id_;
            Term::Type          type_;  //external si commadé a au moins un term External vector<Node*> nodes_;
            std::vector<Node*>  nodes_;
            std::vector<Line*>  lines_;
        } ;
        inline  Cell*                       Net::getCell () const{return owner_;}
        inline  const std::string&          Net::getName () const{return name_;}
        inline  unsigned int                Net::getId   () const{return id_;}
        inline  Term::Type                  Net::getType () const{return type_;}
        inline  const std::vector<Node*>&   Net::getNodes() const{return nodes_;}
        inline  Node*                       Net::getNode ( size_t id ) const{return nodes_[id];}
        inline  const std::vector<Line*>&   Net::getLines() const{ return lines_; } 
        
}  // Netlist namespace.

#endif  // NETLIST_NET_H