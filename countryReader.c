#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <libgen.h>
#include "country.h"



static Country *country_array;
static unsigned int country_array_len;



bool openFile(char *file_path, FILE **out_file_pointer)
{
    printf("openFile started given path %s\n", file_path);
    *out_file_pointer = fopen(file_path, "r");
    if (!(*out_file_pointer))
    {
        printf("Error opening file with path %s in function openFile.\n", file_path);
        return false;
    }
    printf("openFile successfully opened %s\n",file_path);

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

bool populate_country_data(char *country_path, DataPoint **out_country_data, unsigned int *out_country_data_size)
{
    printf("populate_country_data, start\n");
    FILE *fp;
    if (!openFile(country_path, &fp))
    {
        return false;
    }
    printf("populate_country_data, opened file\n");

    unsigned int num_data_points = countLines(fp) - 1;
    printf("populate_country_data, about to allocate country_data array\n");
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

bool create_countries_array(char *base_path, Country **out_country_array, unsigned int *out_country_array_size)
{
    printf("create_countries_array, about to allocate index_path\n");
    char *index_path = malloc(strlen(base_path) + 
                                    strlen(COUNTRY_INDEX_FILENAME) + 1);

    strcpy(index_path, base_path);
    strcat(index_path, COUNTRY_INDEX_FILENAME);

    printf("create_countries_array, about to allocate country_names_path\n");
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
    printf("create_countries_array, about to allocate countries_array\n");
    Country *countries_array = malloc(num_of_countries * sizeof(Country));

    printf("create_countries_array, about to allocate country_file_full_path\n");
    char *country_file_full_path = malloc(strlen(base_path) + MAX_COUNTRY_PATH_LEN  + 1);
    printf("create_countries_array, created country_file_full_path\n");

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
        printf("create_countries_array, country_file_full_path=%s\n",country_file_full_path);
        if (!populate_country_data(country_file_full_path, &(countries_array[i].data_points), &(countries_array[i].data_points_len)))
        {
            return false;
        }
    }

    *out_country_array = countries_array;
    *out_country_array_size = num_of_countries;

    free(country_file_full_path);
    free(country_path_from_index);
    free(country_name);
    fclose(index_fp);

    return true;
}

void print_DataPoint(DataPoint *dp)
{
    printf("Date: %s Confirmed: %d Deaths: %d Recovered: %d Active: %d\n",
           dp->date, dp->confirmed, dp->deaths, dp->recovered, dp->active);
}

void print_data_point(DataPoint *dataPoint)
{
    printf("Date: %s, Confirmed: %d, Deaths: %d, Recovered: %d, Active: %d\n",
           dataPoint->date,
           dataPoint->confirmed,
           dataPoint->deaths,
           dataPoint->recovered,
           dataPoint->active);
}

void print_country(Country *country_p)
{
    printf("Country: %s\n", country_p->name);
    for (int i = 0; i < country_p->data_points_len; ++i)
    {
        print_data_point(&(country_p->data_points[i]));
    }
}

void print_country_array(Country *country_array, unsigned int country_array_len)
{
    for (int i = 0; i < 3; ++i)
    {
        print_country(country_array + i);
    }
}

void free_database()
{
    for (int i = 0; i < country_array_len; ++i)
    {
        free(country_array[i].data_points);
        free(country_array[i].name);
    }
    free(country_array);
}

void load_database(char* program_name){
    char* program_directory = dirname(program_name);
    char* program_relative_base_path = malloc(strlen(program_directory) + 
                                                strlen(COVIDDB_FOLDER_SUF) + 1);
    strcpy(program_relative_base_path, program_directory);
    strcat(program_relative_base_path, COVIDDB_FOLDER_SUF);
    
    create_countries_array(program_relative_base_path, &country_array, &country_array_len);

    free(program_relative_base_path);
}

int main(int argc, char *argv[])
{
    load_database(argv[1]);
    
    // for (int i=0; i<num_data_points; ++i)
    // {
    //     print_DataPoint(&country_data[i]);
    // }
    return 0;
}
