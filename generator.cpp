#include "generator.h"

void Generator::generate_null() {
    txt += "null";
}
void Generator::generate_false() {
    txt += "false";
}
void Generator::generate_true() {
    txt += "true";
}
void Generator::generate_number(Value* v) {
    //txt += to_string(v->get_number());
    char buffer[32];
    int length = sprintf(buffer, "%.17g", v->get_number());
    txt.append(buffer, length);
}
void Generator::generate_string(Value* v) {
    txt += '"';
    txt += v->get_string();
    txt += '"';
}
void Generator::generate_array(Value* v) {
    txt += '[';
    for (auto &it : v->get_array()) {
        generate_value(it);
        txt += ',';
    }
    if (!v->get_array().empty()) {
        txt.pop_back();
    }
    txt += ']';
}
void Generator::generate_object(Value* v) {
    txt += '{';
    for (auto &it : v->get_object()) {
        txt += '"';
        txt += it.first;
        txt += '"';
        txt += ':';
        generate_value(it.second);
        txt += ',';
    }
    if (!v->get_object().empty()) {
        txt.pop_back();
    }
    txt += '}';
}
void Generator::generate_value(Value* v) {
    switch (v->get_type()) {
    case VALUE_NULL:
        return generate_null();
        break;
    case VALUE_FALSE:
        return generate_false();
        break;
    case VALUE_TRUE:
        return generate_true();
        break;
    case VALUE_NUMBER:
        return generate_number(v);
        break;
    case VALUE_STRING:
        return generate_string(v);
        break;
    case VALUE_ARRAY:
        return generate_array(v);
        break;
    case VALUE_OBJECT:
        return generate_object(v);
        break;
    default:
        break;
    }
}
string Generator::generate(Value* v) {
    assert(v != nullptr);
    Generator generator;
    generator.generate_value(v);
    return generator.txt;
}