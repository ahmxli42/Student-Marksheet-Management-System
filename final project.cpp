#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void displayMainMenu();
void enterNewStudentData();
void displayIndividualData();
void deleteIndividualData();
void updateStudentData();
void displayAllMarksheet();
void calculateGrade(int obtainedMarks, int totalMarks);
void rewriteFileExcludingData(const string& regNumber);

void rewriteFileExcludingData(const string& regNumber) {
    ifstream inFile("students.txt");
    ofstream outFile("temp.txt");

    if (!inFile || !outFile) {
        cerr << "Error opening file." << endl;
        return;
    }

    string line;
    bool found = false;
    while (getline(inFile, line)) {
        if (line.find(regNumber) == string::npos) {
            outFile << line << endl;
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();

    if (found) {
        remove("students.txt");
        rename("temp.txt", "students.txt");
    } else {
        remove("temp.txt");
        cout << "Registration number not found." << endl;
    }
}

void displayMainMenu() {
    int choice;
    do {
        cout << "\n===== Main Menu =====" << endl;
        cout << "1. Enter new student data" << endl;
        cout << "2. Display individual student marksheet" << endl;
        cout << "3. Delete student" << endl;
        cout << "4. Update individual student record" << endl;
        cout << "5. Display all class marksheet" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear buffer

        switch (choice) {
            case 1:
                enterNewStudentData();
                break;
            case 2:
                displayIndividualData();
                break;
            case 3:
                deleteIndividualData();
                break;
            case 4:
                updateStudentData();
                break;
            case 5:
                displayAllMarksheet();
                break;
            case 6:
                cout << "Exiting main menu..." << endl;
                return;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    } while (choice != 6);
}

void enterNewStudentData() {
    string name, regNumber;
    int marks;
    const int numSubjects = 4;
    string subjects[numSubjects];

    cout << "Enter student name: ";
    getline(cin, name);
    cout << "Enter registration number: ";
    cin >> regNumber;
    cin.ignore(); // Clear buffer

    ifstream checkFile("students.txt");
    string line;
    bool exists = false;
    while (getline(checkFile, line)) {
        if (line.find(regNumber) != string::npos) {
            exists = true;
            break;
        }
    }
    checkFile.close();

    if (exists) {
        cout << "Registration number already exists." << endl;
        return;
    }

    ofstream file("students.txt", ios::app);
    if (!file) {
        cerr << "Error opening file." << endl;
        return;
    }

    file << name << " " << regNumber << " ";
    for (int i = 0; i < numSubjects; ++i) {
        cout << "Enter subject " << i + 1 << " name: ";
        cin >> subjects[i];
        cout << "Enter marks for " << subjects[i] << ": ";
        cin >> marks;
        file << subjects[i] << " " << marks << " ";
    }

    file << endl;
    file.close();
    cout << "Student data entered successfully!" << endl;
}

void updateStudentData() {
    string regNumber;
    cout << "Enter registration number to update data: ";
    cin >> regNumber;
    cin.ignore(); // Clear buffer

    rewriteFileExcludingData(regNumber);
    cout << "Now enter updated details for the student." << endl;
    enterNewStudentData();
}

void calculateGrade(int obtainedMarks, int totalMarks) {
    double percentage = (static_cast<double>(obtainedMarks) / totalMarks) * 100;
    char grade;

    if (percentage >= 90) grade = 'A';
    else if (percentage >= 80) grade = 'B';
    else if (percentage >= 70) grade = 'C';
    else if (percentage >= 60) grade = 'D';
    else grade = 'F';

    cout << "Grade: " << grade << endl;
}

void displayIndividualData() {
    string regNumber;
    cout << "Enter registration number: ";
    cin >> regNumber;
    cin.ignore(); // Clear buffer

    ifstream file("students.txt");
    if (!file) {
        cerr << "Error opening file." << endl;
        return;
    }

    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(regNumber) != string::npos) {
            found = true;
            istringstream iss(line);
            string name, reg, subject;
            int marks, totalMarks = 400, obtainedMarks = 0;

            iss >> name >> reg;
            cout << "Name: " << name << "\nRegistration Number: " << reg << endl;

            for (int i = 0; i < 4; i++) {
                iss >> subject >> marks;
                cout << subject << ": " << marks << endl;
                obtainedMarks += marks;
            }

            cout << "Total Marks: " << totalMarks << "\nObtained Marks: " << obtainedMarks << endl;
            cout << "Percentage: " << (obtainedMarks * 100.0 / totalMarks) << "%" << endl;
            calculateGrade(obtainedMarks, totalMarks);
            break;
        }
    }

    if (!found) cout << "Student not found." << endl;
    file.close();
}

void deleteIndividualData() {
    string regNumber;
    cout << "Enter registration number to delete data: ";
    cin >> regNumber;
    cin.ignore(); // Clear buffer

    rewriteFileExcludingData(regNumber);
    cout << "Student data deleted successfully." << endl;
}

void displayAllMarksheet() {
    ifstream file("students.txt");
    if (!file) {
        cerr << "Error opening file." << endl;
        return;
    }

    string line;
    bool found = false;

    cout << "Student Marksheets:\n";
    while (getline(file, line)) {
        found = true;
        istringstream iss(line);
        string name, reg, subject;
        int marks;

        iss >> name >> reg;
        cout << "\nName: " << name << "\nRegistration Number: " << reg << endl;

        for (int i = 0; i < 4; i++) {
            iss >> subject >> marks;
            cout << subject << ": " << marks << endl;
        }
    }

    if (!found) cout << "No students found." << endl;
    file.close();
}

int main() {
    displayMainMenu();
    return 0;
}
