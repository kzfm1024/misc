// this should really be in the implementation (.cpp file)
#include <pluginlib/class_list_macros.h>
#include <mynodelet/ListenerNodeletClass.h>

// watch the capitalization carefully
PLUGINLIB_EXPORT_CLASS(example_pkg::ListenerNodeletClass, nodelet::Nodelet)

namespace example_pkg
{
    void chatterCallback(const std_msgs::String::ConstPtr& msg)
    {
        ROS_INFO("I heard: [%s]", msg->data.c_str());
    }

    void ListenerNodeletClass::onInit()
    {
        ROS_INFO("ListenerNodeletClass::onInit");

        m_nh = getNodeHandle();
        m_private_nh = getPrivateNodeHandle();

        // m_sub = m_nh.subscribe(
        //     "chatter", 1000,
        //     boost::bind(& ListenerNodeletClass::messageCb, this, _1));
        m_sub = m_nh.subscribe("chatter", 1000, chatterCallback);
    }

    void ListenerNodeletClass::messageCb(const std_msgs::String::ConstPtr& msg)
    {
        ROS_INFO("I heard: [%s]", msg->data.c_str());
    }
}
