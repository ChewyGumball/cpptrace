#include <cpptrace/cpptrace.hpp>

#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

std::string normalize_filename(std::string name) {
    if(name.find('/') == 0 || (name.find(':') == 1 && std::isupper(name[0]))) {
        // build/test if the file is really an object name resolved by libdl
        auto p = std::min({name.rfind("test/"), name.rfind("test\\"), name.rfind("build/test")});
        return p == std::string::npos ? name : name.substr(p);
    } else {
        return name;
    }
}

void trace() {
    for(const auto& frame : cpptrace::generate_trace()) {
        std::cout
            << normalize_filename(frame.filename)
            << "||"
            << frame.line
            << "||"
            << frame.symbol
            << std::endl;
    }
}

void www(std::string&&, const std::string& str, std::vector<std::string*>&& foobar) {
    trace();
}

void jjj(void(*const arr[5])(float)) {
    www(std::string{}, "", {});
}

namespace Foo {
    struct Bar {};
}

void iii(Foo::Bar) {
    jjj(nullptr);
}

struct S {
    int foo(float) const volatile && {
        return 1;
    }
};

void hhh(int(*(*)[10])[20]) {
    iii(Foo::Bar{});
}

void ggg(const int * const *) {
    hhh(nullptr);
}

void fff(int(S::*)(float) const volatile &&) {
    ggg(nullptr);
}

//void eee(int(*(*(*)[10])())(float)) {
void eee(int(*(* const * volatile(*)[10])())(float)) {
    fff(&S::foo);
}

void ddd(int(*(*)[10])()) {
    eee(nullptr);
}

void ccc(int(*)[5][6][7][8]) {
    ddd(nullptr);
}

void bbb(int(*const (&)[5])(float, const int&)) {
    ccc(nullptr);
}

void aaa(int(&)[5]) {
    int(*const (arr)[5])(float, const int&) = {};
    bbb(arr);
}

int x;

void foo(int n) {
    if(n == 0) {
        x = 0;
        int arr[5];
        aaa(arr);
        x = 0;
    } else {
        x = 0;
        foo(n - 1);
        x = 0;
    }
}

template<typename... Args>
void foo(int x, Args... args) {
    x = 0;
    foo(args...);
    x = 0;
}

void function_two(int, float) {
    x = 0;
    foo(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    x = 0;
}

void function_one(int) {
    x = 0;
    function_two(0, 0);
    x = 0;
}

int main() {
    x = 0;
    function_one(0);
    x = 0;
}
