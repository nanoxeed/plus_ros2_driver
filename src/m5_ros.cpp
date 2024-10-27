#include "rclcpp/rclcpp.hpp"
#include "m5base_controller.hpp"


int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    RCLCPP_INFO(rclcpp::get_logger("plus_m5base"), "Starting plus_m5base node");
    rclcpp::spin(std::make_shared<M5baseController>());
    rclcpp::shutdown();

    return 0;
}
