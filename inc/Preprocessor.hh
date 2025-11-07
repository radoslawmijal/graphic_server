#ifndef PREPROCESSOR_HH
#define PREPROCESSOR_HH

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

class Preprocessor {
public:
    Preprocessor();

    static std::string ProcessFile(const std::string& input_file);
};

#endif 