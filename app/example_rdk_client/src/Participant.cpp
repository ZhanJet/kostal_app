/**
 * @file Participant.cpp
 * @date Nov 24, 2022
 * @author Chenchen Liu
 */

#include "Participant.hpp"

namespace flexiv {

std::shared_ptr<RdkClientPublisher> Participant::createPublisher(
    Node* node, std::string topicName)
{
    auto publisher = node->CreateDefaultPublisher<KostalPubSubType>(topicName);
    return publisher;
}

void Participant::publishTcpMsg(std::shared_ptr<RdkClientPublisher> publisher,
    KostalMsg* pub_msg, std::array<double, 7>* tcpPose_array,
    std::array<double, 7>* flangePose_array,
    std::array<double, 6>* rawForceSensor_array)
{
    // load data
    pub_msg->tcp_pose(*tcpPose_array);
    pub_msg->flange_pose(*flangePose_array);

    // publish data
    publisher->Publish((void*)pub_msg);
}

std::shared_ptr<RdkClientSubscriber> Participant::createSubscriber(
    RobotOperations* robotHandler, Node* node, std::string topicName,
    PlanMsg* sub_msg, flexiv::Robot* robot, flexiv::Log* log)
{
    // create function pointer that integrates other necessary params
    auto subCallback = std::bind(&RobotOperations::executeRobotPlan,
        robotHandler, robot, log, &(sub_msg->plan_name()),
        &(sub_msg->start_execution()), &(sub_msg->stop_execution()));

    // create a subscriber that can call subCallback func
    auto subscriber = node->CreateDefaultSubscription<PlanPubSubType>(
        topicName, subCallback, (void*)sub_msg);

    return subscriber;
}

} /* namespace flexiv */