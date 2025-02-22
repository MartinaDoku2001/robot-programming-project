
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/int32.hpp"
#include "geometry_msgs/msg/pose2_d.hpp"
#include <iostream>
#include <cmath>

#include "../include/grid_map.h"
#include "../include/robot.h"

#define _USE_MATH_DEFINES
#define DATA_DIR "/home/martina/rp_project/robot-programming-project/src/simple_robot/data"

class KeyboardControlNode : public rclcpp::Node {
public:
    KeyboardControlNode() : Node("keyboard_control_node"), laser_active_(false) {
        angle_pub_ = this->create_publisher<std_msgs::msg::Float32>("/robot_angle", 10);
        laser_pub_ = this->create_publisher<std_msgs::msg::Int32>("/laser_toggle", 10);
        key_sub_ = this->create_subscription<std_msgs::msg::Int32>(
            "/key", 10, 
            std::bind(&KeyboardControlNode::key_callback, this, std::placeholders::_1));
    }

private:
    void key_callback(const std_msgs::msg::Int32::SharedPtr msg) {
        key = msg->data;
        float angle = 0.0;
        switch (key) {
            case 82: angle = M_PI / 2; break; // Up
            case 84: angle = -M_PI / 2; break; // Down
            case 81: angle = M_PI; break; // Left
            case 83: angle = 0; break; // Right
            case 113: // 'q' key to toggle laser
            {
                laser_active_ = !laser_active_;
                std::cout << "Laser " << (laser_active_ ? "activated" : "deactivated") << std::endl;
                auto laser_msg = std_msgs::msg::Int32();
                laser_msg.data = laser_active_;
                laser_pub_->publish(laser_msg);
                std::cout << "Publishing laser active: " << laser_active_ << "from keyboard control node\n";
                break;
            }
            case -1: angle = -1; break; // No key pressed
            default:
                std::cout << "Other key: " << key << "\n";
        }

        auto angle_msg = std_msgs::msg::Float32();
        angle_msg.data = angle;
        angle_pub_->publish(angle_msg);
        std::cout << "Publishing angle: " << angle << "from keyboard control node\n";
    }


    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr angle_pub_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr laser_pub_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr key_sub_;
    
    int key = -1;
    bool laser_active_;
};

class RobotNode : public rclcpp::Node {
public:
    RobotNode() : Node("robot_node"), grid_map_(100, 100, 0.1), laser_active_(false), laser_angle_(0.0) {
        std::string imagePath = std::string(DATA_DIR) + "/map.png";
        grid_map_.loadFromImage(imagePath, 0.1);

        angle_sub_ = this->create_subscription<std_msgs::msg::Float32>(
            "/robot_angle", 10, 
            std::bind(&RobotNode::angle_callback, this, std::placeholders::_1));

        laser_sub_ = this->create_subscription<std_msgs::msg::Int32>(
            "/laser_toggle", 10, 
            std::bind(&RobotNode::laser_callback, this, std::placeholders::_1));

        position_pub_ = this->create_publisher<geometry_msgs::msg::Pose2D>("/robot_position", 10);

        laser_angle_pub_ = this->create_publisher<std_msgs::msg::Float32>("/laser_angle", 10);


        

        laser_timer_ = this->create_wall_timer(
            std::chrono::milliseconds(10), 
            std::bind(&RobotNode::laser_spin, this));
    }

private:
    void angle_callback(const std_msgs::msg::Float32::SharedPtr msg) {
        if (msg->data == -1) {
            //publish the same position
            auto position_msg = geometry_msgs::msg::Pose2D();
            position_msg.x = robot_.getPosition()[0];
            position_msg.y = robot_.getPosition()[1];
            position_pub_->publish(position_msg);
            std::cout << "Publishing position: " << position_msg.x << ", " << position_msg.y << "from robot node -no change\n";
            return;
        }
        robot_.step(msg->data, 0.5);

        float x = robot_.getPosition()[0];
        float y = robot_.getPosition()[1];
        bool hit = grid_map_.is_colliding(x, y);

        if (hit) {
            std::cout << "Robot is colliding with an obstacle\n";
            robot_.step(msg->data + M_PI, 0.5);
        }

        auto position_msg = geometry_msgs::msg::Pose2D();
        position_msg.x = robot_.getPosition()[0];
        position_msg.y = robot_.getPosition()[1];
        position_pub_->publish(position_msg);
        std::cout << "Publishing position: " << position_msg.x << ", " << position_msg.y << "from robot node\n";
    }

    void laser_callback(const std_msgs::msg::Int32::SharedPtr msg) {
        laser_active_ = msg->data;
        std::cout << "Laser " << (laser_active_ ? "ON" : "OFF") << std::endl;
    }

    void laser_spin() {
        if (laser_active_) {
            laser_angle_ += M_PI / 18; // Increment by 10 degrees per cycle
            if (laser_angle_ > 2 * M_PI) {
                laser_angle_ -= 2 * M_PI;
            }
    
            // Publish the laser angle
            auto laser_angle_msg = std_msgs::msg::Float32();
            laser_angle_msg.data = laser_angle_;
            laser_angle_pub_->publish(laser_angle_msg);
            std::cout << "Publishing laser angle: " << laser_angle_ << "from robot node\n";
               
        }
     

    }
    

    GridMap grid_map_;
    Robot robot_ = Robot(-40.0, -27.0);
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr angle_sub_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr laser_sub_;
    rclcpp::Publisher<geometry_msgs::msg::Pose2D>::SharedPtr position_pub_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr laser_angle_pub_;
    rclcpp::TimerBase::SharedPtr laser_timer_;

    bool laser_active_;
    float laser_angle_;
};

class MapNode : public rclcpp::Node {
    public:
        MapNode() : Node("map_node"), grid_map_(100, 100, 0.1), laser_active_(false), laser_angle_(0.0) {
            std::string imagePath = std::string(DATA_DIR) + "/map.png";
            grid_map_.loadFromImage(imagePath, 0.1);
    
            map_pub_ = this->create_publisher<std_msgs::msg::Float32>("/map", 10);
            key_pub_ = this->create_publisher<std_msgs::msg::Int32>("/key", 10);
    
            position_sub_ = this->create_subscription<geometry_msgs::msg::Pose2D>(
                "/robot_position", 10, 
                std::bind(&MapNode::position_callback, this, std::placeholders::_1));
    
            laser_sub_ = this->create_subscription<std_msgs::msg::Int32>(
                "/laser_toggle", 10, 
                std::bind(&MapNode::laser_callback, this, std::placeholders::_1));
    
            laser_angle_sub_ = this->create_subscription<std_msgs::msg::Float32>(
                "/laser_angle", 10, 
                std::bind(&MapNode::laser_angle_callback, this, std::placeholders::_1));
    
            // Initialize a timer that runs every 100ms
            refresh_timer_ = this->create_wall_timer(
                std::chrono::milliseconds(100), 
                std::bind(&MapNode::refresh_callback, this));
            
            // Display the map
            display_map();

        }
    
    private:
        void refresh_callback() {
            if (laser_active_) {
                display_map();  // Refresh the map only if laser is active
            }
        }
    
        void display_map() {
            Canvas canvas;
            grid_map_.draw(canvas);
            robot_.draw(canvas, grid_map_, 127, 5);
            
            if (laser_active_) {
                float origin_x = robot_.getPosition()[0], origin_y = robot_.getPosition()[1];
                float dir_x = cos(laser_angle_);
                float dir_y = sin(laser_angle_);
                float max_range = 10.0;
                Vector2f destination;
                grid_map_.scanRay(destination, Vector2f(origin_x, origin_y), Vector2f(dir_x, dir_y), max_range);
                drawLine(canvas, grid_map_.gm.world2grid(Vector2f(origin_x, origin_y)).cast<int>(), 
                         grid_map_.gm.world2grid(destination).cast<int>(), 127);
            }
    
            key = showCanvas(canvas, 10);
            auto key_msg = std_msgs::msg::Int32();
            key_msg.data = key;
            key_pub_->publish(key_msg);
            std::cout << "Publishing key: " << key << "from map node\n";
        }
    
        void position_callback(const geometry_msgs::msg::Pose2D::SharedPtr msg) {
            robot_.setPosition(msg->x, msg->y); 
            
            // If laser is inactive, update the map only on position changes
            if (!laser_active_) {
                display_map();  // Refresh map when the position changes but laser is off
            }
        }
    
        void laser_callback(const std_msgs::msg::Int32::SharedPtr msg) {
            laser_active_ = msg->data;
    
            // When laser state changes, update the map accordingly
            if (laser_active_) {
                display_map();
            }
        }
    
        void laser_angle_callback(const std_msgs::msg::Float32::SharedPtr msg) {
            std::cout << "Received laser angle: " << msg->data << std::endl;
            laser_angle_ = msg->data;
    
            // If laser is active, update the map
            if (laser_active_) {
                display_map();
            }
        }
    
        GridMap grid_map_;
        Robot robot_ = Robot(-40.0, -27.0);
        int key = -1;
        bool laser_active_;
        float laser_angle_;
    
        rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr map_pub_;
        rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr key_pub_;
        rclcpp::Subscription<geometry_msgs::msg::Pose2D>::SharedPtr position_sub_;
        rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr laser_sub_;
        rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr laser_angle_sub_;
    
        rclcpp::TimerBase::SharedPtr refresh_timer_; // Timer to refresh the display
    };
    
    
int main(int argc, char **argv) {
    rclcpp::init(argc, argv);

    // Create nodes
    auto control_node = std::make_shared<KeyboardControlNode>();
    auto robot_node = std::make_shared<RobotNode>();
    auto map_node = std::make_shared<MapNode>();

    // Run control loop for keyboard node

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(control_node);
    executor.add_node(robot_node);
    executor.add_node(map_node);
    
    std::thread spin_thread([&]() { executor.spin(); });
    spin_thread.join();
    
    rclcpp::shutdown();
    return 0;
}
