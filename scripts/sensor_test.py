import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Joy
from std_msgs.msg import Float32


class SensorTest(Node):
    def __init__(self):
        super().__init__("sensor_test")
        self.dist_threshold = 100
        self.pub = self.create_publisher(Joy, "joy", 10)
        self.sub = self.create_subscription(Float32, "dist", self.dist_callback, 10)
        msg = Joy()
        msg.axes = [0, 0.2, 0, 0, 0, 0.2]
        self.pub.publish(msg)

    def dist_callback(self, msg):
        cmd = Joy()
        if msg.data < self.dist_threshold:
            self.get_logger().info("Detected obstacle and stop!")
            cmd.axes = [0, 0, 0, 0, 0, 0]
            self.pub.publish(cmd)
        else:
            cmd.axes = [0, 0.2, 0, 0, 0, 0.2]
            self.pub.publish(cmd)


def main():
    rclpy.init()
    node = SensorTest()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
