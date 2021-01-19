#include <iostream>
#include <string>
#include "file_manager.h"

using std::cout;
using std::endl;

Stream::token_values Stream::GetData(string FileName,
                    token_ids TokenIDs,
                    string KeyColumn,
                    string KeyValue,
                    char delimiter) 
{
    Stream::token_values TokenValues {};
    std::ifstream filestream(FileName);
    if (filestream.is_open()) {
        // process using virtual function defined in derived classes
        return process(filestream, TokenIDs, KeyColumn, KeyValue, delimiter);
    } else {
        return TokenValues;
    }
}

Stream::token_values StreamSingle::process(std::ifstream &filestream,
                        token_ids TokenIDs,
                        string KeyColumn,
                        string KeyValue,
                        char delimiter) 
{
    // process single line file
    Stream::token_values TokenVals {};
    GetLine(filestream, delimiter, TokenIDs, TokenVals);
    return TokenVals;
} 

Stream::token_values StreamMultiple::process(std::ifstream &filestream,
                        token_ids TokenIDs,
                        string KeyColumn,
                        string KeyValue,
                        char delimiter) 
{
    // process multiple line file
    Stream::token_values TokenVals {};
    while (GetLine(filestream, delimiter, TokenIDs, TokenVals)) {
        if (TokenVals[KeyColumn] == KeyValue) { return TokenVals; }
    }
    return TokenVals;
} 

bool Stream::GetLine(std::ifstream &filestream, char delimiter, token_ids &TokenIDs, token_values &TokenVals) {
    string line;
    std::vector <string> RawTokens;
    if (!GetLineExtract(filestream, line)) { return false; }
    GetLineScrub(line);
    GetLineSplit(line, delimiter, RawTokens);
    GetLineTokenize(RawTokens, TokenIDs, TokenVals);
    return true;
}
    
bool Stream::GetLineExtract(std::ifstream &filestream, string &line) {
    // extract line of data from filestream
    return (std::getline(filestream, line)) ? true : false;
}

void Stream::GetLineScrub(string &line) {
    // remove unused characters from line
    std::string::size_type pos;

    // remove "quotes"
    // derived from https://stackoverflow.com/questions/83439/remove-spaces-from-stdstring-in-c
    line.erase(remove(line.begin(), line.end(), '"'), line.end());

    // replace 'tabs' with spaces
    // derived from: http://www.cplusplus.com/forum/beginner/185953/
    while((pos = line.find("\t")) != std::string::npos)
        { line.replace(pos, 1, " "); }

    // remove multiple spaces
    while((pos = line.find("  ")) != std::string::npos)
        { line.replace(pos, 2, " "); }
 }
    
void Stream::GetLineSplit(string &line, char delimiter, std::vector <string> &RawTokens) {
    // create a vector of strings containing all the data items in the 
    // line of data when delimited by the space character
    // derived from: https://www.geeksforgeeks.org/tokenizing-a-string-cpp/
    string intermediate; 
    std::stringstream check1(line); 
    while(getline(check1, intermediate, delimiter)) 
        { RawTokens.push_back(intermediate); }    
}

void Stream::GetLineTokenize(std::vector <string> &RawTokens, token_ids &TokenIDs, token_values &TokenValues) {
    // return vector of tokens corresponding to the TokenIDs    
    for (auto TokenID : TokenIDs) { 
        // confirm there is a RawToken at the index specified TokenID.first, 
        // TokenID indexs begin at 0 
        if (RawTokens.size() >= TokenID.first + 1) 
            { TokenValues[TokenID.second] = RawTokens[TokenID.first]; }
    }
}
