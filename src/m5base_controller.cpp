#include  "rclcpp/rclcpp.hpp"
#include  "sensor_msgs/msg/joy.hpp"
#include  "m5base_controller.hpp"
#include  "motor_control.hpp"

#define DEF_IP  "192.168.0.162"

MotorControl  motor_control = MotorControl();

M5baseController::M5baseController()
: Node("plus_m5base"){

    std::string plus_ip = DEF_IP;
    this->get_parameter("plus_ip", plus_ip);
    strcpy(motor_control.m_ip, plus_ip.c_str());

    subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
        "/joy",
        rclcpp::QoS(10),
        std::bind(&M5baseController::joyCallback, this, std::placeholders::_1)
    );
    motor_control.start();
}

void M5baseController::joyCallback(const sensor_msgs::msg::Joy& joy) {
    motor_control.drive(-(int)(joy.axes[5] * 63.0), -(int)(joy.axes[1] * 63.0));
    RCLCPP_INFO(this->get_logger(), "Joy: %d, %d", -(int)(joy.axes[5] * 63.0), -(int)(joy.axes[1] *63.0));
}
