

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "geometry_msgs/msg/pose2_d.hpp"
#include <iostream>

#include "../include/grid_map.h"
#include "../include/robot.h"

#define _USE_MATH_DEFINES
#define DATA_DIR "/home/martina/rp_project/robot-programming-project/src/simple_robot/data"

#include <cmath> // for M_PI


class KeyboardControlNode : public rclcpp::Node {
public:
    KeyboardControlNode() : Node("keyboard_control_node") {
        angle_pub_ = this->create_publisher<std_msgs::msg::Float32>("/robot_angle", 10);
        position_sub_ = this->create_subscription<geometry_msgs::msg::Pose2D>(
            "/robot_position", 10,
            std::bind(&KeyboardControlNode::position_callback, this, std::placeholders::_1));
    }

    void run() {
        while (rclcpp::ok()) {
            std::cout << "Use arrow keys to move (WASD for alternative controls), Q to quit:" << std::endl;
            char key;
            std::cin >> key;

            float angle = 0.0;
            switch (key) {
                case 'w': case 'W': angle = M_PI / 2; break;
                case 's': case 'S': angle = -M_PI / 2; break;
                case 'a': case 'A': angle = M_PI; break;
                case 'd': case 'D': angle = 0; break;
                case 'q': case 'Q': return;
                default: std::cout << "Invalid key." << std::endl; continue;
            }
            std::cout << "Publishing angle: " << angle << std::endl;
            auto msg = std_msgs::msg::Float32();
            msg.data = angle;
            angle_pub_->publish(msg);
        }
    }

private:
    void position_callback(const geometry_msgs::msg::Pose2D::SharedPtr msg) {
        std::cout << "Robot Position: (" << msg->x << ", " << msg->y << ")" << std::endl;
    }

    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr angle_pub_;
    rclcpp::Subscription<geometry_msgs::msg::Pose2D>::SharedPtr position_sub_;
};


class RobotNode : public rclcpp::Node {
public:
    RobotNode() : Node("robot_node") {
        angle_sub_ = this->create_subscription<std_msgs::msg::Float32>(
            "/robot_angle", 10,
            std::bind(&RobotNode::angle_callback, this, std::placeholders::_1));
        position_pub_ = this->create_publisher<geometry_msgs::msg::Pose2D>("/robot_position", 10);
    }

private:
    void angle_callback(const std_msgs::msg::Float32::SharedPtr msg) {
        std::cout << "Received angle: " << msg->data << std::endl;

        // Move the robot based on the angle
        robot_.step(msg->data, 0.5);

        // Publish the new position
        auto position_msg = geometry_msgs::msg::Pose2D();
        auto position = robot_.getPosition();
        position_msg.x = position[0];
        position_msg.y = position[1];
        position_pub_->publish(position_msg);
    }

    Robot robot_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr angle_sub_;
    rclcpp::Publisher<geometry_msgs::msg::Pose2D>::SharedPtr position_pub_;
};


class MapNode : public rclcpp::Node {
public:
MapNode() : Node("map_node"), grid_map_(100, 100, 0.1) {
    
    // Assuming you have the image path and resolution
    std::string imagePath =  std::string(DATA_DIR) + "/map.png";
    grid_map_.loadFromImage(imagePath, 0.1);  // Load map from image with the specified resolution

    map_pub_ = this->create_publisher<std_msgs::msg::Float32>("/map", 10);
    position_sub_ = this->create_subscription<geometry_msgs::msg::Pose2D>(
        "/robot_position", 10,
        std::bind(&MapNode::position_callback, this, std::placeholders::_1));
}


private:
    void position_callback(const geometry_msgs::msg::Pose2D::SharedPtr msg) {
        std::cout << "Received position: (" << msg->x << ", " << msg->y << ")" << std::endl;

        // Check if the robot is colliding with an obstacle
        bool hit = grid_map_.is_colliding(msg->x, msg->y);
        std::cout << "Robot is colliding: " << (hit ? "true" : "false") << std::endl;

        // Publish the map
        auto map_msg = std_msgs::msg::Float32();
        map_msg.data = hit ? 1.0 : 0.0;
        map_pub_->publish(map_msg);
    }

    GridMap grid_map_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr map_pub_;
    rclcpp::Subscription<geometry_msgs::msg::Pose2D>::SharedPtr position_sub_;
};


int main(int argc, char **argv) {
    rclcpp::init(argc, argv);

    // Create nodes
    auto control_node = std::make_shared<KeyboardControlNode>();
    auto robot_node = std::make_shared<RobotNode>();
    auto map_node = std::make_shared<MapNode>();

    // Run control loop for keyboard node
    std::thread control_thread([&]() { control_node->run(); });

    // Spin the nodes
    rclcpp::spin(robot_node);
    rclcpp::spin(map_node);

    control_thread.join();  // Ensure the control node thread finishes before shutting down

    rclcpp::shutdown();
    return 0;
}
