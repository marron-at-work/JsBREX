#pragma once

#include "../include/brex/regex/brex.h"
#include "../include/brex/regex/brex_parser.h"
#include "../include/brex/regex/brex_compiler.h"

class NSRemapInfo {
    std::string ns; //the namespace that this remap is for
    std::map<std::string, std::string> remap; //the remap rules

    NSRemapInfo(std::string ns) : ns(ns) { ; }
    ~NSRemapInfo() = default;
};

class RegexInfo {
public:
    std::string ns;
    std::string name;

    std::u8string restr;
    std::set<std::string> usedNamedREs;

    RegexInfo(std::string ns, std::string name, std::u8string restr) : ns(ns), name(name), restr(restr) { ; }
    ~RegexInfo() = default;
};
