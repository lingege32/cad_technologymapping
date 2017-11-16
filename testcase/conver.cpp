#include<iostream>
#include<string>
#include<stdio.h>
#include<fstream>
#include<queue>
#include<list>
using namespace std;
int main(int argc,char* argv[])
{
    ifstream fin;
    ofstream fout;
    fin.open(argv[1]);
    char buffer[32];
    int input,output;
    int g=0;
    queue<string> how_to_gate;
    list<string> outt;
    //--------------------------------------------//
    sscanf(argv[1],"case%s",buffer);
    fout.open(buffer);
    fin.getline(buffer,32);
    sscanf(buffer,"%d",&input);
    fin.getline(buffer,32);
    fin.getline(buffer,32);
    sscanf(buffer,"%d",&output);
    for(int x=0;x<input;x++)
        fout<<"INPUT(P"<<x+1<<")"<<endl;
    fout<<endl;
    for(int x=1;x<=output;x++)
    {
        char buffer[16];
        string a;
        sprintf(buffer,"G%d",x);
        a.assign(buffer);
        outt.push_back(a);
    }
    while(1)
    {
        fin.getline(buffer,32);
        if(fin.eof())
            break;
        if(buffer[0]!='\0'||buffer[0]!='\r')
        {   
            char gg[32];
            string gg_new;
            char src1[32];
            char src2[32];
            char gate[32];
            string str_gate;
            g++;
            sscanf(buffer,"%[^ ]",gate);
            str_gate.assign(gate);
            if(str_gate=="INV"){
                sscanf(buffer,"%*s %s",src1);
                sprintf(gg,"G%d = NOT(%s)\n",g,src1);
                string b;
                b.assign(src1);
                outt.remove(src1);
            }
            else
            {
                sscanf(buffer,"%*s %s %s",src1,src2);
                sprintf(gg,"G%d = NAND(%s, %s)\n",g,src1,src2);
                string b;
                b.assign(src1);
                outt.remove(src1);
                b.assign(src2);
                outt.remove(src2);
            }
            gg_new.assign(gg);
            how_to_gate.push(gg_new);
        }
    }
    while(!outt.empty()){
        fout<<"OUTPUT("<<outt.front()<<")"<<endl;
        outt.pop_front();
    }
    fout<<endl;
    while(!how_to_gate.empty()){
        fout<<how_to_gate.front();
        how_to_gate.pop();
    }
    return 0;
}
