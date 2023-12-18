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
using namespace std;
#include "Net.h"
#include "Node.h"
#include "Cell.h"
#include "Term.h"
#include "Line.h"
#include "Indentation.h"                                     
#include "Instance.h"                                    
#include "Point.h"                                                                                                         
#include "XmlUtil.h"                                     
#include "Symbol.h"                                    
#include "Box.h"                                    
#include "Line.h" 
#include <vector>
using namespace std;


namespace Netlist {
    using namespace std;
    Net::Net( Cell* cell, const string& name, Term::Type type)
    : owner_(cell)
    , name_(name)
    ,id_()
    ,type_(type)
    ,nodes_()
    {
        cell->add(this);
    }


    Net::~Net(){
        
        /*for (vector<Node*>::iterator inode = nodes_.begin(); inode != nodes_.end(); ++inode){
            if((*inode) == NULL) continue;
            (*inode)->getTerm()->setNet(NULL);
        }*/
        owner_->remove(this);
    }


    size_t  Net::getFreeNodeId () const{// premier index libre dans le tableau void add(Node*);
        size_t i=0;
        for ( vector<Node*>::const_iterator ifrnode=nodes_.begin() ; ifrnode != nodes_.end() ; ++ifrnode ) {
        if (*ifrnode == NULL)  {
            i++;
            return i;
        }
        }

        return nodes_.size();

    }

    void  Net::add  ( Node* node){// Node peut deja demende un index
        if(not node) return;
        size_t id = node->getId();
        if(id == Node::noid){
            id = getFreeNodeId();
            node ->setId(id);
        }
        if (id < nodes_.size()){
                if(nodes_[id]!=NULL){
                    cerr<<"Oversize"<<id<<endl;
                    nodes_[id]->setId(Node::noid);
                }
            nodes_[id] = node;
        }
        else {
            for(size_t i=nodes_.size();i<id;i++)
                nodes_.push_back(NULL);
            nodes_.push_back(node);
        }
    }

    void  Net::add ( Line* line ) { if (line) lines_.push_back( line ); }

    bool  Net::remove ( Line* line ){
        if (line) {
            for ( vector<Line*>::iterator il = lines_.begin()
                ; il != lines_.end() ; ++il ) {
            if (*il == line) {
                lines_.erase( il );
                return true;
            }
            }
        }
        return false;
    }

    bool Net::remove(Node* node){
        if (node == NULL) return false;
        for ( vector<Node*>::iterator inode=nodes_.begin() ; inode != nodes_.end() ; ++inode ) {
            if (*inode == node) {
                    nodes_.erase( inode );
                    return true;
            }
        }
        return false;
    }

    void  Net::toXml ( ostream& stream )const{
        stream  << indent++ <<"<net name=\"" << name_ << "\" "
                <<"type=\"" << Term::toString(type_) << "\">\n";
        //nodes
        for ( vector<Node*>::const_iterator it = nodes_.begin() ; it != nodes_.end() ; ++it ) {
            if((*it) == NULL) continue;
            (*it)->toXml( cout );
        }
        for ( vector<Line*>::const_iterator it = lines_.begin() ; it != lines_.end() ; ++it ) {
            if((*it) == NULL) continue;
            (*it)->toXml( cout );
        }
        stream  << --indent << "</net>\n";
    }

    


    Net* Net::fromXml(Cell* c, xmlTextReaderPtr reader){
        enum  State { BeginNet = 0                                    
                    , EndNet
                    };
        State state  = EndNet; // commencer par EndNet sinon il ne s'affiche pas la premiere ligne

        const xmlChar* netTag       = xmlTextReaderConstString      ( reader, (const xmlChar*)"net" );                            
        const xmlChar* nodeName     = xmlTextReaderConstLocalName   ( reader );                                                         
        
        string name  = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
	    string type     = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"type" ) );  
        
        if(name.empty() || type.empty()){
            cerr << "[ERROR] Net::fromXml(): Unknown or misplaced tag <" << nodeName
            << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
            exit(1);
        }
        
        else { 
                             
                Term::Type tterm;
                if ( type == "Internal")  tterm = Term::Internal;
                else if ( type == "External")  tterm = Term::External; 
                else return NULL;                
                Net* net = new Net( c, name, tterm);                      
                 
            while ( true ) {
                int status = xmlTextReaderRead(reader);             
                if (status != 1) {
                    if (status != 0) {
                    cerr << "[ERROR] Net::fromXml(): Unexpected termination of the XML parser." << endl;
                    }
                    break;
                }
                switch ( xmlTextReaderNodeType(reader) ) {          
                    case XML_READER_TYPE_COMMENT:
                    case XML_READER_TYPE_WHITESPACE:
                    case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                    continue;
                }

                switch ( state ) {                                  
                    case BeginNet:                                  
                        if  ((nodeName == netTag) and  (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT) ){
                                state = EndNet;                          
                                continue;
                        }
                    break;
                            
                    case EndNet:                                   
                        if ( (nodeName == netTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) ) {
                            state = BeginNet;                      
                            return net;                             
                            continue;
                        } 
                        else {
                            if (Node::fromXml(net,reader)) continue;
                            if (Line::fromXml(net,reader)) continue;
                        }   
                    break;
                    default:
                    break;
                }
            }
        }
        
        return NULL;
    }
}
