#pragma once

#include <Parsers/IAST.h>
#include <Parsers/IParserBase.h>
#include <Parsers/CommonParsers.h>

namespace DB
{

namespace MySQLParser
{

struct OptionDescribe
{
    const char * usage_name;
    String option_name;
    std::shared_ptr<IParser> value_parser;

    OptionDescribe(const char * usage_name_, const String & option_name_, const std::shared_ptr<IParser> & value_parser_)
        :usage_name(usage_name_), option_name(option_name_), value_parser(value_parser_)
    {
    }
};

class ASTDeclareOptions : public IAST
{
public:
    std::unordered_map<String, ASTPtr> changes;

    ASTPtr clone() const override;

    String getID(char /*delimiter*/) const override { return "options declaration"; }
};

class ParserAlwaysTrue : public IParserBase
{
public:
    const char * getName() const override { return "always true"; }

    bool parseImpl(Pos & pos, ASTPtr & node, Expected & expected) override;
};

class ParserAlwaysFalse : public IParserBase
{
public:
    const char * getName() const override { return "always false"; }

    bool parseImpl(Pos & pos, ASTPtr & node, Expected & expected) override;
};

/// Copy and paste from ParserIdentifier,
/// the difference is that multiple tokens are glued if there is no whitespace ASCII between them
struct ParserCharsetName : public IParserBase
{
protected:
    const char * getName() const override { return "charset name"; }

    bool parseImpl(Pos & pos, ASTPtr & node, Expected &) override;
};

template <bool recursive_>
class ParserDeclareOptionImpl : public IParserBase
{
protected:
    bool recursive = recursive_;

    std::vector<OptionDescribe> options_collection;

    const char * getName() const override { return "option declaration"; }

    bool parseImpl(Pos & pos, ASTPtr & node, Expected & expected) override;
public:
    ParserDeclareOptionImpl(const std::vector<OptionDescribe> & options_collection_) : options_collection(options_collection_) {}
};

using ParserDeclareOption = ParserDeclareOptionImpl<false>;
using ParserDeclareOptions = ParserDeclareOptionImpl<true>;

}


}
