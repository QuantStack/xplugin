#include <fubar_base.hpp>
#include <xplugin/xfactory.hpp>

class Foo : public fubar::FubarBase
{
    public:

    Foo(const std::string name) : name_(name) 
    {
    }

    virtual ~Foo() = default;

    std::string fubar() const override 
    {
        return "foo " + name_;
    }

    private:
    std::string name_;
};

using factory_type = xp::xfactory<Foo, fubar::FubarBase, const std::string &>;
using factory_base_type = typename factory_type::factory_base_type;

// extern c function to factory st. we get a demangled name
extern "C" factory_base_type * create_plugin_factory(){
    return new factory_type();
}