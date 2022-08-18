#include "value.h"

Value::Value() {
    this->type = VALUE_NULL;
}

Value::Value(map<string, Value *> object) {
    this->type = VALUE_OBJECT;
    this->object = object;
}

Value::Value(vector<Value*> array) {
    this->type = VALUE_ARRAY;
    this->array = array;
}

Value::Value(string str) {
    this->type = VALUE_STRING;
    this->str = str;
}

Value::Value(double num) {
    this->type = VALUE_NUMBER;
    this->num = num;
}

Value::Value(bool type) {
    if (type) {
        this->type = VALUE_TRUE;
    } else {
        this->type = VALUE_FALSE;
    }

}

map<string, Value *> Value::get_object() {
    return this->object;
}
vector<Value* > Value::get_array() {
    return this->array;
}
string Value::get_string() {
    return this->str;
}
double Value::get_number() {
    return this->num;
}
valueType Value::get_type() {
    return this->type;
}

void Value::delete_value(Value *&v) {
    assert(v != nullptr);
    if (v->type == VALUE_OBJECT) {
        for (auto &it : v->object){
            delete_value(it.second);
        }
    }
    if (v->type == VALUE_ARRAY) {
        for (auto &it : v->array) {
            delete_value(it);
        }
    }
    delete(v);
    v = nullptr;
}