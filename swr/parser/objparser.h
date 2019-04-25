#ifndef OBJPARSER_H
#define OBJPARSER_H

#include "parser.h"
class Model;
class ObjParser : public Parser
{
public:
    ObjParser();

    std::unique_ptr<Model> parse(const std::string& filename) override;
};

#endif