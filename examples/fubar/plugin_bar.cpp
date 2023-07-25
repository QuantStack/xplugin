#include <fubar_base.hpp>
#include <xplugin/xfactory.hpp>

#include <iostream>

class Bar : public fubar::FubarBase
{
    public:

        Bar(const std::string name) : name_(name) {}
        virtual ~Bar() = default;

        std::string fubar() const override {
            return "bar " + name_;
        }
    private:
        std::string name_;
};

using factory_base_type = xp::xfactory_base<fubar::FubarBase, const std::string &>;


class BarFactory : public factory_base_type
{
    public:
        BarFactory() = default;
        std::unique_ptr<instance_type>  create(const std::string & name) override {
            std::cout << "BarFactory::create from " << name << std::endl;
            return std::unique_ptr<instance_type>(new Bar(name));
        }
};

// extern c function to factory st. we get a demangled name
extern "C" factory_base_type * create_plugin_factory(){
    std::cout << "create_plugin_factory " << std::endl;
    return new BarFactory();
}