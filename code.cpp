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
struct MenuModes;
struct ContactMenu;


void set_name(Contact& contact);
void set_phone(Contact& contact);
void set_email(Contact& contact);
void set_address(Contact& contact);
void set_instagram(Contact& contact);
void insert_contact(Book &book);

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
    Contact *members[MAX_CONTACTS];
};

struct Book{
    int qtde_contacts, qtde_groups;
    
    Contact contacts[MAX_CONTACTS];
    Group groups[MAX_GROUPS];
};


namespace contact_space{
    struct Field{
        const int val;
        const char* desctiption;
        void (*p_function)(Contact&);
    };

    struct MenuModes{
                const char* insert = "Inserir";
                const char* update = "Atualizar";
                    };

    struct Menu{
            string mode;
            Field infos[5] = {{.val = 0, .desctiption = "nome", .p_function = &set_name},
                            {.val = 1, .desctiption = "telefone", .p_function = &set_phone},
                            {.val = 2, .desctiption = "e-mail", .p_function = &set_email},
                            {.val = 3, .desctiption = "endereço", .p_function = &set_address},
                            {.val = 4, .desctiption = "instagram", .p_function = &set_instagram}};


            Field options[3] = {{.val = 5, .desctiption = "Cancelar"},
                                {.val = 6, .desctiption = "Salvar"},
                                {.val = 7, .desctiption = "deletar contato"}};
                                    
    };

    void show_option_fields(Menu& menu, bool show_del_option){
        int size_options = sizeof(menu.options) / sizeof(Field);
        
        if(!show_del_option)
            size_options--;

        for(int i=0; i < size_options; ++i){
            cout << menu.options[i].val << " - " << menu.options[i].desctiption << endl;
        }
    }
}


void set_name(Contact& contact){
    cout << "Digite o nome: ";
    cin >> contact.name;
}

void set_phone(Contact& contact){
    cout << "Digite o telefone: ";
    cin >> contact.phone;
}

void set_email(Contact& contact){
    cout << "Digite o e-mail: ";
    cin >> contact.email;
}

void set_address(Contact& contact){
    cout << "Digite o país: ";
    cin >> contact.address.country;
    cout << "Digite a cidade: ";
    cin >> contact.address.city;
    cout << "Digite a CEP (0 se não souber): ";
    cin >> contact.address.postal_code;
}

void set_instagram(Contact& contact){
    cout << "Digite o intagram: ";
    cin >> contact.instagram;
}

namespace book_space{
    struct Field{
        const int val;
        const char* desctiption;
        void (*p_function)(Book&);
    };

    struct Menu{
                Field options[5] = {{.val = 0, .desctiption = "Criar contato", .p_function = &insert_contact},
                                {.val = 1, .desctiption = "Listar contatos"},
                                {.val = 2, .desctiption = "Listar grupos"},
                                {.val = 3, .desctiption = "Buscar contatos"},
                                {.val = 4, .desctiption = "Sair"},
                                };
                                    
            };
    
    void show_option_fields(Menu& menu){
        int size_options = sizeof(menu.options) / sizeof(Field);
        
        for(int i=0; i < size_options; ++i){
            cout << menu.options[i].val << " - " << menu.options[i].desctiption << endl;
        }
    }
}

bool is_update_mode(contact_space::Menu& menu){
    contact_space::MenuModes mode;

    if(!menu.mode.compare(mode.update))
        return true;

    return false;
}



void show_info_filds(contact_space::Menu& menu){
    int size_infos = sizeof(menu.infos) / sizeof(contact_space::Field);

    for(int i=0; i < size_infos; ++i){
        cout << menu.infos[i].val << " - " ;
        cout << menu.mode << " " << menu.infos[i].desctiption << endl;
    }
}

void show_contact_menu(contact_space::Menu& menu){
    
    cout << "## Inserção / Atualização de contato ## \n" << endl;
    
    show_info_filds(menu);

    if(is_update_mode(menu))
        show_option_fields(menu, true);
    else
        show_option_fields(menu, false);
}

void show_book_menu(book_space::Menu& menu){
    
    cout << "## Menu da agenda de contatos ## \n" << endl;
    
    book_space::show_option_fields(menu);
}

bool run_contact_menu(Contact &contact, const char* mode){
    int option = 0;
    bool was_contacted_created;
    contact_space::Menu menu;

    menu.mode = mode;

    show_contact_menu(menu);
    cin >> option;

    

    return was_contacted_created;
}

bool create_contact(Contact &contact){
    bool was_contacted_created;

    contact_space::MenuModes mode;
    
    run_contact_menu(contact, mode.insert);

    return was_contacted_created;
}

void insert_contact(Book &book){
    Contact contact;
    bool was_created;

    was_created = create_contact(contact);

    if(was_created) {
        book.contacts[book.qtde_contacts] = contact;
        book.qtde_contacts++;

    }
}


bool run_option_in_menu(book_space::Menu &menu, Book& book){

    return false;
}

void run_book_menu(Book& book){
    book_space::Menu menu;
    int option = 0;
    bool get_out = false;

    while(true){
        show_book_menu(menu);
        
        cin >> option;

        get_out = run_option_in_menu(menu, book);

        if(get_out)
            break;
    }
}

bool is_there_persistent_book(const char* persistent_file){
    ifstream ifile;
    ifile.open(persistent_file);
    
    if(ifile){
        ifile.close();
        return true;
    }
    return false;
}

void load_persistent_book(Book &book, const char* persistent_file){
    ifstream ifile;

    ifile.read((char*)&book, sizeof(book));
    
    ifile.close(); 
}

void run_bookcontact(){
    Book book;
    const char* persistent_book_name = "book.bin";

    if(is_there_persistent_book(persistent_book_name)){
        cout << "Carregando agenda da memória..." << endl;
        load_persistent_book(book, persistent_book_name);
    }
    else{
        book.qtde_contacts = 0;
        book.qtde_groups = 0;
        
        cout << "Não existe agenda na memória..." << endl;  
    }
        

    run_book_menu(book);
}

int main(){
    run_bookcontact();

    return 0;
}
