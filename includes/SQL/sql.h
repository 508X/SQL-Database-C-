#ifndef DATABASE_SQL_H
#define DATABASE_SQL_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include "../Table/table.h"
#include "../ParseTree/parsetree.h"
#include "../FTok/ftokenizer.h"
#include "../Maps/map.h"
#include "../btree/array_functions.h"

using namespace std;
void sql_interactive();


class SQL
{
public:

    SQL();
    ~SQL();
    void Run(char* command);
    void run_command(char* command);
    void run_batch(string filename);
    void delete_tables();
    bool table_exists(string& filename);
    void readit();
    void writeit();

private:

    Map<string, int> tables;
    vector<string> table_deleting;
    vector<string> existing_tables;

};
bool is_alfa(char a);

#endif //DATABASE_SQL_H
