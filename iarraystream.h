#include <iostream>
namespace ICE {

class iarraystream : public std::istream {

    class iarraystreambuf : public std::streambuf {
    public:
        iarraystreambuf(char *array, size_t length) {
            setg(array, array, array + length);
        };
    };

    iarraystreambuf buf;
public:
    iarraystream(char *array, size_t length) : std::istream(NULL), buf(array, length) {
        rdbuf(&buf);
    };
};

};
