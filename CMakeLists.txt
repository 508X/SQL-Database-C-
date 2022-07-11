cmake_minimum_required(VERSION 3.21)
project(Database)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -static")

add_executable(Database main.cpp includes/bplus/bplustree.h includes/FTok/ftokenizer.cpp includes/FTok/ftokenizer.h includes/Maps/map.h includes/Maps/mmap.h includes/List/list.h includes/Queue/queue.h includes/Record/record.h includes/Stack/stack.h includes/STok/stokenizer.cpp includes/STok/stokenizer.h includes/STok/token.cpp includes/STok/token.h includes/btree/btree.h includes/btree/array_functions.h includes/Node/node.h includes/ParseTree/parsetree.cpp includes/ParseTree/parsetree.h includes/SQL/sql.cpp includes/SQL/sql.h includes/Table/table.cpp includes/Table/table.h)
