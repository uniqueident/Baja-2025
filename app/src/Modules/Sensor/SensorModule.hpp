#include "Core/Pins/pins.hpp"
#include "Core/SharedData.hpp"
#include "Modules/moduletemplate.hpp"
namespace BB{

    class Sensor:public Module{
        public:
            virtual void Init(SharedData* data, phyisical_to_virtual* pincontrol)=0;
            //get data set data and pin control in public;
        protected:
            phyisical_to_virtual* pincontrol =nullptr;
    };

}