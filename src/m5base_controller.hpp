#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <std_msgs/msg/float32.hpp>

class M5baseController : public rclcpp::Node
{
private:
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_dist_;
    rclcpp::TimerBase::SharedPtr timer_;
    void joyCallback(const sensor_msgs::msg::Joy& joy);
    void timerCallback();
public:
    M5baseController();
};
