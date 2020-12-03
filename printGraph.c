#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "databaseAPI.h"

#define NUM_OF_SPACES (80)

bool getParameters(int argc, char* argv[], char** out_country_name, Keys* out_key)
{
    if (5 != argc)
    {
        return false;
    }

    bool validKey = false, validCountry = false;
    
    for(int parameter_num = 1; parameter_num < argc; parameter_num += 2)
    {
        if (0 == strcmp(argv[parameter_num], "--Country"))
        {
            *out_country_name = argv[parameter_num + 1];
            validCountry = true;
        }
        else if (0 == strcmp(argv[parameter_num], "--Key"))
        {
            if (isValidKey(argv[parameter_num + 1], out_key))
            {
                validKey = true;
            }
            else
            {
                printf("The key %s is invalid.\n",argv[parameter_num+1]);
            }
        }
        
    }

    return validKey && validCountry;
}

void findExtrema(Country* country_p,Keys key, int* out_min, int* out_max)
{
    int min = 0, max = 0, val = 0;
    for (int i = 0; i < country_p->data_points_size; ++i)
    {
        getDataPointValueByKey(country_p->data_points + i, key, &val);
        if (val < min)
        {
            min = val;
        }
        else if (val > max)
        {
            max = val;
        }
    }

    *out_min = min;
    *out_max = max;
}

int main(int argc, char* argv[])
{
    char* country_name;
    Keys key;
    if (!getParameters(argc, argv, &country_name, &key))
    {
        printf("Invalid arguments.\n");
        return -1;
    }
    DBInstance* db_instance = createDBInstance(argv[0]);

    Country* country_p = NULL;
    if (!getCountryByName(country_name, db_instance, &country_p))
    {
        printf("Country %s not in current database.\n", country_name);
        return -2;
    }

    int min = 0, max = 0;
    findExtrema(country_p, key, &min, &max);

    float spaces_per_unit = NUM_OF_SPACES / (float)(max - min);
    int num_spaces = 0;
    int val = 0;
    for(int i = 0; i < country_p->data_points_size; ++i)
    {
        getDataPointValueByKey(country_p->data_points + i, key, &val);
        num_spaces = (int)(spaces_per_unit * (val - min));
        printf("%*d\n", num_spaces, val);
    }

    releaseDBInstance(db_instance);
}