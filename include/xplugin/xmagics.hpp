#ifndef XPLUGIN_MAGICS_HPP
#define XPLUGIN_MAGICS_HPP

#include <map>
#include <memory>
#include <string>

namespace xp // Change namespace to xp for xplugin
{
    enum struct xmagic_type
    {
        cell,
        line
    };

    struct xmagic_line
    {
        virtual ~xmagic_line() = default;
        virtual void operator()(const std::string& line) = 0;
    };

    struct xmagic_cell
    {
        virtual ~xmagic_cell() = default;
        virtual void operator()(const std::string& line, const std::string& cell) = 0;
    };

    struct xmagic_line_cell : public xmagic_line,
                              xmagic_cell
    {
    };
}
#endif