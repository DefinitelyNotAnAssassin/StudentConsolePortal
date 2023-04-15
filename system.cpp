#include "sqlite/sqlite3.h"
#include <iostream>
#include <string> 


using namespace std;


// Global Variables to interact with the database
   sqlite3 *db;
   char *zErrMsg = 0;
   sqlite3_stmt* stmt;
   int rc;


// Pre - Definition of Functions 

void create_account();
void login_account();



// Function Pre-declaratation



int main(){
    sqlite3_open("database.db", &db);
    //create_account();


    login_account();

    
}


// Firstname, Lastname, Username, Password, Age, Address, Program, Section

// Function definition

void create_account(){
    string firstname, lastname, username, password, address, program, section;
    int age;

    cout << "Student Console Portal \n\nAccount Registration\n\n\n";
    

    cout << "Username : ";
    cin >> username;

    cout << "Password : ";
    cin >> password;

    cin.ignore();

    cout << "Firstname : ";
    getline(cin, firstname);

    cout << "Lastname : ";
    getline(cin, lastname);

    cout << "Address : ";
    cin >> address;

    cout << "Program : ";
    cin >> program;

    cout << "Section : ";
    cin >> section;

    cout << "Age : ";
    cin >> age;


    string sql = "INSERT INTO account (username, password, firstname, lastname, address, program, section, age) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    
    // Binding the user input to the sql statement 

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, firstname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, lastname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, program.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, section.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 8, age);

    // Everything will get better, just keep moving forward, someday somehow you'll be enlightened - W.M.

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);


    cout << endl << endl << "Account Created Successfully" << endl;



}



// The darkness of this IDE just resonates with the pain 

// ... and I'm making the comments my diary entry lmao.

void login_account(){
    
    cout << "Student Console Portal \n\nAccount Login\n\n\n";


    string username, password;

    cout << "Username : ";
    cin >> username;

    cout << "Password : ";
    cin >> password;

    string sql = "SELECT * FROM account WHERE username = ? AND password = ?";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Words of encouragement for myself, to see while debugging this shit while in pain.

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result == SQLITE_ROW){
        cout << "Successfully login" << endl;
    }
    else{
        cout << "Invalid username/password" <<endl;
    }


    

    

}
