#ifndef DATABASE_TOKEN_H
#define DATABASE_TOKEN_H
#include <iostream>
#include <string>
using namespace std;

enum tokentypes
{
    NUMBERS     =  1,
    FAILSTATE1  =  2,
    DEC_NUMBERS =  3,
    PUNCTUATION =  4,
    SPACEBAR    =  5,
    ALPHABET    =  6,
    FAILSTATE2  =  7,
    HYPHON      =  8,
    PERIOD      =  9,
    UNDEFINED   =  10,
};

const bool token_dbug = false;

class token
{
public:
    token();
    token(string str, int type);
    friend ostream& operator <<(ostream& outs, const token& t){
        outs<<'|'<<t._token<<'|';
        return outs;
    }
    int type();
    string type_string();
    string token_str();
private:
    string _token;
    int _type;
};
#endif //DATABASE_TOKEN_H
