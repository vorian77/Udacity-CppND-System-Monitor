#include "file_manager.h"

using std::cout;
using std::endl;

// print tokens
void PrintTokens(Stream::token_ids TokenIDs, Stream::token_values &TokenVals) {
    for (auto TokenID : TokenIDs) {
        cout << "TOKEN IDX: " << TokenID.first << " NAME: " << TokenID.second << " VALUE: " << TokenVals[TokenID.second] << endl;        
    }
}

Stream::token_values Stream::GetData(string FileName,
                    token_ids TokenIDs,
                    string KeyColumn,
                    string KeyValue,
                    char delimiter) 
{
    Stream::token_values TVs {};
    cout << endl << "Stream.GetData - " << FileName << endl;
    std::ifstream filestream(FileName);
    if (filestream.is_open()) {
        return process(filestream, TokenIDs, KeyColumn, KeyValue, delimiter);
    } else {
        return TVs;
    }
}

Stream::token_values StreamSingle::process(std::ifstream &filestream,
                        token_ids TokenIDs,
                        string KeyColumn,
                        string KeyValue,
                        char delimiter) 
{
    cout << "StreamSingle.process" << endl;
    Stream::token_values TokenVals {};
    if (GetLine(filestream, delimiter, TokenIDs, TokenVals)) {
        PrintTokens(TokenIDs, TokenVals);
        return TokenVals;
    }
} 

Stream::token_values StreamMultiple::process(std::ifstream &filestream,
                        token_ids TokenIDs,
                        string KeyColumn,
                        string KeyValue,
                        char delimiter) 
{
    //sm.GetData(kProcDirectory + kStatFilename, TokenIDs, false, "cpu");  
    cout << "StreamMultiple.process" << endl;
    cout << "KeyColumn: " << KeyColumn << " KeyValue: " << KeyValue << endl;
    Stream::token_values TokenVals {};
    while (GetLine(filestream, delimiter, TokenIDs, TokenVals)) {
        if (TokenVals[KeyColumn] == KeyValue) { 
            PrintTokens(TokenIDs, TokenVals);
            return TokenVals; }
    }
    cout << "StreamMultiple.process - key not found" << endl;
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
    // using algorithm: http://www.cplusplus.com/forum/beginner/185953/
    std::string::size_type pos;

    // replace 'tabs' with spaces
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
    for (auto TokenID : TokenIDs) 
        { TokenValues[TokenID.second] = RawTokens[TokenID.first]; }
}
