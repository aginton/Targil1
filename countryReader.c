#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <libgen.h>

#include "countryReader.h"


// Country *country_array;
// unsigned int country_array_size;



bool openFile(char *file_path, FILE **out_file_pointer)
{
    //printf("openFile started given path %s\n", file_path);
    *out_file_pointer = fopen(file_path, "r");
    if (!(*out_file_pointer))
    {
        printf("Error opening file with path %s in function openFile.\n", file_path);
        return false;
    }
    //printf("openFile successfully opened %s\n",file_path);

    return true;
}

int countLines(FILE *fp)
{
    char *buffer = NULL;
    size_t len = 0;
    int line_number = 0;

    while (getline(&buffer, &len, fp) != -1)
    {
        line_number++;
    }

    free(buffer);
    fseek(fp, 0, SEEK_SET);

    // printf("There are %d lines in the file\n", line_number);

    return line_number;
}

bool populateCountryData(char *country_path, DataPoint **out_country_data, unsigned int *out_country_data_size)
{
    //printf("populateCountryData, start\n");
    FILE *fp;
    if (!openFile(country_path, &fp))
    {
        return false;
    }
    //printf("populateCountryData, opened file\n");

    unsigned int num_data_points = countLines(fp) - 1;
    //printf("populateCountryData, about to allocate country_data array\n");
    DataPoint *country_data = malloc(num_data_points * sizeof(DataPoint));

    char *buffer = NULL;
    size_t len = 0;

    getline(&buffer, &len, fp);
    for (unsigned int i = 0; i < num_data_points; ++i)
    {
        getline(&buffer, &len, fp);
        sscanf(buffer, "%s%d%d%d%d", country_data[i].date,
               &country_data[i].confirmed,
               &country_data[i].deaths,
               &country_data[i].recovered,
               &country_data[i].active);
    }

    *out_country_data = country_data;
    *out_country_data_size = num_data_points;

    free(buffer);
    fclose(fp);

    return true;
}

bool createCountriesArray(char *base_path, DBInstance* db_instance_p)
{
    //printf("createCountriesArray, about to allocate index_path\n");
    char *index_path = malloc(strlen(base_path) + 
                                    strlen(COUNTRY_INDEX_FILENAME) + 1);

    strcpy(index_path, base_path);
    strcat(index_path, COUNTRY_INDEX_FILENAME);

    //printf("createCountriesArray, about to allocate country_names_path\n");
    char *names_path = malloc(strlen(base_path) + 
                                    strlen(COUNTRY_NAMES_FILENAME) + 1);

    strcpy(names_path, base_path);
    strcat(names_path, COUNTRY_NAMES_FILENAME);

    FILE *index_fp, *name_fp;
    if (!openFile(index_path, &index_fp) || !openFile(names_path, &name_fp))
    {
        return false;
    }

    free(index_path);
    free(names_path);
    
    int num_of_countries = countLines(index_fp);
    //printf("createCountriesArray, about to allocate countries_array\n");
    Country *countries_array = malloc(num_of_countries * sizeof(Country));

    //printf("createCountriesArray, about to allocate country_file_full_path\n");
    char *country_file_full_path = malloc(strlen(base_path) + MAX_COUNTRY_PATH_LEN  + 1);
    //printf("createCountriesArray, created country_file_full_path\n");

    char *country_path_from_index = NULL, *country_name = NULL;
    size_t index_size = 0, name_size = 0;

    for (int i = 0; i < num_of_countries; ++i)
    {
        getline(&country_path_from_index, &index_size, index_fp);
        getline(&country_name,&name_size,name_fp);
        // Delete newline:
        country_path_from_index[strlen(country_path_from_index) -1] = '\0';
        country_name[strlen(country_name)-1] = '\0';
       
        countries_array[i].name = malloc(strlen(country_name)+1);
        strcpy(countries_array[i].name, country_name);

        strcpy(country_file_full_path, base_path);
        strcat(country_file_full_path, country_path_from_index);
        //printf("createCountriesArray, country_file_full_path=%s\n",country_file_full_path);
        if (!populateCountryData(country_file_full_path, &(countries_array[i].data_points), &(countries_array[i].data_points_size)))
        {
            return false;
        }
    }

    db_instance_p->country_array = countries_array;
    db_instance_p->country_array_size = num_of_countries;

    free(country_file_full_path);
    free(country_path_from_index);
    free(country_name);
    fclose(index_fp);

    return true;
}



void freeDB(DBInstance* db_instance)
{
    Country* country_array = db_instance->country_array;

    for (int i = 0; i < db_instance->country_array_size; ++i)
    {
        free(country_array[i].data_points);
        free(country_array[i].name);
    }
    free(country_array);
    free(db_instance);
}

char* getDBLocation(char* program_name)
{
    char* program_directory = dirname(program_name);
    char* program_relative_base_path = malloc(strlen(program_directory) + 
                                                strlen(COVIDDB_FOLDER_SUF) + 1);
    strcpy(program_relative_base_path, program_directory);
    strcat(program_relative_base_path, COVIDDB_FOLDER_SUF);
    return program_relative_base_path;
}

DBInstance* loadDB(char* program_relative_base_path){
    DBInstance* db_instance_p = malloc(sizeof(DBInstance));    
    createCountriesArray(program_relative_base_path, db_instance_p);

    return db_instance_p;
}

// int main(int argc, char *argv[])
// {
//     loadDB(argv[1]);
    
//     // for (int i=0; i<num_data_points; ++i)
//     // {
//     //     printDataPoint(&country_data[i]);
//     // }
//     return 0;
// }
