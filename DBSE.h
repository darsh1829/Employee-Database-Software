#define _CRT_SECURE_NO_WARNINGS
#include <mysql.h>
#ifndef DBSTOOLKIT_H
#define DBSTOOLKIT_H

namespace TOOLS {
	struct Employee {
		int empNum;
		char lastName[50];
		char firstName[50];
		char email[100];
		char phone[50];
		char extension[10];
		char reportsTo[100];
		char jobTitle[50];
		char city[50];
	};
	int menu(void);
	int findEmployee(MYSQL*, int, struct Employee&);
	void displayEmployee(MYSQL*, struct Employee&);
	void displayAllEmployees(MYSQL*);
	void insertEmployee(MYSQL*, struct Employee&);
	void updateEmployee(MYSQL*, int);
	void deleteEmployee(MYSQL*, int);
	void pause(void);
}

#endif
