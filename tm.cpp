#include"tm.h"
#include<queue>
#include<stack>
#include<iomanip>
ifstream fin;
ofstream fout;
ofstream path;
//--------------------------------------------////--------------------------------------------//
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
                m_vcell.push_back(final);
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
///--------------------------------------------/////--------------------------------------------//
c_tm::c_tm(parser* p)
{
    vector<node_s*> n;
    p->getnode(n);
    p->getsignal(m_sig);
    for(unsigned int i=0;i< m_sig.size();i++){
        c_node* r;
        r=new c_node(m_sig[i],"signal",1);
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
            for(unsigned int c=0,z=0;z<2;c++){
                if(c==m_vnode.size())
                    c=0;
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
//--------------------------------------------////--------------------------------------------//
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
    for(unsigned int x=0;x<m_vcell.size();x++){
        cout<<m_vcell[x]->getoptname()<<" ";
    }
    cout<<"\n//--------------------------------------------//"<<endl;
    for(unsigned int x=0;x<m_vcell.size();x++){
        queue<c_node* >myqueue;
        myqueue.push(m_vcell[x]);
        while(!myqueue.empty()){
            for(unsigned int x=0;x<myqueue.front()->insize();x++){
                myqueue.push(myqueue.front()->get_in(x));
            }
            cout<<"("<<myqueue.front()->getname()<<","<<myqueue.front()->get_gatename()<<","<<myqueue.front()->get_delay()<<")->";
            myqueue.pop();
        }
            cout<<endl;
    } 
    for(unsigned int x=0;x<m_vtotalout.size();x++){
        cout<<"("<<m_vtotalout[x]->getname()<<","<<m_vtotalout[x]->get_gatename()<<")"<<endl;
    }
    for(unsigned int x=0;x<m_vcell.size();x++){
        vector<c_node* >in;
        vector<string >optimal;
        if(matchtree(m_vtotalout[0],m_vcell[x],in,optimal)){
            cout<<m_vcell[x]->getoptname()<<" 1"<<endl;
            for(unsigned int w=0;w<in.size();w++){
                cout<<in[w]->getname()<<" "<<optimal[w]<<endl;
            }
        }
        else
            cout<<m_vcell[x]->getoptname()<<" 0"<<endl;
    }
}
//--------------------------------------------////--------------------------------------------//
void c_tm::initial()
{
    total_out=0; 
    for(unsigned int x=0;x<m_vnode.size();x++){
        if(m_vnode[x]->outsize()==0 && m_vnode[x]->insize()!=0){
            m_vtotalout.push_back(m_vnode[x]);
            total_out++;
        }    
    }
    for(unsigned int x=0;x<m_vnode.size();x++){
        if((m_vnode[x]->outsize() > 1) && (m_vnode[x]->insize()!=0))
        {
            c_node* r;
            r=new c_node(m_vnode[x]->getname(),"signal",0);
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
            //m_vnode.push_back(r);
            m_vcuthead.push_back(m_vnode[x]);
            m_vcuttail.push_back(r);
        }
    }
}
//--------------------------------------------////--------------------------------------------//
bool c_tm::matchtree(c_node* p_node,c_node* p_cell,vector<c_node*>& p_input,vector<string>& p_innum)
{
    if(p_cell->insize()==0)
    {
        p_input.push_back(p_node);                  //if node of cell is signal,then push input and inputnumber
        p_innum.push_back(p_cell->getname());
        return 1;
    }
    else if(p_cell->get_gatename()==p_node->get_gatename())
    {
        if(p_node->get_gatename()=="NOT"){
            return matchtree(p_node->get_in(0),p_cell->get_in(0),p_input,p_innum);          //return if there children tree is match
        }
        else{                                                                               //the condition of NAND
            /*
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
            else{*/
            vector<c_node*> input_c1;
            vector<string> number_c1;
            vector<c_node* > input_c2;
            vector<string > number_c2;
            if(matchtree(p_node->get_in(0),p_cell->get_in(0),input_c1,number_c1) && matchtree(p_node->get_in(1),p_cell->get_in(1),input_c1,number_c1) &&  
               matchtree(p_node->get_in(0),p_cell->get_in(1),input_c2,number_c2) && matchtree(p_node->get_in(1),p_cell->get_in(0),input_c2,number_c2))
            {
                int tmpc1=0;
                int tmpc2=0;
                for(unsigned int x=0;x<input_c1.size();x++)
                {   
                    int t1;
                    t1 =partition(input_c1[x]);
                    int t2;
                    t2 =partition(input_c2[x]);
                    if(t1>tmpc1)
                        tmpc1=t1;
                    if(t2>tmpc2)
                        tmpc1=t2;
                }
                if(tmpc1>tmpc2){
                    for(unsigned int x=0;x<input_c2.size();x++){
                        p_input.push_back(input_c2[x]);
                        p_innum.push_back(number_c2[x]);
                    }
                }
                else{
                    for(unsigned int x=0;x<input_c1.size();x++){
                        p_input.push_back(input_c1[x]);
                        p_innum.push_back(number_c1[x]);
                    }
                }
                return true;

            } 
            else if(matchtree(p_node->get_in(0),p_cell->get_in(0),p_input,p_innum) && matchtree(p_node->get_in(1),p_cell->get_in(1),p_input,p_innum))
            {   
                //cout<<"("<<p_node->get_in(0)->getname()<<","<<p_cell->get_in(0)->getname()<<")"<<endl;
                //cout<<"("<<p_node->get_in(1)->getname()<<","<<p_cell->get_in(1)->getname()<<")"<<endl;
                //cout<<"("<<p_node->get_in(1)->get_in(0)->getname()<<","<<p_cell->get_in(1)->get_in(0)->getname()<<")"<<endl;
                return 1;
            }
            else if(matchtree(p_node->get_in(0),p_cell->get_in(1),p_input,p_innum) && matchtree(p_node->get_in(1),p_cell->get_in(0),p_input,p_innum))
            {
            return 1;
            }
            else return 0;
            //}
        }
    }
    else
        return 0;
}
//--------------------------------------------////--------------------------------------------//
int c_tm::partition(c_node* p){
    if(p->is_min()){
        return p->get_delay();
    }
    else if(p->get_gatename()=="signal")            //if it's the faout, caculate the node first
    {
        for(unsigned int x=0;x<m_vcuttail.size();x++){
            if(m_vcuttail[x]==p){
                p->set_delay(partition(m_vcuthead[x]));
                p->set_min(true);
                return p->get_delay();
            }
        }
    }
    else
    {
        vector<c_node*> innn;           //document the final input for cell
        vector<string> out;
        for(unsigned int x=0;x<m_vcell.size();x++){
            vector<c_node*> cn_tmp;     //regist the tmp final input for cell
            vector<string> str_name;
            if(matchtree(p,m_vcell[x],cn_tmp,str_name)){
                int mmm=0;
                for(unsigned int y=0;y<cn_tmp.size();y++){
                    if(partition(cn_tmp[y])>mmm)
                        mmm=partition(cn_tmp[y]);
                }
                if(p->get_delay()>mmm+m_vcell[x]->get_delay()){
                    innn=cn_tmp;
                    out=str_name;
                    p->set_delay(mmm+m_vcell[x]->get_delay());
                    p->set_optgate(m_vcell[x]->getoptname());
                }
            }
            //cn_tmp.clear();
            //str_name.clear();
        }
        //let the input in the right place
        vector<c_node* >tmp_in;
        tmp_in.resize(out.size());
        for(unsigned int x=0;x<out.size();x++){
            tmp_in[atoi(out[x].c_str())-1]=innn[x];
        }
        for(unsigned int x=0;x<tmp_in.size();x++){
            p->set_optin(tmp_in[x]);
            tmp_in[x]->set_optout(p);
        }
        p->set_min(true);
        return p->get_delay();
    }
    return p->get_delay();
}
//--------------------------------------------////--------------------------------------------//
void c_tm::mapping()
{
    for(unsigned int x=0;x<m_vtotalout.size();x++)
    {   
        partition(m_vtotalout[x]);
        //cout<<"//--------------------------------------------//\n"<<partition(m_vtotalout[x])<<endl;
    }
    /*
    for(unsigned int x=0;x<m_vtotalout.size();x++){
        cout<<"("<<m_vtotalout[x]->getname()<<","<<m_vtotalout[x]->getoptname()<<")"<<endl;
        for(unsigned int y=0;y<m_vtotalout[x]->insize();y++)
        cout<<"("<<m_vtotalout[x]->get_in(y)->getname()<<","<<m_vtotalout[x]->get_in(y)->getoptname()<<")"<<endl;
    }
    cout<<"//--------------------------------------------//"<<endl;
    for(unsigned int x=0;x<m_vtotalout.size();x++){
        cout<<"("<<m_vtotalout[x]->getname()<<","<<m_vtotalout[x]->getoptname();
        for(unsigned int y=0;y<m_vtotalout[x]->optinsize();y++){
            cout<<","<<m_vtotalout[x]->get_optin(y)->getname();
        }
        cout<<")"<<endl;
    }
    cout<<"//--------------------------------------------//"<<endl;
    for(unsigned int x=0;x<m_vcuthead.size();x++){
        cout<<"("<<m_vcuthead[x]->getname()<<","<<m_vcuthead[x]->get_gatename()<<","<<m_vcuttail[x]->getname()<<","<<m_vcuttail[x]->get_gatename()<<")"<<endl;
    }
    */
}
//--------------------------------------------////--------------------------------------------//
void c_tm::link()
{
    //because link the fanout may change the min delay, so let the bool is_min() of m_vtotalout which fan in is a cutpoint be false
    for(unsigned int x=0;x<m_vcuttail.size();x++){
        for(unsigned int y=0;y<m_vnode.size();y++){
            for(unsigned int z=0;z<m_vnode[y]->optinsize();z++){
                if(m_vnode[y]->get_optin(z)==m_vcuttail[x]){
                    m_vnode[y]->d_opinchange(m_vcuthead[x],m_vcuttail[x]);      //relink fanout fanin
                }
            }
        }
    }
    //link them
   
    /*for(unsigned int x=0;x<m_vtotalout.size();x++){
        cout<<"//--------------------------------------------//"<<endl;
        cout<<"("<<m_vtotalout[x]->getname()<<","<<m_vtotalout[x]->is_min()<<","<<m_vtotalout[x]->getoptname()<<")"<<endl;
        for(unsigned int y=0;y<m_vtotalout[x]->insize();y++)
            cout<<"("<<m_vtotalout[x]->get_in(y)->getname()<<","<<m_vtotalout[x]->get_in(y)->getoptname()<<")"<<endl;
    }
    cout<<"//--------------------------------------------//(totalout,ismin(),optimal gate)"<<endl;
    for(unsigned int x=0;x<m_vtotalout.size();x++){
        cout<<"//--------------------------------------------//"<<endl;
        cout<<"("<<m_vtotalout[x]->get_delay()<<","<<m_vtotalout[x]->getname()<<","<<m_vtotalout[x]->is_min()<<","<<m_vtotalout[x]->getoptname()<<")"<<endl;
        for(unsigned int y=0;y<m_vtotalout[x]->optinsize();y++)
            cout<<"("<<m_vtotalout[x]->get_optin(y)->getname()<<","<<m_vtotalout[x]->get_optin(y)->getoptname()<<")"<<endl;
    }
    */
}
//--------------------------------------------////--------------------------------------------//
void c_tm::printinfo(const char* a,const char* b)
{
    fout.open(a);
    path.open(b);
    fout<<".model "<<a<<endl;
    fout<<".inputs ";
    for(unsigned int x=0;x<m_sig.size()-total_out;x++)
        fout<<m_sig[x]<<" ";
    fout<<"\n.outputs ";
    for(unsigned int x=m_sig.size()-total_out;x<m_sig.size();x++)
        fout<<m_sig[x]<<" ";
    fout<<endl;
    queue<c_node* >q_print;
    for(unsigned int x=0;x<m_vtotalout.size();x++){
        q_print.push(m_vtotalout[x]);
    }
    while(!q_print.empty()){
        c_node* tmp;
        tmp=q_print.front();
        q_print.pop();
        if(!tmp->is_print())
        {
            bool b_print=true;
            for(unsigned int x=0;x<tmp->optinsize();x++){
                b_print*=tmp->get_optin(x)->is_print();
            }
            if(b_print){                                        //if the children are all printed, it can be print
                tmp->set_print(true);
                fout<<".gate "<<left<<setw(5)<<tmp->getoptname()<<" ";
                char ch_name='a';
                for(unsigned int x=0;x<tmp->optinsize();x++){
                    fout<<char(ch_name+x)<<"="<<tmp->get_optin(x)->getname()<<" ";
                } 
            fout<<"O="<<tmp->getname()<<endl;
            }
            else
            {
                for(unsigned int x=0;x<tmp->optinsize();x++){
                    q_print.push(tmp->get_optin(x));
                }
                q_print.push(tmp);
            }
        }
    }
    fout<<".end\n";
    //--------------------------------------------////--------------------------------------------//
    c_node* min_node;
    int tmp=0;
    for(unsigned int x=0;x<total_out;x++){
        if(m_vtotalout[x]->get_delay()>tmp){
            tmp=m_vtotalout[x]->get_delay();
            min_node=m_vtotalout[x];
        }
    }
    path<<tmp<<endl;
    stack<c_node*> sk_node;
    sk_node.push(min_node);
    while(min_node->optinsize()!=0)
    {
        int p_tmp=0;
        c_node* min_cn;
        for(unsigned int x=0;x<min_node->optinsize();x++){
            if(min_node->get_optin(x)->get_delay()>=p_tmp){
                p_tmp=min_node->get_optin(x)->get_delay();
                min_cn=min_node->get_optin(x);
            }
        }
        min_node=min_cn;
        sk_node.push(min_node);
    }
    while(!sk_node.empty())
    {
        path<<sk_node.top()->getname()<<" ";
        //path<<"("<<sk_node.top()->getname()<<","<<sk_node.top()->get_delay()<<","<<sk_node.top()->getoptname()<<")\n";
        sk_node.pop();
    }
    path<<endl;
    path.close();
    fout.close();
}
//--------------------------------------------////--------------------------------------------//





















