/**
 * @file test_kostal.cpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 * @author chenchen.liu@flexiv.com
 */

// Kostal header files
#include <Communications.hpp>

int main()
{
    flexiv::Log log;
    Status result;
    try {
        // we check robot connection and set robot to plan execution mode
        flexiv::Robot robot(ROBOTADDRESS, LOCALADDRESS);

        // put this process into a loop
        while (true) {
            // Instantiation of the com object
            kostal::Communications com;

            // 1st Initialize the server
            result = com.init(&robot);
            if (result == SYSTEM) {
                // failed to initialize, maybe robot error maybe spi error, jump
                // to stateMachine
            } else if (result == SUCCESS) {
                // initialize successfully, jump to stateMachine
            } else {
                // if socket failed, directly break
                break;
            }

            // 2nd Put the flexiv system into a state machine
            com.stateMachine(&robot);

            // 3rd Check whether system is terminated by client
            if (com.checkTerminateStatus()) {
                break;
            }

            // 4th Check whether system meet a fault in loop
            if (com.checkSeriousErrorStatus()) {
                break;
            }
        }

    } catch (const flexiv::Exception& e) {
        log.error(e.what());
    }
}