#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "databaseAPI.h"




bool getParameters(int argc, char* argv[], char** out_date, Keys* out_key)
{
    if (5 != argc)
    {
        return false;
    }

    bool validKey = false, validDate = false;
    
    for(int parameter_num = 1; parameter_num < argc; parameter_num += 2)
    {
        if (0 == strcmp(argv[parameter_num], "--Date"))
        {
            *out_date = argv[parameter_num + 1];
            validDate = true;
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

    return validKey && validDate;
}

bool printKeyByDate(DBInstance* db_instance, char* date_str,Keys key)
{
    Country* country_p = NULL;
    DataPoint* data_point_p = NULL;
    int val = 0;

    for (int i = 0; i<db_instance->country_array_size; ++i)
    {
        country_p = db_instance->country_array + i;
        
        switch(isValidDate(country_p, date_str))
        {
            case DATE_SUCCESS:
                getDataPointByDate(date_str, country_p, &data_point_p);
                //printf("got datapoint\n\n");
                getDataPointValueByKey(data_point_p, key, &val);
                //printf("got value\n\n");
                printf("%s: %d\n", country_p->name, val);
                break;

            case DATE_OUT_OF_SCOPE:
                printf("%s: %s falls out of start and end dates for this country.\n", country_p->name, date_str);
                break;
            
            case DATE_INVALID:
                printf("The date %s is invalid.\n", date_str);
                return false;
            
            case DATE_NULL_COUNTRY_P:
                puts("isValidDate received NULL country_p");
                return false;
        }
    }
    
    return true;
}

int main(int argc, char* argv[])
{
    DBInstance* db_instance = createDBInstance(argv[0]);

    char* date_str;
    Keys key;
    if (!getParameters(argc, argv, &date_str, &key))
    {
        printf("Invalid or missing arguments.\n");
        return -1;
    }
    
    bool success = printKeyByDate(db_instance,date_str,key);

    releaseDBInstance(db_instance);

    if (success) return 0;
    return -2;
}


