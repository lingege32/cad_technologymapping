#include"parser.h"
void parser::parse(const char* p_case)
{
    ifstream fin;
    fin.open(p_case);
    int count=0;
    char buffer[80];
    char in_out[32], signal[32];
    string in_outt,signall;
    while(count<2){
        fin.getline(buffer,80);
        if(buffer[0] == '\0'||buffer[0] == '\r'){
            count++;
            continue;
        }
        sscanf(buffer,"%[^(](%[^)]",in_out,signal);
        signall.assign(signal);
        m_signal.push_back(signal);
    }
    char gate_name[32],gate_out[32], gate_in1[32],gate_in2[32];
    char tmp[32];
    string g_name,g_out,g_in1,g_in2;
    while(!fin.eof()){
        fin.getline(buffer,80);
        if(buffer[0]=='\0' || buffer[0]=='\r')
            continue;
        sscanf(buffer,"%[^ ] = %[^(]%[^)]",gate_out,gate_name,tmp);
        g_name.assign(gate_name);
        g_out.assign(gate_out);
        if(g_name=="NAND")
        {
            sscanf(tmp,"(%[^,],%s",gate_in1,gate_in2);
            g_in1.assign(gate_in1);
            g_in2.assign(gate_in2);
            node_s* r;
            r=new node_s(g_name,g_out,g_in1,g_in2);
            m_node.push_back(r); 
        }
        else if(g_name=="NOT")
        {
            sscanf(tmp,"(%[^)]",gate_in1);
            g_in1.assign(gate_in1);
            node_s* r;
            r=new node_s(g_name,g_out,g_in1);
            m_node.push_back(r); 
        }
    }
    fin.close();
}
//--------------------------------------------------//
//--------------------------------------------------//
void parser::dump()
{
    for(unsigned int i=0;i<m_signal.size();i++)
        cout<<m_signal[i]<<endl;
    for(unsigned int i=0;i<m_node.size();i++){
        node_s* r;
        r=m_node[i];
        cout<<r->m_trg<<"_"<<r->m_name<<"_"<<r->m_src1<<"_"<<r->m_src2<<endl;
    }
}
