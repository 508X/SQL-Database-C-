#include "stokenizer.h"
#include <iostream>

using namespace std;

int stokenizer::_table[MAX_ROWS][MAX_COLUMNS];
int stokenizer::lower_alpha[alphasize];
int stokenizer::upper_alpha[alphasize];
int stokenizer::number_s[numsize];
int stokenizer::punctuation[punctsize];

stokenizer::stokenizer()
        :_buffer(),_pos(){

    CA_lower_alphabet(lower_alpha);CA_upper_alphabet(upper_alpha);
    CA_numbers(number_s);CA_punctuation(punctuation);

    make_table(_table);
}
stokenizer::stokenizer(char str[]){
    _pos = 0;

    strncpy(_buffer, str, MAX_BUFFER-1);

    CA_lower_alphabet(lower_alpha);CA_upper_alphabet(upper_alpha);
    CA_numbers(number_s);CA_punctuation(punctuation);

    if(debug){
        for(int i=0; i<alphasize; i++){
            char c = lower_alpha[i];
            cout<<c;
        }
        cout<<endl;
        for(int i=0; i<alphasize; i++){
            char c = upper_alpha[i];
            cout<<c;
        }
        cout<<endl;
        for(int i=0; i<numsize; i++){
            char c = number_s[i];
            cout<<c;
        }
        cout<<endl;
        for(int i=0; i<punctsize; i++){
            char c = punctuation[i];
            cout<<c;
        }
        cout<<endl;
    }

    make_table(_table);
}
bool stokenizer::done(){
    if(_pos==strlen(_buffer)){return true;}
    else{return false;}
}
bool stokenizer::more(){
    if(_buffer[_pos]=='\0'){return false;}
    else{return true;}
}
void stokenizer::set_string(char str[]){
    strcpy(_buffer,str);
}
void stokenizer::mark_table(int from_state, int to_state, int array[], int array_size){
    assert(from_state<MAX_ROWS);
    for(int j = from_state, k=0;k<array_size;k++){
        assert(array[k]<MAX_COLUMNS);

        _table[j][array[k]] = to_state;

    }
}
void stokenizer::mark_location(int from_state, int to_state, int column){
    assert(from_state<MAX_ROWS);
    assert(column<MAX_COLUMNS);

    _table[from_state][column] = to_state;

}
void stokenizer::make_table(int table[MAX_ROWS][MAX_COLUMNS]){

    for(int a=0;a<MAX_ROWS;a++){
        for(int b=0;b<MAX_COLUMNS;b++){
            table[a][b] = -1;
        }
    }

    mark_table(0, 6 , lower_alpha, alphasize); // from row [0] lowercase letters from 'a' to 'z' are marked [6]
    mark_table(6, 6 , lower_alpha, alphasize); // from row [6] lowercase letters from 'a' to 'z' are marked [6]
    mark_table(0, 6 , upper_alpha, alphasize); // from row [0] uppercase letters from 'A' to 'Z' are marked [6]
    mark_table(6, 6 , upper_alpha, alphasize); // from row [6] uppercase letters from 'A' to 'Z' are marked [6]
    mark_table(0, 1 , number_s   , numsize  ); // from row [0] numbers from '0' to '9' are marked [1]
    mark_table(1, 1 , number_s   , numsize  ); // from row [1] numbers from '0' to '9' are marked [1]
    mark_table(2, 3 , number_s   , numsize  ); // from row [2] numbers from '0' to '9' are marked [3]
    mark_table(3, 3 , number_s   , numsize  ); // from row [3] numbers from '0' to '9' are marked [3]
    mark_table(8, 1 , number_s   , numsize  ); // from row [8] numbers from '0' to '9' are marked [1]
    mark_table(9, 3 , number_s   , numsize  ); // from row [9] numbers from '0' to '9' are marked [3]
    mark_table(0, 4 , punctuation, punctsize); // from row [0] punctutation is marked [4]
//  mark_table(4, 4 , punctuation, punctsize); // from row [4] punctutation is marked [4]

    mark_location(4, 4 , '=');
    mark_location(4, 4 , '>');
    mark_location(4, 4 , '<');
    mark_location(0, 8, '-');                  // from row [0] '-' is marked [8]
    mark_location(8, 8, '-');                  // from row [8] '-' is marked [8]
    mark_location(0, 9, '.');                  // from row [0] '.' is marked [2]
    mark_location(1, 2, '.');                  // from row [1] '.' is marked [2]
    mark_location(8, 2, '.');                  // from row [8] '.' is marked [2]
    mark_location(0, 5 , ' ');                 // from row [0] spacebar is marked [5]
    mark_location(5, 5 , ' ');                 // from row [5] spacebar is marked [5]

//    hold++;
//    array[hold] = '>';
//    hold++;
//    array[hold] = '<';
//    hold++;
//    array[hold] = '=';



}
bool stokenizer::get_token(token& t){

    if(!is_ascii(_buffer[_pos])){
//        cout<<"NOT ASCII"<<endl;
        string NA;
        NA[0] = _buffer[_pos];
        t = token(NA,UNDEFINED);
        _pos++;
        return false;
    }

    int start_pos = _pos;
    int current_state = 0;
    int last_state = 0;

    //Given an empty string

    if(!more()){

        t = token();return false;
    }
//    cout<<"PASSED GIVEN EMPTY"<<endl;

    //Given a non-ASCII character

//    cout<<"PASSED IS_ASCII()"<<endl;


    while(more() && current_state!=-1 && is_ascii(_buffer[_pos])){


        if(debug){

            for(int i=0; i<strlen(_buffer); i++){
                cout<<_buffer[i];
            }
            cout<<endl;
            for(int i=0; i<start_pos; i++){
                cout<<" ";
            }
            for(int i=start_pos; i<_pos; i++){
                cout<<'.';
            }
            cout<<'^'<<endl<<endl;
        }
        // [1] Current state jumps within the matrix depending on
        //     what character we are currently observing

        current_state = _table[current_state][_buffer[_pos]];

        // [2] Update last state if it isn't failing

        if(current_state!=-1 && current_state!=2 && current_state!=7){
            if(debug){cout<<"Moving: "<<last_state<<" to "<<current_state<<endl;}
            last_state = current_state;
        }

        else{if(debug){cout<<"current Fstate: "<<current_state<<endl;}} //Debugging only

        // [3] While im not in a (-1) state,
        //     update my position

        if(current_state!=-1)
            _pos++;

    }


    // [4] Creating token string to pass into
    //     token object

    string tkSTR;
    for(int i=start_pos; i<_pos; i++){

        tkSTR.push_back(_buffer[i]);

    }
    if(last_state!=2 && last_state!=7){
        t = token(tkSTR, last_state);
        return true;
    }
    else{
        t = token();
        return false;
    }


}

//-------------------------------------------------
// Array initialization functions

void CA_lower_alphabet(int array[]){
    for(int i=0,k='a'; k<='z'; i++,k++){
        array[i] = k;
    }
}
void CA_upper_alphabet(int array[]){
    for(int i=0,k='A'; k<='Z'; i++,k++){
        array[i] = k;
    }
}
void CA_numbers(int array[]){
    for(int i=0,k='0'; k<='9'; i++,k++){
        array[i] = k;
    }
}
void CA_punctuation(int array[]){
    for(int i=0 ,k='!'; k<='/'; i++,k++){
        array[i] = k;
    }
    for(int i=15,k=':'; k<='@'; i++,k++){
        array[i] = k;
    }
    for(int i=22,k='['; k<='`'; i++,k++){
        array[i] = k;
    }
    for(int i=28,k='{'; k<='~'; i++,k++){
        array[i] = k;
    }
}
bool is_ascii(char& c){
    if(c<32 || c>126){return false;}
    else{return true;}

}
