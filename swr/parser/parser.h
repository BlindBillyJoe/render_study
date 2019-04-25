#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <memory>

class Model;
class Parser
{
public:
    virtual std::unique_ptr<Model> parse(const std::string& data) = 0;
};

#endif //PARSER_H