#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

// Course class to store course information
class Course {
public:
    string courseNumber;
    string courseName;
    vector<string> prerequisites;

    Course() {}

    Course(string number, string name) {
        courseNumber = number;
        courseName = name;
    }

    void addPrerequisite(string prereq) {
        if (!prereq.empty()) {
            prerequisites.push_back(prereq);
        }
    }
};

//CourseManager class to handle course data and operations
class CourseManager {
private:
    vector<Course> courses;

    //helper function to split string by delimiter
    vector<string> split(const string& s, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) {
            //remove any leading/trailing whitespace
            token.erase(0, token.find_first_not_of(" \t\r\n"));
            token.erase(token.find_last_not_of(" \t\r\n") + 1);
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    // Load courses from CSV file
    bool loadDataStructure(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Unable to open file '" << filename << "'" << endl;
            return false;
        }

        courses.clear();  //Clear kept data
        string line;
        
        while (getline(file, line)) {
            if (line.empty()) continue;  //skip empty lines
            
            vector<string> parts = split(line, ',');
            if (parts.size() >= 2) {
                Course course(parts[0], parts[1]);
                // Add prerequisites if they exist
                for (size_t i = 2; i < parts.size(); i++) {
                    course.addPrerequisite(parts[i]);
                }
                courses.push_back(course);
            }
        }
        
        file.close();

        //Sort courses by course number
        sort(courses.begin(), courses.end(), 
            [](const Course& a, const Course& b) {
                return a.courseNumber < b.courseNumber;
            });

        return true;
    }

    //Print all courses
    void printCourseList() {
        if (courses.empty()) {
            cout << "No courses loaded." << endl;
            return;
        }

        cout << "Here is a sample schedule:" << endl;
        for (const Course& course : courses) {
            cout << course.courseNumber << ", " << course.courseName << endl;
        }
    }

    // Print specific course information
    void printCourse(const string& courseNumber) {
        // Convert courseNumber to uppercase for case-insensitive comparison
        string upperCourseNumber = courseNumber;
        transform(upperCourseNumber.begin(), upperCourseNumber.end(), 
                 upperCourseNumber.begin(), ::toupper);

        for (const Course& course : courses) {
            string upperCurrentCourse = course.courseNumber;
            transform(upperCurrentCourse.begin(), upperCurrentCourse.end(), 
                     upperCurrentCourse.begin(), ::toupper);

            if (upperCurrentCourse == upperCourseNumber) {
                cout << course.courseNumber << ", " << course.courseName << endl;
                if (!course.prerequisites.empty()) {
                    cout << "Prerequisites: ";
                    for (size_t i = 0; i < course.prerequisites.size(); i++) {
                        if (i > 0) cout << ", ";
                        cout << course.prerequisites[i];
                    }
                    cout << endl;
                }
                return;
            }
        }
        cout << "Course not found." << endl;
    }
};

int main() {
    CourseManager courseManager;
    int choice;
    string input;
    string courseNumber;
    
    // Load data at startup
    if (!courseManager.loadDataStructure("CS 300 ABCU_Advising_Program_Input.csv")) {
        cout << "Failed to load the initial course data." << endl;
        return 1;
    }
    
    cout << "Welcome to the course planner." << endl;

    while (true) {
        // Display menu
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        
        // Get user input
        getline(cin, input);
        
        // convert input to integer
        try {
            choice = stoi(input);
        } catch (...) {
            choice = 0;  //invalid input
        }

        // Process user choice
        switch (choice) {
            case 1:
                courseManager.loadDataStructure("ABCU_Advising_Program_Input.csv");
                break;

            case 2:
                courseManager.printCourseList();
                break;

            case 3:
                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);
                courseManager.printCourse(courseNumber);
                break;

            case 9:
                cout << "Thank you for using the course planner!" << endl;
                return 0;

            default:
                cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}