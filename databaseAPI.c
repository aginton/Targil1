#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <libgen.h>

#include "country.h"

extern Country *country_array;
extern unsigned int country_array_len;

bool getCountryByName(char* country_name, Country** out_country_p)
{
    int curr = 0, begin_idx = 0, end_idx = country_array_len - 1;

    while (begin_idx <= end_idx)
    {
        curr = begin_idx + ((end_idx - begin_idx) / 2);

        int val = strcmp(country_array[curr].name, country_name);
        if (val == 0)
        {
            *out_country_p = country_array + curr;
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

bool getDataPointByDate(char* date, Country* country_p, DataPoint** out_data_point)
{

}

bool getValueByKey(DataPoint* data_point_p, Keys key, int* out_value)
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