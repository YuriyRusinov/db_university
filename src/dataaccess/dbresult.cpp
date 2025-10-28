#include <iostream>
#include <sstream>
#include "dbresult.hpp"

using std::stringstream;
using std::cerr;
using std::endl;

int DbResult::getCellAsInt(int row, int column, bool * ok) const {
    std::string v = getCell(row, column);
    if( v.empty() ) {
        if(ok) *ok = false;
        return 0;
    }
    stringstream vs(v);
    int x;
    vs >> x;
    if( vs.eof() && ok )
        *ok = true;
    else if( ok )
        *ok = false;
    return x;
} // Возвращает результат sql-запроса в виде целого числа

long long DbResult::getCellAsInt64(int row, int column, bool * ok) const {
    std::string v = getCell(row, column);
    if( v.empty() ) {
        if(ok) *ok = false;
        return 0;
    }
    stringstream vs(v);
    long long x;
    vs >> x;
    if( vs.eof() && ok )
        *ok = true;
    else if( ok )
        *ok = false;
    return x;
}
// Возвращает результат sql-запроса в виде целого числа

bool DbResult::getCellAsBool(int row, int column, bool * ok) const {
    std::string v = getCell(row, column);
    if( v.empty() ) {
        if(ok) *ok = false;
        return false;
    }
    stringstream vs(v);
    bool x;
    vs >> x;
    if( vs.eof() && ok )
        *ok = true;
    else if( ok )
        *ok = false;
    return x;
}// Возвращает результат sql-запроса в булевом виде

std::string DbResult::getCellAsString(int row, int column, bool * ok) const {\
    std::string v = getCell(row, column);
    if(ok)
        *ok = true;
    return v;
}// строка

double DbResult::getCellAsDouble(int row, int column, bool * ok) const {
    std::string v = getCell(row, column);
    if( v.empty() ) {
        if(ok) *ok = false;
        return 0;
    }
    stringstream vs(v);
    double x;
    vs >> x;
    if( vs.eof() && ok )
        *ok = true;
    else if( ok )
        *ok = false;
    return x;
}// число двойной точности

std::vector< std::string > DbResult::getCellAsStringList(int row, int column, bool * ok) const {
    std::string v = getCell(row, column);
    if( v.empty() ) {
        if(ok) *ok = false;
        return std::vector< std::string >();
    }
    stringstream vs(v);
    char delim = ' ';
    std::vector< std::string > res;
    while( !vs.eof() ) {
        std::string w;
        getline(vs, w, delim);
        res.push_back( w );
    }
    return res;
}// список строк

std::string DbResult::getCellAsDateTime(int row, int column, bool * ok) const {
    std::string v = getCell(row, column);
    if( v.empty() ) {
        if(ok) *ok = false;
        return std::string();
    }
    return v;
    //stringstream vs(v);
}// date and time in ISO format 
