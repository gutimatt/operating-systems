/**
* loads the data into a struct
*
* Completion time: .5
*
* @author Gutierrez
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////

#include "DataLoader.h"

struct test_scenario* load_test_data(char* filename) {
    FILE* file = fopen(filename, "r");

    struct test_scenario* data = (struct test_scenario*) malloc(sizeof(struct test_scenario));

    fscanf(file, "%d %d %d",
           &data->page_count,
           &data->frame_count,
           &data->refstr_len);

    for (int i = 0; i < data->refstr_len; ++i) {
        fscanf(file, "%d", &data->refstr[i]);
    }

    fclose(file);

    return data;
}