#include "rclcpp/rclcpp.hpp"
#include "grid_map.h"
#include "robot.h"

class RobotController : public rclcpp::Node {
public:
    RobotController() : Node("robot_controller"), robot(0.0, 0.0) {
        RCLCPP_INFO(this->get_logger(), "Starting Robot Controller");
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100), std::bind(&RobotController::update, this));
    }

private:
    void update() {
        static float angle = M_PI / 4;
        float step_size = 4.0;

        robot.step(angle, step_size);
        RCLCPP_INFO(this->get_logger(), "Robot position: [%.2f, %.2f]",
                    robot.getPosition().x(), robot.getPosition().y());
    }

    rclcpp::TimerBase::SharedPtr timer_;
    Robot robot;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RobotController>());
    rclcpp::shutdown();
    return 0;
}
