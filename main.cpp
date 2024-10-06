#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

const char* fileName = "Employee.txt";

class Employee {
private:
    int EmpID;
    string EmpName, Post, Department;
    float Salary;

public:
    void ReadData();
    int GetID();
    void DisplayRecord();
    string GetDepartment();
};

void Employee::ReadData() {
    cout << endl << "Employee ID: ";
    cin >> EmpID;
    cout << "Employee Name: ";
    cin.ignore(); // To handle the newline left in the input buffer
    getline(cin, EmpName);
    cout << "Employee's Post: ";
    getline(cin, Post);
    cout << "Employee's Department: ";
    getline(cin, Department);
    cout << "Salary: ";
    cin >> Salary;
}

void Employee::DisplayRecord() {
    cout << endl << "_______________________________";
    cout << endl << setw(5) << EmpID << setw(15) << EmpName << setw(15) << Post << setw(15) << Department << setw(8) << Salary;
}

int Employee::GetID() {
    return EmpID;
}

string Employee::GetDepartment() {
    return Department;
}

int main() {
    Employee emp, e;
    char option, ch;
    string Dept;
    int ID, isFound;
    
    fstream file;
    file.open(fileName, ios::in | ios::out | ios::app | ios::binary); // Using ios::app to append records

    if (!file) {
        cout << "Error in opening file!";
        return 1;
    }

    do {
        cout << "\n******* Menu ********";
        cout << "\n1 => Add a new record";
        cout << "\n2 => Search record by employee ID";
        cout << "\n3 => List employees by department";
        cout << "\n4 => Display all employees";
        cout << "\n5 => Update employee record";
        cout << "\n6 => Delete employee record";
        cout << "\n7 => Exit";
        cout << "\n*********************";
        cout << "\nEnter your option: ";
        cin >> option;

        switch (option) {
            case '1': { // Add new record
                emp.ReadData();
                isFound = 0;
                file.seekg(0, ios::beg);
                while (file.read((char*)&e, sizeof(e))) {
                    if (e.GetID() == emp.GetID()) {
                        cout << "This ID already exists... Try another ID.";
                        isFound = 1;
                        break;
                    }
                }
                if (isFound == 1) break;
                file.clear();
                file.seekp(0, ios::end);
                file.write((char*)&emp, sizeof(emp));
                cout << endl << "New record added successfully.";
                break;
            }
            case '2': { // Search by ID
                isFound = 0;
                cout << "\nEnter employee ID to search: ";
                cin >> ID;
                file.seekg(0, ios::beg);
                while (file.read((char*)&e, sizeof(e))) {
                    if (e.GetID() == ID) {
                        cout << "\nRecord found: ";
                        cout << "\n" << setw(5) << "ID" << setw(15) << "Name" << setw(15) << "Post" << setw(15) << "Department" << setw(8) << "Salary";
                        e.DisplayRecord();
                        isFound = 1;
                        break;
                    }
                }
                if (!isFound)
                    cout << "\nData not found for employee ID# " << ID;
                file.clear();
                break;
            }
            case '3': { // List employees by department
                isFound = 0;
                cout << "Enter department name to list employees: ";
                cin.ignore(); // clear the buffer
                getline(cin, Dept);
                file.seekg(0, ios::beg);
                while (file.read((char*)&e, sizeof(e))) {
                    if (e.GetDepartment() == Dept) {
                        cout << "\nRecord found for department " << Dept;
                        cout << "\n" << setw(5) << "ID" << setw(15) << "Name" << setw(15) << "Post" << setw(15) << "Department" << setw(8) << "Salary";
                        e.DisplayRecord();
                        isFound = 1;
                    }
                }
                if (!isFound)
                    cout << "\nNo employees found in department " << Dept;
                file.clear();
                break;
            }
            case '4': { // Display all employees
                cout << "\nEmployee records:";
                file.clear();
                file.seekg(0, ios::beg);
                int counter = 0;
                while (file.read((char*)&e, sizeof(e))) {
                    if (counter == 0) {
                        cout << "\n" << setw(5) << "ID" << setw(15) << "Name" << setw(15) << "Post" << setw(15) << "Department" << setw(8) << "Salary";
                    }
                    e.DisplayRecord();
                    counter++;
                }
                cout << "\n" << counter << " records found.";
                file.clear();
                break;
            }
            case '5': { // Update employee record
                int recordNo = 0;
                cout << "\nEnter employee ID to update: ";
                cin >> ID;
                isFound = 0;
                file.seekg(0, ios::beg);
                while (file.read((char*)&e, sizeof(e))) {
                    recordNo++;
                    if (e.GetID() == ID) {
                        cout << "Old record of employee ID " << ID << " is:";
                        e.DisplayRecord();
                        isFound = 1;
                        break;
                    }
                }
                if (!isFound) {
                    cout << "\nData not found for employee ID# " << ID;
                    break;
                }
                file.clear();
                int location = (recordNo - 1) * sizeof(e);
                file.seekp(location, ios::beg);
                cout << "\nEnter new record for employee ID " << ID << ": ";
                e.ReadData();
                file.write((char*)&e, sizeof(e));
                break;
            }
            case '6': { // Delete employee record
                int recordNo = 0;
                cout << "\nEnter employee ID to delete: ";
                cin >> ID;
                isFound = 0;
                fstream temp("temp.txt", ios::out | ios::binary);
                file.seekg(0, ios::beg);
                while (file.read((char*)&e, sizeof(e))) {
                    if (e.GetID() == ID) {
                        cout << "Old record of employee ID " << ID << " is:";
                        e.DisplayRecord();
                        isFound = 1;
                    } else {
                        temp.write((char*)&e, sizeof(e));
                    }
                }
                if (!isFound) {
                    cout << "\nData not found for employee ID# " << ID;
                    break;
                }
                file.close();
                temp.close();
                remove(fileName);
                rename("temp.txt", fileName);
                file.open(fileName, ios::in | ios::out | ios::binary);
                break;
            }
            case '7':
                cout << "Exiting the program...";
                file.close();
                return 0;
            default:
                cout << "Invalid option. Please try again.";
        }
        cout << "\nDo you want to continue (y/n)? ";
        cin >> ch;
    } while (ch != 'n');

    file.close();
    return 0;
}
