#ifndef PARSER_H
#define PARSER_H

#include "value.h"

typedef enum {
    PARSE_OK = 0,
    PARSE_EXPECT_VALUE,
    PARSE_INVALID_VALUE,
    PARSE_ROOT_NOT_SINGULAR,
    PARSE_NUMBER_TOO_BIG,
    PARSE_MISS_QUOTATION_MARK,
    PARSE_INVALID_STRING_ESCAPE,
    PARSE_INVALID_STRING_CHAR,
    PARSE_INVALID_UNICODE_HEX,
    PARSE_INVALID_UNICODE_SURROGATE,
    PARSE_MISS_COMMA_OR_SQUARE_BRACKET,
    PARSE_MISS_KEY,
    PARSE_MISS_COLON,
    PARSE_MISS_COMMA_OR_CURLY_BRACKET
}exceptType;

class Parser {
private:
    char *txt;
    vector<Value* > collector;

    Parser(char *txt);
    Value* collect(Value *v);
    void parse_space();
    Value* parse_null();
    Value* parse_false();
    Value* parse_true();
    Value* parse_number();
    Value* parse_string();
    Value* parse_array();
    Value* parse_object();
    Value* parse_value();

public:
    static Value* parse(char *json);
};
#endif