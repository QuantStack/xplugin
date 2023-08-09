#include <fubar_base.hpp>
#include <xplugin/xfactory.hpp>

class XPLUGIN_API Bar : public fubar::FubarBase
{
  public:
    Bar(const std::string name)
        : name_(name)
    {
    }
    virtual ~Bar() = default;

    std::string fubar() const override
    {
        return "bar " + name_;
    }

  private:
    std::string name_;
};

template class XPLUGIN_API xp::xfactory<Bar, fubar::FubarBase, const std::string &>;

using factory_type = xp::xfactory<Bar, fubar::FubarBase, const std::string &>;
XPLUGIN_CREATE_XPLUGIN_FACTORY(factory_type);
