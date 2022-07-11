#include "ftokenizer.h"

ftokenizer::ftokenizer(char* frame)
        :_blockPos()
{
    /*On open check if file fails*/
    _f.open(frame);
    if(_f.fail()){assert("File failed to open!"=="");}
    /*create a new buffer from the file*/
    char buffer[MAX_BLOCK+1];
    _f.read(buffer,MAX_BLOCK);
    int bytes_read = _f.gcount();
    buffer[bytes_read] = '\0';
    /*set our stk string as the new buffer*/
    _stk.set_string(buffer);
    /*close the file if you dont read anything,
    else: continue*/
    if(bytes_read==0){
        _more = false;
        _f.close();
    }
    else{
        _more = true;
    }
}
ftokenizer::~ftokenizer(){
    _f.close();
}
token ftokenizer::next_token(){
    assert(!_stk.done());
    token t;
    _stk>>t;
    return t;
}

bool ftokenizer::more(){

    if(!_f.eof() && !_stk.more()){
        _more = get_new_block();
    }
    else if(!_f.eof() && _stk.more()){
        _more = true;
    }
    else if(_f.eof() && _stk.more()){
        _more = true;
    }
    else if(_f.eof() && !_stk.more()){
        _more = false;
    }
    return _more;
}

int ftokenizer::pos(){
    return _pos;
}
int ftokenizer::block_pos(){
    return _blockPos;
}

ftokenizer& operator >>(ftokenizer& f, token& t){
    t = f.next_token();
    return f;
}

bool ftokenizer::get_new_block(){


    /*Create a new buffer from the
    given max block*/
    char xbuffer[MAX_BLOCK+1];
    _f.read(xbuffer, MAX_BLOCK);
    int xbytes_read = _f.gcount();
    xbuffer[xbytes_read] = '\0';

    /*close the file if you cant read*/
    if(xbytes_read==0){
        cout<<"No bytes read!"<<endl;
        _f.close();
        return false;
    }
    else{
        /*Setstring as new buffer*/
        _stk.set_string(xbuffer);
        return true;
    }
}

void test_f_tokenize_simple(){
    token t;
    char* filename = "solitude.txt";
    ftokenizer ftk(filename);
    int token_count = 0;
    while(ftk.more()){
        ftk>>t;
        if(t.type_string()=="Word"){
            if(token_count%2111==0){
                cout<<setw(10)<<token_count
                    <<setw(3)<<left<<":"<<setw(25)<<left<<t.token_str()
                    <<t.type_string()<<endl;
            }
            token_count++;
        }
    }
    cout<<"[Word] Tokens Found: "<<token_count<<endl;
    cout<<"=========="<<endl;
}
