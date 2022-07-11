#include "parsetree.h"

bool isoperator(string str){
    if(str=="==" || str=="="  ||
       str==">=" || str=="<=" ||
       str==">"  || str=="<"  ){
        return true;
    }
    return false;
}
bool iscomp(string str){
    if(str=="and" || str=="or"){return true;}
    return false;
}
bool isword(string str){
    if(!isoperator(str) && !iscomp(str)){
        return true;
    }
    return false;
}
bool isalphabetical(string str){
    return (str[0]>=65 && str[0]<=90) || (str[0]>=97 && str[0]<=122);
}

parser::parser(char* s){
    //copy string to internal buffer
    //initialize, "make" adjacency table
    //build keyword list
    //set_string

//    cout<<"Parser CTOR"<<endl;
    set_string(s);
//    cout<<"Set String"<<endl;
    build_keyword_list();
//    cout<<"Built Keyword List"<<endl;
    setupmatrix();
//    cout<<"Made Matrix"<<endl;
    if(fail()){
        cout<<"failed!"<<endl;
    }
    else{
        //cout<<"success!"<<endl;
    }



}
Queue<string> parser::parser_postfix(){
//    cout<<"Parser Postfix"<<endl;
//    cout<<"Conditions: "<<ptree["conditions"]<<endl;
    Queue<string> infix(ptree["conditions"]);
    Queue<string> wordqueue;
    Stack<string> operstack;
    Queue<string> postfix;
    int type = 0;
    // row 5
    // ==  3
    // and 2
    // or  1
    string temp;
    while(!infix.empty()){
        string str = infix.pop();
        if(iscomp(str)){
            if(str=="and"){
                type = 2; //AND
            }
            else if(str=="or"){
                type = 3; //OR
            }
        }
        else if(isoperator(str)){
            type = 1; //OPERATOR
        }
        else{
            type = 0;
        }
        switch(type){
            case 0:
                postfix.push(str);
                break;
            case 1: //comparisons
                operstack.push(str);
                break;
            case 2: //a nd
                while(!operstack.empty() &&
                      isoperator(*operstack.STKBegin())){
                    postfix.push(operstack.pop());
                }
                operstack.push(str);
                break;
            case 3: //or
                while(!operstack.empty() &&
                      (isoperator(*operstack.STKBegin())
                       || *operstack.STKBegin() == "and")){
                    postfix.push(operstack.pop());
                }
                operstack.push(str);
                break;
            default:
                cout<<"Parse Tree Defaulted"<<endl;
                break;
        }
    }
    string strtemp;
    while(!operstack.empty()){
        strtemp = operstack.pop();
        postfix.push(strtemp);
    }
    if(infix.empty() && !operstack.empty()){
        while((isoperator(*operstack.STKBegin()) || iscomp(*operstack.STKBegin()))
              && !operstack.empty()){
            strtemp = operstack.pop();
            postfix.push(strtemp);
        }

    }
    return postfix;
}
bool parser::fail(){
    failflag = get_parse_tree();
    return failflag;
}
MMap<string, string> parser::parse_tree(){
    return ptree;
}

int  parser::get_column(string str){

    if(keywords.contains(Pair<string, int>(str))){
//        cout<<"Contains: "<<keywords[str]<<endl;
//        cout<<"STR = "<<str<<endl;
        return keywords[str];
    }
    else{
//        cout<<"!Contains Returning keywords: symbol: "<<keywords["symbol"]<<" of "<<str<<endl;
        return keywords["symbol"];
    }

}
void parser::set_string(char* s){
    stokenizer temp(s);
    while(!temp.done()){
        token t;
        temp>>t;
//        cout<<"Token: "<<t.token_str()<<endl;

        if(t.type_string() == "Word" || t.type_string() == "Punct" || t.type_string() == "Number"){
            input_q.push(t);
        }
    }
}
void parser::build_keyword_list(){

    keywords["select"]  = SELECT;

    keywords["from"]    =   FROM;
    keywords["where"]   =  WHERE;

    keywords["compare"] =   COMP;
    keywords["is"]      =   COMP;
    keywords["=="]      =   COMP;
    keywords["="]       =   COMP;
    keywords[">="]      =   COMP;
    keywords["<="]      =   COMP;
    keywords["=>"]      =   COMP;
    keywords["=<"]      =   COMP;
    keywords["<"]       =   COMP;
    keywords[">"]       =   COMP;

    keywords["symbol"]  =    SYM;

    keywords["and"]     =  ANDOR;
    keywords["or"]      =  ANDOR;

    keywords["*"]       =   STAR;
    keywords["all"]     =   STAR;

    keywords[","]       =  COMMA;
    keywords["\""]      = DPUNCT;

    keywords["create"]  = CREATE;
    keywords["make"]    = CREATE;

    keywords["insert"]  = INSERT;
    keywords["into"]    =   INTO;



    keywords["values"]  = VALUES;
    keywords["table"]   =  TABLE;
    keywords["fields"]  =  FIELD;

}
void parser::setupmatrix(){

    for(int i=0; i<adjx; i++){
        for(int j=0; j<adjy; j++){
            adjtable[i][j] = -1;
        }
    }

    adjtable[0][0]  =  1;

    adjtable[1][2]  =  3;
    adjtable[1][12] =  3;

    adjtable[2][4]  =  5;

    adjtable[3][6]  =  7;

    adjtable[4][1]  =  2;
    adjtable[4][3]  =  4;
    adjtable[4][5]  =  6;
    adjtable[4][7]  = 10;
    adjtable[4][8]  =  9;
    adjtable[4][9]  =  9;
    adjtable[4][11] =  6;
    adjtable[4][15] =  2;

    adjtable[5][10] = 11;

    adjtable[6][1]  = 12;

    adjtable[7][2]  = 15;

    adjtable[8][7]  =  8;
    adjtable[8][9]  = 10;

    adjtable[9][0]  = 16;
    adjtable[10][16]= 17;
    adjtable[4][17] = 18;
    adjtable[11][18]= 19;
    adjtable[4][19] = 20;
    adjtable[7][20] = 21;
    adjtable[4][21] = 20;


    adjtable[12][0] = 23;
    adjtable[13][23]= 24;
    adjtable[4][24] = 25;
    adjtable[14][25]= 26;

    adjtable[4][26] = 27;

    adjtable[7][27] = 31;

    adjtable[8][26] = 28;
    adjtable[4][28] = 29;
    adjtable[4][29] = 29;
    adjtable[8][29] = 30;

    adjtable[7][30] = 31;



    adjtable[4][31] = 32;

    adjtable[7][32] = 31;



    adjtable[8][31] = 33;
    adjtable[4][33] = 34;
    adjtable[4][34] = 34;
    adjtable[8][34] = 35;

    adjtable[7][35] = 31;


//    adjtable[8][26] = 29;

//    adjtable[7][27] = 28;

//    adjtable[4][28] = 27;
//    adjtable[8][28] = 29;

//    adjtable[4][29] = 30;
//    adjtable[8][30] = 31;

//    adjtable[7][31] = 28;

}
bool parser::get_parse_tree(){

    int current_state = 0;
    bool failing = false;
    int column = 0;
    bool punctflag = false;
    int firstword = 0;
    token t;
    token mult;
    string temp;
    string str = "yes";
    bool selflag = false;
    bool selfieldflag = false;
    bool seltableflag = false;
    bool insflag = false;
    bool instablename = false;
    bool makeflag= false;
    bool fieldflag = false;
    bool tablename = false;
    bool valflag = false;
    int prevstate = 0;
//    bool
    int a = 0;
    while(!input_q.empty() && current_state!=-1){
        prevstate = current_state;
//        cout<<"P-TOKEN: "<<t.token_str()<<"  C: "<<column<<"  ST: "<<current_state<<endl;

        t = input_q.pop();
        column = get_column(t.token_str());
//        cout<<"Got Column: "<<column<<endl;
//        cout<<"PFLAG IS: "<<punctflag<<endl;

        current_state = adjtable[column][current_state];
//        cout<<"  TOKEN: "<<t.token_str()<<"  of type "<<t.type_string()<<endl
//        <<"["<<column<<"]["<<prevstate<<"] -> "<<"["<<current_state<<"]"<<endl;
        switch(column){
            case SELECT:
                ptree["command"]+=t.token_str();
                selflag=true;
                selfieldflag = true;
                break;
            case INSERT:
                ptree["command"]+=t.token_str();
                insflag=true;
                break;
            case INTO:
                if(t.token_str()!="into"){failing = true;}
                instablename = true;
                break;
            case CREATE:
                ptree["command"]+=t.token_str();
                makeflag = true;
                break;
            case FIELD:
                fieldflag = true;
                break;
            case TABLE:
                tablename = true;
                break;
            case FROM:
                selfieldflag = false;
                seltableflag = true;
                break;
            case WHERE:
                ptree["where"]+=str;
                break;

            case COMP:
                ptree["compare"]+=t.token_str();
                ptree["conditions"]+=t.token_str();
                break;
            case SYM:
                if(selflag){
                    if(selfieldflag){
//                    cout<<"ADDING FIELD: "<<t.token_str()<<endl;
                        ptree["fields"]+=t.token_str();
                    }
                    else if(seltableflag){
                        ptree["table"]+=t.token_str();
                        seltableflag = false;
                    }
                    else if(!selfieldflag && !seltableflag){
                        if(!punctflag){
//                    cout<<"No flag: pushing "<<t.token_str()<<endl;
                            ptree["conditions"]+=t.token_str();
                        }
                        else{
//                    cout<<"FLAG: ";
                            if(firstword==1){
                                temp += " ";
                                temp += t.token_str();
                            }
                            else{
                                temp += t.token_str();
                                firstword = 1;
                            }
//                    cout<<"New string: "<<temp<<endl;
                        }
                    }
                }
                else if(makeflag){
                    if(tablename){
                        ptree["table"]+= t.token_str();
                        tablename = false;
                    }
                    else if(fieldflag){
                        ptree["fields"]+= t.token_str();
                    }
                }
                else if(insflag){
                    if(instablename){
                        ptree["table"] += t.token_str();
                        instablename = false;
                    }
                    else if(valflag){
//                    cout<<"VALFLAG of: "<<t.token_str()<<endl;
                        if(punctflag==false){
//                        cout<<"No flag: pushing "<<t.token_str()<<endl;
                            ptree["values"]+=t.token_str();
                        }
                        else{
//                        cout<<"FLAG: ADDING: "<<t.token_str()<<endl;
                            if(firstword==1){
                                temp += " ";
                                temp += t.token_str();
                            }
                            else{
                                temp += t.token_str();
                                firstword = 1;
                            }
//                        cout<<"New string: "<<temp<<endl;
                        }
//                    ptree["values"] += t.token_str();
                    }
                }
                break;
            case VALUES:
                valflag=true;
                break;
            case ANDOR:
                ptree["conditions"]+=t.token_str();
                ptree["andor"]+=t.token_str();
                break;
            case COMMA:
//            cout<<"READ COMMA"<<endl;
//            temp.clear();
                break;
            case DPUNCT:
//            cout<<"DETECTED QUOTE SWITCHING FROM: "<<punctflag<<" to "<<!punctflag<<endl;
                punctflag = !punctflag;
//            cout<<"// // // // // // PUNCT FLAG SWITCHED // // // // //"<<endl;
                if(punctflag==false){
                    mult = token(temp, 0);
                    if(selflag){
                        ptree["conditions"]+=mult.token_str();
                    }
                    else if(insflag){
                        ptree["values"]+=mult.token_str();
                    }
                    temp.clear();
                    firstword = 0;
                }
                break;
            case STAR:
                ptree["fields"]+=t.token_str();
                break;
            default:
                cout<<"PTree defaulted"<<endl;
                break;
        }
        a++;

    }
    if(failing){
        input_q.clear();
        ptree.clear();
    }
    return failing;
}