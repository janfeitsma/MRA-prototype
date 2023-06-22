#include "json_convert.hpp"
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>


std::string MRA::read_file_as_string(std::string filename)
{
    std::ifstream infile(filename);
    if (!infile)
    {
        boost::filesystem::path cwd = boost::filesystem::current_path();
        throw std::runtime_error("file " + filename + " not found at " + std::string(cwd.string()));
    }
    std::ostringstream sstr;
    sstr << infile.rdbuf();
    return sstr.str();
}

