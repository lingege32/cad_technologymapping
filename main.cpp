#include"parser.h"
#include"tm.h"

int main(int argc,char* argv[])
{
    parser *par;
    c_tm* tm;
    par=new parser();
    par->parse(argv[1]);
    //par->dump();
    tm = new c_tm(par);
    tm->parsecell(argv[2]);
    tm->initial();
    tm->dump();
    return 0;    
}
