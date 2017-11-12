#ifndef _TM_H_
#define _TM_H_
#include"parser.h"
class c_node
{
    private:
        string m_name;                  //signal name
        string m_gatename;              //gate name
        vector<c_node* > m_innode;       //initial input node
        vector<c_node* > m_outnode;      //initial output node
        string m_optgatename;               //after optimal name
        vector<c_node* > m_optin;        //after optimal innode
        vector<c_node* > m_optout;       //after optimal outnode
    public:
        //constructor
        c_node(string p_name,string p_gatename){
            m_name=p_name; 
            m_gatename=p_gatename;
        }

        //accesser
        string getname(){return m_name;}
        c_node* get_out(unsigned int p){return m_outnode[p];}
        c_node* get_in(unsigned int p){return m_innode[p];}
        unsigned int outsize(){return m_outnode.size();} 
        unsigned int insize(){return m_innode.size();}
        void set_innode(c_node* p){m_innode.push_back(p);}
        void set_outnode(c_node* p){m_outnode.push_back(p);}
        


};


class c_tm
{
    private:
        vector<c_node* >m_vnode;
        
    public:
        //constructor
        c_tm(parser* );
        ~c_tm(){;}
        //accessor
        
        //function
        void dump();
};




#endif
