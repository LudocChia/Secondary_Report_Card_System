#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include <limits>
#include <cstdlib>

using namespace std;

struct StudentInfo
{
    string studentId;
    string studentName;
    string className;
    float grades[5]; // 5 grades for 5 subjects

    StudentInfo()
    {
        for (int i = 0; i < 5; ++i)
        {
            grades[i] = 0.0;
        }
    }
};

class Teacher
{
public:
    Teacher() {}

    static void loadStudents(const string &filename, StudentInfo *&students, int &studentCount)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Failed to open " << filename << endl;
            return;
        }

        string line;
        studentCount = 0;
        while (getline(file, line))
        {
            stringstream ss(line);
            StudentInfo student;
            string gradeStr;

            getline(ss, student.studentId, '|');
            getline(ss, student.studentName, '|');
            getline(ss, student.className, '|');
            for (int i = 0; i < 5; ++i)
            {
                getline(ss, gradeStr, '|');
                student.grades[i] = stof(gradeStr);
            }

            StudentInfo *newStudents = new StudentInfo[studentCount + 1];
            for (int i = 0; i < studentCount; ++i)
            {
                newStudents[i] = students[i];
            }
            newStudents[studentCount++] = student;
            delete[] students;
            students = newStudents;
        }

        file.close();
    }

    static void saveStudents(const StudentInfo students[], int studentCount, const string &filename)
    {
        ofstream file(filename);

        for (int i = 0; i < studentCount; ++i)
        {
            const StudentInfo &student = students[i];
            file << student.studentId << "|" << student.studentName << "|" << student.className;
            for (const auto &grade : student.grades)
            {
                file << "|" << grade;
            }
            file << "\n";
        }

        file.close();
    }

    bool isDuplicateId(const StudentInfo students[], int studentCount, const string &id)
    {
        for (int i = 0; i < studentCount; ++i)
        {
            if (students[i].studentId == id)
            {
                return true;
            }
        }
        return false;
    }

    string getGradeLetter(float grade)
    {
        if (grade == -1)
            return "-";
        else if (grade >= 90)
            return "A+";
        else if (grade >= 80)
            return "A";
        else if (grade >= 76)
            return "B+";
        else if (grade >= 72)
            return "B";
        else if (grade >= 68)
            return "B-";
        else if (grade >= 65)
            return "C+";
        else if (grade >= 60)
            return "C";
        else if (grade >= 56)
            return "C-";
        else if (grade >= 50)
            return "D+";
        else if (grade >= 40)
            return "D";
        else
            return "F";
    }

    void addStudentAndGrades()
    {
        StudentInfo *students = nullptr;
        int studentCount = 0;
        loadStudents("students.txt", students, studentCount);

        while (true)
        {
            system("cls");
            cout << "================================================================================" << endl;
            cout << "                            ADD STUDENT AND GRADES MENU                         " << endl;
            cout << "================================================================================" << endl;

            cout << "Please enter student information" << endl;

            StudentInfo newStudent;
            cout << "Student ID   : ";
            cin >> newStudent.studentId;

            if (isDuplicateId(students, studentCount, newStudent.studentId))
            {
                cout << "\033[1;31mStudent ID already exists. Please enter a different ID.\033[0m" << endl;
                char choice;
                cout << "Do you want to retry? (Y/N): ";
                cin >> choice;
                if (choice == 'Y' || choice == 'y')
                {
                    continue;
                }
                else
                {
                    cout << "\033[1;32mReturned Successfully.\033[0m" << endl;
                    system("pause");
                    system("cls");
                    return;
                }
            }

            cout << "Student Name : ";
            cin.ignore();
            getline(cin, newStudent.studentName);

            cout << "Student Class: ";
            getline(cin, newStudent.className);

            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "Please enter subject mark:" << endl;

            string subjects[5] = {"Bahasa Melayu", "English", "Mathematics", "History", "Science"};
            for (int i = 0; i < 5; ++i)
            {
                cout << setw(13) << left << subjects[i] << ": ";
                cin >> newStudent.grades[i];
            }

            StudentInfo *newStudents = new StudentInfo[studentCount + 1];
            for (int i = 0; i < studentCount; ++i)
            {
                newStudents[i] = students[i];
            }
            newStudents[studentCount++] = newStudent;
            delete[] students;
            students = newStudents;

            saveStudents(students, studentCount, "students.txt");
            cout << "================================================================================" << endl;
            cout << "\033[1;32mStudent and grades added successfully.\033[0m" << endl;
            system("pause");
            system("cls");
            return;
        }
    }

    void viewClassGrades()
    {
        system("cls");

        StudentInfo *students = nullptr;
        int studentCount = 0;
        loadStudents("students.txt", students, studentCount);

        cout << "==============================================================================================" << endl;
        cout << "                                CLASS GRADES (Unsorted Data)" << endl;
        cout << "==============================================================================================" << endl;
        cout << "| No. | Student ID      | Student Name       | Class    | Subject         | Mark    | Grade  |" << endl;
        cout << "----------------------------------------------------------------------------------------------" << endl;

        string subjects[5] = {"Bahasa Melayu", "English", "Mathematics", "History", "Science"};
        int no = 1;
        for (int i = 0; i < studentCount; ++i)
        {
            const StudentInfo &student = students[i];
            for (int j = 0; j < 5; ++j)
            {
                if (j == 2)
                {
                    cout << "| " << setw(3) << left << no++ << " | " << setw(15) << left << student.studentId << " | " << setw(18) << left 
                    << student.studentName << " | " << setw(8) << left << student.className << " | " << setw(15) << left << subjects[j] << " |  ";
                }
                else
                {
                    cout << "| " << setw(3) << left << "" << " | " << setw(15) << left << "" << " | " << setw(18) << left << "" << " | " << setw(8) 
                    << left << "" << " | " << setw(15) << left << subjects[j] << " |  ";
                }

                if (student.grades[j] < 40)
                {
                    cout << "\033[1;31m" << setw(6) << left << student.grades[j];
                }
                else
                {
                    cout << "\033[1;32m" << setw(6) << left << student.grades[j];
                }
                cout << "\033[0m" << " |  " << setw(6) << left << getGradeLetter(student.grades[j]) << "|" << endl;
            }
            if (i != studentCount - 1)
            {
                cout << "----------------------------------------------------------------------------------------------" << endl;
            }
        }
        cout << "==============================================================================================" << endl;
        system("pause");
        system("cls");
    }

    /*         cocktail sort              */

    void swap(StudentInfo &a, StudentInfo &b) {//swap value of two object
        StudentInfo temp = a;
        a = b;
        b = temp;
    }

    void CocktailSort(StudentInfo s[], int n, int choice, int s_index = -1) {//s_index is used if choice is 4
        bool swapped = true; //track issit have any elements to swap
        int start = 0;
        int end = n - 1;

        //if true then continue loop
        while (swapped) {
            swapped = false;

            for (int i = start; i < end; ++i) {
                bool check = false;
                switch (choice) {
                    case 1:
                        check = s[i].studentId > s[i + 1].studentId;//compare id
                        break;
                    case 2:
                        check = s[i].studentName > s[i + 1].studentName;//compare name
                        break;
                    case 3:
                        check = s[i].className > s[i + 1].className;//compare subject
                        break;
                    case 4:
                        check = s[i].grades[s_index] > s[i + 1].grades[s_index];//compare grades
                        break;
                }

                //if out of order, then swap them n set swapped to true
                if (check) {
                    swap(s[i], s[i + 1]);
                    swapped = true;
                }
            }

            //check if no swaps occur
            if (!swapped)
                break;


            //reverse iterate back
            swapped = false;
            --end;

            for (int i = end - 1; i >= start; --i) {
                bool check = false;
                switch (choice) {
                    case 1:
                        check = s[i].studentId > s[i + 1].studentId;
                        break;
                    case 2:
                        check = s[i].studentName > s[i + 1].studentName;
                        break;
                    case 3:
                        check = s[i].className > s[i + 1].className;
                        break;
                    case 4:
                        check = s[i].grades[s_index] > s[i + 1].grades[s_index];
                        break;
                }
                if (check) {
                    swap(s[i], s[i + 1]);
                    swapped = true;
                }
            }

            ++start;//increment because first elemtn is in correct position
        }
    }

    string getGrade(float mark) {//get mark
        if (mark >= 90) return "A+";
        if (mark >= 80) return "A";
        if (mark >= 75) return "A-";
        if (mark >= 70) return "B+";
        if (mark >= 68) return "B-";
        if (mark >= 65) return "C+";
        if (mark >= 60) return "C";
        if (mark >= 56) return "C-";
        if (mark >= 50) return "D+";
        if (mark >= 40) return "D";
        return "F";
    }

    void printHeader() {
        
        cout << "==============================================================================================\n";
        cout << "                                SORTED RESULT\n";
        cout << "==============================================================================================\n";
        cout << "| No. | Student ID      | Student Name       | Class    | Subject         | Mark    | Grade  |\n";
        cout << "----------------------------------------------------------------------------------------------\n";
    }

    void printStudents(const StudentInfo s[], int n, int choice = -1, int s_index = -1) {
        printHeader();
        string subjects[] = {"Bahasa Melayu", "English", "Mathematics", "History", "Science"};

        for (int i = 1; i < n; ++i) {
            //print info
            cout << "| " << left << setw(3) << (i + 1) << " | "
                << left << setw(15) << s[i].studentId << " | "
                << left << setw(17) << s[i].studentName << " | "
                << left << setw(8) << s[i].className << " |";

            //if choice is 4 n got sindex
            if (choice == 4 && s_index >= 0) {

                //display red font for grade n specific subject
                if (s[i].grades[s_index] < 40) {
                    cout << " " << left << setw(16) << subjects[s_index] << " | "
                        << "\033[1;31m" << left << setw(10) << s[i].grades[s_index] << "\033[0m | "
                        << left << setw(7) << getGrade(s[i].grades[s_index]) << " |\n";

                //display green font for grade
                } else {
                    cout << " " << left << setw(16) << subjects[s_index] << " | "
                        << "\033[1;32m" << left << setw(10) << s[i].grades[s_index] << "\033[0m | "
                        << left << setw(7) << getGrade(s[i].grades[s_index]) << " |\n";
                }

            //print grades for all subject
            } else {
                for (int j = 0; j < 5; ++j) {
                    if (s[i].grades[j] < 40) {
                        if (j > 0) {
                            cout << "|     |                 |                   |          | ";
                        }
                        cout <<left << setw(16) <<subjects[j] << " | "
                            << "\033[1;31m" << left << setw(8) << s[i].grades[j] << "\033[0m | "
                            << left << setw(7) << getGrade(s[i].grades[j]) << " |\n";
                    } else {
                        if (j > 0) {
                            cout << "|     |                 |                   |          | ";
                        }
                        cout << left << setw(16) <<subjects[j] << " | "
                            << "\033[1;32m" << left << setw(8) << s[i].grades[j] << "\033[0m | "
                            << left << setw(7) << getGrade(s[i].grades[j]) << " |\n";
                    }
                }
            }

            cout << "----------------------------------------------------------------------------------------------\n";
        }

        cout << "==============================================================================================\n";
    }

    int readFile(const string &filename, StudentInfo s[], int maxSize) {
        ifstream infile(filename.c_str());//read from file
        string line;
        int count = 0;//track the num of stud read

        while (getline(infile, line) && count < maxSize) {
            stringstream ss(line);
            string token;//hold each token parse from line

            getline(ss, s[count].studentId, '|');
            getline(ss, s[count].studentName, '|');
            getline(ss, s[count].className, '|');

            for (int i = 0; i < 5; ++i) {
                getline(ss, token, '|');
                stringstream gradeStream(token);
                gradeStream >> s[count].grades[i];//store in grades array
            }

            ++count;//move to next student
        }

        return count;//return how many student read
    }

    void keypress() {
        cout << "Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignore many characters as possible
        cin.get();//wait user press enter
    }


    void sortAndDisplay(StudentInfo s[], int n, int choice) {
        int s_index = -1;

        if (choice == 4) {
            cout << "Choose the subject to sort by:\n";
            cout << "[0] Bahasa Melayu\n";
            cout << "[1] English\n";
            cout << "[2] Mathematics\n";
            cout << "[3] History\n";
            cout << "[4] Science\n";
            cout << "Enter your choice: ";
            cin >> s_index;
        }
        
        for (int i = 0; i < 50; ++i) {
            cout << endl;
        }

        //pass choice and swap then display corresponding header
        CocktailSort(s, n, choice, s_index);
        cout << "Sorted data by ";
        switch (choice) {
            case 1:
                cout << "Student ID:\n";
                break;
            case 2:
                cout << "Student Name:\n";
                break;
            case 3:
                cout << "Student Class:\n";
                break;
            case 4:
                cout << "Subject Marks:\n";
                break;
        }

        //print body
        printStudents(s, n, choice, s_index);

        keypress();
    }

    //menu of cocktail sort
    void cocktailSort() {
        const int maxSize = 100;
        StudentInfo s[maxSize];
        int n = readFile("students.txt", s, maxSize);

        int choice = 0;
        while (true) {
            system("cls");
            cout << "================================================================================\n";
            cout << "                                 SORT CATEGORY                                \n";
            cout << "================================================================================\n";
            cout << "Choose the category to sort by:\n";
            cout << "[1] Student ID\n";
            cout << "[2] Student Name\n";
            cout << "[3] Student Class\n";
            cout << "[4] Subject Marks\n";
            cout << "[5] Back to Assignment Menu\n";
            cout << "================================================================================\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice >= 1 && choice <= 4) {
                system("cls");
                sortAndDisplay(s, n, choice);
            } else if (choice == 5) {
                break;
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    /*          end of cocktail sort              */

    void stringSearch()
    {
        StudentInfo *students = nullptr;
        int studentCount = 0;
        loadStudents("students.txt", students, studentCount); // Load student data from file

        int searchChoice;
        system("cls"); // Clear the console
        cout << "================================================================================" << endl;
        cout << "                                 SEARCH CATEGORY                                " << endl;
        cout << "================================================================================" << endl;
        cout << "Choose the category to search by:" << endl;
        cout << "[1] Student ID" << endl;
        cout << "[2] Student Name" << endl;
        cout << "[3] Student Class" << endl;
        cout << "[4] Subject" << endl;
        cout << "[5] Back to Assignment Menu" << endl;
        cout << "================================================================================" << endl;
        cout << "Enter your choice: ";
        cin >> searchChoice; // User selects the search category

        if (searchChoice == 5)
        {
            return; // Return to the previous menu(assignment menu) if user chooses option 5
        }

        string searchString;
        if (searchChoice >= 1 && searchChoice <= 4)
        {
            cout << "Enter the string to search for: ";
            cin.ignore();
            getline(cin, searchString); // Get the search string from user
        }
        else
        {
            cout << "Invalid choice!" << endl;
            return; // Invalid choice, return to previous menu
        }

        system("cls");
        bool found = false;
        string subjects[5] = {"Bahasa Melayu", "English", "Mathematics", "History", "Science"};
        int no = 1; // Record counter for output

        cout << "==============================================================================================" << endl;
        cout << "                                       SEARCH RESULTS" << endl;
        cout << "==============================================================================================" << endl;
        cout << "| No. | Student ID      | Student Name       | Class    | Subject         | Mark    | Grade  |" << endl;
        cout << "----------------------------------------------------------------------------------------------" << endl;

        // Lambda function for string searching
        bool (*stringSearch)(const string &, const string &) = [](const string &text, const string &pattern)
        {
            int textLength = text.length();
            int patternLength = pattern.length();
            for (int i = 0; i <= textLength - patternLength; ++i)
            {
                int j;
                for (j = 0; j < patternLength; ++j)
                {
                    if (text[i + j] != pattern[j])
                        break; // Break if character mismatch
                }
                if (j == patternLength)
                    return true; // Return true if pattern is found
            }
            return false; // Return false if pattern is not found
        };

        // Iterate through student records and search for matching criteria
        for (int studentIndex = 0; studentIndex < studentCount; ++studentIndex)
        {
            const StudentInfo &student = students[studentIndex];
            bool match = false;
            if (searchChoice == 1 && stringSearch(student.studentId, searchString))
                match = true;
            else if (searchChoice == 2 && stringSearch(student.studentName, searchString))
                match = true;
            else if (searchChoice == 3 && stringSearch(student.className, searchString))
                match = true;
            else if (searchChoice == 4)
            {
                for (int i = 0; i < 5; ++i)
                {
                    if (stringSearch(subjects[i], searchString))
                    {
                        cout << "| " << setw(3) << left << no++ << " | " << setw(15) << left << student.studentId << " | " << setw(18) << left << student.studentName 
                        << " | " << setw(8) << left << student.className << " | " << setw(15) << left << subjects[i] << " |  ";
                        if (student.grades[i] < 40)
                        {
                            cout << "\033[1;31m" << setw(6) << left << student.grades[i]; // Red font for low grades
                        }
                        else
                        {
                            cout << "\033[1;32m" << setw(6) << left << student.grades[i]; // Green font for passing grades
                        }
                        cout << "\033[0m" << " |  " << setw(6) << left << getGradeLetter(student.grades[i]) << "|" << endl;
                        if (studentIndex != studentCount - 1 || i != 4)
                        {
                            cout << "----------------------------------------------------------------------------------------------" << endl;
                        }
                        found = true;
                    }
                }
            }

             // Display search results
            if (match && searchChoice != 4)
            {
                found = true;
                cout << "| " << setw(3) << left << no++ << " | " << setw(15) << left << student.studentId << " | " << setw(18) << left << student.studentName 
                << " | " << setw(8) << left << student.className << " | " << setw(15) << left << subjects[0] << " | ";
                if (student.grades[0] < 40)
                {
                    cout << "\033[1;31m" << setw(6) << left << student.grades[0];
                }
                else
                {
                    cout << "\033[1;32m" << setw(6) << left << student.grades[0];
                }
                cout << "\033[0m" << "  |  " << setw(6) << left << getGradeLetter(student.grades[0]) << "|" << endl;
                for (int i = 1; i < 5; ++i)
                {
                    cout << "|     |                 |                    |          | " << setw(15) << left << subjects[i] << " | ";
                    if (student.grades[i] < 40)
                    {
                        cout << "\033[1;31m" << setw(6) << left << student.grades[i];
                    }
                    else
                    {
                        cout << "\033[1;32m" << setw(6) << left << student.grades[i];
                    }
                    cout << "\033[0m" << "  |  " << setw(6) << left << getGradeLetter(student.grades[i]) << "|" << endl;
                }
                if (studentIndex != studentCount - 1)
                {
                    cout << "----------------------------------------------------------------------------------------------" << endl;
                }
            }
        }

        if (!found)
        {
            cout << "No student information found with the given criteria." << endl;
        }
        cout << "==============================================================================================" << endl;
        system("pause"); // Pause system to view results
    }

void ternarySearch(StudentInfo *students, const string &key, int left, int right, int *results, int &resultsCount, bool byName = false)
{
    if (right >= left)
    {
        int mid1 = left + (right - left) / 3;
        int mid2 = right - (right - left) / 3;

        bool found_mid1 = false;
        for (int i = 0; i < resultsCount; ++i)
        {
            if (results[i] == mid1)
            {
                found_mid1 = true;
                break; // Avoid duplicate results
            }
        }

        bool match_mid1 = false;
        if (byName)
        {
            if (students[mid1].studentName.substr(0, key.size()) == key)
            {
                match_mid1 = true; // Match based on student name
            }
        }
        else
        {
            if (students[mid1].studentId.substr(0, key.size()) == key)
            {
                match_mid1 = true; // Match based on student ID
            }
        }

        if (match_mid1 && !found_mid1)
        {
            results[resultsCount++] = mid1; // Add result if matched
        }

        bool found_mid2 = false;
        for (int i = 0; i < resultsCount; ++i)
        {
            if (results[i] == mid2)
            {
                found_mid2 = true;
                break; // Avoid duplicate results
            }
        }

        bool match_mid2 = false;
        if (byName)
        {
            if (students[mid2].studentName.substr(0, key.size()) == key)
            {
                match_mid2 = true; // Match based on student name
            }
        }
        else
        {
            if (students[mid2].studentId.substr(0, key.size()) == key)
            {
                match_mid2 = true; // Match based on student ID
            }
        }

        if (match_mid2 && !found_mid2)
        {
            results[resultsCount++] = mid2;
        }

         // Recursive calls to search the remaining portions
        ternarySearch(students, key, left, mid1 - 1, results, resultsCount, byName);
        ternarySearch(students, key, mid1 + 1, mid2 - 1, results, resultsCount, byName);
        ternarySearch(students, key, mid2 + 1, right, results, resultsCount, byName);
    }
}

void searchByStudentID(StudentInfo *students, int studentCount, const string &id)
{
    system("cls");
    int *results = new int[studentCount]; // Dynamic array to store search results
    int resultsCount = 0;
    ternarySearch(students, id, 0, studentCount - 1, results, resultsCount); // Call ternary search for student ID

    if (resultsCount > 0)
    {
        string subjects[5] = {"Bahasa Melayu", "English", "Mathematics", "History", "Science"};

        cout << "==============================================================================================" << endl;
        cout << "                                SEARCH RESULTS" << endl;
        cout << "==============================================================================================" << endl;
        cout << "| No. | Student ID      | Student Name       | Class    | Subject         | Mark    | Grade  |" << endl;
        cout << "----------------------------------------------------------------------------------------------" << endl;
        int no = 1;
        for (int i = 0; resultsCount && i < resultsCount; ++i)
        {
            const StudentInfo &student = students[results[i]];
            for (int j = 0; j < 5; ++j)
            {
                if (j == 2)
                {
                    cout << "| " << setw(3) << left << no++ << " | " << setw(15) << left << student.studentId << " | " << setw(18) << left << student.studentName << " | " 
                    << setw(8) << left << student.className << " | " << setw(15) << left << subjects[j] << " | ";
                }
                else
                {
                    cout << "| " << setw(3) << left << "" << " | " << setw(15) << left << "" << " | " << setw(18) << left << "" << " | " << setw(8) << left << "" << " | " 
                    << setw(15) << left << subjects[j] << " | ";
                }

                if (student.grades[j] < 40.0f)
                {
                    cout << "\033[1;31m" << setw(4) << right << fixed << setprecision(1) << student.grades[j] << "   ";
                }
                else
                {
                    cout << "\033[1;32m" << setw(4) << right << fixed << setprecision(1) << student.grades[j] << "   ";
                }
                cout << "\033[0m" << " | " << setw(6) << left << getGradeLetter(student.grades[j]) << " |" << endl;
            }
            if (i != resultsCount - 1)
            {
                cout << "----------------------------------------------------------------------------------------------" << endl;
            }
        }
        cout << "==============================================================================================" << endl;
    }
    else
    {
        cout << "\033[1;31mStudent ID not found.\033[0m" << endl;
    }
    delete[] results;
}

void searchByStudentName(StudentInfo *students, int studentCount, const string &name)
{
    system("cls");
    int *results = new int[studentCount]; // Dynamic array to store search results
    int resultsCount = 0;
    ternarySearch(students, name, 0, studentCount - 1, results, resultsCount, true); // Call ternary search for student name


    if (resultsCount > 0)
    {
        string subjects[5] = {"Bahasa Melayu", "English", "Mathematics", "History", "Science"};

        cout << "==============================================================================================" << endl;
        cout << "                                SEARCH RESULTS" << endl;
        cout << "==============================================================================================" << endl;
        cout << "| No. | Student ID      | Student Name       | Class    | Subject         | Mark    | Grade  |" << endl;
        cout << "----------------------------------------------------------------------------------------------" << endl;
        int no = 1;
        for (int i = 0; i < resultsCount; ++i)
        {
            const StudentInfo &student = students[results[i]];
            for (int j = 0; j < 5; ++j)
            {
                if (j == 2)
                {
                    cout << "| " << setw(3) << left << no++ << " | " << setw(15) << left << student.studentId << " | " << setw(18) << left << student.studentName 
                    << " | " << setw(8) << left << student.className << " | " << setw(15) << left << subjects[j] << " | ";
                }
                else
                {
                    cout << "| " << setw(3) << left << "" << " | " << setw(15) << left << "" << " | " << setw(18) << left << "" << " | " << setw(8) << left 
                    << "" << " | " << setw(15) << left << subjects[j] << " | ";
                }

                if (student.grades[j] < 40.0f)
                {
                    cout << "\033[1;31m" << setw(4) << right << fixed << setprecision(1) << student.grades[j] << "   ";
                }
                else
                {
                    cout << "\033[1;32m" << setw(4) << right << fixed << setprecision(1) << student.grades[j] << "   ";
                }
                cout << "\033[0m" << " | " << setw(6) << left << getGradeLetter(student.grades[j]) << " |" << endl;
            }
            if (i != resultsCount - 1)
            {
                cout << "----------------------------------------------------------------------------------------------" << endl;
            }
        }
        cout << "==============================================================================================" << endl;
    }
    else
    {
        cout << "\033[1;31mStudent Name not found.\033[0m" << endl;
    }
    delete[] results;
}


void ternarySearchByClass(StudentInfo *students, const string &className, int left, int right, int *results, int &resultsCount)
{
    if (right >= left)
    {
        int mid1 = left + (right - left) / 3;
        int mid2 = right - (right - left) / 3;

        bool found_mid1 = false;
        for (int i = 0; i < resultsCount; ++i)
        {
            if (results[i] == mid1)
            {
                found_mid1 = true;
                break;
            }
        }

        bool match_mid1 = false;
        for (size_t i = 0; i < students[mid1].className.size(); ++i)
        {
            if (students[mid1].className.substr(i, className.size()) == className)
            {
                match_mid1 = true;
                break; // Match based on class name that entered by user
            }
        }

        if (match_mid1 && !found_mid1)
        {
            results[resultsCount++] = mid1;
        }

        bool found_mid2 = false;
        for (int i = 0; i < resultsCount; ++i)
        {
            if (results[i] == mid2)
            {
                found_mid2 = true;
                break;
            }
        }

        bool match_mid2 = false;
        for (size_t i = 0; i < students[mid2].className.size(); ++i)
        {
            if (students[mid2].className.substr(i, className.size()) == className)
            {
                match_mid2 = true;
                break; // Match based on class name that entered by user
            }
        }

        if (match_mid2 && !found_mid2)
        {
            results[resultsCount++] = mid2;
        }

        // Recursive calls to search the remaining portions
        ternarySearchByClass(students, className, left, mid1 - 1, results, resultsCount);
        ternarySearchByClass(students, className, mid1 + 1, mid2 - 1, results, resultsCount);
        ternarySearchByClass(students, className, mid2 + 1, right, results, resultsCount);
    }
}

void searchByClass(StudentInfo *students, int studentCount, const string &className)
{
    system("cls");
    int *results = new int[studentCount];
    int resultsCount = 0;
    ternarySearchByClass(students, className, 0, studentCount - 1, results, resultsCount); // Call ternary search for class

    if (resultsCount > 0)
    {
        string subjects[5] = {"Bahasa Melayu", "English", "Mathematics", "History", "Science"};

        cout << "==============================================================================================" << endl;
        cout << "                                SEARCH RESULTS" << endl;
        cout << "==============================================================================================" << endl;
        cout << "| No. | Student ID      | Student Name       | Class    | Subject         | Mark    | Grade  |" << endl;
        cout << "----------------------------------------------------------------------------------------------" << endl;
        int no = 1;
        for (int i = 0; i < resultsCount; ++i)
        {
            const StudentInfo &student = students[results[i]];
            for (int j = 0; j < 5; ++j)
            {
                if (j == 2)
                {
                    cout << "| " << setw(3) << left << no++ << " | " << setw(15) << left << student.studentId << " | " << setw(18) << left << student.studentName 
                    << " | " << setw(8) << left << student.className << " | " << setw(15) << left << subjects[j] << " | ";
                }
                else
                {
                    cout << "| " << setw(3) << left << "" << " | " << setw(15) << left << "" << " | " << setw(18) << left << "" << " | " << setw(8) << left 
                    << "" << " | " << setw(15) << left << subjects[j] << " | ";
                }

                if (student.grades[j] < 40.0f)
                {
                    cout << "\033[1;31m" << setw(4) << right << fixed << setprecision(1) << student.grades[j] << "   ";
                }
                else
                {
                    cout << "\033[1;32m" << setw(4) << right << fixed << setprecision(1) << student.grades[j] << "   ";
                }
                cout << "\033[0m" << " | " << setw(6) << left << getGradeLetter(student.grades[j]) << " |" << endl;
            }
            if (i != resultsCount - 1)
            {
                cout << "----------------------------------------------------------------------------------------------" << endl;
            }
        }
        cout << "==============================================================================================" << endl;
    }
    else
    {
        cout << "\033[1;31mClass not found.\033[0m" << endl;
    }
    delete[] results;
}

void ternarySearchBySubject(StudentInfo *students, const string &subject, int subjectIndex, int left, int right, int *results, int &resultsCount)
{
    if (right >= left)
    {
        int mid1 = left + (right - left) / 3;
        int mid2 = right - (right - left) / 3;

        bool found_mid1 = false;
        for (int i = 0; i < resultsCount; ++i)
        {
            if (results[i] == mid1)
            {
                found_mid1 = true;
                break; // Avoid duplicate results
            }
        }

        if (students[mid1].grades[subjectIndex] != -1 && !found_mid1)
        {
            results[resultsCount++] = mid1; // Add result if matched
        }

        bool found_mid2 = false;
        for (int i = 0; i < resultsCount; ++i)
        {
            if (results[i] == mid2)
            {
                found_mid2 = true;
                break; // Avoid duplicate results
            }
        }

        if (students[mid2].grades[subjectIndex] != -1 && !found_mid2)
        {
            results[resultsCount++] = mid2; // Add result if matched
        }

        // Recursive calls to search the remaining portions
        ternarySearchBySubject(students, subject, subjectIndex, left, mid1 - 1, results, resultsCount);
        ternarySearchBySubject(students, subject, subjectIndex, mid1 + 1, mid2 - 1, results, resultsCount);
        ternarySearchBySubject(students, subject, subjectIndex, mid2 + 1, right, results, resultsCount);
    }
}

void searchBySubject(StudentInfo *students, int studentCount, const string &subject)
{
    system("cls");
    int subjectIndex = -1;
    string subjects[5] = {"Bahasa Melayu", "English", "Mathematics", "History", "Science"};

    for (size_t i = 0; i < 5; ++i)
    {
        bool match = false;
        for (size_t j = 0; j < subjects[i].size(); ++j)
        {
            if (subjects[i].substr(j, subject.size()) == subject)
            {
                match = true;
                break; // Match based on subject name

            }
        }
        if (match)
        {
            subjectIndex = i;
            break; // Get subject index for searching
        }
    }

    if (subjectIndex == -1)
    {
        cout << "\033[1;31mInvalid subject.\033[0m" << endl;
        return; // Invalid subject
    }

    int *results = new int[studentCount];
    int resultsCount = 0;
    ternarySearchBySubject(students, subject, subjectIndex, 0, studentCount - 1, results, resultsCount);

    if (resultsCount > 0)
    {
        cout << "==============================================================================================" << endl;
        cout << "                                       SEARCH RESULTS" << endl;
        cout << "==============================================================================================" << endl;
        cout << "| No. | Student ID      | Student Name       | Class    | Subject         | Mark    | Grade  |" << endl;
        cout << "----------------------------------------------------------------------------------------------" << endl;
        int no = 1;
        for (int i = 0; i < resultsCount; ++i)
        {
            const StudentInfo &student = students[results[i]];
            cout << "| " << setw(3) << left << no++ << " | " << setw(15) << left << student.studentId << " | " << setw(18) << left << student.studentName 
            << " | " << setw(8) << left << student.className << " | " << setw(15) << left << subjects[subjectIndex] << " | ";
            if (student.grades[subjectIndex] < 40.0f)
            {
                cout << "\033[1;31m" << setw(4) << right << fixed << setprecision(1) << student.grades[subjectIndex] << "   ";
            }
            else
            {
                cout << "\033[1;32m" << setw(4) << right << fixed << setprecision(1) << student.grades[subjectIndex] << "   ";
            }
            cout << "\033[0m" << " | " << setw(6) << left << getGradeLetter(student.grades[subjectIndex]) << " |" << endl;
            if (i != resultsCount - 1)
            {
                cout << "----------------------------------------------------------------------------------------------" << endl;
            }
        }
        cout << "----------------------------------------------------------------------------------------------" << endl;
        cout << "==============================================================================================" << endl;
    }
    else
    {
        cout << "\033[1;31mNo results found for subject \"" << subject << "\".\033[0m" << endl;
    }
    delete[] results;
}


    void ternarySearchMenu()
    {
        system("cls");

        StudentInfo *students = nullptr;
        int studentCount = 0;
        loadStudents("students.txt", students, studentCount); // Load students from file students.txt

        cout << "==============================================================================================" << endl;
        cout << "                                  SEARCH CATEGORY" << endl;
        cout << "==============================================================================================" << endl;

        int choice;
        cout << "[1] Search by Student ID" << endl;
        cout << "[2] Search by Student Name" << endl;
        cout << "[3] Search by Class" << endl;
        cout << "[4] Search by Subject" << endl;
        cout << "[5] Back to Assignment Menu" << endl;
        cout << "==============================================================================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice; // Get user choice for search category

        string searchStr;
        float searchGrade;
        switch (choice)
        {
        case 1:
            cout << "Enter Student ID to search: ";
            cin >> searchStr;
            searchByStudentID(students, studentCount, searchStr); // Call search by Student ID
            break;
        case 2:
            cout << "Enter Student Name to search: ";
            cin.ignore();
            getline(cin, searchStr);
            searchByStudentName(students, studentCount, searchStr); // Call search by Student Name
            break;
        case 3:
            cout << "Enter Class to search: ";
            cin.ignore();
            getline(cin, searchStr);
            searchByClass(students, studentCount, searchStr); // Call search by Class
            break;
        case 4:
            cout << "Enter Subject to search: ";
            cin.ignore();
            getline(cin, searchStr);
            searchBySubject(students, studentCount, searchStr); // Call search by Subject
            break;
        case 5:
            return;// Exit to the previous menu (assignment menu)
        default:
            cout << "\033[1;31mInvalid choice.\033[0m" << endl;
            system("pause");
            ternarySearchMenu(); // Re-display the menu on invalid choice
            break;
        }

        system("pause");
        system("cls");
    }

    void heapify(StudentInfo *students, int n, int i, int sortChoice, int subjectIndex = 0)
    {
        int largest = i; // Initialize largest as root
        int left = 2 * i + 1; // left = 2*i + 1
        int right = 2 * i + 2; // right = 2*i + 2

        switch (sortChoice) // Compare left child with the largest node based on sortChoice
        {
        case 1:// Sort by studentId
            if (left < n && students[left].studentId > students[largest].studentId)
                largest = left; // Update largest if left child is larger
            if (right < n && students[right].studentId > students[largest].studentId)
                largest = right; // Update largest if right child is larger
            break;
        case 2:// Sort by studentName
            if (left < n && students[left].studentName > students[largest].studentName)
                largest = left;  // Update largest if left child is larger
            if (right < n && students[right].studentName > students[largest].studentName)
                largest = right;// Update largest if right child is larger
            break;
        case 3:// Sort by className
            if (left < n && students[left].className > students[largest].className)
                largest = left;// Update largest if left child is larger
            if (right < n && students[right].className > students[largest].className)
                largest = right;// Update largest if right child is larger
        break;
        case 4:// Sort by grades[subjectIndex]
            if (left < n && students[left].grades[subjectIndex] > students[largest].grades[subjectIndex])
                largest = left;// Update largest if left child is larger
            if (right < n && students[right].grades[subjectIndex] > students[largest].grades[subjectIndex])
                largest = right;// Update largest if right child is larger
            break;
        default:
            break;// No valid sort choice
        }

        if (largest != i)// If the largest element is not the root
        {
            swap(students[i], students[largest]);// Swap the root with the largest element
            heapify(students, n, largest, sortChoice, subjectIndex); // Recursively heapify the affected sub-tree
        }
    }

    void heapSort()
    {
        StudentInfo *students = nullptr; // Pointer to hold array of students
        int studentCount = 0;  // Initialize the number of students
        loadStudents("students.txt", students, studentCount);// Load students from file

        int sortChoice;// Variable to hold user's sorting choice
        cout << "Choose the field to sort by:" << endl;
        cout << "[1] Student ID" << endl;
        cout << "[2] Student Name" << endl;
        cout << "[3] Student Class" << endl;
        cout << "[4] Subject Mark" << endl;
        cout << "[5] Back to Assignment Menu" << endl;
        cout << "Enter your choice: ";
        cin >> sortChoice;// Get user's sorting choice

        if (sortChoice == 5)
        {
            return;// Exit function if choice is to go back
        }
        int subjectIndex = 0; // Index to specify which subject's grades to sort by
        if (sortChoice == 4)
        {
            cout << "Choose the subject to sort by:" << endl;
            cout << "[0] Bahasa Melayu" << endl;
            cout << "[1] English" << endl;
            cout << "[2] Mathematics" << endl;
            cout << "[3] History" << endl;
            cout << "[4] Science" << endl;
            cout << "Enter your choice: ";
            cin >> subjectIndex;// Get subject index for sorting
            int n = studentCount;// Total number of students
            
            for (int i = n / 2 - 1; i >= 0; i--)// Build heap (rearrange array)
            {
                heapify(students, n, i, sortChoice, subjectIndex);// Heapify each subtree
            }

            for (int i = n - 1; i > 0; i--)// Extract elements one by one from heap
            {
                swap(students[0], students[i]);// Move current root to end
                heapify(students, i, 0, sortChoice, subjectIndex);// Call heapify on reduced heap
            }
            // Print sorted result
            cout << "==============================================================================================" << endl;
            cout << "                                SORTED RESULT" << endl;
            cout << "==============================================================================================" << endl;
            cout << "| No. | Student ID      | Student Name       | Class    | Subject         | Mark    | Grade  |" << endl;
            cout << "----------------------------------------------------------------------------------------------" << endl;

            string subjects[5] = {"Bahasa Melayu", "English", "Mathematics", "History", "Science"};
            int no = 1;// Counter for student number in output

            for (int i = 0; i < n; ++i)
            {
                const StudentInfo &student = students[i];// Reference to current student
                cout << "| " << setw(3) << left << no++ << " | " << setw(15) << left << student.studentId << " | " << setw(18) << left << student.studentName 
                << " | " << setw(8) << left << student.className << " | " << setw(15) << left << subjects[subjectIndex] << " | ";
                if (student.grades[subjectIndex] < 40.0f)
                {
                    // Print student details for the first subject
                    cout << "\033[1;31m" << setw(8) << left << student.grades[subjectIndex];
                }
                else
                {
                    // Print only the subject and grade for subsequent subjects
                    cout << "\033[1;32m" << setw(8) << left << student.grades[subjectIndex];
                }
                cout << "\033[0m" << " | " << setw(6) << left << getGradeLetter(student.grades[subjectIndex]) << " |" << endl;
            }

            cout << "==============================================================================================" << endl;
            system("pause"); // Pause the system to view results
            return;
        }

        int n = studentCount;

        for (int i = n / 2 - 1; i >= 0; i--)
        {
            heapify(students, n, i, sortChoice, subjectIndex);
        }

        for (int i = n - 1; i > 0; i--)
        {
            swap(students[0], students[i]);
            heapify(students, i, 0, sortChoice, subjectIndex);
        }

        cout << "==============================================================================================" << endl;
        cout << "                                SORTED RESULT" << endl;
        cout << "==============================================================================================" << endl;
        cout << "| No. | Student ID      | Student Name       | Class    | Subject         | Mark    | Grade  |" << endl;
        cout << "----------------------------------------------------------------------------------------------" << endl;

        string subjects[5] = {"Bahasa Melayu", "English", "Mathematics", "History", "Science"};
        int no = 1;

        for (int i = 0; i < n; ++i)
        {
            const StudentInfo &student = students[i];
            for (int j = 0; j < 5; ++j)
            {
                if (j == 0)
                {
                    cout << "| " << setw(3) << left << no++ << " | " << setw(15) << left << student.studentId << " | " << setw(18) << left << student.studentName 
                    << " | " << setw(8) << left << student.className << " | " << setw(15) << left << subjects[j] << " | ";
                }
                else
                {
                    cout << "| " << setw(3) << left << "" << " | " << setw(15) << left << "" << " | " << setw(18) << left << "" << " | " << setw(8) << left 
                    << "" << " | " << setw(15) << left << subjects[j] << " | ";
                }

                if (student.grades[j] < 40.0f)
                {
                    cout << "\033[1;31m" << setw(8) << left << student.grades[j]; // Red color for marks < 40
                }
                else
                {
                    cout << "\033[1;32m" << setw(8) << left << student.grades[j];// Green color for marks >= 40
                }
                cout << "\033[0m" << " | " << setw(6) << left << getGradeLetter(student.grades[j]) << " |" << endl;// Print grade
            }
            if (i != n - 1)
            {
                cout << "----------------------------------------------------------------------------------------------" << endl;
            }
        }
        cout << "==============================================================================================" << endl;// Separator between students
        system("pause");// Pause the system to view results
    }

    void assignmentMenu()
    {
        int choice = 0;
        do
        {
            system("cls");
            cout << "================================================================================" << endl;
            cout << "                                 ASSIGNMENT MENU                                " << endl;
            cout << "================================================================================" << endl;
            cout << "[1] String Searching" << endl;
            cout << "[2] Ternary Searching" << endl;
            cout << "[3] Cocktail Sorting" << endl;
            cout << "[4] Heap Sorting" << endl;
            cout << "[5] Back to Main Menu" << endl;
            cout << "================================================================================" << endl;
            cout << "Please Enter Your Choice: ";
            cin >> choice;
            system("cls");

            switch (choice)
            {
            case 1:
                stringSearch();
                break;
            case 2:
                ternarySearchMenu();
                break;
            case 3:
                cocktailSort();
                break;
            case 4:
                heapSort();
                break;
            case 5:
                cout << "\033[1;32mReturned Successfully.\033[0m" << endl;
                system("pause");
                system("cls");
                return;
            default:
                cout << "\033[1;31mInvalid Choice. Please Try Again.\033[0m" << endl;
            }
        } while (true);
    }

    void openMenu()
    {
        int choice = 0;
        do
        {
            system("cls");
            cout << "================================================================================" << endl;
            cout << "                          Secondary Report Card System                          " << endl;
            cout << "================================================================================" << endl;
            time_t now = time(NULL);
            tm *local = localtime(&now);
            cout << "\t\t\t\t\t\t" << "Friday  " << local->tm_mday << "/" << local->tm_mon + 1 << "/" << local->tm_year + 1900 << endl;
            cout << "[1] Add Student and Grades" << endl;
            cout << "[2] View Class Grades" << endl;
            cout << "[3] Assignment" << endl;
            cout << "[4] Exit" << endl;
            cout << "================================================================================" << endl;
            cout << "Please Enter Your Choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                addStudentAndGrades();
                break;
            case 2:
                viewClassGrades();
                break;
            case 3:
                assignmentMenu();
                break;
            case 4:
                cout << "\033[1;32mExited the system successfully.\033[0m" << endl;
                return;
            default:
                cout << "\033[1;31mInvalid Choice. Please Try Again.\033[0m" << endl;
            }
        } while (true);
    }
};

int main()
{
    Teacher teacher;
    teacher.openMenu();

    return 0;
}