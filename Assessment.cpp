#define _CRT_SECURE_NO_WARNINGS
#include "Assessment.h"
#include <iostream>
#include <stdlib.h>
#include <cstring>

namespace seneca
{
    Assessment a;
    bool read(int &value, FILE *fptr)
    {
        return fscanf(fptr, "%d", &value) == 1;
    }
    bool read(double &value, FILE *fptr)
    {
        return fscanf(fptr, "%lf", &value) == 1;
    }
    bool read(char *cstr, FILE *fptr)
    {
        return fscanf(fptr, ",%60[^\n]\n", cstr) == 1;
    }
    bool read(Assessment& assess, FILE* fptr) {
        double tempMark;
        char tempTitle[60 + 1]; // Temporary buffer for the title, +1 for null terminator

        // Attempt to read the mark and the title from the file
        if (read(tempMark, fptr) && read(tempTitle, fptr)) {
            // Allocate memory for the mark
            assess.m_mark = new double[sizeof(tempMark) + 1];
            if (!assess.m_mark) {
                // Memory allocation for the mark failed
                return false;
            }

            // Copy the mark into the dynamically allocated memory
            *assess.m_mark = tempMark;

            // Allocate memory for the title with the exact size needed
            int titleLength = strlen(tempTitle);
            assess.m_title = new char[titleLength + 1]; // +1 for null terminator
            if (!assess.m_title) {
                // Memory allocation for the title failed
                free(assess.m_mark); // Free the previously allocated memory for the mark
                assess.m_mark = NULL; // Ensure the pointer is null after freeing
                return false;
            }

            // Copy the title into the dynamically allocated memory
            std::strcpy(assess.m_title, tempTitle);

            return true; // Both the mark and title were successfully read and stored
        }

        // If reading the mark or title failed, or memory allocation failed
        return false;
    }
    void freeMem(Assessment*& aptr, int size) {
        for (int i = 0; i < size; ++i) {
            delete aptr[i].m_mark; // Free the memory for the mark
            delete[] aptr[i].m_title; // Free the memory for the title (allocated as an array)
            aptr[i].m_mark = nullptr;
            aptr[i].m_title = nullptr;
        }
        delete[] aptr; // Free the entire array
        aptr = nullptr; // Avoid dangling pointer
    }

    int read(Assessment*& aptr, FILE* fptr) {
        int numberOfRecords = 0;

        // Read the number of upcoming records
        if (fscanf(fptr, "%d", &numberOfRecords) == 0) {
            std::cout << numberOfRecords;
            return 0; // Failed to read the number of records or invalid number
        }

        // Allocate memory for the dynamic array of Assessments using new
        aptr = new Assessment[numberOfRecords];

        int successfulReads = 0;
        for (int i = 0; i < numberOfRecords; i++) {
            // Initialize pointers to nullptr to avoid undefined behavior in case read fails
            aptr[i].m_mark = new double[numberOfRecords + 1];
            aptr[i].m_title = new char[numberOfRecords + 1];
            aptr[i].m_mark = nullptr;
            aptr[i].m_title = nullptr;

            // Read each record; if read fails, break the loop
            if (!read(aptr[i], fptr)) {
                break;
            }
            successfulReads++;
        }

        if (successfulReads != numberOfRecords) {
            // If the number of successful reads doesn't match the expected number, clean up
            freeMem(aptr, successfulReads); // Use the already defined freeMem function
            return 0;
        }

        // If all records are successfully read, return the number of records
        return numberOfRecords;
    }
}