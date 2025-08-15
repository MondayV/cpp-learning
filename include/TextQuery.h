#ifndef TEXTQUERY_H
#define TEXTQUERY_H
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include "QueryResult.h"

class QueryResult; // declaration needed for return type in the query function
class TextQuery {
public:
	using line_no = std::vector<std::string>::size_type;
	TextQuery(std::ifstream&);
QueryResult query(const std::string&) const; 
void display_map();        // debugging aid: print the map
private:
    StrBlob file;
    std::map<std::string, 
	         std::shared_ptr<std::set<line_no>>> wm;  

	// canonicalizes text: removes punctuation and makes everything lower case
    static std::string cleanup_str(const std::string&);
};
#endif
