#ifndef DATABASE_FTOKENIZER_H
#define DATABASE_FTOKENIZER_H

#include "../STok/stokenizer.h"
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;
void test_f_tokenize_simple();
class ftokenizer
{
public:
    const static int MAX_BLOCK = MAX_BUFFER;
    ftokenizer(char* fname);
    ~ftokenizer();
    token next_token();
    bool more();        //returns the current value of _more
    int pos();          //returns the value of _pos
    int block_pos();     //returns the value of _blockPos
    friend ftokenizer& operator >> (ftokenizer& f, token& t);
private:
    bool get_new_block(); //gets the new block from the file
    ifstream _f;   //file being tokenized
    stokenizer _stk;     //The STokenizer object to tokenize current block
    int _pos;           //Current position in the file
    int _blockPos;      //Current position in the current block
    bool _more;         //false if last token of the last block
    //  has been processed and now we are at
    //  the end of the last block.
};

#endif //DATABASE_FTOKENIZER_H
