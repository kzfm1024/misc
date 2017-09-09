#include "ros/ros.h"
#include "nodelet/nodelet.h"
#include "std_msgs/String.h"

namespace example_pkg
{
    class ListenerNodeletClass : public nodelet::Nodelet
    {
    public:
        virtual void onInit();

    private:
        void messageCb(const std_msgs::String::ConstPtr& msg);

        ros::NodeHandle m_nh;
        ros::NodeHandle m_private_nh;
        ros::Subscriber m_sub;
    };
}
