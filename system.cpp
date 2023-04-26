#include "sqlite/sqlite3.h"
#include <iostream>
#include <string> 
#include <conio.h>


using namespace std;


// Global Variables to interact with the database
   sqlite3 *db;
   char *zErrMsg = 0;
   sqlite3_stmt* stmt;
   int rc;


string input(string text);
void create_user();
void login_user();

void insert_course();


int main(){
    sqlite3_open("database.db", &db);
    insert_course();
}

void create_user(){
    string username, password, confirm_password, email, role;

    cout << "Username : ";
    cin >> username;

    cout << "Email: ";
    cin >> email;
    password = input("Password: ");
    confirm_password = input("Confirm Password: ");

    // TODO 
    role = "student";

    if (password == confirm_password){
        string query = "INSERT INTO users (username, email, password, role) VALUES (?, ?, ?, ?);";
        // string query = "INSERT INTO users (username, email ,password, role)"


        // ? = wildcard or placeholder 


       





        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, role.c_str(), -1, SQLITE_STATIC);



        // string query = "INSERT INTO users (username, email, password, role) VALUES ("admin", "admin@sdca.edu.ph", "passsword", "sudent")"
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        cout << "User Created Successfully" << endl << endl;

    

    }
    else{
        cout << "Password Doesn't Match" << endl;
    }

    

}


string input(string text){
    char key = 0;
    string password = "";//"ba"
    cout << text;
    while (key != 13)// while the key is not Enter or 13
    {
        key = getch();
        //
        
        if (key != 13 && key != 8 && key != 32){
            cout << '*';
            password.push_back(key);
        }


       
    }
    cout << endl;
    return password;
}



void insert_course(){

    string name, description, start_date, end_date;
    int professor_id;


    cout << "Name: ";
    cin >> name;

    cout << "Description: ";
    cin >> description;

    cout << "Start Date (YYYY-MM-DD): ";
    cin >> start_date;

    cout << "End Date (YYYY-MM-DD): ";
    cin >> end_date;

    cout << "Instructor ID: ";
    cin >> professor_id;


    string sql = "INSERT INTO courses (name, description, start_date, end_date, instructor_id) VALUES (?, ?, ?, ?, ?)";

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    // Prepareation > Binding of Placeholder 


    sqlite3_bind_text(stmt, 1, name.c_str(), -1,  SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1,  SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, start_date.c_str(), -1,  SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, end_date.c_str(), -1,  SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, professor_id);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    




   

}
