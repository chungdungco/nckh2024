/**
 * @file Energesis_LM35.h
 * @author José Guerra Carmenate <joseguerracarmenate@gmail.com>
 * @brief Controlador para Sensor de temperatura LM35.
 * 
 * Controlador para la serie de sensores de temperatura LM35.
 * Ver datasheet: https://www.ti.com/lit/ds/symlink/lm35.pdf
 * 
 * @version 0.1
 * @date 2022-05-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _ENERGESIS_LM35_H_
#define _ENERGESIS_LM35_H_ 

#include <Energesis_Sensor.h>

namespace Energesis{
#define ENERGESIS_LM35_VERSION_MAJOR 0
#define ENERGESIS_LM35_VERSION_MINOR 1


/**
 * @brief Distintos modelos de la serie
 * 
 */
enum lm35_series_t{
  LM35,
  LM35_A,
  LM35_C,
  LM35_CA,
  LM35_D,
};

class Energesis_LM35: public Energesis_Sensor, public Energesis_AnalogSensor,public Energesis_TemperatureSensor{
  public:
    /**
     * @brief Crea un objeto Energesis_LM35 nuevo
     * 
     * @param serie Especifica el modelo de LM35 en la serie. LM35 de forma predeterminada.
     */
    Energesis_LM35( lm35_series_t serie = LM35 ):
      Energesis_AnalogSensor(), Energesis_TemperatureSensor(), m_pin(NOT_A_PIN), m_serie(serie){
      setVref(-INFINITY);
    };

    /**
     * @brief Crea un objeto Energesis_LM35 nuevo
     * 
     * @param pin Pin del MCU conectado al pin Vout del LM35. Tiene 
     * que ser un pin analógico.
     * @param v_ref Voltaje de referencia en milivoltios.
     * @param serie Especifica el modelo de LM35 en la serie. LM35 de forma predeterminada.
     */
    Energesis_LM35( int16_t pin, float v_ref = -1, lm35_series_t serie = LM35 ):
      Energesis_AnalogSensor(),Energesis_TemperatureSensor(){
      setPin(pin);
      m_serie = serie;
      setVref(v_ref);      
    }

    /**
     * @brief Inicializa el controlador de LM35.
     * 
     * @param analog_res resolución del ADC en bits. 
     * @param v_ref Voltaje de referencia en milivoltios.
     * @param pin Pin del MCU conectado al pin Vout del sensor. Tiene 
     * que ser un pin analógico.
     * @return true Controlador inicializado correctamente.
     * @return false Error al inicializar el controlador.
     * 
     * @note 
     */
    bool begin(int16_t pin, uint8_t analog_res, float v_ref);

    /**
     * @brief Especifica el pin del MCU conectado al pin Vout del sensor.
     * 
     * @param pin Pin del MCU conectado al pin Vout del sensor. Tiene 
     * que ser un pin analógico.
     * 
     * @return true Pin asignado válido
     * @return false Pin inválido
     */
    bool setPin( int16_t pin );
    
    /**
     * @brief Obtener el pin del MCU conectado al pin Vout del sensor.
     * 
     * @return int16_t pin del MCU conectado al pin Vout del sensor.
     */
    int16_t getPin(){ return m_pin; }

    /**
     * @brief Retorna la temperatura en ºC
     * 
     * @return float 
     */
    float getTemperature() override;

    /**
     * @brief Retorna una instancia de controlador genérico
     * 
     * @return Energesis_Sensor* 
     */
    Energesis_Sensor *getTemperatureSensor(){ return this; }
    
    /**
     * @brief Retorna el valor leído por el ADC, en lugar de la temperatura
     * 
     * @return uint16_t Valor leído por el ADC. 
     */
    uint16_t getRaw();

    /** @see Energesis_Sensor::getSample */
    bool getSample( sensor_sample_t *sample );

    /** @see Energesis_Sensor::getSensorDetails */
    void getSensorDetails( sensor_details_t *details );


  private:
    int16_t m_pin;        //!< Pin al que se conecta la salida del sensor
    lm35_series_t m_serie;//!< Identifica la serie del sensor
};


};


#endif