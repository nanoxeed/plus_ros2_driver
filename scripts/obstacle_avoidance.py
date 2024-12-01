#!/usr/bin/python3
import time

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Joy
from std_msgs.msg import Float32


class ObstacleAvoidance(Node):
    def __init__(self):
        super().__init__("obstacle_avoidance")
        self.dist_threshold = 130
        self.pub = self.create_publisher(Joy, "joy", 10)
        self.sub = self.create_subscription(Float32, "dist", self.dist_callback, 10)
        msg = Joy()
        msg.axes = [0, 0.2, 0, 0, 0, 0.2]
        self.pub.publish(msg)

    def avoid_obstacle(self):
        cmd = Joy()
        cmd.axes = [0, 0, 0, 0, 0, 0]
        self.pub.publish(cmd)

        value = 0.2
        # Left turn
        cmd = Joy()
        cmd.axes = [0, -value, 0, 0, 0, value]
        self.pub.publish(cmd)
        time.sleep(3.5)

        # Forward
        cmd = Joy()
        cmd.axes = [0, 0.2, 0, 0, 0, 0.2]
        self.pub.publish(cmd)
        time.sleep(5.0)

        # Right turn
        cmd = Joy()
        cmd.axes = [0, value, 0, 0, 0, -value]
        self.pub.publish(cmd)
        time.sleep(3.5)

        # Forward
        cmd = Joy()
        cmd.axes = [0, 0.2, 0, 0, 0, 0.2]
        self.pub.publish(cmd)

    def dist_callback(self, msg):
        cmd = Joy()
        if msg.data < self.dist_threshold:
            self.get_logger().info("Detected obstacle and stop!")
            self.avoid_obstacle()
        else:
            cmd.axes = [0, 0.2, 0, 0, 0, 0.2]
            self.pub.publish(cmd)


def main():
    rclpy.init()
    node = ObstacleAvoidance()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
