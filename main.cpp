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
void create_contact(Book &book);
void list_contacts(Book &book);
void list_groups(Book &book);
void delete_contact(Book &book);
void save_file(Book &book, const char* file_name);

namespace contact_space{

    struct States{
        int save = 0;
        int cancel = 1;
        int editing = 2;
    };

    States available_states; 

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

    int process_input(Contact &contact, int input){
        int state = available_states.editing;

        switch(input){
        case 0: set_name(contact); break;
        case 1: set_phone(contact); break;
        case 2: set_email(contact); break;
        case 3: set_address(contact); break;
        case 4: set_instagram(contact); break;
        case 5: state = available_states.cancel; break;
        case 6: state = available_states.save; break;
        default:
            cout << "Opção inválida." << endl;
        }

        return state;
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

    bool process_input(Book&book, int input, const char* file_name){
        bool want_exit = false;

        switch(input){
        case 0: create_contact(book); save_file(book, file_name); break;
        case 1: list_contacts(book); break;
        case 2: list_groups(book); break;

        case 4: delete_contact(book); save_file(book, file_name); break;
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

    if(!book.qtde_contacts)
        cout << "Nenhum contato cadastrado." << endl;
    else{
        for(int i = 0; i < book.qtde_contacts; i++){
            cout << "ID: " << i << endl;
            cout << "Nome: " << book.contacts[i].name << endl;
            cout << "Número: " << book.contacts[i].phone << endl;
            cout << "---------------------------" << endl;
        }
    }
}

void list_groups(Book& book){
    cout << ">> GRUPOS" << endl;
    
    if(!book.qtde_groups)
        cout << "Nenhum grupo cadastrado." << endl;
    else{
        for(int i = 0; i < book.qtde_groups; i++){
            cout << "Nome do grupo: "<< book.groups[i].name << endl;
            cout << "Membros: " << endl;

            for(int j=0; j < book.groups->qtde_members; j++){
                cout << " - "<< book.groups[i].members[j] << endl;
            }

            cout << "---------------------------" << endl;
        }
    }
}

void create_contact(Book &book){
    Contact contact;
    int state;
    int input;

    while(true){
        contact_space::show_insert_menu();
        
        cout << "Opçao desejada: ";
        cin >> input;

        state = contact_space::process_input(contact, input);

        if(state == contact_space::available_states.save){
            book.contacts[book.qtde_contacts] = contact;
            book.qtde_contacts++;
            break;
        }

        if(state == contact_space::available_states.cancel){
            break;
        }
    }   
}

void run_book_menu(Book& book, const char* file_name){
    bool want_exit = false;
    int input;
        
    while(true){
        book_space::show_menu();

        cout << "Opçao desejada: ";
        cin >> input;

        want_exit = book_space::process_input(book, input, file_name);
        
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

void load_file(Book &book, const char* file_name){
    // ifstream ifile;

    // ifile.read((char*)&book, sizeof(book));
    
    // ifile.close(); 
    // FILE* fin = fopen(file_name, "rb");
    // fread(&book, sizeof(Book), 1, fin);
    // fclose(fin);

    FILE *fout = fopen(file_name, "rb");
    fread(&book.qtde_contacts, sizeof(int), 1, fout);
    fread(&book.qtde_groups, sizeof(int), 1, fout);

    fread(book.contacts, sizeof(Contact), book.qtde_contacts, fout);
    fread(book.groups, sizeof(Group), book.qtde_groups, fout);
}

void save_file(Book &book, const char* file_name){
    // ifstream ifile;

    FILE *fout = fopen(file_name, "wb");
    fwrite(&book.qtde_contacts, sizeof(int), 1, fout);
    fwrite(&book.qtde_groups, sizeof(int), 1, fout);

    fwrite(book.contacts, sizeof(Contact), book.qtde_contacts, fout);
    fwrite(book.groups, sizeof(Group), book.qtde_groups, fout);
    // for(int i=0; i < book.qtde_contacts; ++i){
    //     fwrite(&book.contacts[i], sizeof(Contact), 1, fout);
    // }
    
    // for(int i=0; i < book.qtde_groups; ++i){
    //     fwrite(&book.groups[i], sizeof(Group), 1, fout);
    // }

    fclose(fout);
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
        
    run_book_menu(book, file_name);
}

int main(){
    run_bookcontact();

    return 0;
}
