#include <algorithm>    // For transform
#include <cstdlib>      // For exit()
#include <fstream>      // For file handling
#include <iostream>     // For input/output
#include <string>       // For strings

#include "facultyLogin.h"
#include "adminLogin.h"

using namespace std;

// Forward declarations
void handleAdminLogin();
void handleFacultyLogin();

// Global objects
admin adminObj;
faculty facultyObj;

int loginAttempts = 0;

int main()
{
    int option;
    cout << "\n\n====================================================================" << endl;
    cout << "                  Training Schedule Management System                " << endl;
    cout << "====================================================================" << endl;
    cout << "\n\tWelcome to the Home Page" << endl;
    cout << "\n\t[1] Admin Login" << endl;
    cout << "\t[2] Faculty Login" << endl;
    cout << "\t[3] Exit" << endl;
    cout << "\nEnter your choice: ";
    cin >> option;

    // Input validation
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(512, '\n');
        cout << "\nInvalid input! Please enter a number." << endl;
        return main();  // restart menu
    }

    switch (option)
    {
    case 1:
        handleAdminLogin();
        break;
    case 2:
        handleFacultyLogin();
        break;
    case 3:
        cout << "\nGoodbye! Have a nice day." << endl;
        break;
    default:
        cout << "\nInvalid option! Please select from the menu above." << endl;
        return main();
    }

    return 0;
}

// ----------------- ADMIN LOGIN -----------------
void handleAdminLogin()
{
    const string adminUser = "admin";
    const string adminPass = "12345";
    string enteredUser, enteredPass;

    cout << "\n\n---------------- ADMIN LOGIN ----------------" << endl;
    cout << "Please enter your Admin credentials:" << endl;

    cout << "Username: ";
    cin >> enteredUser;

    cout << "Password: ";
    cin >> enteredPass;

    if (enteredUser == adminUser && enteredPass == adminPass)
    {
        cout << "\nWelcome, " << enteredUser << "!" << endl;
        cout << "Login successful as Administrator.\n" << endl;
        adminObj.adminModules();
    }
    else
    {
        cout << "\nLogin Failed! Incorrect username or password.\n" << endl;
        loginAttempts++;
        if (loginAttempts >= 3)
        {
            cout << "\nACCESS DENIED. Too many failed attempts." << endl;
            exit(0);
        }
        main();
    }
}

// ----------------- FACULTY LOGIN -----------------
void handleFacultyLogin()
{
    string enteredUser, enteredPass, enteredTech;
    string fileUser, filePass, fileTech;
    bool isValid = false;

    cout << "\n\n---------------- FACULTY LOGIN ----------------" << endl;
    cout << "Please enter your Faculty credentials:" << endl;

    cout << "Name: ";
    cin >> enteredUser;

    cout << "Password: ";
    cin >> enteredPass;

    cout << "Technology: ";
    cin >> enteredTech;

    ifstream inputFile("./../data/facultyLogin.csv");
    while (inputFile >> fileUser >> filePass >> fileTech)
    {
        if (fileUser == enteredUser && filePass == enteredPass && fileTech == enteredTech)
        {
            isValid = true;
            break;
        }
    }
    inputFile.close();

    if (isValid)
    {
        cout << "\nWelcome, " << enteredUser << "!" << endl;
        cout << "Login successful as Faculty.\n" << endl;
        facultyObj.loginId = enteredUser;
        facultyObj.facultyModules();
    }
    else
    {
        cout << "\nLogin Failed! Please check your details and try again.\n" << endl;
        loginAttempts++;
        if (loginAttempts >= 3)
        {
            cout << "\nACCESS DENIED. Too many failed attempts." << endl;
            exit(0);
        }
        main();
    }
}
