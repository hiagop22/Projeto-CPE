#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <ctime>

#define MAX_CONTACTS 1000
#define MAX_GROUPS 500

using namespace std;

struct Date;
struct Address;
struct Contact;
struct Book;

struct Date{
    string day;
    string month;
    string year;
};

struct Address {
    string country;
    string city;
    string postal_code;
};

struct Contact{
    string name;
    string phone;
    string email;
    Address address;
    string instagram;
    Date created;
    Date last_modification;
};

struct Book{
    int qtde_contacts;
    
    Contact *contacts[MAX_CONTACTS];
};

#endif