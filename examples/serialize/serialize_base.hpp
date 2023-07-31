#ifndef SERIALIZE_BASE_HPP
#define SERIALIZE_BASE_HPP

#include <string>
#include <unordered_map>

class SerializeBase
{
  public:
    virtual ~SerializeBase() = default;
    virtual std::string name() const = 0;
    virtual std::string serialize(const std::unordered_map<std::string, double> &data) const = 0;
    virtual std::unordered_map<std::string, double> deserialize(const std::string &data) const = 0;
};

#endif // SERIALIZE_BASE_HPP
