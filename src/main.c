#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

//void print_usage(char *argv[]) { //<-- orginally this?
void print_usage() {
    printf("Usage: -n -f <file>\n");
    printf("\t -a  - User string to add name,addr,hours . e.g \"Timmy, 123 Fake Street, 120\" .\n");
    printf("\t -d  - Employee number to delete.\n");
    printf("\t -f  - file to create.\n");
    printf("\t -h  - show help menu.\n");
    printf("\t -n  - create new database.\n");
    return;
}

int main(int argc, char *argv[]) {
    char *filepath = NULL;
    char *addstring = NULL;
    char *deletestring = NULL;
    int employee_number = -1;
    bool newfile = false ;
    bool do_delete = false ;
    bool list = false ;
    int dbfd = -1;
    int c;
    struct dbheader_t *dbhdr = NULL;
    struct employee_t *employees = NULL;

    while ((c = getopt(argc, argv, "a:hlnf:")) != -1) {
        switch (c) {
            case 'a':
                    addstring = optarg;
                    break;
            case 'n':
                    newfile = true;
                    break;
            case 'f':
                    filepath = optarg;
                    break;
            case 'l':
                    list = true;
                    break;
            case 'h':
                    print_usage();
                    return 0;
            default : /* '?' */
                    print_usage();
                    return -1;
        }
    }	

    if (filepath == NULL) {
        printf("Filepath is a required argument.\n");
        print_usage(&argv[0]);
        return 0;
    }

    // If flag set, creat new file
    if (newfile) {
        dbfd = create_db_file(filepath); // returns database filedescriptor

        if (dbfd == STATUS_ERROR) {
            printf("Unable to create db file, %s\n");
            return STATUS_ERROR;
        }

        if (create_db_header(&dbhdr) == STATUS_ERROR ) {
            printf("Failed to create db header\n");
            return STATUS_ERROR;
        }
        output_file(dbfd, dbhdr, employees);

    } else {
        dbfd = open_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
             printf("Unable to open db file, %s\n", filepath);
             return -1;
        }
        if (validate_db_header(dbfd, &dbhdr) == -1) {
            printf("Bad db header.\n");
            return -1;
        }
    }

    if (read_employees(dbfd, dbhdr, &employees) != STATUS_SUCCESS) {
        printf("Failed to read employees");
        return 0;
    }

    if (addstring) {
        if (add_employee(&dbhdr, &employees, addstring)) {
            printf("Error adding employee.\n");
            return STATUS_ERROR;
        }
        output_file(dbfd, dbhdr, employees);
    }

    if (list) {
        list_employees(dbhdr, employees);
    }

    //printf("Newfile: %d\n", newfile);
    //printf("Filepath: %s\n", filepath);
    return 0;
}
