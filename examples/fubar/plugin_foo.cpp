#include <fubar_base.hpp>
#include <xplugin/xfactory.hpp>


class Foo : public fubar::FubarBase
{
    public:

        Foo(const std::string name) : name_(name) {}
        virtual ~Foo() = default;

        std::string fubar() const override {
            return "foo " + name_;
        }
    private:
        std::string name_;
};

using factory_base_type = xp::xfactory_base<fubar::FubarBase, const std::string &>;

class FooFactory : public factory_base_type
{
    public:
        FooFactory() = default;
        std::unique_ptr<instance_type>  create(const std::string & name) override {
            return std::unique_ptr<instance_type>(new Foo(name));
        }
};

// extern c function to factory st. we get a demangled name
extern "C" factory_base_type * create_plugin_factory(){
    return new FooFactory();
}