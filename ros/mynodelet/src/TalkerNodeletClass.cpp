// this should really be in the implementation (.cpp file)
#include <pluginlib/class_list_macros.h>
#include <mynodelet/TalkerNodeletClass.h>

// watch the capitalization carefully
PLUGINLIB_EXPORT_CLASS(example_pkg::TalkerNodeletClass, nodelet::Nodelet)

namespace example_pkg
{
    void TalkerNodeletClass::onInit()
    {
        ROS_INFO("TalkerNodeletClass::onInit");

        m_nh = getNodeHandle();
        m_private_nh = getPrivateNodeHandle();

        m_timer = m_nh.createTimer(
            ros::Duration(1.0),
            boost::bind(& TalkerNodeletClass::timerCb, this, _1));
                                             
        m_pub = m_nh.advertise<std_msgs::String>("chatter", 1000);
    }

    void TalkerNodeletClass::timerCb(const ros::TimerEvent& event)
    {
        static int count = 0;
        
        std_msgs::String msg;
        std::stringstream ss;
        ss << "hello world " << count++;
        msg.data = ss.str();

        ROS_INFO("%s", msg.data.c_str());
        m_pub.publish(msg);
    }
}
