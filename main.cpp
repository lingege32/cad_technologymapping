#include"parser.h"

int main(int argc,char* argv[])
{
    parser *par;
    par=new parser();
    par->parse(argv[1]);
    par->dump();
    
    return 0;    
}
