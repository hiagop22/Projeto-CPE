#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <bits/stdc++.h>
#include "structs.h"
#include "utils.h"

void set_name(Contact *contact);
void set_phone(Contact *contact);
void set_email(Contact *contact);
void set_address(Contact *contact);
void set_instagram(Contact *contact);
void create_contact(Book &book);
void list_contacts(Book &book);
void menu_find_contact(Book& book);
void menu_delete_contact(Book &book);
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

    int process_input(Contact *p_contact, int input){
        int state = available_states.editing;

        switch(input){
        case 0: set_name(p_contact); break;
        case 1: set_phone(p_contact); break;
        case 2: set_email(p_contact); break;
        case 3: set_address(p_contact); break;
        case 4: set_instagram(p_contact); break;
        case 5: state = available_states.cancel; break;
        case 6: state = available_states.save; break;
        default:
            cout << "Opção inválida." << endl;
        }

        return state;
    }
}

void set_name(Contact *contact){
    cout << "Digite o nome: ";
    getline(cin, (*contact).name);
}

void set_phone(Contact *contact){
    cout << "Digite o telefone: ";
    getline(cin, (*contact).phone);
}

void set_email(Contact *contact){
    cout << "Digite o e-mail: ";
    getline(cin, (*contact).email);
}

void set_address(Contact *contact){
    cout << "Digite o país: ";
    getline(cin, (*contact).address.country);
    cout << "Digite a cidade: ";
    getline(cin, (*contact).address.city);
    cout << "Digite a CEP (0 se não souber): ";
    getline(cin, (*contact).address.postal_code);
}

void set_instagram(Contact *contact){
    cout << "Digite o intagram: ";
    cin >> (*contact).instagram;
}

namespace book_space{
    void show_menu(){
        cout << "## Menu da agenda de contatos ## \n" << endl;
        cout << "0 - Criar usuário" << endl;
        cout << "1 - Listar contatos" << endl;
        cout << "2 - Buscar contatos" << endl;
        cout << "3 - Deletar contato" << endl;
        cout << "4 - Sair" << endl;
    }

    bool process_input(Book&book, int input, const char* file_name){
        bool want_exit = false;

        switch(input){
        case 0: create_contact(book); save_file(book, file_name); break;
        case 1: list_contacts(book); break;
        case 2: menu_find_contact(book); break;
        case 3: menu_delete_contact(book); save_file(book, file_name); break;
        case 4: want_exit = true; break;
        default:
            cout << "Opção inválida." << endl;
        }
        
        return want_exit;
    }   
}

bool compare_contacts(const Contact *p_contact1, const Contact *p_contact2){
    if((*p_contact2).name > (*p_contact1).name)
        return 1;

    if((*p_contact2).name == (*p_contact1).name){
        if((*p_contact2).phone > (*p_contact1).phone)
            return 1;
    }

    return 0;
}

void sort_book(Book& book){
    sort(book.contacts, book.contacts + book.qtde_contacts, &compare_contacts);
}

int find_contact_id(Book& book, string name){
    // A mixed combination of 
    // https://cplusplus.com/reference/string/string/find/
    // and https://www.scaler.com/topics/removing-whitespace-from-a-string-in-cpp/
    // Robust to case sensitive, non complete names and white spaces
    // If there is a contact named Hiago Santos it will find it,
    // but if there are the registers Hiago Santos and Santos Silva
    // you will receive the second occurance, because the algorithm will
    // prioritize the leftmost name
    int id = -1;
    string name2find = string_tolower(name);

    name2find.erase(remove_if(name2find.begin(), name2find.end(), ::isspace), name2find.end());

    string candidate_name;
    size_t found;
    size_t last_post_found;

    for(int i=0; i < book.qtde_contacts; ++i){
        candidate_name = string_tolower(book.contacts[i]->name);

        candidate_name.erase(remove_if(candidate_name.begin(), candidate_name.end(), ::isspace), candidate_name.end());

        found = candidate_name.find(name2find);

        if (found!=string::npos){
            if(found < last_post_found){
                id = i;
                }
            }
        }

    return id;
}

int show_contact_info(Book& book, string name){
    int contact_id;

    contact_id = find_contact_id(book, name);

    if(contact_id>=0){
        cout << "Nome: " << book.contacts[contact_id]->name << endl;
        cout << "Telefone: " << book.contacts[contact_id]->phone << endl;
        cout << "E-mail: " << book.contacts[contact_id]->email << endl;

        cout << "Address: " << endl;
        cout << "- País: " << book.contacts[contact_id]->address.country << endl;
        cout << "- Cidade: " << book.contacts[contact_id]->address.city << endl;
        cout << "- CEP: " << book.contacts[contact_id]->address.postal_code << endl;
        
        cout << "Data criação: " << endl;
        cout << "- Dia: " << book.contacts[contact_id]->created.day << endl;
        cout << "- Mês: " << book.contacts[contact_id]->created.month << endl;
        cout << "- Ano: " << book.contacts[contact_id]->created.year << endl;

        cout << "Data última modificação: " << endl;
        cout << "- Dia: " << book.contacts[contact_id]->last_modification.day << endl;
        cout << "- Mês: " << book.contacts[contact_id]->last_modification.month << endl;
        cout << "- Ano: " << book.contacts[contact_id]->last_modification.year << endl;
    }

    return contact_id;
}

void delete_contact(Book& book, int contact_id, bool show_messages=true){
    delete book.contacts[contact_id];

    if(book.qtde_contacts > 1){
        book.contacts[contact_id] = book.contacts[book.qtde_contacts-1];

        if(show_messages)
            cout << "Contato deletado com sucesso" << endl;
    }
    else{
        if(show_messages){
            cout << "A agenda agora está vazia" << endl;
        }
    }

    book.qtde_contacts--;            
}

void clean_book(Book& book){

    for(int i = 0; i < book.qtde_contacts; i++)
        delete_contact(book, i, false);
}

void menu_find_contact(Book& book){
    string input;
    int contact_id;
    cout << "Digite o nome do contato a ser buscado: ";
    
    getline(cin, input);
    contact_id = show_contact_info(book, input);

    if(contact_id < 0)
        cout << "Contato não encontrado na agenda." << endl;
}

void menu_delete_contact(Book& book){
    string input;
    int contact_id = -1;

    cout << "Nome do contato para remover: ";
    getline(cin, input);
    contact_id = show_contact_info(book, input);

    if(contact_id < 0){
        cout << "Contato não encontrado na agenda." << endl;
    }
    else{
        cout << "Contato encontrado. Deseja mesmo deletar? (y/n)" ;
        cin >> input;
        
        if(!string_tolower(input).compare("y")){
            delete_contact(book, contact_id);
        }
        else{
            cout << "Operação de delete cancelada" << endl;
        }
    }
}

void list_contacts(Book& book){
    cout << ">> Contatos" << endl;

    if(!book.qtde_contacts)
        cout << "Nenhum contato cadastrado." << endl;
    else{
        for(int i = 0; i < book.qtde_contacts; i++){
            cout << "Nome: " << book.contacts[i]->name << endl;
            cout << "Número: " << book.contacts[i]->phone << endl;
            cout << "---------------------------" << endl;
        }
    }
}

void create_contact(Book &book){
    Contact *p_contact = new Contact;
    int state;
    int input;

    while(true){
        contact_space::show_insert_menu();
        
        cout << "Opçao desejada: ";
        cin >> input;
        cin.ignore();
        state = contact_space::process_input(p_contact, input);

        if(state == contact_space::available_states.save){
            book.contacts[book.qtde_contacts] = p_contact;
            book.qtde_contacts++;
            break;
        }

        if(state == contact_space::available_states.cancel){
            delete p_contact;
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
        cin.ignore();
        want_exit = book_space::process_input(book, input, file_name);
        
        if(want_exit)
            break;
    }
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

    fread(book.contacts, sizeof(Contact), MAX_CONTACTS, fout);
    
    fclose(fout);
}


// MEXER NISSO DEPOIS
void save_file(Book &book, const char* file_name){
    return;

    FILE *fout = fopen(file_name, "wb");
    fwrite(&book.qtde_contacts, sizeof(int), 1, fout);

    fwrite(book.contacts, sizeof(Contact), MAX_CONTACTS, fout);
    // for(int i=0; i < book.qtde_contacts; ++i){
    //     fwrite(&book.contacts[i], sizeof(Contact), 1, fout);
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
        
        cout << "Não existe agenda na memória..." << endl;  
    }
        
    run_book_menu(book, file_name);
}

int main(){
    run_bookcontact();

    return 0;
}
