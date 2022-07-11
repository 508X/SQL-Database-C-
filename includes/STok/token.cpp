#include "token.h"

token::token():_token(),_type(){}
token::token(string str, int type_t) :_token(str), _type(type_t){
    if(token_dbug){
        cout<<"Token Constructor"<<endl;
        cout<<"type_t: "<<type_t<<endl;
        cout<<"str   : "<<str<<endl;
        cout<<"_token: "<<_token<<endl;
        cout<<"_type : "<<_type<<endl;
        cout<<"type()  : "<<type()<<endl;
        cout<<"type_str: "<<type_string()<<endl;
        cout<<"tokn_str: "<<token_str()<<endl;
    }
}

int    token::type(){
    return _type;
}
string token::type_string(){
    switch(_type){
        case 0:
            return "Start";
            break;
        case 1:
            return "Number";
            break;
        case 2:
            return "F2";
            break;
        case 3:
            return "Number";
            break;
        case 4:
            return "Punct";
            break;
        case 5:
            return "Space";
            break;
        case 6:
            return "Word";
            break;
        case 7:
            return "F7";
            break;
        case 8:
            return "Hyphon";
            break;
        case 9:
            return "Period";
            break;
        case 10:
            return "Undefined";
            break;
        default:
            return "tk: Program Defaulted ";
            break;
    }
}
string token::token_str(){
    return _token;
}