#ifndef GENERATOR_H
#define GENERATOR_H

#include "value.h"

class Generator {
private:
    string txt;

    void generate_null();
    void generate_false();
    void generate_true();
    void generate_number(Value* v);
    void generate_string(Value* v);
    void generate_array(Value* v);
    void generate_object(Value* v);
    void generate_value(Value* v);

public:
    static string generate(Value* v); 
};

#endif