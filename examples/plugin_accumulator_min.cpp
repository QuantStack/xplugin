#include <accumulator_base.hpp>


class AccumulatorMin : public acc::AccumulatorBase
{
    public:

        AccumulatorMin() = default;
        virtual ~AccumulatorMin() = default;

        virtual std::vector<double> accumulate(const std::vector<double>& data) override
        {
            return std::vector<double>(1,*std::min_element(data.begin(), data.end()));
        }
        virtual std::string name() const override{
            return "min";
        }
};



class AccumulatorMinFactory : public xp::xfactory_base<acc::AccumulatorBase>
{
    public:
        AccumulatorMinFactory() = default;
        std::unique_ptr<instance_type>  create() override {
            return std::unique_ptr<instance_type>(new AccumulatorMin());
        }
};

// extern c function to factory st. we get a demangled name
extern "C" xp::xfactory_base<acc::AccumulatorBase> * create_plugin_factory(){
    return new AccumulatorMinFactory();
}