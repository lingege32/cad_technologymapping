#include"tm.h"
#include<queue>
void c_tm::parsecell(const char* p)
{
    ifstream fin;
    fin.open(p);
    int n_in;           //how many input
    bool change=0;      //change to next gate?
    char buffer[80];    //buffer
    char optgate[32];   //aoi22 ...etc
    char gate[32];      //NAND NOT
    c_node* final;
    char tmp[32];
    char tmp_out[32];
    char tmp_in1[32];
    char tmp_in2[32];
    string s_optgate,s_gate,s_out,s_in1,s_in2;
    int delay=0;          //delay time
    while(!fin.eof()){
        vector<c_node* > here;
        change=0;
        fin.getline(buffer,80);
        sscanf(buffer,"%s %d",optgate,&n_in);   //"NOR4 4" in buffer
        s_optgate.assign(optgate);
        fin.getline(buffer,80);                 //delay time
        delay=atoi(buffer);
        for(int x=1;x<=n_in;x++){                //construct input node
            sprintf(gate,"%d",x);
            s_gate.assign(gate);
            c_node* r;
            r= new c_node(s_gate,"signal",1);
            r->set_optgate(s_optgate);
            r->set_delay(delay);
            here.push_back(r);
        }
       
        while(1){
            fin.getline(buffer,80);
            if(buffer[0]=='\0' || buffer[0]=='\r' || fin.eof()) {
                if(final->get_gatename()=="NAND")
                    m_vcellnand.push_back(final);
                else
                    m_vcellnot.push_back(final);    
                break;
            }
           
            sscanf(buffer,"%[^ ] = %[^(]%s",tmp_out,gate,tmp);
            s_gate.assign(gate);
            s_out.assign(tmp_out);
            c_node* r;
            r=new c_node(s_out,s_gate,1);
            if(s_gate=="NAND"){
                sscanf(tmp,"(%[^,],%[^)]",tmp_in1,tmp_in2);
                s_in1.assign(tmp_in1);
                s_in2.assign(tmp_in2);
                for(unsigned int x=0,z=0;z<2;x++){
                    if(s_in1==here[x]->getname()||s_in2==here[x]->getname()){
                        here[x]->set_outnode(r);
                        r->set_innode(here[x]);
                        z++;
                    }
                }
            }
            else if(s_gate=="NOT"){
                sscanf(tmp,"(%[^)]",tmp_in1);
                s_in1.assign(tmp_in1);
                for(unsigned int x=0;x<here.size();x++){
                    if(s_in1==here[x]->getname()){
                        here[x]->set_outnode(r);
                        r->set_innode(here[x]);
                        break;
                    }
                }
            }
            r->set_optgate(s_optgate);
            r->set_delay(delay);
            final=r;
            here.push_back(r);
        }
    }
    
    fin.close();
}
c_tm::c_tm(parser* p)
{
    vector<node_s*> n;
    vector<string> sig;
    p->getnode(n);
    p->getsignal(sig);
    for(unsigned int i=0;i< sig.size();i++){
        c_node* r;
        r=new c_node(sig[i],"signal",1);
        m_vnode.push_back(r);
    }
    for(unsigned int x=0;x < n.size();x++){
        for(unsigned int y=0;y<m_vnode.size();y++){
            if(m_vnode[y]->getname()==n[x]->m_trg)
            {
                delete m_vnode[y];
                m_vnode.erase(m_vnode.begin()+y);
                y--;
            }
        }
        c_node* r;
        r=new c_node(n[x]-> m_trg,n[x]->m_name,0);
        r->set_delay(2147483647);
        if(n[x]->m_name=="NAND"){
            for(int c=0,z=0;z<2;c++){
                if(m_vnode[c]->getname()==n[x]->m_src1 || m_vnode[c]->getname()==n[x]->m_src2){
                    z++;
                    m_vnode[c]->set_outnode(r);
                    r->set_innode(m_vnode[c]);
                }
            }
        }
        else if(n[x]->m_name=="NOT"){
            for(unsigned c=0;c<m_vnode.size();c++){
                if(m_vnode[c]->getname()==n[x]->m_src1){
                    r->set_innode(m_vnode[c]);
                    m_vnode[c]->set_outnode(r);
                    break;
                }
            }
        }
        m_vnode.push_back(r);
    }
}
void c_tm::dump()
{
    for(unsigned int x=0;x<m_vnode.size();x++){
        if(m_vnode[x]->outsize()>0)
            for(unsigned int y=0;y<m_vnode[x]->outsize();y++)
            cout<<m_vnode[x]->getname()<<" "<<m_vnode[x]->get_out(y)->getname()<<endl;
    }
    cout<<"//--------------------------------------------//"<<endl;
    for(unsigned int x=0;x<m_vnode.size();x++){
        if(m_vnode[x]->insize()>0)
            for(unsigned int y=0;y<m_vnode[x]->insize();y++)
            cout<<m_vnode[x]->getname()<<" "<<m_vnode[x]->get_in(y)->getname()<<endl;
    }
    cout<<"//--------------------------------------------//"<<endl;
    for(unsigned int x=0;x<m_vcellnand.size();x++){
        cout<<m_vcellnand[x]->getoptname()<<" ";
    }
    cout<<endl;
    for(unsigned int x=0;x<m_vcellnot.size();x++)
        cout<<m_vcellnot[x]->getoptname()<<" ";
    cout<<endl;
    cout<<"//--------------------------------------------//"<<endl;
    for(unsigned int x=0;x<m_vcellnand.size();x++){
        queue<c_node* >myqueue;
        myqueue.push(m_vcellnand[x]);
        while(!myqueue.empty()){
            for(unsigned int x=0;x<myqueue.front()->insize();x++){
                myqueue.push(myqueue.front()->get_in(x));
            }
            cout<<"("<<myqueue.front()->getname()<<","<<myqueue.front()->get_gatename()<<","<<myqueue.front()->get_delay()<<")->";
            myqueue.pop();
        }
            cout<<endl;
    } 
    for(unsigned int x=0;x<m_vcellnot.size();x++){
        queue<c_node* >myqueue;
        myqueue.push(m_vcellnot[x]);
        while(!myqueue.empty()){
            for(unsigned int x=0;x<myqueue.front()->insize();x++){
                myqueue.push(myqueue.front()->get_in(x));
            }
            cout<<"("<<myqueue.front()->getname()<<","<<myqueue.front()->get_gatename()<<")->";
            myqueue.pop();
        }
            cout<<endl;
    } 
    for(unsigned int x=0;x<m_vtotalout.size();x++){
        cout<<"("<<m_vtotalout[x]->getname()<<","<<m_vtotalout[x]->get_gatename()<<")"<<endl;
    }
    cout<<"//--------------------------------------------//"<<endl;
    cout<<"total is " <<m_vtotalout[0]->getname()<<endl;
    cout<<m_vtotalout[0]->get_in(0)->get_gatename()<<" "<<endl<<m_vtotalout[0]->get_in(1)->get_gatename()<<endl;

    for(unsigned int x=0;x<m_vcellnot.size();x++){
        vector<c_node* >in;
        vector<string >optimal;
        if(matchtree(m_vtotalout[0],m_vcellnot[x],in,optimal))
            cout<<m_vcellnot[x]->getoptname()<<" 1"<<endl;
        else
            cout<<m_vcellnot[x]->getoptname()<<" 0"<<endl;
    }
    for(unsigned int x=0;x<m_vcellnand.size();x++){
        vector<c_node* >in;
        vector<string >optimal;
        if(matchtree(m_vtotalout[0],m_vcellnand[x],in,optimal))
            cout<<m_vcellnand[x]->getoptname()<<" 1"<<endl;
        else
            cout<<m_vcellnand[x]->getoptname()<<" 0"<<endl;
    }
}
void c_tm::initial()
{
    total_out=0; 
    for(unsigned int x=0;x<m_vnode.size();x++){
        if(!m_vnode[x]->outsize() && m_vnode[x]->insize()){
            m_vtotalout.push_back(m_vnode[x]);
            total_out++;
        }    
    
    }
    for(unsigned int x=0;x<m_vnode.size();x++){
        if((m_vnode[x]->outsize() > 1) && (m_vnode[x]->insize()!=0))
        {
            c_node* r;
            r=new c_node(m_vnode[x]->getname(),"signal",1);
            for(unsigned int y=0;y<m_vnode[x]->outsize();y++)
            {
                c_node* tmp;
                vector<c_node*> tmp_v;
                tmp=m_vnode[x]->get_out(y);
                tmp->get_vinnode(tmp_v);
                tmp->clearin();
                for(unsigned int z=0;z<tmp_v.size();z++){
                    if(tmp_v[z]==m_vnode[x])
                        tmp->set_innode(r);
                    else
                        tmp->set_innode(tmp_v[z]);

                }
                r->set_outnode(m_vnode[x]->get_out(y));
            }
            m_vtotalout.push_back(m_vnode[x]);
            m_vnode[x]->clearout();
            m_vnode.push_back(r);
        }
    }
}
bool c_tm::matchtree(c_node* p_node,c_node* p_cell,vector<c_node*>& p_input,vector<string>& p_innum)
{
    if(p_cell->get_gatename()=="signal")
    {
        p_input.push_back(p_node);                  //if this is signal,then push input and inputnumber
        p_innum.push_back(p_cell->getoptname());
        return 1;
    }
    else if(p_cell->get_gatename()==p_node->get_gatename())
    {
        if(p_node->get_gatename()=="NOT"){
            return matchtree(p_node->get_in(0),p_cell->get_in(0),p_input,p_innum);          //return if there children tree is match
        }
        else{                                                                               //the condition of NAND
            if(p_cell->get_in(0)->get_gatename()!=p_cell->get_in(1)->get_gatename()){
                if(p_node->get_in(0)->get_gatename()==p_node->get_in(1)->get_gatename())                //if children of cell and node different return 0
                    return 0;
                else{
                    if(p_node->get_in(0)->get_gatename()==p_cell->get_in(0)->get_gatename())           //the condition of two different gate of a NAND gate
                        return matchtree(p_node->get_in(0),p_cell->get_in(0),p_input,p_innum) & matchtree(p_node->get_in(1),p_cell->get_in(1),p_input,p_innum);
                    else
                        return matchtree(p_node->get_in(0),p_cell->get_in(1),p_input,p_innum) & matchtree(p_node->get_in(1),p_cell->get_in(0),p_input,p_innum);
                }
            }
            else{
                if(matchtree(p_node->get_in(0),p_cell->get_in(0),p_input,p_innum) && matchtree(p_node->get_in(1),p_cell->get_in(1),p_input,p_innum))
                    return 1;
                else if(matchtree(p_node->get_in(0),p_cell->get_in(1),p_input,p_innum) && matchtree(p_node->get_in(1),p_cell->get_in(0),p_input,p_innum))
                    return 1;
                else return 0;
            }
        }
    }
    else
        return 0;
}
/*
int c_tm::partition(c_node* p){
    if(p->is_min()){
        return p->get_delay();
    }
    else{
        vector<c_node*> tmp;
        if(p->get_gatename()=="NAND"){
            tmp=m_vcellnand;
        }  
        else{
            tmp=m_vcellnot;
        }
   }
}*/
void c_tm::mapping()
{

}
