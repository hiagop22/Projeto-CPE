#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <ctime>

#define MAX_CONTACTS 1000
#define MAX_GROUPS 500
#define CHAR_ARRAY_SIZE 30

using namespace std;

struct Date;
struct Address;
struct Contact;
struct Book;

struct Date{
    char day[CHAR_ARRAY_SIZE];
    char month[CHAR_ARRAY_SIZE];
    char year[CHAR_ARRAY_SIZE];
};

struct Address {
    char country[CHAR_ARRAY_SIZE];
    char city[CHAR_ARRAY_SIZE];
    char postal_code[CHAR_ARRAY_SIZE];
};

struct Contact{
    char name[CHAR_ARRAY_SIZE];
    char phone[CHAR_ARRAY_SIZE];
    char email[CHAR_ARRAY_SIZE];
    Address address;
    char instagram[CHAR_ARRAY_SIZE];
    Date created;
    Date last_modification;
};

struct Book{
    int qtde_contacts;
    
    Contact *contacts[MAX_CONTACTS];
};

#endif