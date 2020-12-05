#pragma once

#include <Arduino.h>

typedef struct sensorData
{
	double temperature;
	double target;
} sensor_data;

extern sensorData lastdata;

class Sensor
{
public:
	void Initialize();
	String LastValue(void);
	sensorData Messure();
	static String ConvertToJson(sensorData value);
};