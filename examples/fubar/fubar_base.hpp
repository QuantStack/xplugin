#ifndef FUBAR_BASE_HPP
#define FUBAR_BASE_HPP

#include <vector>
#include <string>

namespace fubar
{
    class FubarBase
    {
        public:
            virtual ~FubarBase() {}
            virtual std::string fubar() const = 0;
    };
}

#endif // FUBAR_BASE_HPP