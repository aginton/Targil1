#pragma once

#define DATE_STRING_LEN 10

#define MAX_COUNTRY_NAME_LEN (100)
#define MAX_COUNTRY_PATH_LEN (MAX_COUNTRY_NAME_LEN + 50)
#define COUNTRY_NAMES_FILENAME "country_names"
#define COUNTRY_INDEX_FILENAME "country_index"
#define COVIDDB_FOLDER_SUF "/CovidDB/"
#define NUMBER_OF_KEYS 4
#include <stdbool.h>

typedef enum {CONFIRMED, DEATHS, RECOVERED, ACTIVE, NA} Keys;

typedef struct dataPoint
{
    char date[DATE_STRING_LEN + 1];
    unsigned int confirmed;
    unsigned int deaths;
    unsigned int recovered;
    unsigned int active;
} DataPoint;

typedef struct country
{
    char *name;
    DataPoint *data_points;
    unsigned int data_points_size;
} Country;

typedef struct dbInstance
{
    Country *country_array;
    unsigned int country_array_size;
} DBInstance;