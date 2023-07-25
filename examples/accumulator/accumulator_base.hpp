#include <vector>
#include <string>

namespace acc
{
    class AccumulatorBase
    {
        public:

            virtual ~AccumulatorBase() {}

            virtual double accumulate(const std::vector<double>& data) = 0;
            virtual std::string name() const = 0;
    };
}