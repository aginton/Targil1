#pragma once

#include "country.h"
extern char* valid_keys[NUMBER_OF_KEYS];

DBInstance* createDBInstance(char* program_name);
void releaseDBInstance(DBInstance* db_instance_p);
bool getCountryByName(char* country_name, DBInstance* db_instance_p, Country** out_country_p);
bool getDataPointByDate(char* date, Country* country_p, DataPoint** out_data_point_p);
bool getDataPointValueByKey(DataPoint* data_point_p, Keys key, int* out_value);
void printDataPoint(DataPoint *dataPoint);
void printCountry(Country *country_p);
void printCountryArray(Country *country_array, unsigned int country_array_size);
bool isValidKey(char* key, Keys* key_enum);
bool isValidDate(Country* country_p, char* date_str);