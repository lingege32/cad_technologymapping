#include"tm.h"

c_tm::c_tm(parser* p)
{
    vector<node_s*> n;
    vector<string> sig;
    p->getnode(n);
    p->getsignal(sig);
    for(unsigned int i=0;i< sig.size();i++){
        c_node* r;
        r=new c_node(sig[i],"signal");
        m_vnode.push_back(r);
    }
    for(unsigned x=0;x < n.size();x++){
        c_node* r;
        r=new c_node(n[x]-> m_trg,n[x]->m_name);
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
    for(unsigned int x=0;x<m_vnode.size();x++){
        if(m_vnode[x]->insize()>0)
            for(unsigned int y=0;y<m_vnode[x]->insize();y++)
            cout<<m_vnode[x]->getname()<<" "<<m_vnode[x]->get_in(y)->getname()<<endl;
    }
}
