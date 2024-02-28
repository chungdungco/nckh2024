/**
 * @file LM35_basic.ino
 * @author José Guerra Carmenate <joseguerracarmenate@gmail.com>
 * @brief Get temperature using LM35 sensor
 * @version 0.1
 * @date 2022-05-31
 * 
 * @copyright Copyright (c) 2022
 */
#include <Arduino.h>
#include <Energesis_LM35.h>

using namespace Energesis;

//Energesis_LM35 lm35; 
Energesis_LM35 lm35( LM35_CA ); // specify LM35 serie

const char *type2str( sensor_type_t t ){
  switch (t) {
    case SENSOR_TYPE_TEMPERATURE:       return "Temperature";
    case SENSOR_TYPE_CURRENT:           return "Current";
    case SENSOR_TYPE_RELATIVE_HUMIDITY: return "Relative Humidity";
    case SENSOR_TYPE_VOLTAGE:           return "Voltage";
    default:                            return "Invalid";

  }
}

const char *type2unit( sensor_type_t t ){
  switch (t){
    case SENSOR_TYPE_TEMPERATURE:       return "ºC";
    case SENSOR_TYPE_CURRENT:           return "mA";
    case SENSOR_TYPE_RELATIVE_HUMIDITY: return "%";
    case SENSOR_TYPE_VOLTAGE:           return "V";
    default:                            return "Invalid";
  }
}

void printDetails (sensor_details_t &d){
  Serial.println("---------------------------------");
  Serial.print( "Name: " );

  // print sensor name and type
  Serial.println( String(d.name) + " -> " + type2str(d.type) );

  Serial.print( "Serial Number: " );
  // lm35 no have serial number. Must be zero
  Serial.println( (unsigned long)(d.sensor_id) );
  
  Serial.print( "Max value: " );
  Serial.print( d.max_value);
  Serial.println( type2unit( d.type ) );

  Serial.print( "Min value: " );
  Serial.print( d.min_value);
  Serial.println( type2unit( d.type ) );

  // current consum (see datasheet)
  Serial.print( "Current consum[mA]: " );
  Serial.println( d.power );

  Serial.print( "Resolution: " );
  Serial.print( d.resolution );
  Serial.println( type2unit( d.type ) );

  Serial.println("-----------------------------------------");
}

void setup() {

  // init serial port
  Serial.begin(9600);
  delay(500);

  // init LM35 temperature sensor
  // lm35.begin( Pin, ADC_Resolution, Vref[milivolts] )
  if( !lm35.begin( A0, 10, 5000 ) ){
    Serial.println("LM35: Init error");
    while(1)
      delay(100);
  }

  sensor_details_t det;

  // All Energesis_Sensor derived driver have this function
  lm35.getSensorDetails( &det );

  printDetails( det );

}

void loop() {

  Serial.print( "Temperature: " );
  Serial.print( lm35.getTemperature() ); // temperature ºC
  Serial.print( " ºC, " );
  Serial.print( lm35.getTemperatureFahrenheit() ); // temperature ºF
  Serial.print( " ºF, " );
  Serial.print( lm35.getTemperatureKelvin() );  // temperature K
  Serial.println( " K" );
  delay(3000);
}
