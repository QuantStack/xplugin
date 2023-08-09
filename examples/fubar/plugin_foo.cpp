#include <fubar_base.hpp>
#include <xplugin/xfactory.hpp>

class Foo : public fubar::FubarBase
{
  public:
    Foo(const std::string name)
        : name_(name)
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

template class XPLUGIN_API xp::xfactory<Foo, fubar::FubarBase, const std::string &>;

using factory_type = xp::xfactory<Foo, fubar::FubarBase, const std::string &>;
XPLUGIN_CREATE_XPLUGIN_FACTORY(factory_type);
