#include "parser.h"

#define ISDIGIT(ch)         ((ch) >= '0' && (ch <= '9'))
#define ISDIGIT1T9(ch)      ((ch) >= '1' && (ch <= '9'))

Parser::Parser(char *txt) {
    collector.clear();
    this->txt = txt;
}

Value* Parser::collect(Value *v) {
    collector.push_back(v);
    return v;
}

void Parser::parse_space() {
    char *p = txt;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
        p++;
    }
    txt = p;
}

Value* Parser::parse_null() {
    if (txt[0] != 'n' || txt[1] != 'u' || txt[2] != 'l' || txt[3] != 'l') {
        throw PARSE_INVALID_VALUE;
    }
    txt += 4;
    return collect(new Value());
}

Value* Parser::parse_false() {
    if (txt[0] != 'f' || txt[1] != 'a' || txt[2] != 'l' || txt[3] != 's' || txt[4] != 'e') {
        throw PARSE_INVALID_VALUE;
    }
    txt += 5;
    return collect(new Value(false));
}

Value* Parser::parse_true() {
    if (txt[0] != 't' || txt[1] != 'r' || txt[2] != 'u' || txt[3] != 'e') {
        throw PARSE_INVALID_VALUE;
    }
    txt += 4;
    return collect(new Value(true));
}

Value* Parser::parse_number() {
    char *p = txt;
    //负号可选
    if (*p == '-') {
        p++;
    }
    //整数
    if (*p == '0') { //以0开头，后面不能跟数字，012非法
        p++;
    } else if (ISDIGIT1T9(*p)) { //以1-9开头，后面可跟数字
        p++;
        for (p; ISDIGIT(*p); ++p);
    } else {
        throw PARSE_INVALID_VALUE;
    }
    //小数
    if (*p == '.') {
        p++;
        if (ISDIGIT(*p)) {
            p++;
            for (p; ISDIGIT(*p); p++);
        } else {
            throw PARSE_INVALID_VALUE; //小数点后必须有内容
        }
    }
    //指数
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') { //正负号可选
            p++;
        }
        if (ISDIGIT(*p)) {
            p++;
            for (p; ISDIGIT(*p); p++);
        } else {
            throw PARSE_INVALID_VALUE; //E/e后必须有内容
        }
    }
    //C库函数 double strtod(const char *str, char **endptr)把参数str所指向的字符串转换为一个浮点数(类型为double型)。
    //如果 endptr 不为空，则指向转换中最后一个字符后的字符的指针会存储在endptr引用的位置。
    double num = strtod(txt, nullptr);
    txt = p;
    return collect(new Value(num));
}

Value* Parser::parse_string() {
    string str;
    txt++;
    while (*txt != '"') {
        if (*txt == '\0') {
            throw PARSE_MISS_QUOTATION_MARK;
        }
        str.push_back(*txt);
        txt++;
    }
    txt++;
    return collect(new Value(str));
}

//[ obj, obj, obj ]
Value* Parser::parse_array() {
    vector<Value* > array;
    txt++;
    parse_space();//可能存在空格
    if (*txt == ']') {
        txt++;
        return collect(new Value(array));
    }
    while (true) {
        array.push_back(parse_value());
        parse_space();
        if (*txt == ',') {
            txt++;
            parse_space();
        } else if (*txt == ']') {
            txt++;
            return collect(new Value(array));
        } else {
            throw PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
        }
    }
}

//{"str" : obj, "str' : [obj]}
Value* Parser::parse_object() {
    map<string, Value*> object;
    txt++;
    parse_space();
    if (*txt == '}') {
        txt++;
        return collect(new Value(object));
    }
    while (true) {
        if (*txt != '"') {
            throw PARSE_MISS_KEY;
        }
        txt++;
        pair<string, Value*> p;
        while (*txt != '"') {
            if (*txt == '\0') {
                throw PARSE_MISS_QUOTATION_MARK;
            }
            p.first.push_back(*txt);
            txt++;
        }
        txt++;
        parse_space();
        if (*txt != ':') {
            throw PARSE_MISS_COLON;
        }
        txt++;
        parse_space();
        p.second = parse_value();
        object.insert(p);
        parse_space();
        if (*txt == ',') {
            txt++;
            parse_space();
        } else if (*txt == '}') {
            txt++;
            return collect(new Value(object));
        } else {
            throw PARSE_MISS_COMMA_OR_CURLY_BRACKET;
        }
    }
}

Value* Parser::parse_value() {
    switch(*txt) {
        case 'n':
            return parse_null();
            break;
        case 'f':
            return parse_false();
            break;
        case 't':
            return parse_true();
            break;
        case '[':
            return parse_array();
            break;
        case '"':
            return parse_string();
            break;
        case '{':
            return parse_object();
            break;
        case '\0':
            throw PARSE_EXPECT_VALUE;
            break;
        default:
            return parse_number();
    }
}

Value* Parser::parse(char *json) {
    assert(json != nullptr);
    Parser parser(json);
    try {
        parser.parse_space();
        Value* v = parser.parse_value();
        parser.parse_space();
        if (*parser.txt != '\0') {
            throw  PARSE_ROOT_NOT_SINGULAR;
        }
        return  v;
    } catch (exceptType e) {
        for (auto &it : parser.collector) {
            delete(it);
        }
        throw e;
    }
}