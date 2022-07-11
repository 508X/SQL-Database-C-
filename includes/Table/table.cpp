#include "table.h"

using namespace std;


/**
 * Constructs a table object
 * @param name
 */
table::table(const string& name){
    table_name = name;
    file_name = table_name+".tbl";

    last_recno = 0;

    string temp = file_name;
    string temp2= table_name+"_fields.txt";

    fstream ffield = fstream();
    open_fileRW(ffield, temp2.c_str());
    Record tempfield = Record();
    tempfield.read(ffield, 0);

    for(int i=0; tempfield.row2str(i)!=""; i++){
        field_vect.push_back(tempfield.row2str(i));
    }

    set_fields(field_vect);
    Record rec(field_vect);
    fstream f = fstream();
    fstream f2 = fstream();



    open_fileRW(f, temp.c_str());
    open_fileRW(f2, temp2.c_str());
    rec.write(f2);
    for(size_t i=0; i<field_vect.size(); i++){
        indices.push_back(MMap<string, long>());
    }


    f.close();
    f2.close();
    reindex();
}

table::table(const string& name, const vector<string>& fields)  {
    field_vect = fields;
    table_name = name;
    file_name = table_name+".tbl";
    last_recno = 0;

    set_fields(field_vect);
    Record rec(field_vect);
    fstream f;
    fstream f2;

    string temp = file_name;
    string temp2= table_name+"_fields.txt";

    open_fileW(f, temp.c_str());
    open_fileW(f2, temp2.c_str());
    rec.write(f2);
    for(size_t i=0; i<field_vect.size(); i++){
        indices.push_back(MMap<string, long>());
    }
    f.close();
    f2.close();
}

void    table::insert_into(const vector<string>& vect)  {

    /*
    inserting a vector of values

    file already exists
    values are in order of field names

    insert a new row of values into the
    table: write these values into a file

    insert each of these values, along
    with the record number, into the corresponding
    index structure

    indices are Map<field names, MMap<types, record numbers> >
                    "firstname", <"Harris", 73>

    WRITE TO FILE
    */

    Record temp(vect);
    fstream f;

    open_fileRW(f, file_name.c_str());

    long writepos = temp.write(f);
    long recordsize = temp.size();
    last_recno = writepos/recordsize;

    for(size_t i=0; i<field_vect.size(); i++){
        indices[i][vect[i]].push_back(last_recno);
    }

    last_recno++;
    f.close();
}

void    table::reindex()                                {
    fstream f1;
    int recno = 0;

    open_fileRW(f1, file_name.c_str());

    Record r;
    r.read(f1, 0);

    while(r.read(f1, recno)!=0){

        vector<string> str = r.get_record();

        for(int i=0; i<field_index.size(); i++){
            indices[i][str[i]] += last_recno;
        }

        last_recno++;
        recno++;
    }

    f1.close();
}

void    table::set_fields(const vector<string>& tempfieldlist){
    field_vect = tempfieldlist;
    for(int i=0; i<tempfieldlist.size(); i++){
        field_index.insert(field_vect[i], (long)i);
    }
}

table   table::select(const vector<string>& conditions) {

    vector<long> recordnums = recnoseq(conditions);
    table return_me = get_records(recordnums);
    return return_me;

}

table   table::select_all()                             {

    vector<long> recordnums;
    recordnums.reserve(last_recno);
    for(int i=0; i<last_recno; i++){
        recordnums.push_back(static_cast<long>(i));
    }
    table return_me = get_records(recordnums);
    return return_me;


}

table   table::get_records_sp(const vector<string>& myfields, const vector<long> &recnums){

    fstream f;
    open_fileRW(f, file_name.c_str());
    string str = "temp_table.tbl";
    fstream f2;
    open_fileW(f2, str.c_str());
    f2.close();

    table temporary("temp_table", myfields);

    vector<long> fieldindexes;
    fieldindexes.reserve(myfields.size());
    for(int i=0; i<myfields.size(); i++){
        fieldindexes.push_back(field_index[myfields[i]]);
    }
    for(int i=0; i<recnums.size(); i++){
        Record temp;
        temp.read(f, recnums[i]);
        temporary.insert_into(temp.pickfields(fieldindexes));
    }

    f.close();
    f2.close();
    return temporary;
}

table   table::get_records(const vector<long>& recnums) {

    table temporary("temp_table", field_vect);
    fstream f;
    open_fileRW(f, file_name.c_str());

    for(int i=0; i<recnums.size(); i++){
        Record temp;
        temp.read(f, recnums[i]);
        temporary.insert_into(temp.get_record());
    }
    f.close();
    return temporary;


}

vector<string>  table::get_fields()                             {
    return field_vect;
}

vector<long>    table::recnoseq(const vector<string>& cond)     {
    string a = cond[0];
    string b = cond[2];
    long ind_field = field_index[a];
    vector<long> recnums;
    recnums = indices[ind_field][b];
    return recnums;

}

vector<long>    table::recnos(const vector<string>& cond)       {
//    cout<<"::::RECNOS:::: of "<<cond<<endl;
    vector<long>  recnums;
    string a    = cond[0];
    string b    = cond[2];

    string comp = cond[1];
    long ind_field;

    ind_field = field_index[a];

    MMap<string, long>::Iterator begin = indices[ind_field].begin();
    MMap<string, long>::Iterator ub = indices[ind_field].upper_bound(b);
    MMap<string, long>::Iterator lb = indices[ind_field].lower_bound(b);
    MMap<string, long>::Iterator end = indices[ind_field].end();

    if(comp=="==" || comp=="="){
        recnums = indices[ind_field][b];
    }
    else if(comp=="<"){

//        cout<<"< Operator"<<endl;
        for(MMap<string, long>::Iterator it = begin;
            it != indices[ind_field].lower_bound(b);
            it++){

//            cout<<"Iterating"<<endl;
//            recnums = unionize(recnums, (*it).value_list);
            recnums += (*it).value_list;
        }

    }
    else if(comp=="<="){
        for(MMap<string, long>::Iterator it = begin; it != ub; it++){
            recnums += (*it).value_list;
        }
    }
    else if(comp==">"){
//        cout<<"> Operator"<<endl;
        ind_field = field_index[a];
        for(MMap<string, long>::Iterator it = indices[ind_field].upper_bound(b);
            it!=end;
            it++){
            recnums += (*it).value_list;
        }
    }
    else if(comp==">="){
//        cout<<">= Operator"<<endl;
        for(MMap<string, long>::Iterator it = lb; it!=end; it++){
            vector<long> temprecs = (*it).value_list;
//            cout<<"beep"<<endl;
            recnums += (*it).value_list;
        }
    }
    else{
//        cout<<"Defaulted! returning base vector<long>"<<endl;
        return vector<long>();
    }
//    cout<<"recnos returning: "<<recnums<<endl;
    return recnums;
}

table           table::select(const vector<string>& fields,
                              const vector<string>& conditions){

    vector<long> recordnums;
    if(!conditions.empty()){
        for(int i=0; i<last_recno; i++){
            recordnums.push_back(static_cast<long>(i));
        }
        return get_records_sp(fields, recordnums);

    }
    else{
        recordnums = recnos(conditions);
        return get_records(recordnums);
    }

}

table           table::select_recno(const vector<string>& ffields,
                                    const vector<long>& recs){

    if(ffields[0]=="*" || ffields[0]=="all"){
        return get_records(recs);
    }
    else{
        return get_records_sp(ffields, recs);
    }
}

vector<long>    table::RPN(const Queue<string>& post)           {

    vector<vector<long> > RecordSets;
    Queue<string> postfix(post); if(postfix.empty()){return vector<long>();}
    int sizerec = 0;

    Stack<string> wordpar;

    vector<long> Set1;
    vector<long> Set2;

    string word1;       string word2;
    string tempword;    string temp;
    string comp1;       string oper1;

    vector<string> recset = vector<string>();
    vector<long> tempvect = vector<long>();

    while(!postfix.empty()){

        temp = *postfix.QBegin();

        if(isword(temp)){

            tempword = postfix.pop();
            wordpar.push(tempword);

        }
        else if(isoperator(temp)){

            word1 = wordpar.pop();
            word2 = wordpar.pop();
            oper1 = postfix.pop();

            recset.push_back(word2);
            recset.push_back(oper1);
            recset.push_back(word1);

            tempvect = recnos(recset);

            RecordSets.push_back(tempvect);
            sizerec++;
            recset = vector<string>();

        }
        else if(iscomp(temp)){

            Set1 = RecordSets[RecordSets.size()-1];
            RecordSets.pop_back();

            Set2 = RecordSets[RecordSets.size()-1];
            RecordSets.pop_back();

            postfix.pop();

            vector<long>::iterator it1 = Set1.begin();
            vector<long>::iterator it2 = Set1.end();

            sort(it1, it2);

            vector<long>::iterator it3 = Set2.begin();
            vector<long>::iterator it4 = Set2.end();

            sort(it3, it4);
            if(temp=="and"){
                //INTERSECT
                tempvect = intersect(Set1, Set2);
            }
            else if(temp=="or"){
                //UNIONIZE
                tempvect = unionize(Set1, Set2);
            }
            RecordSets.push_back(tempvect);
        }
    }

    tempvect = RecordSets[RecordSets.size()-1];
    RecordSets.pop_back();
    return tempvect;
}

table           table::select_fields(const vector<string>& fields){
    vector<long> RecordNumbers;
    RecordNumbers.reserve(last_recno);

    for(int i=0; i<last_recno; i++){
        RecordNumbers.push_back(static_cast<long>(i));
    }

    table returnme = get_records_sp(fields, RecordNumbers);
    return returnme;
}

long Record::write(fstream &outs){
    //write to the end of the file.
    outs.seekg(0, outs.end);
    long pos = outs.tellp();

    //outs.write(&record[0], sizeof(record));
    outs.write(&record[0][0], sizeof(record));

    return pos;
}

long Record::read(fstream &ins, long RecordNumber){
    long pos = RecordNumber * (long)sizeof(record);
    ins.seekg(pos, ios_base::beg);

    ins.read(&record[0][0], sizeof(record));
    return ins.gcount();
}

vstr Record::get_record(){
    vstr vect;
    for(int i=0; i<ROW_MAX && record[i][0]!='\0'; i++){
        vect.push_back(row2str(i));
    }
    return vect;
}

bool file_exists(const char filename[]){
    fstream ff;
    ff.open (filename,
             std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug)
            cout<<"file_exists(): File does NOT exist: "<<filename<<endl;

        return false;
    }
    if (debug)
        cout<<"file_exists(): File DOES exist: "<<filename<<endl;

    ff.close();
    return true;
}
void open_fileRW(fstream& f, const char filename[]) noexcept(false){

    if (!file_exists(filename)){
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug)
                cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
                std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}
void open_fileW(fstream& f, const char filename[]) noexcept(false){
    f.open (filename,
            std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}
