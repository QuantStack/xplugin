#ifndef SERIALIZE_JSON_HPP
#define SERIALIZE_JSON_HPP

#include <nlohmann/json.hpp>
#include <serialize_base.hpp>

#include <xplugin/xfactory.hpp>

class SerializeJson : public SerializeBase
{
  public:
    virtual ~SerializeJson() = default;

    std::string name() const override
    {
        return "json";
    }

    std::string serialize(const std::unordered_map<std::string, double> &data) const override
    {
        // convert to json
        nlohmann::json j;
        for (auto &kv : data)
        {
            j[kv.first] = kv.second;
        }
        return j.dump();
    }

    std::unordered_map<std::string, double> deserialize(const std::string &data) const override
    {
        // convert from json
        nlohmann::json j = nlohmann::json::parse(data);
        std::unordered_map<std::string, double> ret;
        for (auto &kv : j.items())
        {
            ret[kv.key()] = kv.value();
        }
        return ret;
    }
};

XPLUGIN_CREATE_XPLUGIN_FACTORY((xp::xfactory<SerializeJson, SerializeBase>));

#endif // SERIALIZE_JSON_HPP
