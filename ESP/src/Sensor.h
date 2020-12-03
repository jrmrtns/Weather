#pragma once

#include <Arduino.h>

typedef struct sensorData
{
	float temperature;
	float humidity;
	float pressure;
} sensor_data;

class Sensor
{
public:
	void Initialize();
	String LastValue(void);
	String Messure();
private:
	String ConvertToJson(sensorData value);

	sensorData m_lastValue;
};