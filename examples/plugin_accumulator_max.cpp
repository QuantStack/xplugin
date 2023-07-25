#include <accumulator_base.hpp>


class AccumulatorMax : public acc::AccumulatorBase
{
    public:

        AccumulatorMax() = default;
        virtual ~AccumulatorMax() = default;

        virtual std::vector<double> accumulate(const std::vector<double>& data) override
        {
            return std::vector<double>(1,*std::max_element(data.begin(), data.end()));
        }
        virtual std::string name() const override{
            return "max";
        }
};

class AccumulatorMaxFactory : public xp::xfactory_base<acc::AccumulatorBase>
{
    public:
        AccumulatorMaxFactory() = default;
        std::unique_ptr<instance_type>  create() override {
            return std::unique_ptr<instance_type>(new AccumulatorMax());
        }
};

// extern c function to factory st. we get a demangled name
extern "C" xp::xfactory_base<acc::AccumulatorBase> * create_plugin_factory(){
    return new AccumulatorMaxFactory();
}

