#include <iostream>
#include "sqlite/sqlite3.h"
#include <string>

// Function to create a new course in the courses table
using namespace std;
sqlite3* db;
void inputCourse();
void createCourse(sqlite3* db, const char* name, const char* description, const char* instructor, const char* start_date, const char* end_date);
void readCourses(sqlite3* db, const char* filter);
void createUser(string username, string email, string password, string role);
bool login(string username, string password);

int main(){
   sqlite3_open("database.db", &db);
    login("professor", "123");
}



void inputCourse(){
    string name, description, instructor, start_date, end_date;
    
    cout << "Course Name: ";
    getline(cin, name);

    cout << "Course Description: ";
  
    getline(cin, description);

    cout << "Course Instructor ID: ";
    cin >> instructor;
    
    cout << "Start Date(YYYY-MM-DD): ";
    cin >> start_date;

    cout << "End Date(YYYY-MM-DD): ";
    cin >> end_date;

    createCourse(db, name.c_str(), description.c_str(), instructor.c_str(), start_date.c_str(), end_date.c_str());

}
void createCourse(sqlite3* db, const char* name, const char* description, const char* instructor, const char* start_date, const char* end_date) {
    char* errorMessage;
    string sql = "INSERT INTO courses (name, description, instructor_id, start_date, end_date) VALUES ('" + string(name) + "', '" + string(description) + "', '" + string(instructor) + "', '" + string(start_date) + "', '" + string(end_date) + "');";

    int result = sqlite3_exec(db, sql.c_str(), NULL, 0, &errorMessage);
    if (result != SQLITE_OK) {
        cerr << "Error creating course: " << sqlite3_errmsg(db) << endl;
        sqlite3_free(errorMessage);
    }
    else {
        cout << "Course created successfully." << endl;
    }
}

void readCourses(sqlite3* db, const char* filter = NULL) {
    sqlite3_stmt* stmt;
    int result;
    const char* tail;

    if (filter == NULL) {
        result = sqlite3_prepare_v2(db, "SELECT * FROM courses;", -1, &stmt, &tail);
    }
    else {
        string sql = "SELECT * FROM courses WHERE name LIKE '%" + string(filter) + "%';";
        result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, &tail);
    }

    if (result != SQLITE_OK) {
        cerr << "Error reading courses: " << sqlite3_errmsg(db) << endl;
    }
    else {
        cout << "Courses:" << endl;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            cout << "ID: " << sqlite3_column_int(stmt, 0) << endl;
            cout << "Name: " << sqlite3_column_text(stmt, 1) << endl;
            cout << "Description: " << sqlite3_column_text(stmt, 2) << endl;
            cout << "Instructor: " << sqlite3_column_text(stmt, 3) << endl;
            cout << "Start Date: " << sqlite3_column_text(stmt, 4) << endl;
            cout << "End Date: " << sqlite3_column_text(stmt, 5) << endl;
        }
    }

    sqlite3_finalize(stmt);
}

void createUser(string username, string email, string password, string role) {
    char* zErrMsg = 0;
    string sql = "INSERT INTO users (username, email, password, role) "
                 "VALUES ('" + username + "','" + email + "', '" + password + "', '" + role + "');";
    int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating user: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "User created successfully!" << endl;
    }
}

bool login(string username, string password) {
    sqlite3_stmt *stmt;
    string sql = "SELECT * FROM users WHERE username = ? AND password = ?";
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        cout << "Login successful!" << endl;
        return true;
    } else {
        cerr << "Error logging in: " << sqlite3_errmsg(db) << endl;
        return false;
    }
}


