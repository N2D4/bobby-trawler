#include "intstruct.h"

IntStruct::IntStruct(int data) : data(data) { }

bool IntStruct::operator==(const IntStruct& w) {
    return this->data == w.data;
}

bool IntStruct::operator!=(const IntStruct& w) {
    return this->data != w.data;
}
