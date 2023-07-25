#include <vector>
#include <string>



namespace fubar
{
    class FubarBase
    {
        public:
            virtual ~FubarBase() {}
            virtual std::string fubar() const = 0;
    };
}