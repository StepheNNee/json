#ifndef VALUE_H
#define VALUE_H

#include <vector>
#include <string>
#include <map>
#include <cassert>

using namespace std;

typedef enum {
    VALUE_NULL,
    VALUE_FALSE,
    VALUE_TRUE,
    VALUE_NUMBER,
    VALUE_STRING,
    VALUE_ARRAY,
    VALUE_OBJECT
} valueType;

class Value {
private:
    map<string, Value *> object;//object
    vector<Value *> array;//array
    string str;//string
    double num;//number
    valueType type;//type
public:
    Value();
    Value(bool type);
    Value(map<string, Value *> object);
    Value(vector<Value*> array);
    Value(string str);
    Value(double num);

    map<string, Value *> get_object();
    vector<Value* > get_array();
    string get_string();
    double get_number();
    valueType get_type();

    static void delete_value(Value *&v);
};

#endif