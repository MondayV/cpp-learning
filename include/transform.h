#include <map>
#include <string>
#include <istream>

std::map<std::string, std::string> build_map(std::istream &map_stream);
const std::string &transform(const std::string &word, const std::map<std::string, std::string> &trans_map);
void word_transform(std::istream &map_stream, std::istream &input_stream);