#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include<string>
#include "DBSE.h"
using namespace std;
namespace TOOLS {

	void pause(void) {
		system("pause");
		return;
	}
	int menu() { //menu Loader
		int select;

		cout << "********************* HR Menu *********************" << endl;
		cout << "1)  Find Employee" << endl;
		cout << "2)  Employees Report" << endl;
		cout << "3)  Add Employee" << endl;
		cout << "4)  Update Employee" << endl;
		cout << "5)  Remove Employee" << endl;
		cout << "0)  Exit" << endl;
		cin >> select;
		if (select  <= 5 && select >=0) {
			return select;
		}
		else {
			return 6;
		}
		
	}

	int findEmployee(MYSQL* funconn, int funcEmployNum, struct Employee& funcEmp) {
		MYSQL_ROW row;
		MYSQL_ROW reportRow;
		MYSQL_RES* reportRes;
		MYSQL_RES* res;
		int queResult;
		int size;
		int reportNum;
		char* reportName;

		if (funconn) {
			//string query = ("SELECT * FROM employees WHERE employeeNumber =" + to_string(funcEmployNum));
			string query = ("SELECT e.employeeNumber, e.lastName, e.firstName, e.email, o.phone, e.extension, e.reportsTo, e.jobTitle, o.city "
				" FROM offices o INNER JOIN employees e ON o.officeCode = e.officeCode "
				" WHERE e.employeeNumber = " + to_string(funcEmployNum)); //Initial query
			const char* q = query.c_str();
			queResult = mysql_query(funconn, q);

			if (!queResult) {
				res = mysql_store_result(funconn);
				if (res == nullptr)  { // If the query is done in error, display message
					cout << "Query Error" << mysql_errno(funconn) << " " << mysql_error(funconn) << endl;;
					return 0;
				}
				
				else {
					while (row = mysql_fetch_row(res)) {
						//main logic
						funcEmp.empNum = stoi(row[0]);
						//below is an extra failsafe, on instances of testing, the function would erroneously believe an empNum of 0 is valid, this prevents such errors occurring
						if (funcEmp.empNum == 0) {
							return 0;
						}
						strcpy(funcEmp.lastName, row[1]);
						strcpy(funcEmp.firstName, row[2]);
						strcpy(funcEmp.email, row[3]);
						strcpy(funcEmp.phone, row[4]);
						strcpy(funcEmp.extension, row[5]);
						strcpy(funcEmp.jobTitle, row[7]);
						strcpy(funcEmp.city, row[8]);
						//store the reportsTo id inside report num, make a new query
						//if this value is null however, set this value to an empty state and continue execution
						if (row[6] == NULL) {
							funcEmp.reportsTo[0] = '\0';
						}
						else {
							reportNum = stoi(row[6]); //conversion to int and assignment of report ID.
							string queryReports = ("SELECT lastName, firstName FROM employees "
								" WHERE employeeNumber IN(SELECT reportsTo FROM employees WHERE reportsTo = " + to_string(reportNum) + ");");
							const char* repQ = queryReports.c_str();
							queResult = mysql_query(funconn, repQ); //a new query is formed
							reportRes = mysql_store_result(funconn);
							//searching for manager name
							while (reportRow = mysql_fetch_row(reportRes)) {
								size = (strlen(reportRow[0]) + strlen(reportRow[1])); //allocate a char array with the size of the name plus 2 for terminator and space 
								reportName = new char[(size + 2)];
								strcpy(reportName, reportRow[1]); //set reportName to first name, concat a space, then concat last name
								strcat(reportName, " ");
								strcat(reportName, reportRow[0]);
								strcpy(funcEmp.reportsTo, reportName);//set full name to reportsTo of function
							}
						}
						//Secondary failsafe. prevents empNum from being returned while null.
						if (funcEmp.empNum == 0) {
							return 0;
						}
						else {
						return 1;
						}
						
					}

				}
			}

		}
		else {
			cout << "connection failed: " << mysql_errno(funconn) << " " << mysql_error(funconn) << endl;
			return 0;
		}
	}

	void displayEmployee(MYSQL* conn, struct Employee& emp) {
		cout << "Employee Number: " << emp.empNum << endl;
		cout << "Last Name: " << emp.lastName << endl;
		cout << "First Name: " << emp.firstName << endl;
		cout << "Email: " << emp.email << endl;
		cout << "Phone: " << emp.phone << endl;
		cout << "Extension: " << emp.extension << endl;
		cout << "Reports To: " << emp.reportsTo << endl;
		cout << "Job Title: " << emp.jobTitle << endl;
		cout << "City: " << emp.city << endl;
		return;
	}

	void displayAllEmployees(MYSQL* conn) {
		Employee temp;
		MYSQL_ROW norRow;
		MYSQL_ROW reportRow;
		MYSQL_RES* repRes;
		MYSQL_RES* norRes;
		int queResult;
		int size;
		int secondSize;
		int repNum;
		char* repName;
		char* fullname;
		if (conn) {
			string query = ("SELECT e.employeeNumber, e.lastName, e.firstName, e.email, o.phone, e.extension, e.reportsTo "
				"FROM offices o INNER JOIN employees e ON o.officeCode = e.officeCode ");
			const char* q = query.c_str();
			queResult = mysql_query(conn, q);

			if (!queResult) {
				norRes = mysql_store_result(conn);
				if (norRes == nullptr) {
					cout << "Query Error: " << mysql_errno(conn) << " " << mysql_error(conn) << endl;
					return;
				}
				else {
					cout << left << setw(10) << "ID" << setw(19) << "Name" << setw(40) << "Email" << setw(20)<< "Phone" <<setw(5) << "Ext       " <<setw(50) << "Reports to" <<setw(20) << endl;;
					while (norRow = mysql_fetch_row(norRes)) {
						temp.empNum = stoi(norRow[0]);
						strcpy(temp.lastName, norRow[1]);
						strcpy(temp.firstName, norRow[2]);
						strcpy(temp.email, norRow[3]);
						strcpy(temp.phone, norRow[4]);
						strcpy(temp.extension, norRow[5]);
						if (norRow[6] == NULL) {
							temp.reportsTo[0] = '\0';
						}
						else {
							repNum = stoi(norRow[6]);
							string querReport = ("SELECT lastName, firstName FROM employees "
								" WHERE employeeNumber IN(SELECT reportsTo FROM employees WHERE reportsTo = " + to_string(repNum) + ");");
							const char* repQ = querReport.c_str();
							queResult = mysql_query(conn, repQ);
							repRes = mysql_store_result(conn);

							while (reportRow = mysql_fetch_row(repRes)) {
								size = (strlen(reportRow[0]) + strlen(reportRow[1]));
								repName = new char[(size + 2)];
								strcpy(repName, reportRow[1]);
								strcat(repName, " ");
								strcat(repName, reportRow[0]);
								strcpy(temp.reportsTo, repName);
							}

						}
						//Final DB output
						secondSize = (strlen(temp.firstName) + strlen(temp.lastName));
						fullname = new char[(secondSize + 2)];
						strcpy(fullname, temp.firstName);
						strcat(fullname, " ");
						strcat(fullname, temp.lastName);
						cout << left << setw(10) << temp.empNum << setw(19) << fullname << setw(40) << temp.email << setw(20) << temp.phone << setw(10) << temp.extension << setw(20) << temp.reportsTo << endl;
					}
				}
			}
		}
	}

	void insertEmployee(MYSQL* conn, struct Employee& emp) {
		int queResult;
		MYSQL_RES* res;
		if (conn) {
			string query = ("INSERT INTO schoolschema.employees(employeeNumber, lastName, firstName, extension, email, officeCode, reportsTo, jobTitle)"
				" VALUES (" + std::string(" \' " ) + to_string(emp.empNum)  + std::string(" \' ") + " , " + " \' " + emp.lastName + " \' " + " , " + " \' " + emp.firstName + " \' " + " , " + std::string(" \' ") + emp.extension + " \' " + " , " + " \' " + emp.email + " \' " + " , " + to_string(1) + " , " + std::string(" \' ") + emp.reportsTo + " \' "+ " , " + " \' " + emp.jobTitle + " \' " + ");");
			const char* q = query.c_str();
			queResult = mysql_query(conn, q);
			if (!queResult) {
				cout << "Employee successfully added, check to verify by checking the number of the employee from the main menu, or using your DBMS application" << endl;
				return;
			}
			else {
				cout << "Error occurred: " << mysql_errno(conn) << " " << mysql_error(conn) << endl;
			}
		}
	}

	void updateEmployee(MYSQL* conn, int employeeNum) {
		Employee e = { 0 };
		int result = 0;
		int queResult;
		string ext;
		do {
			result = findEmployee(conn, employeeNum, e);
			if (result == 1) {
				if (e.empNum == 0) {
					result = 0;
					cout << "Employee does not exist, please enter a valid employee number: ";
					
				}
				else {
					cout << "Employee Found, please enter an updated phone extension" << endl;
					cin >> ext;
					if (conn) {
						string query=("UPDATE employees SET extension = "+ std::string(" \'") + ext + std::string("\' ") + " WHERE employeeNumber=" + to_string(employeeNum) +";");
						const char* q = query.c_str();
						queResult = mysql_query(conn, q);
						if (!queResult) {
							cout << "Employee successfully updated, check to verify by checking the number\n of the employee from the main menu, or using your DBMS application" << endl;
							return;
						}
						else {
							cout << "\nError occurred: " << mysql_errno(conn) << " " << mysql_error(conn) << endl;
						}
					}
				}
				
			}
			if (result == 0) {
				cin >> employeeNum;
			}
		} while (result == 0);
	}

	void deleteEmployee(MYSQL* conn, int employeeNum) {
		Employee e = { 0 };
		int result = 0;
		int queResult;
		
		do {
			result = findEmployee(conn, employeeNum, e);
			if (result == 1) {
				if (e.empNum == 0) {
					result = 0;
					cout << "Employee does not exist, please enter a valid employee number: ";

				}
				else {
					cout << "Employee Found, deleting" << endl;
					if (conn) {
						int failres;
						string query = ("DELETE FROM employees WHERE employeeNumber = " + std::string(" \' ") + to_string(employeeNum) + std::string(" \' ") + ";");
						string failsafe = ("SET FOREIGN_KEY_CHECKS = " + to_string(0) + ";");
						const char* x = failsafe.c_str();
						failres = mysql_query(conn, x);
						const char* q = query.c_str();
						queResult = mysql_query(conn, q);
						 failsafe = ("SET FOREIGN_KEY_CHECKS = " + to_string(1) + ";");
						const char* y = failsafe.c_str();
						failres = mysql_query(conn, y);
						if (!queResult) {
							cout << "Employee successfully deleted, check to verify by checking the number\n of the employee from the main menu, or using your DBMS application" << endl;
							return;
						}
						else {
							cout << "\nError occurred: " << mysql_errno(conn) << " " << mysql_error(conn) << endl;
						}
					}
				}

			}
			if (result == 0) {
				cin >> employeeNum;
			}
		} while (result == 0);
	}
}
