#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H
#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <string>
#include "../ParseTree/parsetree.h"
#include "../Queue/queue.h"
#include "../Stack/stack.h"
#include "../Maps/map.h"
#include "../Maps/mmap.h"


using namespace std;

typedef vector<string> vstr;
typedef vector<long> vlong;

bool file_exists(const char filename[]);
void open_fileRW(fstream& f, const char filename[]) noexcept(false);
void open_fileW(fstream& f, const char filename[]) noexcept(false);


class Record
{
public:

    Record(){
        for (int i = 0; i<ROW_MAX; i++)
            record[i][0] = '\0';
        recno = -1;
    }
    Record(const vstr& v){
        for (int i = 0; i<ROW_MAX; i++)
            record[i][0] = '\0';
        for (int i=0; i<v.size(); i++)
            strcpy(record[i], v[i].c_str());
    }
    long size(){
        return sizeof (record);
    }

    long write(fstream& outs); //returns where we started writing
    long read(fstream& ins, long RecordNumber); //returns size of bytes read
    vstr get_record();
    friend ostream& operator<<(ostream& outs, const Record& r){
        for (int i=0; i < Record::ROW_MAX && r.record[i][0] != '\0'; i++){
            outs<<setw(16)<<r.record[i];
        }
        return outs;
    }
    string row2str(int i){
        if(i>=ROW_MAX){return string();}

        string temp;
        if(record[i][0]!='\0'){
            temp+=record[i];
        }
        return temp;
    }
    vstr pickfields(const vlong& fields){
        vstr result;
        for(int i=0; i<fields.size(); i++){
            result.push_back(row2str(fields[i]));
        }
        return result;
    }

private:
    static const int ROW_MAX = 10;
    static const int COL_MAX = 500;
    int recno;
    char record[ROW_MAX][COL_MAX];
};

class table
{


public:

    table(const string& name); //copy constructor make a copy of file
    table(const string& name, const vector<string>& fields);
    void insert_into(const vector<string>& vect);
    void reindex();
    table select_all();
    table select(const vector<string>& conditions);
    table select_recno(const vector<string>& ffields, const vector<long>& recs);
    table select_rpn(const vector<string>& fields, const vector<long>& recs);
    table select(const vector<string>& fields, const vector<string>& conditions);
    table select_fields(const vector<string>& fields);
    vector<long> RPN(const Queue<string>& postfix);
    friend ostream& operator <<(ostream& outs, const table& T){
        fstream f;
        open_fileRW(f, (T.file_name).c_str());


        string s = "|"; for(int i=0; i<T.field_vect.size()*16+4; i++){s+="-";} s+="|";
        cout<<s<<endl;

        cout<<"|"<<"[--]";

        for (int i=0; i<T.field_vect.size(); i++){
            outs<<setw(16)<<T.field_vect[i];
        }

        cout<<"|"<<endl;
        cout<<s<<endl;

        for(int i=0; i<T.last_recno; i++){
            Record temp;
            temp.read(f, i);
            outs<<"|"<<"["<<setw(2)<<i<<"]"<<temp<<"|"<<endl;
        }
        cout<<s<<endl;
        f.close();

        return outs;
    }

private:

    string table_name;
    string file_name;
    void set_fields(const vector<string>& fieldlist);
    vector<string> get_fields();
    vector<long> recnoseq(const vector<string>& cond);
    vector<long> recnos(const vector<string>& cond);
    vector<long> getallrecnos();
    table get_records_sp(const vector<string>& myfields, const vector<long> &recnums);
    table get_records(const vector<long>& recnums);
    vector<MMap<string, long> > indices;
    Map<string, long> field_index;
    vector<string> field_vect;
    bool _empty;
    long last_recno;

};


#endif //DATABASE_TABLE_H

#pragma clang diagnostic pop