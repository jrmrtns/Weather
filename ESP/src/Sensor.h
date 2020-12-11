#pragma once

#include <Arduino.h>

typedef struct sensorData
{
	double temperature;
	double target;
} sensor_data;

extern sensorData lastdata;
extern double output;

class Sensor
{
public:
	void Initialize();
	String LastValue(void);
	sensorData Messure();
	static String ConvertToJson(sensorData value);
};