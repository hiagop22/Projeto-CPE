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
struct Group;
struct Book;

struct Date{
    int dia;
    int mes;
    int ano;
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

struct Group{
    string name;
    int qtde_members;
    Contact *members[MAX_CONTACTS];
};

struct Book{
    int qtde_contacts, qtde_groups;
    
    Contact contacts[MAX_CONTACTS];
    Group groups[MAX_GROUPS];
};

#endif