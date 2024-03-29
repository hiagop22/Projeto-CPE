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
void menu_update_contact(Book &book);
void save_file(Book &book, const char* file_name);

namespace contact_space{

    struct States{
        int save = 0;
        int cancel = 1;
        int editing = 2;
    };

    States available_states; 

    void show_insert_menu(){
        cout << "## Inserção / Atualização de contato ## \n" << endl;
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

    int process_input(Contact *p_contact, string input){
        int state = available_states.editing;

        if(!is_a_number(input))
            input = "-1";

        switch(stoi(input)){
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
    cin.getline((*contact).name, CHAR_ARRAY_SIZE);
}

void set_phone(Contact *contact){
    cout << "Digite o telefone: ";
    cin.getline((*contact).phone, CHAR_ARRAY_SIZE);
}

void set_email(Contact *contact){
    cout << "Digite o e-mail: ";
    cin.getline((*contact).email, CHAR_ARRAY_SIZE);
}

void set_address(Contact *contact){
    cout << "Digite o país: ";
    cin.getline((*contact).address.country, CHAR_ARRAY_SIZE);
    cout << "Digite a cidade: ";
    cin.getline((*contact).address.city, CHAR_ARRAY_SIZE);
    cout << "Digite a CEP (0 se não souber): ";
    cin.getline((*contact).address.postal_code, CHAR_ARRAY_SIZE);
}

void set_instagram(Contact *contact){
    cout << "Digite o intagram: ";
    cin.getline((*contact).instagram, CHAR_ARRAY_SIZE);
}

namespace book_space{
    void show_menu(){
        cout << "## Menu da agenda de contatos ## \n" << endl;
        cout << "0 - Criar usuário" << endl;
        cout << "1 - Listar contatos" << endl;
        cout << "2 - Buscar contatos" << endl;
        cout << "3 - Deletar contato" << endl;
        cout << "4 - Atualizar contato" << endl;
        cout << "5 - Sair" << endl;
    }

    bool process_input(Book&book, string input, const char* file_name){
        bool want_exit = false;
        
        if(!is_a_number(input))
            input = "-1";
        
        switch(stoi(input)){
        case 0: create_contact(book); save_file(book, file_name); break;
        case 1: list_contacts(book); break;
        case 2: menu_find_contact(book); break;
        case 3: menu_delete_contact(book); save_file(book, file_name); break;
        case 4: menu_update_contact(book); save_file(book, file_name); break;
        case 5: want_exit = true; break;
        default:
            cout << "Opção inválida." << endl;
        }
        
        return want_exit;
    }   
}

bool compare_contacts(const Contact *p_contact1, const Contact *p_contact2){
    string name1 = std::string((*p_contact1).name);
    string name2 = std::string((*p_contact2).name);

    string phone1 = std::string((*p_contact1).phone);
    string phone2 = std::string((*p_contact2).phone);

    if(name2 > name1)
        return 1;

    if(name2 == name1){
        if(phone2 > phone1)
            return 1;
    }

    return 0;
}

void sort_book(Book& book){
    sort(book.contacts, book.contacts + book.qtde_contacts, compare_contacts);
}

int find_contact_id(Book& book, string name){
    // A mixed combination of 
    // https://cplusplus.com/reference/string/string/find/
    // and https://www.scaler.com/topics/removing-whitespace-from-a-string-in-cpp/
    // Robust to case sensitive and non complete names
    // If there is a contact named Hiago Santos it will find it if you only write santos,
    // but if there are the registers Hiago Santos and Santos Silva
    // you will receive the second occurrence, because the algorithm 
    // prioritize the leftmost occurrence
    int id = -1;
    string name2find = string_tolower(name);

    name2find.erase(remove_if(name2find.begin(), name2find.end(), ::isspace), name2find.end());

    string candidate_name;
    size_t found;
    size_t last_post_found = 10000;

    for(int i=0; i < book.qtde_contacts; ++i){
        candidate_name = string_tolower(book.contacts[i]->name);

        candidate_name.erase(remove_if(candidate_name.begin(), candidate_name.end(), ::isspace), candidate_name.end());

        found = candidate_name.find(name2find);

        if (found!=string::npos){
            if(found < last_post_found){
                id = i;
                last_post_found = found;
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
        cout << "Instagram: " << book.contacts[contact_id]->instagram << endl;

        cout << "Address: " << endl;
        cout << "- País: " << book.contacts[contact_id]->address.country << endl;
        cout << "- Cidade: " << book.contacts[contact_id]->address.city << endl;
        cout << "- CEP: " << book.contacts[contact_id]->address.postal_code << endl;
        
        // cout << "Data criação: " << endl;
        // cout << "- Dia: " << book.contacts[contact_id]->created.day << endl;
        // cout << "- Mês: " << book.contacts[contact_id]->created.month << endl;
        // cout << "- Ano: " << book.contacts[contact_id]->created.year << endl;

        // cout << "Data última modificação: " << endl;
        // cout << "- Dia: " << book.contacts[contact_id]->last_modification.day << endl;
        // cout << "- Mês: " << book.contacts[contact_id]->last_modification.month << endl;
        // cout << "- Ano: " << book.contacts[contact_id]->last_modification.year << endl;
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

void menu_update_contact(Book &book){
    string input_name;
    int contact_id;
    cout << "Digite o nome do contato a ser atualizado: ";
    
    getline(cin, input_name);
    contact_id = show_contact_info(book, input_name);
    if(contact_id < 0)
        cout << "Contato não encontrado na agenda." << endl;
    else{
        cout << "Esse o contato mesmo que deseja atualizar ? (y/n) ";

        string input;
        getline(cin, input);
        if(!string_tolower(input).compare("y")){
            Contact *p_contact = new Contact;
            *p_contact = *book.contacts[contact_id];

            int state;
            string input_menu;

            while(true){
                contact_space::show_insert_menu();
                
                cout << "Opçao desejada: ";
                
                getline(cin, input_menu);
                state = contact_space::process_input(p_contact, input_menu);

                if(state == contact_space::available_states.save){
                    Contact *tmp_contact = book.contacts[contact_id];
                    book.contacts[contact_id] = p_contact;
                    delete tmp_contact;
                    break;
                }

                if(state == contact_space::available_states.cancel){
                    delete p_contact;
                    break;
                }
            }   
            }
        }
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
        cout << "Contato encontrado. Deseja mesmo deletar? (y/n) " ;
        
        // cin >> input;
        getline(cin, input);
        
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

void clear_strings_in_contact(Contact *p_contact){
    (*p_contact).name[0] = '\0';
    (*p_contact).phone[0] = '\0';
    (*p_contact).email[0] = '\0';
    (*p_contact).instagram[0] = '\0';
    (*p_contact).address.country[0] = '\0';
    (*p_contact).address.city[0] = '\0';
    (*p_contact).address.postal_code[0] = '\0';
}

void create_contact(Book &book){
    Contact *p_contact = new Contact;
    int state;
    string input;

    clear_strings_in_contact(p_contact);
    while(true){
        contact_space::show_insert_menu();
        
        cout << "Opçao desejada: ";
        
        getline(cin, input);
        state = contact_space::process_input(p_contact, input);

        if(state == contact_space::available_states.save){
            book.contacts[book.qtde_contacts] = p_contact;
            book.qtde_contacts++;
            
            sort_book(book);
            
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
    string input;
        
    while(true){
        book_space::show_menu();

        cout << "Opçao desejada: ";
        
        getline(cin, input);
        want_exit = book_space::process_input(book, input, file_name);
        
        if(want_exit){
            clean_book(book);
            break;
        }
    }
}

void load_file(Book &book, const char* file_name){
    FILE *file = fopen(file_name, "rb");
    fread(&book.qtde_contacts, sizeof(int), 1, file);

    for(int i=0; i < book.qtde_contacts; i++){
        Contact *p_contact = new Contact;
        fread(p_contact, sizeof(Contact), 1, file);
        book.contacts[i] = p_contact;
    }
    
    fclose(file);
}


void save_file(Book &book, const char* file_name){
    FILE* file = fopen(file_name, "wb");

    fwrite(&book.qtde_contacts, sizeof(int), 1, file);

    for(int i=0; i < book.qtde_contacts; ++i)
        fwrite(book.contacts[i], sizeof(Contact), 1, file);
    
    fclose(file);
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
