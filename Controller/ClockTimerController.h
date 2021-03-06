/*
 * Copyright (c) 2012 Timo Kerstan.  All right reserved.
 *
 * This file is part of Aquaduino.
 *
 * Aquaduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Aquaduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Aquaduino.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CLOCKTIMERCONTROLLER_H_
#define CLOCKTIMERCONTROLLER_H_

#include <Framework/Controller.h>
#include "ClockTimer.h"

/**
 * \brief Controller implementation to provide clocktimers for actuators.
 *
 * The number of available clocktimers is defined by #MAX_CLOCKTIMERS.
 * The number of timers per clocktimer is defined by #CLOCKTIMER_MAX_TIMERS
 *
 * The class implements a mapping of its assigned actuators to its clocktimers
 * using the array m_ActuatorMapping. Within this array the index(key) of
 * the actuator assigned to the clocktimer are stored. m_ActuatorMapping[0] = 0
 * represents the mapping of clocktimer 0 to the actuator with index(key) 0 of
 * the Aquaduino object.
 *
 * The WebInterface of this controller allows for the assignment of actuators
 * to clocktimers. Only one actuator must be assigned to a a single clocktimer.
 *
 */
class ClockTimerController: public Controller
{
public:
    ClockTimerController(const char* name);
    virtual ~ClockTimerController();

    ClockTimer* getClockTimer(int8_t id);
    void  assignActuatorToClockTimer(int8_t clockTimerID, int8_t actuatorID);
    int8_t getAssignedActuatorID(int8_t clockTimerID);

    virtual uint16_t serialize(Stream* s);
    virtual uint16_t deserialize(Stream* s);

    virtual int8_t run();

private:
    ClockTimerController(ClockTimerController&);
    ClockTimerController(const ClockTimerController&);

    ClockTimer m_Timers[MAX_CLOCKTIMERS];
    int8_t m_ActuatorMapping[MAX_CLOCKTIMERS];
    int8_t m_SelectedTimer;
    int8_t m_SelectedActuator;
};

#endif /* CLOCKTIMERCONTROLLER_H_ */
