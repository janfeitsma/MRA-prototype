#include "json_convert.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>


std::string MRA::read_file_as_string(std::string filename)
{
    std::ifstream infile(filename);
    if (!infile)
    {
        std::filesystem::path cwd = std::filesystem::current_path();
        throw std::runtime_error("file " + filename + " not found at " + std::string(cwd));
    }
    std::ostringstream sstr;
    sstr << infile.rdbuf();
    return sstr.str();
}

