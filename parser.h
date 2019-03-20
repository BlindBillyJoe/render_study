#ifndef PARSER_H
#define PARSER_H

#include <string>

class Model;
class Parser
{
public:
    virtual Model* parse(const std::string& data) = 0;
};

#endif //PARSER_H