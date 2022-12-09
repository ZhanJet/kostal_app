/**
 * @file Participant.cpp
 * @date Dec 1, 2022
 * @author Chenchen Liu
 */

#include "Participant.hpp"

namespace kostal {

std::shared_ptr<RdkClientPublisher> Participant::createPublisher(
    Node* node, std::string topicName)
{
    auto publisher = node->CreateDefaultPublisher<KostalPubSubType>(topicName);
    return publisher;
}

void Participant::publishMsg(std::shared_ptr<RdkClientPublisher> publisher,
    KostalMsg* pub_msg, uint8_t statusLight)
{
    pub_msg->status(statusLight);
    publisher->Publish((void*)pub_msg);
}

void Participant::publishMsg(
    std::shared_ptr<RdkClientPublisher> publisher, KostalMsg* pub_msg)
{
    pub_msg->cpha(std::to_string(g_CPHA));
    pub_msg->cpol(std::to_string(g_CPOL));
    pub_msg->lsb(std::to_string(g_LSB));
    pub_msg->selp(std::to_string(g_SELPOL));

    publisher->Publish((void*)pub_msg);
}

void Participant::publishMsg(std::shared_ptr<RdkClientPublisher> publisher,
    KostalMsg* pub_msg, std::string* task_name, std::string* task_type)
{
    pub_msg->task_name(*task_name);
    pub_msg->task_type(*task_type);

    publisher->Publish((void*)pub_msg);
}

} /* namespace kostal */