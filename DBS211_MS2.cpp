/*
 DBS211_F2021

 Darsh Chirag Padaria: 145537205
 dcpadaria@myseneca.ca
*/
#include <iostream>
#include <occi.h>

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

struct Employee
{
	int employeeNumber;
	string lastName;
	string firstName;
	string extension;
	string email;
	string officeCode;
	int reportsTo;
	string jobTitle;
};

int menu(void);
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp);
void displayEmployee(Connection* conn, struct Employee emp);
void displayAllEmployees(Connection* conn);

int main(void)
{
	// OCCI Variables
	Environment* env = nullptr;
	Connection* conn = nullptr;
	// User Variables
	string str;
	string usr = "dbs211_213zb25";
	string pass = "30381245";
	string srv = "myoracle12c.senecacollege.ca:1521/oracle12c";

	try
	{
		env = Environment::createEnvironment(Environment::DEFAULT);
		conn = env->createConnection(usr, pass, srv);
		cout << "Connection is Successful!" << endl << endl;

		struct Employee employee;

		int option, num;
		do
		{
			option = menu();

			switch (option)
			{
			case 1:
				// find employee
				cout << "Enter Employee Number: ";
				cin >> num;
				if (findEmployee(conn, num, &employee) == 1)
				{
					// displays employee info
					displayEmployee(conn, employee);
				}
				else
				{
					cout << "Employee " << num << " does not exist.\n" << endl;
				}
				break;
			case 2:
				// employees report
				displayAllEmployees(conn);
				break;
			case 3:
				// add employee
				
				break;
			case 4:
				// update employee
				cout << "Employee Number: ";
				cin >> num;
				
				break;
			case 5:
				// remove employee
				cout << "Employee Number: ";
				cin >> num;
				
				break;
			case 0:
				// exit the program
				return 0;
			}
		} while (option >= 0 && option <= 5);

		env->terminateConnection(conn);
		Environment::terminateEnvironment(env);
	}
	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
	return 0;
}

// displays menu options and returns user selection option (int)
int menu(void)
{
	cout << "********************* HR Menu *********************" << endl;

	cout << "1) Find Employee" << endl;
	cout << "2) Employees Report" << endl;
	cout << "3) Add Employee" << endl;
	cout << "4) Update Employee" << endl;
	cout << "5) Remove Employee" << endl;
	cout << "0) Exit" << endl;
	cout << "Enter an option (0-5): ";

	int choice;
	char newline;
	bool done;
	do
	{
		cin >> choice;
		newline = cin.get();
		if (cin.fail() || newline != '\n')
		{
			done = false;
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid. Enter an option from 0 to 5: ";
		}
		else
		{
			done = choice >= 0 && choice <= 5;
			if (!done)
			{
				cout << "Invalid. Enter an option from 0 to 5: ";
			}
		}
	} while (!done);
	return choice;
}

// returns 1 if employee exists and then stores their info, otherwise returns 0.
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp)
{
	Statement* stmt = conn->createStatement();
	ResultSet* rs = stmt->executeQuery("SELECT employeenumber, lastname, firstname, extension, email, officecode, reportsto, jobtitle FROM employees WHERE employeenumber=" + to_string(employeeNumber));

	if (!rs->next())
	{
		return 0;
	}
	else
	{
		// store employee info in Employee variable
		emp->employeeNumber = rs->getInt(1);
		emp->firstName = rs->getString(2);
		emp->lastName = rs->getString(3);
		emp->extension = rs->getString(4);
		emp->email = rs->getString(5);
		emp->officeCode = rs->getString(6);
		emp->reportsTo = rs->getInt(7);
		emp->jobTitle = rs->getString(8);

		return 1;
	}
}

// displays employee information
void displayEmployee(Connection* conn, struct Employee emp)
{
	cout << "\n-------------- Employee Information -------------" << endl;
	cout << "Employee Number: " << emp.employeeNumber << endl;
	cout << "Last Name: " << emp.lastName << endl;
	cout << "First Name: " << emp.firstName << endl;
	cout << "Extension: " << emp.extension << endl;
	cout << "Email: " << emp.email << endl;
	cout << "Office Code: " << emp.officeCode << endl;
	cout << "Manager ID: " << emp.reportsTo << endl;
	cout << "Job Title: " << emp.jobTitle << endl << endl;
}

// displays all employees' information if exists
void displayAllEmployees(Connection* conn)
{
	Statement* stmt = conn->createStatement();
	ResultSet* rs = stmt->executeQuery("SELECT a.employeenumber, a.firstname ||' '|| a.lastname, a.email, phone, a.extension, b.firstname ||' '|| b.lastname FROM employees a FULL OUTER JOIN employees b ON a.reportsto = b.employeenumber JOIN offices ON a.officecode = offices.officecode ORDER BY a.employeenumber");

	if (!rs->next())
	{
		// display message if query does not return any rows
		cout << "There is no employees' information to be displayed." << endl;
	}
	else
	{
		cout << "\n------   ---------------   ---------------------------------  ----------------  ---------  -----------------" << endl;
		cout.setf(ios::left);
		cout.width(9);
		cout << "ID";
		cout.width(18);
		cout << "Employee Name";
		cout.width(35);
		cout << "Email";
		cout.width(18);
		cout << "Phone";
		cout.width(11);
		cout << "Extension";
		cout << "Manager Name" << endl;

		cout << "------   ---------------   ---------------------------------  ----------------  ---------  -----------------" << endl;
		do
		{
			cout.setf(ios::left);
			cout.width(9);
			cout << rs->getInt(1);
			cout.width(18);
			cout << rs->getString(2);
			cout.width(35);
			cout << rs->getString(3);
			cout.width(18);
			cout << rs->getString(4);
			cout.width(11);
			cout << rs->getString(5);
			cout << rs->getString(6) << endl;

		} while (rs->next());
		cout << endl;
	}
}
