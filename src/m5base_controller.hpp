#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>

class M5baseController : public rclcpp::Node
{
private:
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
    void joyCallback(const sensor_msgs::msg::Joy& joy);
public:
    M5baseController();
};
