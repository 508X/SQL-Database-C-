#ifndef DATABASE_PARSETREE_H
#define DATABASE_PARSETREE_H

#include "../Queue/queue.h"
#include "../Stack/stack.h"
#include "../STok/stokenizer.h"
#include "../Maps/map.h"
#include "../Maps/mmap.h"
#include "../bplus/bplustree.h"

using namespace std;

//          [0]      [1]    [2]  [3]    [4]  [5]    [6]   [7]   [8]    [9]
enum keys{SELECT=0, FROM, WHERE, COMP, SYM, ANDOR, STAR, COMMA, DPUNCT, CREATE, TABLE, FIELD, INSERT, INTO, VALUES};

class parser
{

public:

    const static int adjx = 100;
    const static int adjy = 100;

    parser(char* s);
    bool fail();
    MMap<string, string> parse_tree();
    void build_keyword_list();
    bool get_parse_tree();
    int get_column(string str);
    Queue<string> parser_postfix();
//    Vector<long> RPN(const Queue<string>& postfix);
    void set_string(char* s);
    void setupmatrix();

private:
    Queue<token> input_q;
    MMap<string, string> ptree;
    Map<string, int> keywords;
    vector<string> keywordlist;
    bool failflag;
    int adjtable[adjx][adjy];
    char _innerbuffer[150];
};

bool isoperator(string str);
bool iscomp(string str);
bool isword(string str);


#endif //DATABASE_PARSETREE_H
