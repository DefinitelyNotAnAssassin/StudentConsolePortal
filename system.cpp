#include <iostream>
#include "sqlite/sqlite3.h"
#include <string>

// Function to create a new course in the courses table
using namespace std;
sqlite3* db;


void inputCourse();
void createCourse(sqlite3* db, const char* name, const char* description, const char* instructor, const char* start_date, const char* end_date);
void readCourses(sqlite3* db,  string username);

void createUser(string username, string email, string password, string role);
bool login(string username, string password);

int createEnrollment(sqlite3* db, int user_id, int course_id, string enrollment_date);
void read_enrollments(sqlite3* db, const string& username);

int createGrade(sqlite3* db, int user_id, int content_id, int grade_value, string date);
int readGrade(sqlite3* db, const string& username);



int main(){
   sqlite3_open("database.db", &db);


    while(1){
    string username, password;

    cout << "\n\n\nStudent Learning Management System (Console)" << endl << endl;
    cout << "Login" <<endl << endl;
    
    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    if (login(username, password)){
        int option;
        cout << "Welcome " << username << endl <<"Choose an action: " << endl <<"1. View Courses\n2. View Grades\n\nAction: ";
        cin >> option;

        switch(option){
            case 1:
                read_enrollments(db, username);
            case 2:
                break;
            default:
                cout << "Invalid Option";
        }
    }
    else{
        continue;
    }

    }
    


    

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

void readCourses(sqlite3* db, string username) {
    sqlite3_stmt* stmt;
    string sql = "SELECT courses.name, courses.description, users.username, courses.start_date, courses.end_date "
                 "FROM enrollments "
                 "JOIN courses ON enrollments.course_id = courses.id "
                 "JOIN users ON courses.instructor_id = users.id "
                 "WHERE enrollments.user_id = (SELECT id FROM users WHERE username = ?)";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        cerr << "Failed to bind parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    cout << "Courses enrolled by " << username << ":" << endl;
    cout << "-----------------------------------------------" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "Name: " << sqlite3_column_text(stmt, 0) << endl;
        cout << "Description: " << sqlite3_column_text(stmt, 1) << endl;
        cout << "Instructor: " << sqlite3_column_text(stmt, 2) << endl;
        cout << "Start Date: " << sqlite3_column_text(stmt, 3) << endl;
        cout << "End Date: " << sqlite3_column_text(stmt, 4) << endl;
        cout << "-----------------------------------------------" << endl;
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
        cerr << "\n\nIncorrect Username/Password"<< endl;
        return false;
    }
}


int create_enrollment(sqlite3* db, int user_id, int course_id, string enrollment_date) {
    string sql = "INSERT INTO enrollments (user_id, course_id, enrollment_date) "
                      "VALUES (" + to_string(user_id) + ", " + to_string(course_id) + ", '" + enrollment_date + "');";

    char* errMsg;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "Error inserting enrollment: " << errMsg << endl;
        sqlite3_free(errMsg);
        return -1;
    }

    return sqlite3_last_insert_rowid(db);
}
void read_enrollments(sqlite3* db, const string& username) {
    string sql = "SELECT enrollments.id, users.username, courses.name, enrollments.enrollment_date "
                 "FROM enrollments "
                 "JOIN users ON enrollments.user_id = users.id "
                 "JOIN courses ON enrollments.course_id = courses.id "
                 "WHERE users.username = ?";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    if (rc != SQLITE_OK) {
        cerr << "Failed to bind parameter: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "Enrollments for " << username << ":" << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int enrollment_id = sqlite3_column_int(stmt, 0);
        string username = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        string course_name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        string enrollment_date = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));

        cout << "Enrollment ID: " << enrollment_id << endl;
        cout << "Username: " << username << endl;
        cout << "Course Name: " << course_name << endl;
        cout << "Enrollment Date: " << enrollment_date << endl;
    }

    sqlite3_finalize(stmt);
}
int create_grade(sqlite3* db, int user_id, int content_id, int grade_value, string date) {
    string sql = "INSERT INTO grades (user_id, content_id, grade_value, date) VALUES (" +
                        to_string(user_id) + ", " + to_string(content_id) + ", " + 
                        to_string(grade_value) + ", '" + date + "')";

    char* errMsg;
    int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating grade: " << errMsg << endl;
        sqlite3_free(errMsg);
        return 1;
    }

    cout << "Grade created successfully" << endl;
    return 0;
}
int readGrade(sqlite3* db, const string& username) {
    string sql = "SELECT c.name, g.grade_value, g.date "
                      "FROM grades g "
                      "JOIN contents ct ON g.content_id = ct.id "
                      "JOIN courses c ON ct.course_id = c.id "
                      "JOIN enrollments e ON c.id = e.course_id "
                      "JOIN users u ON e.user_id = u.id "
                      "WHERE u.username = '" + username + "'";

    // Execute the SQL statement
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return rc;
    }

    // Print the results
    cout << "Grades for user " << username << endl;
    cout << "-------------------------" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string courseName = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        int gradeValue = sqlite3_column_int(stmt, 1);
        string date = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        cout << courseName << " - Grade: " << gradeValue << " - Date: " << date << endl;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    return SQLITE_OK;
}

