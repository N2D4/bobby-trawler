#ifndef BOBBY_TRAWLER_VARIOUS_INTSTRUCT_H_
#define BOBBY_TRAWLER_VARIOUS_INTSTRUCT_H_

/**
 * A type of structures that have a backing integer data value. Implements the == and != operators for convenience.
 */
struct IntStruct {
    protected:
        int data;
        IntStruct(const int data);
    
    public:
        bool operator==(const IntStruct& w);
        bool operator!=(const IntStruct& w);
};

#endif  // BOBBY_TRAWLER_VARIOUS_INTSTRUCT_H_


