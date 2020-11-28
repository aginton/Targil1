#pragma once

#include <stdio.h>
#include "country.h"

bool openFile(char *file_path, FILE **out_file_pointer);
int countLines(FILE *fp);
bool populateCountryData(char *country_path, DataPoint **out_country_data, unsigned int *out_country_data_size);
bool createCountriesArray(char *base_path, DBInstance* db_instance_p);
void freeDB(DBInstance* db_instance);
char* getDBLocation(char* program_name);
DBInstance* loadDB(char* program_relative_base_path);
