#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "databaseAPI.h"




int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Not enough arguments\n");
        return 0;
    }

    DBInstance* db_instance = createDBInstance(argv[0]);
    Country* country_p;
    getCountryByName(argv[1], db_instance, &country_p);
    puts(country_p->name);
    DataPoint* my_data_point;
    getDataPointByDate(argv[2], country_p, &my_data_point); //"2020-06-30"

    printDataPoint(my_data_point);
    // int confirmed_in_30_6 = 0;
    // getDataPointValueByKey(my_data_point, CONFIRMED, &confirmed_in_30_6);

    // printf("%d\n", confirmed_in_30_6);

    releaseDBInstance(db_instance);
}


