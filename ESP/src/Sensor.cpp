#include "Sensor.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "ArduinoJson.h"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

void Sensor::Initialize()
{
	unsigned status;

	status = bme.begin();
	if (!status)
	{
		Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
		Serial.print("SensorID was: 0x");
		Serial.println(bme.sensorID(), 16);
		while (1)
			delay(10);
	}

	bme.setSampling(Adafruit_BME280::MODE_NORMAL,
									Adafruit_BME280::SAMPLING_X1, // temperature
									Adafruit_BME280::SAMPLING_X1, // pressure
									Adafruit_BME280::SAMPLING_X1, // humidity
									Adafruit_BME280::FILTER_OFF);

	Serial.println(F("BME280 initilized"));

	Serial.println();
}

sensorData Sensor::Messure()
{
	sensorData data = sensorData();
	data.temperature = bme.readTemperature();
	data.target = lastdata.target;
	lastdata = data;
	return data;
}

String Sensor::LastValue()
{
	return ConvertToJson(lastdata);
}

String Sensor::ConvertToJson(sensorData value)
{
	DynamicJsonDocument doc(JSON_OBJECT_SIZE(3));
	doc["temperature"] = value.temperature;
	doc["output"] = output;
	doc["target"] = lastdata.target;

	char output[250];
	serializeJson(doc, output);
	return String(output);
}
