#include "ros/ros.h"
#include "nodelet/nodelet.h"
#include "std_msgs/String.h"

namespace example_pkg
{
    class TalkerNodeletClass : public nodelet::Nodelet
    {
    public:
        virtual void onInit();

    private:
        void timerCb(const ros::TimerEvent& event);

        ros::NodeHandle m_nh;
        ros::NodeHandle m_private_nh;
        ros::Publisher m_pub;
        ros::Timer m_timer;
    };
}
