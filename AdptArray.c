#include <stdio.h>
#include <stdlib.h>
#include "AdptArray.h"
#include "book.h"

typedef struct AdptArray_ {
    PElement* mem_block;
    int curr_size;
    int capacity;
    COPY_FUNC copy_function;
    DEL_FUNC delete_function;
    PRINT_FUNC print_function;
} AdptArray;

// this function is used to initialize the data in a block of PElement
void initializeMemory(int old_index , int new_index, PElement* memory_block){
    for (size_t i = old_index; i < new_index ; i++) {
        memory_block[i]= NULL;
    }
}

PAdptArray CreateAdptArray(COPY_FUNC copy_function, DEL_FUNC delete_function, PRINT_FUNC print_function){
    PAdptArray new_arr = (PAdptArray) malloc(sizeof(AdptArray));
    // maybe an error can occur
    if (new_arr == NULL) {
        return NULL;
    }
    new_arr->curr_size = 0;
    new_arr->capacity = 0;
    new_arr->copy_function = copy_function;
    new_arr->delete_function = delete_function;
    new_arr->print_function = print_function;
    new_arr->mem_block = NULL;
    return new_arr;
}


void DeleteAdptArray(PAdptArray arr){
    if (arr == NULL ) {
        printf("In delete function the given array is empty");
    } else {
        for (int i = 0  ; i < arr->capacity; i++) {
            if (arr->mem_block[i] != NULL) {
                arr->delete_function(arr->mem_block[i]);
            }else{
                free(arr->mem_block[i]);
            }
        }
    }
    free(arr->mem_block);
    free(arr);
}

Result SetAdptArrayAt(PAdptArray arr, int position, PElement to_insert) {
    // NON-authorized argument
    if (position < 0) {
        return FAIL;
    }
    if (position >= arr->capacity) {
        // during an expansion of the memory block we exand it up to the double of the given position
        int new_capacity = ( position + 1 ) * 2;
        PElement* new_mem_block = (PElement*) realloc(arr->mem_block, new_capacity * sizeof(PElement));
        initializeMemory(arr->capacity, new_capacity, new_mem_block);
        if (new_mem_block == NULL) {
            return FAIL;
        }
        arr->mem_block = new_mem_block;
        arr->capacity = new_capacity;
    }
    // if there is already something there, we replace it
    if (arr->mem_block[position] != NULL) {
        arr->delete_function(arr->mem_block[position]);
    }
    arr->mem_block[position] = arr->copy_function(to_insert);
    // the new position is above our last(position) element
    if (position > arr->curr_size) {
        arr->curr_size = position + 1;
    }
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray arr, int position){
    if (arr == NULL || arr->curr_size <= position || position < 0) {
        return NULL;
    }
    if (arr->mem_block[position] == NULL) {
        return NULL;
    }
    return  arr->copy_function(arr->mem_block[position]);
}

int GetAdptArraySize(PAdptArray arr ) {
    if (arr == NULL) {
        return -1;
    }
    return  arr->curr_size;
}

void PrintDB(PAdptArray arr){
    if (arr == NULL) {
        printf("in function PrintDB : the input arr is empty");
    }
    for (size_t i = 0; i < arr->curr_size; i++) {
        if (arr->mem_block[i] != NULL) {
            arr->print_function(arr->mem_block[i]);
        }
    }
}


// Ressources used for this Assignement 
// -------------------------------------
// https://youtu.be/6Ir4l0VuI7Y
// https://youtu.be/6c1nCsbwUEk
// matseget tirgul 11 of C course 
// https://stackoverflow.com/questions/2141277/how-to-zero-out-new-memory-after-realloc

