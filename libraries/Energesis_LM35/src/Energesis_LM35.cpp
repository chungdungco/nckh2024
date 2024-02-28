/**
 * @file Energesis_LM35.cpp
 * @author José Guerra Carmenate <joseguerracarmenate@gmail.com>
 * @brief Implementación del controlador de LM35 para el framework Arduino 
 * @version 0.1
 * @date 2022-05-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <Arduino.h>
#include "Energesis_LM35.h"


namespace Energesis{

void Energesis_LM35::getSensorDetails( sensor_details_t *details ){

    switch (m_serie) {
      case LM35_C:
        strlcpy(details->name, "LM35C", sizeof(details->name) );
        details->max_value = 110;
        details->min_value = -40;
        break;

      case LM35_CA:
        strlcpy(details->name, "LM35CA", sizeof(details->name) );
        details->max_value = 110;
        details->min_value = -40;
        break;
      
      case LM35_D:
        strlcpy(details->name, "LM35D", sizeof(details->name) );
        details->max_value = 100;
        details->min_value = 0;
        break;
        
      case LM35_A:
        strlcpy(details->name, "LM35A", sizeof(details->name) );
        details->max_value = 150;
        details->min_value = -55;
        break;

      case LM35:
      default:
        strlcpy(details->name, "LM35", sizeof(details->name) );
        details->max_value = 150;
        details->min_value = -55;
        break;
    }

    details->power = 0.060; //mA
    details->resolution = 1; // ºC
    details->sensor_id = 0;
    details->version = ENERGESIS_LM35_VERSION_MAJOR;
    details->type = SENSOR_TYPE_TEMPERATURE;
}

inline uint16_t Energesis_LM35::getRaw(){
  return analogRead(m_pin);
}

float Energesis_LM35::getTemperature(){
  #if defined(ESP32)
    return float(analogReadMilliVolts(m_pin))/10.0;
  #else
    return (((float)analogRead(m_pin))*getVref())/(1<<getAnalogResolution())/10.0;
  #endif


}

bool Energesis_LM35::getSample( sensor_sample_t *sample ){
  sample->temperature = getTemperature();
  sample->type = SENSOR_TYPE_TEMPERATURE;
  sample->timestamp = millis();
  sample->sensor_id = 0;
  return true;
}

bool Energesis_LM35::begin(int16_t pin, uint8_t analog_res, float v_ref) {

  if( !setPin(pin))
    return false; // invalid pin

  setAnalogResolution(analog_res);

  if( v_ref > 0 )
    setVref(v_ref);


  return (m_pin > 0);
}

bool Energesis_LM35::setPin( int16_t pin ){
  if( pin < 0 )
    return false;

  m_pin = pin;

#ifdef ESP32
  if( digitalPinToAnalogChannel(m_pin) == -1 )
    return false;
#endif

  return m_pin > 0;
}

}// namespace
