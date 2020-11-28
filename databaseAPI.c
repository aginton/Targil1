#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <libgen.h>
#include <time.h>
#include "databaseAPI.h"
#include "countryReader.h"

char* valid_keys[NUMBER_OF_KEYS] = {"Confirmed", "Deaths", "Recovered", "Active"};

DBInstance* createDBInstance(char* program_name)
{
    char* db_path = getDBLocation(program_name);
    DBInstance* res = loadDB(db_path);
    free(db_path);
    return res;
}

void releaseDBInstance(DBInstance* db_instance_p)
{
    freeDB(db_instance_p);
}

bool getCountryByName(char* country_name, DBInstance* db_instance_p, Country** out_country_p)
{
    int curr = 0, begin_idx = 0, end_idx = db_instance_p->country_array_size - 1;

    while (begin_idx <= end_idx)
    {
        curr = begin_idx + ((end_idx - begin_idx) / 2);

        int val = strcmp(db_instance_p->country_array[curr].name, country_name);
        if (val == 0)
        {
            *out_country_p = db_instance_p->country_array + curr;
            return true;
        }
        
        if (val < 0)
        {
            begin_idx = curr + 1;
        } else{
            end_idx = curr - 1;
        }
    }
    return false;
}

bool getDataPointByDate(char* date, Country* country_p, DataPoint** out_data_point_p)
{
    int curr = 0, begin_idx = 0, end_idx = country_p->data_points_size - 1;
    while (begin_idx <= end_idx)
    {
        curr = begin_idx + ((end_idx - begin_idx) / 2);
        int val = strcmp(country_p->data_points[curr].date, date);
        if (val == 0)
        {
            *out_data_point_p = &(country_p->data_points[curr]);
            return true;
        }
        if (val < 0)
        {
            begin_idx = curr + 1;
        } else{
            end_idx = curr - 1;
        }
    }
    return false;
}

bool getDataPointValueByKey(DataPoint* data_point_p, Keys key, int* out_value)
{
    switch (key)
    {
        case CONFIRMED:
        *out_value = data_point_p->confirmed;
        break;
        case DEATHS:
        *out_value = data_point_p->deaths;
        break;
        case RECOVERED:
        *out_value = data_point_p->recovered;
        break;
        case ACTIVE:
        *out_value = data_point_p->active;
        break;
        default:
        return false;
    }

    return true;
}



void printDataPoint(DataPoint *dataPoint)
{
    printf("Date: %s, Confirmed: %d, Deaths: %d, Recovered: %d, Active: %d\n",
           dataPoint->date,
           dataPoint->confirmed,
           dataPoint->deaths,
           dataPoint->recovered,
           dataPoint->active);
}

void printCountry(Country *country_p)
{
    printf("Country: %s\n", country_p->name);
    for (int i = 0; i < country_p->data_points_size; ++i)
    {
        printDataPoint(&(country_p->data_points[i]));
    }
}

void printCountryArray(Country *country_array, unsigned int country_array_size)
{
    for (int i = 0; i < 3; ++i)
    {
        printCountry(country_array + i);
    }
}

bool isValidKey(char* key, Keys* key_enum)
{
    for (int i = 0; i < NUMBER_OF_KEYS; ++i)
    {
        if (0 == strcmp(key, valid_keys[i]))
        {
            *key_enum = (Keys)i;
            return true;
        }
    }
    return false;
}

bool isValidDate(Country* country_p, char* date_str)
{ 
    struct tm* tm_p = getdate(date_str);
    if(NULL == tm_p)
    {
        return false;
    }

    time_t input_date = mktime(tm_p);

    // struct tm DB_start_date_tm = {0};
    // DB_start_date_tm.tm_mday = 22;
    // DB_start_date_tm.tm_mon = 0; // January
    // DB_start_date_tm.tm_year = 120; // Since 1900
    // time_t DB_start_date = mktime(&DB_start_date_tm);

    time_t DB_start_date = mktime(getdate(country_p->data_points[0].date));
    time_t DB_end_date = mktime(getdate(
        country_p->data_points[country_p->data_points_size - 1].date));
    
    return (input_date >= DB_start_date) && (input_date <= DB_end_date);
}