/**
 * @file publisher_example.cc
 * @copyright Flexiv Robotics 2021
 */

#include "Participant.hpp"

// Some const variables
const std::string robotIP = "127.0.0.1";
const std::string localIP = "127.0.0.1";
const std::string topicKostal = "topic_kostal_states";
const std::string topicPlan = "topic_plan_name";

// Create an array for storing tcpPose
std::array<double, 7> tcpPose_array;
// Create an array for storing flangePose
std::array<double, 7> flangePose_array;
// Create an array for storing rawForceSensorData
std::array<double, 6> rawForceSensor_array;

int main()
{
    // Log object for printing message with timestamp and coloring
    flexiv::Log log;
    try {
        // Instantiate robot interface
        flexiv::Robot robot(robotIP, localIP);

        // Create pub/sub messages respectively
        kostal_gui_msgs::msg::KostalLever pub_msg;
        plan_msgs::msg::PlanExecution sub_msg;

        // Create a participant and robot operations handler respectively
        flexiv::Participant participant;
        flexiv::RobotOperations robot_handler;

        // Get robot prepared
        int result = robot_handler.prepareRobot(&robot, &log);
        if (result == 1) {
            return 0;
        }

        // Create publisher node but do not publish yet
        std::unique_ptr<flexiv::middleware2::FastRTPSNode> pub_node;
        pub_node = std::make_unique<flexiv::middleware2::FastRTPSNode>(
            "rdk_publisher");
        auto publisher
            = participant.createPublisher(pub_node.get(), topicKostal);

        // Create subscriber node and subscribe to plan execution topic
        std::unique_ptr<flexiv::middleware2::FastRTPSNode> sub_node;
        sub_node = std::make_unique<flexiv::middleware2::FastRTPSNode>(
            "rdk_subscriber");
        auto subscriber = participant.createSubscriber(
            &robot_handler, sub_node.get(), topicPlan, &sub_msg, &robot, &log);

        // Keep fetching robot states in a loop
        while (true) {
            // First, fetch robot data to assigned arrays
            robot_handler.fetchRobotStates(&robot, &tcpPose_array,
                &flangePose_array, &rawForceSensor_array);

            // Then publish these arrays to assigned topic
            participant.publishTcpMsg(publisher, &pub_msg, &tcpPose_array,
                &flangePose_array, &rawForceSensor_array);
        }

    } catch (const flexiv::Exception& e) {
        log.error(e.what());
        return 1;
    }
    return 0;
}