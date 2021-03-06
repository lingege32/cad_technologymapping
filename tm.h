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
        int m_delay;                     //delay time
        bool m_min;                      //is this the min delay?
        bool m_prin;                   //is this node print?
    public:
        //constructor
        c_node(string p_name,string p_gatename,bool p_min){
            m_name=p_name; 
            m_gatename=p_gatename;
            if(p_gatename=="signal")
                m_optgatename="signal";
            m_delay=0;
            m_min=p_min;
            m_prin=p_min;
        }
        //accesser
        //min//
        bool is_min(){return m_min;}
        void set_min(bool p){m_min=p;}
        bool is_print(){return m_prin;}
        void set_print(bool p){m_prin=p;}
        //get function
        string getname(){return m_name;}
        string get_gatename(){return m_gatename;}
        void get_vinnode(vector<c_node*>& p){p=m_innode;}
        c_node* get_out(unsigned int p){return m_outnode[p];}
        c_node* get_in(unsigned int p){return m_innode[p];}
        string getoptname(){return m_optgatename;}
        int get_delay(){return m_delay;}
        c_node* get_optin(unsigned int p){return m_optin[p];}
        c_node* get_optout(unsigned int p){return m_optout[p];}
        unsigned int outsize(){return m_outnode.size();} 
        unsigned int insize(){return m_innode.size();}
        unsigned int optoutsize(){return m_optout.size();}
        unsigned int optinsize(){return m_optin.size();}
        //set function//
        void set_innode(c_node* p){m_innode.push_back(p);}
        void set_outnode(c_node* p){m_outnode.push_back(p);}
        void set_optgate(string p){m_optgatename=p;}
        void set_delay(int p){m_delay=p;}
        void set_optin(c_node* p){m_optin.push_back(p);}
        void set_optout(c_node* p){m_optout.push_back(p);}
        //clear//
        void clearout(){m_outnode.clear();}                     //if there are fan-out cut it
        void clearin(){m_innode.clear();}
        void d_opinchange(c_node*p_head,c_node* p_tail){
            for(unsigned int x=0;x<m_optin.size();x++){
                if(m_optin[x]==p_tail){
                    p_head->set_optout(this);
                    m_optin[x]=p_head;
                }
            }
        }
};
class c_tm
{
    private:
        vector<c_node* >m_vnode;
        vector<c_node* >m_vcell;            //document the cell 
        vector<c_node* >m_vtotalout;        //total circuit output 
        vector<c_node* >m_vcuthead;
        vector<c_node* >m_vcuttail;
        unsigned int total_out;
        int partition(c_node* );
        bool matchtree(c_node* ,c_node*, vector<c_node*>&, vector<string>& );
        string m_filename;
        vector<string > m_sig;
    public:
        //constructor
        c_tm(parser* );
        ~c_tm(){;}
        void parsecell(const char*);
        //accessor
        
        //function
        void dump();
        void initial();                 //cut the fan-out
        void mapping();                 //mapping all totalroot and caculate cost
        void link();                    //link the cutting fan-out
        void printinfo(const char*,const char*);              //print
};




#endif
