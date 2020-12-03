#include "Sensor.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "ArduinoJson.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

void Sensor::Initialize()
{
	unsigned status;

	// default settings
	status = bme.begin();
	// You can also pass in a Wire library object like &Wire2
	// status = bme.begin(0x76, &Wire2)
	if (!status)
	{
		Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
		Serial.print("SensorID was: 0x");
		Serial.println(bme.sensorID(), 16);
		Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
		Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
		Serial.print("        ID of 0x60 represents a BME 280.\n");
		Serial.print("        ID of 0x61 represents a BME 680.\n");
		while (1)
			delay(10);
	}

	bme.setSampling(Adafruit_BME280::MODE_NORMAL,
									Adafruit_BME280::SAMPLING_X4, // temperature
									Adafruit_BME280::SAMPLING_X4, // pressure
									Adafruit_BME280::SAMPLING_X4, // humidity
									Adafruit_BME280::FILTER_X4);

	Serial.println(F("BME280 initilized"));

	Serial.println();
}

String Sensor::Messure()
{
	sensorData data = sensorData();
	data.pressure = (bme.readPressure() / 100.0F) + 50;
	data.temperature = bme.readTemperature();
	data.humidity = bme.readHumidity();
	m_lastValue = data;
	return ConvertToJson(data);
}

String Sensor::LastValue()
{
	return ConvertToJson(m_lastValue);
}

String Sensor::ConvertToJson(sensorData value)
{
	DynamicJsonDocument doc(JSON_OBJECT_SIZE(3));
	doc["temperature"] = value.temperature;
	doc["pressure"] = value.pressure;
	doc["humidity"] = value.humidity;

	char output[250];
	serializeJson(doc, output);
	return String(output);
}
