#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "databaseAPI.h"


// bool getParameters(int argc, char* argv[], char** out_date, char** out_key)
// {
//     bool isValid = true;
    
//     for(int parameter_num = 1; parameter_num <= argc; parameter_num += 2)
//     {
//         if (0 == strcmp(argv[parameter_num], "--Date"))
//         {
//             if (isValidDate(argv[parameter_num+1]))
//             {
//                 *out_date = argv[parameter_num + 1];
//             }
//             else
//             {
//                 isValid = false;
//                 break;
//             }
//         }
//         else if (0 == strcmp(argv[parameter_num], "--Key"))
//         {
//             if (isValidKey(argv[parameter_num + 1]))
//             {
//                 *out_key = argv[parameter_num + 1];
//             }
//             else
//             {
//                 isValid = false;
//                 break;
//             }
//         }
//         else
//         {
//             isValid = false;
//             break;
//         }
        
//     }

//     return isValid;
// }

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
        
        if(isValidDate(country_p, date_str))
        {
            getDataPointByDate(date_str, country_p, &data_point_p);
            //printf("got datapoint\n\n");
            getDataPointValueByKey(data_point_p, key, &val);
            //printf("got value\n\n");
            printf("%s: %d\n", country_p->name, val);
        }
        else
        {
            printf("%s falls out of start and end dates for country %s\n", date_str,country_p->name);
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
        printf("Invalid arguments.\n");
        return -1;
    }
    
    bool success = printKeyByDate(db_instance,date_str,key);

    releaseDBInstance(db_instance);

    if (success) return 0;
    return -2;
}


