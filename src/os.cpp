#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../include/xplugin/os.hpp" // Updated include path

namespace xp
{
    void writefile::operator()(const std::string& line, const std::string& cell)
    {
        std::cout << "writefile::operator()\n";
    }
}
