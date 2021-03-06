/*
 ` * GUIServer.cpp
 *
 *  Created on: 12.03.2014
 *      Author: Timo
 */

#include <Aquaduino.h>
#include <Framework/GUIServer.h>
#include <Arduino.h>
#include <Controller/ClockTimerController.h>

enum {
	GETVERSION = 0,
	GETALLSENSORS = 1,
	GETSENSORDATA = 2,
	SETSENSORCONFIG = 3,
	GETALLACTUATORS = 4,
	GETACTUATORDATA = 5,
	SETACTUATORDATA = 6,
	SETACTUATORCONFIG = 7,
	GETALLCONTROLLERS = 8,
	GETCLOCKTIMERS = 9
};

GUIServer::GUIServer(uint16_t port) {
	m_Port = port;
	m_UdpServer.begin(m_Port);
	Serial.println("GUIServer Start");
}

GUIServer::~GUIServer() {
}

int8_t GUIServer::receiveCommand() {
	if (!m_UdpServer.parsePacket()) {
		return 0;
	}

	Serial.print(F("UDP Packet of "));
	Serial.print(m_UdpServer.available());
	Serial.println(F(" Bytes available"));
	Serial.print(F("Got "));
	Serial.print(m_UdpServer.read(m_Buffer, sizeof(m_Buffer)));
	Serial.println(F(" Bytes"));
	return 1;
}

void GUIServer::run() {
	if (receiveCommand()) {
		m_UdpServer.beginPacket(m_UdpServer.remoteIP(),
				m_UdpServer.remotePort());
		m_UdpServer.write(m_Buffer[1]);
		m_UdpServer.write(m_Buffer[0]);

		Serial.print(F("Request ID: "));
		Serial.println(m_Buffer[0]);
		Serial.print(F("Method ID: "));
		Serial.println(m_Buffer[1]);
		Serial.print(F("value: "));
		Serial.println(m_Buffer[2]);
		switch (m_Buffer[1]) {
		case GETVERSION:
			m_UdpServer.write((uint8_t) 0);
			m_UdpServer.write(1);
			break;
		case GETALLSENSORS:
			getAllSensors();
			break;
		case GETSENSORDATA:
			getSensorData(m_Buffer[2]);
			break;
		case SETSENSORCONFIG:
			switch (m_Buffer[3]) {
			case 1:
				break;
			case 2:
				setSensorConfig(m_Buffer[2], m_Buffer[3], (char*) &m_Buffer[4]);
				break;
			case 3:
				setSensorConfig(m_Buffer[2], m_Buffer[3], (char*) &m_Buffer[4]);
				break;
			case 4:
				setSensorConfig(m_Buffer[2], m_Buffer[3],
						(uint8_t) m_Buffer[4]);
				break;
			case 5:
				setSensorConfig(m_Buffer[2], m_Buffer[3],
						(uint8_t) m_Buffer[4]);
				break;
			}
			break;
		case GETALLACTUATORS:
			getAllActuators();
			break;
		case GETACTUATORDATA:
			getActuatorData(m_Buffer[2]);
			break;
		case SETACTUATORDATA:
			switch (m_Buffer[3]) {
			case 1:
			case 3:
			case 4:
			case 5:
			case 6:
				setActuatorData(m_Buffer[2], m_Buffer[3],
						(uint8_t) m_Buffer[4]);
				break;
			case 2:
				setActuatorData(m_Buffer[2], m_Buffer[3], (char*) m_Buffer[4]);
			}
			break;
		case SETACTUATORCONFIG:
			switch (m_Buffer[3]) {
			case 1:
				break;
			case 2:
				setActuatorConfig(m_Buffer[2], m_Buffer[3],
						(char*) &m_Buffer[4]);
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			}
			break;
		case GETCLOCKTIMERS:
			getClockTimers(m_Buffer[2]);
			break;
		default:
			break;
		}
		m_UdpServer.endPacket();
	}

}
size_t GUIServer::write(uint32_t value, EthernetUDP* udpServer) {
	size_t res = 0;
	for (uint8_t x = 0; x < 4; x++) {
		res = udpServer->write(value & 255);
		value >> 8;
	}

	return res;

}
void GUIServer::getAllSensors() {
	//errorcode 0
	m_UdpServer.write((uint8_t) 0);
	//num of sensors
	m_UdpServer.write((uint8_t) __aquaduino->getNrOfSensors());
	//Serial.println("NrOfSensors()");
	// Serial.println(__aquaduino->getNrOfSensors());
	//sensor information
	Sensor* sensor;
	__aquaduino->resetSensorIterator();
	while (__aquaduino->getNextSensor(&sensor) != -1) {
		m_UdpServer.write(__aquaduino->getSensorID(sensor));
		//Name:String
		m_UdpServer.write(strlen(sensor->getName()));
		m_UdpServer.write(sensor->getName());
		//Type:int
		m_UdpServer.write(sensor->getType());
		//Unit:String
		m_UdpServer.write((uint8_t) 8);
		m_UdpServer.write("TestUnit");
		//visible:Boolean
		m_UdpServer.write(true);
		//calibrationInterval(days):int
		m_UdpServer.write((uint8_t) 0);
	}
}

void GUIServer::getSensorData(uint8_t sensorId) {
	Sensor* sensor = __aquaduino->getSensor(sensorId);

	//errorcode 0
	m_UdpServer.write((uint8_t) 0);

	//sensorId:int
	m_UdpServer.write(__aquaduino->getSensorValue(sensorId));
	//valueAct:float * 1000 -> uint32
	//m_UdpServer.write((uint32_t) __aquaduino->getSensorValue(sensorId) * 1000);
	write((uint32_t) __aquaduino->getSensorValue(sensorId) * 1000,
			&m_UdpServer);
	//valueMax24h:float * 1000 -> uint32
	m_UdpServer.write((uint32_t) 65800);
	//valueMax24hTime:time
	m_UdpServer.write((uint32_t) 1395867979);
	//valueMin24h:float
	m_UdpServer.write((uint32_t) 4.01);
	//valueMin24hTime:time
	m_UdpServer.write((uint32_t) 1395867979);
	//lastCalibration:dateTime
	m_UdpServer.write((uint32_t) 1395867979);
	//operatingHours:int
	m_UdpServer.write((uint8_t) 13);
	//lastOperatingHoursReset
	m_UdpServer.write((uint32_t) 1395867979);

}
void GUIServer::getAllActuators() {
	//errorcode 0
	m_UdpServer.write((uint8_t) 0);
	//num of actuators
	m_UdpServer.write((uint8_t) __aquaduino->getNrOfActuators());
	Serial.print("num of Actuators: ");
	Serial.println(__aquaduino->getNrOfActuators());
	//sensor information
	Actuator* actuator;
	__aquaduino->resetActuatorIterator();
	while (__aquaduino->getNextActuator(&actuator) != -1) {
		m_UdpServer.write(__aquaduino->getActuatorID(actuator));
		m_UdpServer.write(strlen(actuator->getName()));
		m_UdpServer.write(actuator->getName());
		//influencesStream:bool
		m_UdpServer.write((uint8_t) 0);
		//influencesHeat:bool
		m_UdpServer.write((uint8_t) 0);
		//ControllerSemanticValue:int
		m_UdpServer.write((uint8_t) 0);
		//calibrationInterval(days):int
		m_UdpServer.write((uint8_t) 0);
	}
}

void GUIServer::getActuatorData(uint8_t actuatorId) {
	Serial.println("getActuatorData");
	Actuator* actuator = __aquaduino->getActuator(actuatorId);

	//errorcode 0
	m_UdpServer.write((uint8_t) 0);
	//actuatorID:int
	m_UdpServer.write(actuatorId);
	//value:int
	m_UdpServer.write(actuator->isOn());
	//operatingHours:int
	m_UdpServer.write((uint8_t) 0);
	//lastOperatingHoursReset:dateTime
	m_UdpServer.write((uint32_t) 1395867979);
	//lastCalibration:dateTime
	m_UdpServer.write((uint32_t) 1395867979);
}

void GUIServer::setSensorConfig(uint8_t sensorId, uint8_t type, char* value) {
	Sensor* sensor = __aquaduino->getSensor(sensorId);
	if (type == 2) {
		sensor->setName(value);
	}
	if (type == 3) {
		//sensorUnit����not implemented yet
	}
	__aquaduino->writeConfig(sensor);
}
void GUIServer::setSensorConfig(uint8_t sensorId, uint8_t type, uint8_t value) {
	Serial.print("set Name: ");
	Serial.println(value);
	Sensor* sensor = __aquaduino->getSensor(sensorId);
	if (type == 1) {
		//sensor->resetOperatinHours();
	}
	if (type == 4) {
		//sensor->setVisible(visible)
	}
	if (type == 5) {
		//sensor->setCalibratioInterval(value)
	}
	//__aquaduino->writeConfig(sensor);
}

void GUIServer::setActuatorData(uint8_t actuatorId, uint8_t dataType,
		uint8_t data) {
	Actuator* actuator = __aquaduino->getActuator(actuatorId);
	if (dataType == 1) {
		//actuator->resetOperatingHours();
	}
	if (dataType == 3) {
		//actuator->influenceBitmask(data);
	}
	if (dataType == 4) {
		//actuator->controllerSemanticValue(data);
	}
	if (dataType == 5) {
		//actuator->calibrationInterval(data);
	}
	if (dataType == 6) {
		//actuator->assignedControllerID(data);
	}
	__aquaduino->writeConfig(actuator);
}

void GUIServer::setActuatorData(uint8_t actuatorId, uint8_t dataType,
		char* data) {
	Actuator* actuator = __aquaduino->getActuator(actuatorId);
	//Serial.println("setActuatorData: 2");
	if (dataType == 2) {
		actuator->setName(data);
	}
	__aquaduino->writeConfig(actuator);
}
void GUIServer::setActuatorConfig(uint8_t actuatorId, uint8_t type,
		char* value) {
	Actuator* actuator = __aquaduino->getActuator(actuatorId);
	if (type == 2) {
		actuator->setName(value);
	}
	if (type == 3) {
		//sensorUnit����not implemented yet
	}
	__aquaduino->writeConfig(actuator);
}
/*
 void GUIServer::setSensorConfig
 void GUIServer::setActuatorData
 */
void GUIServer::getAllControllers() {
	//errorcode 0
	m_UdpServer.write((uint8_t) 0);
	//num of actuators
	m_UdpServer.write((uint8_t) __aquaduino->getNrOfControllers());
	Serial.print("num of Controllers: ");
	Serial.println(__aquaduino->getNrOfControllers());
	//sensor information
	Controller* controller;
	__aquaduino->resetActuatorIterator();
	while (__aquaduino->getNextController(&controller) != -1) {
		m_UdpServer.write(__aquaduino->getControllerID(controller));
		m_UdpServer.write(strlen(controller->getName()));
		m_UdpServer.write(controller->getName());
		//Type
		m_UdpServer.write(controller->getType());
	}
}
void GUIServer::getClockTimers(uint8_t controllerId) {
	ClockTimerController* controller;
	if (__aquaduino->getController(controllerId)->getType()
			== CONTROLLER_CLOCKTIMER) {
		controller = (ClockTimerController*) __aquaduino->getController(
				controllerId);
	} else {
		//errorcode 10
		m_UdpServer.write((uint8_t) 10);
		return;
	}

	//num of timers
	m_UdpServer.write((uint8_t) MAX_CLOCKTIMERS);
	//num of timers per timer
	m_UdpServer.write((uint8_t) CLOCKTIMER_MAX_TIMERS);
	ClockTimer* timer;
	int i = 0;
	int j = 0;
	while (i < MAX_CLOCKTIMERS) {
		timer = controller->getClockTimer(i);
		//ClockTimerId
		m_UdpServer.write(i);
		while (j < CLOCKTIMER_MAX_TIMERS) {
			m_UdpServer.write((uint8_t) timer->getHourOn(j));
			m_UdpServer.write((uint8_t) timer->getMinuteOn(j));
			m_UdpServer.write((uint8_t) 0);/*timer->getSecondOn(j)*/
			m_UdpServer.write((uint8_t) timer->getHourOff(j));
			m_UdpServer.write((uint8_t) timer->getMinuteOff(j));
			m_UdpServer.write((uint8_t) 0);/*timer->getSecondOff(j)*/
			j++;
		}
		m_UdpServer.write(timer->isMondayEnabled());
		m_UdpServer.write(timer->isTuesdayEnabled());
		m_UdpServer.write(timer->isWednesdayEnabled());
		m_UdpServer.write(timer->isThursdayEnabled());
		m_UdpServer.write(timer->isFridayEnabled());
		m_UdpServer.write(timer->isSaturdayEnabled());
		m_UdpServer.write(timer->isSundayEnabled());
		m_UdpServer.write(controller->getAssignedActuatorID(i));
		i++;
	}
}
