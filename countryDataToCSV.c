#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>
#include "databaseAPI.h"

#define FILE_EXTENTION ".csv"

bool isValidParams(int argc, char* argv[])
{
    if (3 != argc)
    {
        return false;
    }
    return 0 == strcmp(argv[1], "--Country");
}

char* generateOutputPath(char* program_name, char* country_name)
{
    char* program_directory = dirname(program_name);
    char* output_file_path = malloc(strlen(program_directory) + 
                                                strlen(country_name) + 
                                                strlen(FILE_EXTENTION) + 2);
    strcpy(output_file_path, program_directory);
    strcat(output_file_path, "/");
    strcat(output_file_path, country_name);
    strcat(output_file_path, FILE_EXTENTION);
    return output_file_path;
}

void createCSV(FILE* output_fp, Country* country_p)
{
    fprintf(output_fp, "Date");
    for (int i = 0; i < NUMBER_OF_KEYS; ++i)
    {
        fprintf(output_fp, ",%s", valid_keys[i]);
    }
    fprintf(output_fp, "\n");

    for (int i = 0; i < country_p->data_points_size; ++i)
    {
        fprintf(output_fp, "%s,%d,%d,%d,%d\n",
            country_p->data_points[i].date,
            country_p->data_points[i].confirmed,
            country_p->data_points[i].deaths,
            country_p->data_points[i].recovered,
            country_p->data_points[i].active);
    }
}

bool openOutputFile(char* program_name, char* country_name, FILE** output_fp_p)
{
    char* output_file_path = generateOutputPath(program_name, country_name);
    *output_fp_p = fopen(output_file_path, "w");   

    //printf("File location: %s\n", output_file_path);

    free(output_file_path);
    if(!(*output_fp_p))
    {
    
        return false;
    }

    return true;
}

int main(int argc, char* argv[])
{
    if (!isValidParams(argc, argv))
    {
        printf("Invalid arguments.\n");
        return -1;
    }

    DBInstance* db_instance = createDBInstance(argv[0]);
    Country* country_p = NULL;
    if (!getCountryByName(argv[2], db_instance, &country_p))
    {
        printf("Country %s is not in current database.\n", argv[2]);
        return -2;
    }

    FILE* output_fp = NULL;
    if(openOutputFile(argv[0], country_p->name, &output_fp))
    {
        createCSV(output_fp, country_p);
        fclose(output_fp);
    }
    else
    {
        printf("Error opening output file.\n");
    }
    
    releaseDBInstance(db_instance);

    return 0;
}