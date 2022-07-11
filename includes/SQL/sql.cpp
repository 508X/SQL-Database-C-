#include "sql.h"

const static int MAX = 130;

SQL::SQL(){
    //opens a text file of all tablenames
    fstream f;
    open_fileRW(f, "table_names.txt");

    readit();
    cout<<"___________________"<<endl;
    cout<<"::Existing Tables::"<<endl;
    for(size_t i=0; i<existing_tables.size(); i++){
        cout<<"|";
        cout<<existing_tables[i];
        cout<<"|"<<endl;
    }
    cout<<"___________________"<<endl;

    f.close();

}
SQL::~SQL(){

    writeit();

}
void sql_interactive(){

    bool done = false;
    string infix;
    SQL sql;


    do{
        char* buffer = new char[250];
        cout<<">>:";
        cin.getline(buffer, 249);



        if(buffer[0] == 'd'){
            done=true;
        }
        else{
            sql.Run(buffer);
        }

        delete [] buffer;

    }while(!done);





}
void SQL::Run(char* command){

    if(strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0){
        exit(1);
    }

    if(command[0] < 32){
//        cout<<"Null Command"<<endl;
        return;
    }
    stokenizer s(command);
    vstr vt;
    string filenamebatch;
    int k = 0;
    while(!s.done()){
        token t;
        s>>t;
//        if(t.type_string()!="Space")
        if(t.type_string()!="Space"){
//            cout<<"Type: "<<t.type_string()<<endl;
//            cout<<"Token:"<<t.token_str()<<endl;
            vt.push_back(t.token_str());
            if(k>0){
                filenamebatch+=t.token_str();
            }
        }
        k++;
    }
    if(vt[0] == "run"){

        if(vt.size()>1){
            string filename = filenamebatch+".txt";
            cout<<"Running file: "<<filename<<endl;
            run_batch(filename);
        }
        else{
            cout<<"Empty Run"<<endl;
        }
    }
    else if(vt[0] == "select" || vt[0]=="create" || vt[0]=="make" || vt[0] == "insert"){
        run_command(command);
    }
    else{
        cout<<"Invalid Command"<<endl;
    }


}
void SQL::run_command(char* command){

    //get command from user
    //runs either batch file or user command


    parser p(command);

    if(p.fail()){
        assert("INVALID INFIX"=="");
    }

    MMap<string, string> ptree = p.parse_tree();

    if(!table_exists(ptree["table"][0])){
        if(ptree["command"][0]=="create" || ptree["command"][0]=="make"){

            string tablename__ = ptree["table"][0];
            existing_tables += tablename__;
            tables.insert(tablename__, existing_tables.size()-1);

        }
        else{
            cout<<"Invalid Table"<<endl;
            return;
        }
    };

    if(ptree["command"].size()==0){
        cout<<"Null Command"<<endl;
        return;
    }

    if(ptree["command"][0]=="select"){
        string tablename = ptree["table"][0];
        table t(tablename);
        Queue<string> postf = p.parser_postfix();

        if(!ptree["where"].empty()) {
            cout << t.select_recno(ptree["fields"], t.RPN(postf)) << endl;
        }
        else{
            if(ptree["fields"][0]=="*"){
                cout<<t.select_all();
            }
            else{
                cout<<t.select_fields(ptree["fields"]);
            }
        }
    }
    else if(ptree["command"][0]=="insert"){

        string tablename = ptree["table"][0];
        table t(tablename);
        t.insert_into(ptree["values"]);
        cout<<t<<endl;

    }
    else if(ptree["command"][0]=="create" || ptree["command"][0]=="make"){

        string tablename = ptree["table"][0];

        table t(tablename, ptree["fields"]);
        cout<<t<<endl;

    }
    else{

        cout<<"Invalid Command"<<endl;
        return;

    }

}
void SQL::run_batch(string filename){


    fstream f;
    int j=0;
    f.open(filename);
    char buffer[MAX];

    if(f.fail()){

        cout<<"Failed to open batch"<<endl;
    }
    else{
        while(f.getline(buffer, MAX-1)){
            if(strlen(buffer) == 0 || buffer[0] == '/'){
                cout<<buffer<<endl;
            }
            else{
                cout<<"LINE {"<<j<<"} "<<buffer<<endl;
                run_command(buffer);
//                    cout<<"Ran command:"<<j<<endl;
                j++;
            }
        }

        f.close();

    }

}

bool SQL::table_exists(string& filename){
    if(tables.contains(Pair<string, int>(filename))){
        return true;
    }
    else{
        return false;
    }
}

void SQL::readit(){

    fstream file;
    open_fileRW(file, "table_names.txt");
    char* line = new char[21];
    string s;
    if(file.fail()){
        cout<<"not managing tables"<<endl;
    }
    else{
        int i=0;
        while(file.getline(line, 20)){

            string strtemp;

            for(int i=0; is_alfa(line[i]); i++){
                strtemp+=line[i];
            }

            tables.insert(strtemp, i);
            existing_tables.push_back(strtemp);
            i++;
        }
    }
    file.close();
}
void SQL::writeit(){

    fstream f;
    open_fileW(f,"table_names.txt");
    for(size_t i=0; i<existing_tables.size(); i++){
        f<<existing_tables[i]<<endl;
    }

    f.close();

}
bool is_alfa(char a){

    if(a>=65 && a<=90){
        return true;
    }
    if(a>=97 && a<=122){
        return true;
    }
    if(a>=48 && a<=57){
        return true;
    }
    if(a=='_'){
        return true;
    }
    return false;
}