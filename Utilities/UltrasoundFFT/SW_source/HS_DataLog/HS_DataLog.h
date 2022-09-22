/**
  ******************************************************************************
  * @file    HS_DataLog.h
  * @author  SRA
  * @brief   header file for high speed datalog library
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  ******************************************************************************
  */

#ifndef HS_DATALOG_H
#define HS_DATALOG_H

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define ST_HS_DATALOG_OK        0
#define ST_HS_DATALOG_ERROR    -1


/**
 * @brief Initialization of the ST device managment library.
 *
 * This function populates internal data structures needed to keep track
 * of the st devices currently connected to the host. Each board is automatically
 * assigned with an identification number (id) starting from 0 to
 * (number of devices - 1) and it's caracterized by a univoque serial number.
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 * otherwise
 */
int hs_datalog_open(void);

/**
 * @brief De-initialization of the ST device managment library.
 *
 * This fucntion must be called at the end of the operations.
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 * otherwise
 */
int hs_datalog_close(void);

/**
 * @brief retrieve the number of connected devices.
 *
 * @param nDevices will contain the number of connected devices.
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 */
int hs_datalog_get_device_number(int * nDevices);

/**
 * @brief Send a JSOM message to a device.
 *
 * This function enables the communication with the board by means of user generated
 * JSON messages. It allows the configuration and the managment of the device withouth
 * the need of any additional function. When using this function, the user is in charge
 * of creating and parsing the messages using the correct format.
 *
 * @param dId Id of the device to address.
 * @param msg message to be sent; refer to the documentation for a detailed description
 *        of the message formats
 * @param msg_len length of the message
 * @param data_len length of the returned data in the case of a GET message
 * @param data data returned, internally allocated, to be freed with the provided free function
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 */
int hs_datalog_send_message(int dId, char * msg, int msg_len, int * data_len, char ** data);


/**
 * @brief Sends a "GET device" message to retrieve the whole JSON device descriptor.
 *
 * @param dId Id of the device to address.
 * @param device null terminated char string  containing the requested information
 *               in json format.
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 */
int hs_datalog_get_device(int dId, char ** device);

/**
 * @brief Sends a "GET device descriptor" message to retrieve the JSON device info.
 *
 * @param dId Id of the device to address.
 * @param device null terminated char string  containing the requested information
 *               in json format.
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 */
int hs_datalog_get_device_descriptor(int dId, char ** device_descriptor);

/**
 * @brief Sends a "GET sensor descriptor" message to retrieve the whole JSON sensor descriptor.
 *
 * @param dId Id of the device to address.
 * @param device null terminated char string  containing the requested information
 *               in json format.
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 */
int hs_datalog_get_sensor_descriptor(int dId, int sId, char ** sensor_descriptor);

/**
 * @brief Sends a "GET sensor status" message to retrieve the whole JSON sensor status.
 *
 * @param dId Id of the device to address.
 * @param device null terminated char string  containing the requested information
 *               in json format.
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 */
int hs_datalog_get_sensor_status(int dId, int sId, char ** sensor_status);

/**
 * @brief Sends a "GET subsensor descriptor" message to retrieve the whole JSON subsensor descriptor.
 *
 * @param dId Id of the device to address.
 * @param device null terminated char string  containing the requested information
 *               in json format.
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 */
int hs_datalog_get_subsensor_descriptor(int dId, int sId, int ssId, char ** subsensor_descriptor);

/**
 * @brief Sends a "GET subsensor status" message to retrieve the whole JSON subsensor status.
 *
 * @param dId Id of the device to address.
 * @param device null terminated char string  containing the requested information
 *               in json format.
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 */
int hs_datalog_get_subsensor_status(int dId, int sId, int ssId, char ** subsensor_status);

/**
 * @brief retrieve the number of sensors of a specific device.
 *
 * @param dId Id of the device to address.
 * @param nSensors will contain the number of sensors of the device.
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 */
int hs_datalog_get_sensor_number(int dId, int * nSensors);

/**
 * @brief retrieve the number of sub sensors of sensors on a specific device.
 *
 * @param dId Id of the device to address.
 * @param sId Id of the sensor to address.
 * @param nSensors will contain the number of sensors of the device.
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 */
int hs_datalog_get_sub_sensor_number(int dId, int sId, int * nSubSensors);

/**
 * @brief retrieve the name of the sensor on a specific device.
 *
 * @param dId Id of the device to address.
 * @param sId Id of the sensor to address.
 * @param device null terminated char string  containing the requested information
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 */
int hs_datalog_get_sensor_name(int dId, int sId, char ** name);

/**
 * @brief retrieve the name of a specific device.
 *
 * @param sId Id of the sensor to address.
 * @param device null terminated char string  containing the requested information
 *
 * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
 */
int hs_datalog_get_device_name(int dId, char ** name);

/**
  * @brief Set the nominal ODR for a specific sensor on a specific device.
  *
  * This function can be used to setup the Output Data Rate for a specific sensor.
  *
  * @param dId Id of the device to address.
  * @param sId Id of the sensor to address.
  * @param ODR Output Data Rate to be set.
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  */
int hs_datalog_set_ODR(int dId, int sId, float ODR);

/**
  * @brief Get the nominal ODR for a specific sensor on a specific device.
  *
  * This function can be used to get the current nominal ODR for the specific
  * sensor / device .
  *
  * @param dId Id of the device to address.
  * @param sId Id of the sensor to address.
  * @param *ODR variable will contain the current nominal ODR value
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  */
int hs_datalog_get_ODR(int dId, int sId, float * ODR);

/**
  * @brief Get the measured ODR for a specific sensor on a specific device.
  *
  * The measured ODR is the sampling rate of the sensor measured respect to the
  * MCU frequency. This allows to have the same time base for all the sensors
  * on the same board and may be used generate accurate and sinchronized tymestamp
  * for all the sensors.
  *
  * @param dId Id of the device to address.
  * @param sId Id of the sensor to address.
  * @param *ODR will contain the measured ODR value
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  *
  */
int hs_datalog_get_measured_ODR(int dId, int sId, float * ODR);

/**
  * @brief Get the measured initial offset for a specific sensor on a specific device.
  *
  * The initial offset is the time stamp of the first sample of the stream
  * of the specific sensor measured respect to the MCU frequency. This allows to
  * align signals coming from several sensors on the same board
  *
  * @param dId Id of the device to address.
  * @param sId Id of the sensor to address.
  * @param *offset variable which will contain the measured initial offset
  * value
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  *
  */
int hs_datalog_get_initial_offset(int dId, int sId, float * offset);

/**
  * @brief Set the full scale for a specific sensor on a specific device.
  *
  * This function can be used to setup the full scale for a specific sensor.
  * "subId" parameter must be used to adress the right sensing element in
  * combo sensors
  *
  * @param dId Id of the device to address.
  * @param sId Id of the sensor to address.
  * @param subId Id of the sensing elemnt in case of combo devices.
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  *
  * */
int hs_datalog_set_FS(int dId, int sId, int subId, float FS);

/**
  * @brief Set the full scale for a specific sensor on a specific device.
  *
  * This function can be used to retrieve the current full scale setting
  * for a specific sensor.
  * "subId" parameter must be used to adress the right sensing element in
  * combo sensors
  *
  * @param dId Id of the device to address.
  * @param sId Id of the sensor to address.
  * @param subId Id of the sensing elemnt in case of combo devices.
  * @param FS will contain the  current FS.
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  *
  * */
int hs_datalog_get_FS(int dId, int sId, int subId, float * FS);

/**
  * @brief Set the number of data samples for each timestamp.
  *
  * The application data logger embeds a timestamp inside data streams.
  * Whith this function, the user can set the length of the frame of data
  * covered by a single timestamp, that is the number of samples between
  * the timestamps.
  * Timestamp is always referred to the last sample of the frame.
  *
  * @param dId Id of the device to address.
  * @param sId Id of the sensor to address.
  * @param samples number of samples for each timestamp.
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  *
  * */
int hs_datalog_set_samples_per_timestamp(int dId, int sId, int samples);

/**
  * @brief Get the number of data samples for each timestamp.
  *
  * The application data logger embeds a timestamp inside data streams.
  * Whith this function, the user can set the length of the frame of data
  * covered by a single timestamp, that is the number of samples between
  * the timestamps.
  * Timestamp is always referred to the last sample of the frame.
  *
  * @param dId Id of the device to address.
  * @param sId Id of the sensor to address.
  * @param samples will contain the number of samples for each timestamp.
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  *
  * */
int hs_datalog_get_samples_per_timestamp(int dId, int sId, int * samples);

/**
  * @brief Set the logging status of a sensor on a specific device.
  *
  * Each sensor on a specific board can be independently activated or
  * de-activated. When value parameter is set to true, the sensor will be put
  * in active state, ready to start logging after the call to
  * "int st_usb_datalog_start_log(int dId)"
  *
  * @param dId Id of the device to address.
  * @param sId Id of the sensor to address.
  * @param subId Id of the sensing elemnt in case of combo devices.
  * @param value new state for the sensor
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  *
  * */
int hs_datalog_set_sensor_active(int dId, int sId, bool value);

/**
  * @brief Get the current logging status of a sensor on a specific device.
  *
  * @param dId Id of the device to address.
  * @param sId Id of the sensor to address.
  * @param * status will contain the current status of the sensor.
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  *
  * */
int hs_datalog_get_sensor_active(int dId, int sId, bool * status);

/**
  * @brief Set the logging status of a "sub-sensor" on a specific combo device.
  *
  * Each sensing element of a specific combo device can be independently activated or
  * de-activated. When value parameter is set to true, the sensor will be put
  * in active state, ready to start logging after the call to
  * "int st_usb_datalog_start_log(int dId)"
  *
  * @param dId Id of the device to address.
  * @param sId Id of the sensor to address.
  * @param subId Id of the sensing elemnt in case of combo devices.
  * @param value new state for the sensor
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  *
  * */
int hs_datalog_set_sub_sensor_active(int dId, int sId, int subId, bool status);

/**
  * @brief Get the logging status of a a "sub-sensor" on a specific combo device.
  *
  *
  * @param dId Id of the device to address.
  * @param sId Id of the sensor to address.
  * @param subId Id of the sensing elemnt in case of combo devices.
  * @param * status will contain the current status of the sensor.
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  *
  * */
int hs_datalog_get_sub_sensor_active(int dId, int sId, int subId, bool *status);

/**
  * @brief Start data logging on a specific board.
  *
  * @param dId Id of the device to address.
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  *
  * */
int hs_datalog_start_log(int dId);

/**
  * @brief Stop data logging on a specific board.
  *
  * @param dId Id of the device to address.
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  *
  * */
int hs_datalog_stop_log(int dId);

/**
  * @brief Get the amount of available data for a sensor on a specific device.
  *
  * When no callback function is used to handle data ready events, the user should
  * use this function to poll periodically the device / sensor for available data.
  * Data can be retrieved using the hs_datalog_get_data function.
  *
  * @param dId Id of the device.
  * @param sId Id of the sensor.
  * @param size will contain the amount of data available for the chosen sensor
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  * */
int hs_datalog_get_available_data_size(int dId, int sId, int * size);

/**
  * @brief Get data for a specific sensor on a specific device.
  *
  * This function is used to get data from a sensor when  the function
  * hs_datalog_get_available_data_size returns a size > 0
  *
  * @param dId Id of the device.
  * @param sId Id of the sensor.
  * @param data will be filled with data
  * @param size amount of data requested
  * @param actual will be filled with actual size of data retrieved
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  * */
int hs_datalog_get_data(int dId, int sId, uint8_t * data, int size, int * actual);

/**
  * @brief Send data to a specific device.
  *
  * @param dId Id of the device.
  * @param data data to be sent
  * @param size amount of data
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  * */
int hs_datalog_send_data(int dId, uint8_t * data, int size);

/**
  * @brief Link a callback to the data ready event of a specific sensor.
  *
  * For ease of use, the callback have the following parameters:
  * @param dId Id of the device which generates the event.
  * @param sId Id of the sensor which generates the event..
  * @param data pointer to the available data
  * @param size size to the available data
  *
  * The same function can be linked to all the sensor events. The specific
  * device and sensor which generated the call is then available inside the
  * function.
  *
  * The call is done inside an high priority acquisition thread. User
  * operations inside the callback must be very limited in order not to stuck
  * the acquisition process.
  *
  * User is responsible to read data as soon as possible: data is overwritten
  * when new data is available.
  *
  * This callback hook is meant to be used for advanced operation. For standard
  * scenarios use the polling scheme with the functions:
  * st_usb_datalog_get_data_available_size() and  st_usb_datalog_get_data()
  *
  * @param dId Id of the device.
  * @param sId Id of the sensor.
  * @param callback callback to be called
  *
  * @return ST_HS_DATALOG_OK if no error occurred or ST_HS_DATALOG_ERROR
  * in case of error
  * */
int hs_datalog_set_data_ready_callback(int devId, int sId, int (*callback)(int devIdx, int ch, uint8_t *data, int size));

/**
  * @brief Free previously allocated memory.
  *
  * Do not use on memory not allocated by this library.
  *
  * @param value new state for the sensor
  *
  * @return ST_HS_DATALOG_OK if no error occurred, ST_HS_DATALOG_ERROR
  * otherwise
  * */
int hs_datalog_free(char * ptr);

/**
  * @brief Get library version.
  *
  * @param ** version will contain the current library version.
  *
  * @return length of the version string
  * */
int hs_datalog_get_version(char **version);

#ifdef __cplusplus
}
#endif


#endif // HS_DATALOG_H
