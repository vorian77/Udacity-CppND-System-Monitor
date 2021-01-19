#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <iostream>  // std::cout, std::endl
#include <fstream>  // std::ifstream
#include <sstream>  // std::stringstream
#include <regex>  // std::replace
#include <vector>
#include <string>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;

/*
File management classes that use virtual functions to facilitate
different variations on data extraction from files
*/

// base class
class Stream {
 public:
    typedef std::vector<std::pair<int, std::string> > token_ids;  // index, name
    typedef std::unordered_map<std::string, std::string>  token_values;  // name, value

    virtual token_values process(std::ifstream & filestream,
                        token_ids TokenIDs,
                        string KeyColumn="",
                        string KeyValue="",
                        char delimiter=' ') = 0;
    
    token_values GetData(string FileName,
                        token_ids TokenIDs,
                        string KeyColumn="",
                        string KeyValue="",
                        char delimiter=' ');

    bool GetLine(std::ifstream &filestream, char delimiter, token_ids &TokenIDs, token_values &TokenValues);
    bool GetLineExtract(std::ifstream &filestream, string &line);
    void GetLineScrub(string &line);
    void GetLineSplit(string &line, char delimiter, std::vector <string> &RawTokens);
    void GetLineTokenize(std::vector <string> &RawTokens, token_ids &TokenIDs, token_values &TokenValues);
};


// class used for processing single line files
class StreamSingle: public Stream 
{
 public:
     token_values process(std::ifstream &filestream,
                        token_ids TokenIDs,
                        string KeyColumn="",
                        string KeyValue="",
                        char delimiter=' ');
};


// class used for processing multiple line files
class StreamMultiple: public Stream 
{
 public:
    token_values process(std::ifstream &filestream,
                        token_ids TokenIDs,
                        string KeyColumn="",
                        string KeyValue="",
                        char delimiter=' ');
};


// class used for processing special case multiple line file to extract operting system

#endif