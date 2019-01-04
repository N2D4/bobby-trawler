#ifndef BOBBY_TRAWLER_VARIOUS_INTSTRUCT_H_
#define BOBBY_TRAWLER_VARIOUS_INTSTRUCT_H_

/**
 * A type of structures that have a backing integer data value. Implements the == and != operators for convenience.
 */
struct IntStruct {
    protected:
        int data;
        constexpr IntStruct(const int data) : data(data) { }
    
    public:
        constexpr bool operator==(const IntStruct& w) const { return this->data == w.data; }
        constexpr bool operator!=(const IntStruct& w) const { return this->data != w.data; }
};

#endif  // BOBBY_TRAWLER_VARIOUS_INTSTRUCT_H_


