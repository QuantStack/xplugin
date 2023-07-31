#ifndef SERIALIZE_CSV_HPP
#define SERIALIZE_CSV_HPP

#include <serialize_base.hpp>
#include <sstream>

#include <xplugin/xfactory.hpp>

class SerializeSimple : public SerializeBase
{
  public:
    virtual ~SerializeSimple() = default;

    std::string name() const override
    {
        return "simple";
    }

    std::string serialize(const std::unordered_map<std::string, double> &data) const override
    {
        std::stringstream ss;
        for (auto &kv : data)
        {
            ss << kv.first << " " << kv.second << " ";
        }
        return ss.str();
    }

    std::unordered_map<std::string, double> deserialize(const std::string &data) const override
    {
        std::unordered_map<std::string, double> ret;
        std::stringstream ss(data);
        std::string key;
        double value;
        while (ss >> key >> value)
        {
            ret[key] = value;
        }
        return ret;
    }
};

XPLUGIN_CREATE_XPLUGIN_FACTORY((xp::xfactory<SerializeSimple, SerializeBase>));

#endif // SERIALIZE_CSV_HPP
