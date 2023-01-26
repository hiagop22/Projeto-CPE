#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "structs.h"


void set_name(Contact& contact);
void set_phone(Contact& contact);
void set_email(Contact& contact);
void set_address(Contact& contact);
void set_instagram(Contact& contact);
bool insert_contact(Book &book);



namespace contact_space{
    void show_insert_menu(){
        cout << "## Inserção de contato ## \n" << endl;
        cout << "0 - Inserir nome" << endl;
        cout << "1 - Inserir telefone" << endl;
        cout << "2 - Inserir e-mail" << endl;
        cout << "3 - Inserir endereço" << endl;
        cout << "4 - Inserir instagram" << endl;
        cout << "5 - Cancelar" << endl;
        cout << "6 - Salvar" << endl;
    }

    void show_update_menu(){
        cout << "## Alteração de contato ## \n" << endl;
        cout << "0 - Atualizar nome" << endl;
        cout << "1 - Atualizar telefone" << endl;
        cout << "2 - Atualizar e-mail" << endl;
        cout << "3 - Atualizar endereço" << endl;
        cout << "4 - Atualizar instagram" << endl;
        cout << "5 - Cancelar" << endl;
        cout << "6 - Salvar" << endl;
    }

    bool process_input(Contact &contact, int input){
        bool save; 
        bool stop = false;

        do{
            switch(input){
            case 0: set_name(contact); break;
            case 1: set_phone(contact); break;
            case 2: set_email(contact); break;
            case 3: set_address(contact); break;
            case 4: set_instagram(contact); break;
            case 5: stop = true; save = false; break;
            case 6: stop = true; save = true; break;
            default:
                cout << "Opção inválida." << endl;
            }
        }while(stop);
        
        return save;
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
    void show_menu(){
        cout << "## Menu da agenda de contatos ## \n" << endl;
        cout << "0 - Criar usuário" << endl;
        cout << "1 - Listar contatos" << endl;
        cout << "2 - Listar grupos" << endl;
        cout << "3 - Buscar contatos" << endl;
        cout << "4 - Deletar contato" << endl;
        cout << "5 - Sair" << endl;
    }

    bool process_input(Book&book, int input){
        bool want_exit = false;

        switch(input){
        case 0: create_contact(book); break;
        case 1: list_contacts(book); break;
        case 2: list_groups(book); break;

        case 4: delete_contact(book); break;
        case 5: want_exit = true; break;
        default:
            cout << "Opção inválida." << endl;
        }
        
        return want_exit;
    }   
}

void delete_contact(Book& book){
    // Checar se o contato está em algum grupo e deletar do grupo também
}

void list_contacts(Book& book){
    cout << ">> Contatos" << endl;

    for(int i = 0; i < book.qtde_contacts; i++){
        cout << "ID: " << i << endl;
        cout << "Nome: " << book.contacts[i].name << endl;
        cout << "Número: " << book.contacts[i].phone << endl;
        cout << "---------------------------" << endl;
    }
}

void list_groups(Book& book){
    cout << ">> GRUPOS" << endl;

    for(int i = 0; i < book.qtde_groups; i++){
        cout << "Nome do grupo: "<< book.groups[i].name << endl;
        cout << "Membros: " << endl;

        for(int j=0; j < book.groups->qtde_members; j++){
            cout << " - "<< book.groups[i].members[j] << endl;
        }

        cout << "---------------------------" << endl;
    }
}

void create_contact(Book &book){
    Contact contact;
    int option;
    bool was_contact_created;

    contact_space::show_insert_menu();
    cin >> option;

    was_contact_created = contact_space::process_input(contact, option);

    if(was_contact_created){

    }
}

void run_book_menu(Book& book){
    bool want_exit = false;
    int option;

    while(true){
        book_space::show_menu();
        cin >> option;

        want_exit = book_space::process_input(book, option);
        
        if(want_exit)
            break;
    }
}

bool exists_file(const char* persistent_file){
    ifstream ifile;
    ifile.open(persistent_file);
    
    if(ifile){
        ifile.close();
        return true;
    }
    return false;
}

void load_file(Book &book, const char* persistent_file){
    ifstream ifile;

    ifile.read((char*)&book, sizeof(book));
    
    ifile.close(); 
}

void run_bookcontact(){
    Book book;
    const char* file_name = "book.bin";

    if(exists_file(file_name)){
        cout << "Carregando agenda da memória..." << endl;
        load_file(book, file_name);
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
