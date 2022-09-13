#define _CRT_SECURE_NO_WARNINGS
#include "DBSE.h"
#include <stdlib.h>
#include <iostream>
#pragma optimize( "", off )
using namespace TOOLS;
using namespace std;
int main() {
	bool opt = false; //Assists with option selection
	int empNum; //user defined value, passed to findEmployee function
	int failsafe; //Used in the event that findEmployee does NOT return a proper value.
	int correct = 0;
	MYSQL* conn; //SQL connector
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, "127.0.0.1", "root", "Dreadburner2!", "schoolschema", 3306, nullptr, 0); // Connects to personal database on localhost
	struct Employee emp = { 0 }; //Empty struct for function operations
	//beginning of program
	while (opt == false) {
		switch (menu()) {
		case 1:
			cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
			cout << "Enter the Employee Number:";
			cin >> empNum;
			failsafe = findEmployee(conn, empNum, emp); //the function is run and the return value is passed to failsafe
			if (failsafe == 1) { //In some instances of testing, the function returned a 1 when it was not meant to. this lead to erronous output. 
				if (emp.empNum == 0) { //if a value of emp is 0, the rest of the code is immediately invalidated as well and an error is passed
					cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
					cout << "Error: This Employee does not exist or is not yet in our records" << endl;
					cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
					pause();
					break;
				}
				else {
					cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
				displayEmployee(conn, emp); //if the function previously executed normally, the employee is displayed.
				cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
				pause();
				emp = { 0 }; //resets the emp struct to a default empty state.
				system("CLS");
				}

			}
			else {
				cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
				cout << "Error: This Employee does not exist or is not yet in our records" << endl;
				cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
				pause();
			}
			break;
		case 2:
			system("CLS");
			cout << "*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
			displayAllEmployees(conn); //Perform a SELECT * FROM employees query
			cout << "*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
			emp = { 0 }; //reset to empty default state.
			pause();
			system("CLS");
			break;
			//below functions are not yet implemented and subject to change.
		case 3:
			cout << "Please Enter the employee number of the employee to add" << endl;
			cin >> empNum;
			failsafe = findEmployee(conn, empNum, emp);
			if (failsafe == 1) {
				if (emp.empNum == 0) {
					cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
					cout << "Employee does not exist or is not in our records. Entry can proceed" << endl;
					cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
					pause();
					cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
					cout << "Adding previously entered number as this employee's id" << endl;
					emp.empNum = empNum;
					cin.ignore();
					cout << "Enter the employee's first name: ";
					cin >> emp.firstName;
					cout << "Last name: ";
					cin >> emp.lastName;
					cout << "Enter Employee Email: ";
					cin >> emp.email;
					cout << "Enter Employee's Extension: ";
					cin >> emp.extension;
					cout << "Enter the employee number of the person this employee reports to: ";
					cin >> emp.reportsTo;
					cout << "What is this employees' current position? ";
					cin >> emp.jobTitle;
					cout << "What city is this employee working within? ";
					cin >> emp.city;
					cout << "Compiling employee data" << endl;
					cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
					pause();
					system("CLS");
					insertEmployee(conn, emp);
					
				}
				else {
					cout << empNum;
					cout << "An employee with the same number exists." << endl;
				}
			}
			else if (failsafe == 0) {
				cout << "If you are seeing this message, an unintentional and unknown error has occurred. Please report \"Error Code: Serval\" to my email account in the event this occurs and try running this program once more." << endl;
			}
			break;
		case 4:
			system("CLS");
			cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
			cout << "Enter employee number to update: ";
			cin >> empNum;
			updateEmployee(conn, empNum);
			cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
			pause();
			system("CLS");
			break;
		case 5:
			system("CLS");
			cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
			cout << "Enter employee number to delete: ";
			cin >> empNum;
			deleteEmployee(conn, empNum);
			cout << "*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*" << endl;
			pause();
			system("CLS");
			break;
		case 0:
			system("CLS");
			cout << "Quitting" << endl;
			opt = true;
			Sleep(1000);
			exit(0);
			break;
		default:
			cout << "Please enter a valid option" << endl;

			opt = false;
		}
	}
}
#pragma optimize( "", on )
