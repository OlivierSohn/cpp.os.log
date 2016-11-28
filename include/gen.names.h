#pragma once

#include <string>

namespace imajuscule {
    class shortIds {
    public:
        static std::string generate();
    private:
        static std::string toString();
        static void increment();
        static int count;
        static char c;
    };
    
    template<char T> struct ascii;
    // with 'enum { value = ... }', the value at runtime was always 0 (don't know why) so I use constexpr instead
    template<> struct ascii<'0'> { static constexpr unsigned char value = 48; };
    template<> struct ascii<'9'> { static constexpr unsigned char value = 57; };
    template<> struct ascii<'A'> { static constexpr unsigned char value = 65; };
    template<> struct ascii<'Z'> { static constexpr unsigned char value = 90; };
    template<> struct ascii<'a'> { static constexpr unsigned char value = 97; };
    template<> struct ascii<'z'> { static constexpr unsigned char value = 122; };
    template<> struct ascii<'_'> { static constexpr unsigned char value = 137; };
    
    enum Case { UpperCase, LowerCase, AnyCase };

    template<Case C>
    struct charNameIterator {
        
        static unsigned char first() {
            return ascii<'0'>::value;
        }
        static unsigned char last() {
            return ascii<'_'>::value;
        }
        
        static char next(unsigned char v) {
            A(v >= first());
            A(v <= last());
            if(v == ascii<'9'>::value) {
                return (C == LowerCase) ? ascii<'a'>::value : ascii<'A'>::value;
            }
            if(C==AnyCase && v == ascii<'Z'>::value) {
                return ascii<'a'>::value;
            }
            if((C!=UpperCase && v == ascii<'z'>::value) || (C==UpperCase && v == ascii<'Z'>::value)) {
                return ascii<'_'>::value;
            }
            if(v == ascii<'_'>::value) {
                return ascii<'0'>::value;
            }
            return v+1;
        }
    };

    /*
     * Generates unique names with a particular case.
     */
    template<Case C = UpperCase>
    struct UniqueNames {
        using char_gen = charNameIterator<C>;

        UniqueNames()
        : name(1, static_cast<char>(char_gen::first()))
        {}
        
        static UniqueNames & getInstance() {
            static UniqueNames i;
            return i;
        }
        
        std::string next() {
            auto str = name;
            increment();
            return str;
        }
        
    private:
        void increment(){
            if(name.back() == char_gen::last()) {
                name.push_back(char_gen::first());
                return;
            }
            name.back() = char_gen::next(name.back());
        }
        std::string name;
    };
}
