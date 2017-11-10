#ifndef _PARSER_H_
#define _PARSER_H_
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>
#include<string.h>
#include<vector>
using namespace std;
class node_s
{
    public:
      node_s(string p_name,string p_trg,string p_src){
          m_name=p_name;
          m_trg=p_trg;
          m_src1=p_src;
          m_src2="\0";
      }
      node_s(string p_name,string p_trg,string p_src1,string p_src2){
          m_name=p_name;
          m_trg=p_trg;
          m_src1=p_src1;
          m_src2=p_src2;
      }
      string m_name;
      string m_trg;
      string m_src1;
      string m_src2;  
};
class parser
{
    private:
        vector<node_s*> m_net;
        vector<string> m_input;
        vector<string> m_output;
    public:
        parser(){;}
        ~parser(){;}
        void parse(const char*);
        void dump();
};


#endif
