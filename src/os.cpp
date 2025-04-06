 #include <fstream>
 #include <iostream>
 #include <string>
 #include <vector>
 
 #include "os.hpp"
 
 namespace xp
 {
    void writefile::operator()(const std::string& line, const std::string& cell)
    {
        std::cout << "writefile::operator()\n";
    }
}
 