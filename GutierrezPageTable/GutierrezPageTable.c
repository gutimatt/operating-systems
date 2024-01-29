/**
* simulates a page table
*
* Completion time: 3
*
* @author Gutierrez
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////

#include "PageTable.h"

struct page_table_entry{
    int page;
    int frame;
    unsigned int valid;
    int order;
    int last;
    int freq;
};

struct page_table{
    int page_count;
    int frame_count;
    enum replacement_algorithm algorithm;
    struct page_table_entry** entries;
    int frameIndex;
    int faults;
    int verbose;
};

int order = 1;

struct page_table* page_table_create(int page_count, int frame_count, enum replacement_algorithm algorithm, int verbose) {
    struct page_table* table = (struct page_table*) malloc(sizeof(struct page_table));

    table->page_count = page_count;
    table->frame_count = frame_count;
    table->algorithm = algorithm;
    table->entries = (struct page_table_entry**) malloc(sizeof(struct page_table_entry*) * page_count);

    for (int i = 0; i < page_count; ++i) {
        struct page_table_entry *entry = (struct page_table_entry*) malloc(sizeof(struct page_table_entry));
        entry->page = i;
        entry->valid = 0;
        entry->freq = 0;
        table->entries[i] = entry;
    }

    table->frameIndex = 0;
    table->faults = 0;
    table->verbose = verbose;

    return table;
}

void page_table_destroy(struct page_table** pt) {
    for (int i = 0; i < (*pt)->page_count; ++i) {
        free((*pt)->entries[i]);
    }
    free((*pt)->entries);
    free(*pt);
    *pt = NULL;
}

void page_table_access_page(struct page_table *pt, int page) {
    struct page_table_entry* entry = pt->entries[page];
    entry->freq++;

    if(entry->valid == 1) {
        return;
    }

    pt->faults++;
    entry->order = order;
    order++;

    if(pt->frameIndex < pt->frame_count) {
        entry->frame = pt->frameIndex;
        pt->frameIndex++;
    } else {
        int minIndex = 0;
        int minPage = pt->entries[0]->order;
        for (int i = 1; i < pt->page_count; ++i) {
            if (pt->entries[i]->order < minPage && entry->page != pt->entries[i]->page) {
                minPage = pt->entries[i]->order;
                minIndex = i;
            }
        }
        pt->entries[minIndex]->valid = 0;
        entry->frame = pt->entries[minIndex]->frame;
    }

    entry->valid = 1;
}

void page_table_display(struct page_table *pt){
    printf("\n== Page Table ==\n");
    char* s;
    switch (pt->algorithm) {
        case FIFO:
            s = "FIFO";
            break;
        case LRU:
            s = "LRU";
            break;
        case MFU:
            s = "MFU";
            break;
    }
    printf("Mode: %s\n", s);
    printf("Page Faults: %d\n", pt->faults);
    printf("page frame | dirty valid\n");

    page_table_display_contents(pt);
}

void page_table_display_contents(struct page_table *pt){
    for (int i = 0; i < pt->page_count; ++i) {
    struct page_table_entry* entry = pt->entries[i];

    printf("%d\t %d |\t 0 %d\n",
        entry->page,
        entry->frame,
        entry->valid);
    }
}