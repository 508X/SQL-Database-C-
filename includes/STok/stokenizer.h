#ifndef DATABASE_STOKENIZER_H
#define DATABASE_STOKENIZER_H

#include "token.h"
#include <cassert>
#include <cstring>
const bool debug = false;
const static int MAX_ROWS = 12;
const static int MAX_COLUMNS = 128;
const static int MAX_BUFFER = 10000;

const static int alphasize = 26;
const static int numsize = 10;
const static int punctsize = 32;

void CA_lower_alphabet(int array[]);
void CA_upper_alphabet(int array[]);
void CA_numbers(int array[]);
void CA_punctuation(int array[]);

class stokenizer
{
public:

    stokenizer();
    stokenizer(char str[]);
    bool done();
    bool more();

    friend stokenizer& operator >>(stokenizer& s, token &t){
        s.get_token(t);
        return s;
    }

    void set_string(char str[]);
    void mark_table(int from_state, int to_state, int array[], int array_size);
    void mark_location(int from_state, int to_state, int column);
    static int lower_alpha[alphasize];
    static int upper_alpha[alphasize];
    static int number_s[numsize];
    static int punctuation[punctsize];


private:

    void make_table(int table[][MAX_COLUMNS]);
    bool get_token(token& t);

    char _buffer[MAX_BUFFER];
    int  _pos;

    static int _table[MAX_ROWS][MAX_COLUMNS];


};

bool is_ascii(char& c);



#endif //DATABASE_STOKENIZER_H
