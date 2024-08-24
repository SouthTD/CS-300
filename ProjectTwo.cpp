//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Tonya Sanders
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>

using namespace std;

// Default time for sleep
const int GLOBAL_SLEEP_TIME = 5000;

/*
Struct to hold the course information
*/
struct Course {
    string courseId;
    string courseName;
    vector<string> preList;
};

class BinarySearchTree {
private:
    /*
    Define a node structure to hold course data and pointers
    to the left and right children
    */
    struct Node {
        Course course;
        Node* right;
        Node* left;

        // Default constructor
        Node() {
            left = nullptr;
            right = nullptr;
        }

        // Initialize node with a course
        Node(Course aCourse) {
            course = aCourse;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

    // Recursive function to traverse the tree in order
    void inOrder(Node* node);

    int size = 0;

public:
    // Constructor
    BinarySearchTree();

    // Function to traverse the tree in order
    void InOrder();

    // Function to insert a course into the tree
    void Insert(Course aCourse);

    // Function to remove a course from the tree by ID
    void Remove(string courseId);

    // Function to search for a course by ID
    Course Search(string courseId);

    // Function to return the size of the tree
    int Size();
};

/*
Default constructor
*/
BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;
}

/*
Traverse the tree in order
*/
void BinarySearchTree::InOrder() {
    inOrder(root);
}

/*
Insert a course into the tree
*/
void BinarySearchTree::Insert(Course aCourse) {
    Node* currentNode = root;

    if (root == NULL) {
        root = new Node(aCourse);
    } else {
        while (currentNode != NULL) {
            if (aCourse.courseId < currentNode->course.courseId) {
                if (currentNode->left == nullptr) {
                    currentNode->left = new Node(aCourse);
                    currentNode = NULL;
                } else {
                    currentNode = currentNode->left;
                }
            } else {
                if (currentNode->right == nullptr) {
                    currentNode->right = new Node(aCourse);
                    currentNode = NULL;
                } else {
                    currentNode = currentNode->right;
                }
            }
        }
    }
    size++;
}

/*
Remove a course from the tree by ID
*/
void BinarySearchTree::Remove(string courseId) {
    Node* par = NULL;
    Node* curr = root;

    while (curr != NULL) {
        if (curr->course.courseId == courseId) {
            if (curr->left == NULL && curr->right == NULL) {
                if (par == NULL) {
                    root = nullptr;
                } else if (par->left == curr) {
                    par->left = NULL;
                } else {
                    par->right = NULL;
                }
            } else if (curr->right == NULL) {
                if (par == NULL) {
                    root = curr->left;
                } else if (par->left == curr) {
                    par->left = curr->left;
                } else {
                    par->right = curr->left;
                }
            } else if (curr->left == NULL) {
                if (par == NULL) {
                    root = curr->right;
                } else if (par->left == curr) {
                    par->left = curr->right;
                } else {
                    par->right = curr->right;
                }
            } else {
                Node* suc = curr->right;
                while (suc->left != NULL) {
                    suc = suc->left;
                }
                Node successorData = Node(suc->course);
                Remove(suc->course.courseId);
                curr->course = successorData.course;
            }
            return;
        } else if (curr->course.courseId < courseId) {
            par = curr;
            curr = curr->right;
        } else {
            par = curr;
            curr = curr->left;
        }
    }
    cout << "\nValue not found" << endl;
    return;
}

/*
Search for a course by ID
*/
Course BinarySearchTree::Search(string courseId) {
    Course aCourse;
    Node* currentNode = root;

    while (currentNode != NULL) {
        if (currentNode->course.courseId == courseId) {
            return currentNode->course;
        } else if (courseId < currentNode->course.courseId) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
    }

    return aCourse;
}

/*
Recursive function to traverse the tree in order and print nodes
*/
void BinarySearchTree::inOrder(Node* node) {
    if (node == NULL) {
        return;
    }
    inOrder(node->left);

    // Print the node
    cout << node->course.courseId << ", " << node->course.courseName << endl;

    inOrder(node->right);
}

/*
Return the size of the tree
*/
int BinarySearchTree::Size() {
    return size;
}

/*
Function to split a string by a delimiter and return a vector of tokens
*/
vector<string> Split(string lineFeed) {
    char delim = ',';

    // Include a delimiter at the end so the last word is also read
    lineFeed += delim;
    vector<string> lineTokens;
    string temp = "";

    for (int i = 0; i < lineFeed.length(); i++) {
        if (lineFeed[i] == delim) {
            // Store words in token vector
            lineTokens.push_back(temp);
            temp = "";
            i++;
        }
        temp += lineFeed[i];
    }
    return lineTokens;
}

/*
Function to load courses from a CSV file into the BinarySearchTree
*/
void loadCourses(string csvPath, BinarySearchTree* courseList) {
    ifstream inFS; // Input stream to read file
    string line; // Line feed
    vector<string> stringTokens;

    // Open the file
    inFS.open(csvPath);

    // Check if the file opened successfully
    if (!inFS.is_open()) {
        cout << "Could not open file. Please check inputs." << endl;
        return;
    }

    // Read the file line by line
    while (!inFS.eof()) {
        Course aCourse; // Create a new struct for each line
        getline(inFS, line);
        stringTokens = Split(line); // Split the line into tokens using the delimiter

        // Check if the line is properly formatted
        if (stringTokens.size() < 2) {
            cout << "\nError. Skipping line." << endl;
        } else {
            aCourse.courseId = stringTokens.at(0);
            aCourse.courseName = stringTokens.at(1);

            for (unsigned int i = 2; i < stringTokens.size(); i++) {
                aCourse.preList.push_back(stringTokens.at(i));
            }

            // Insert the course into the tree
            courseList->Insert(aCourse);
        }
    }

    // Close the file
    inFS.close();
}

/*
Display the details of a course
*/
void displayCourse(Course aCourse) {
    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";

    // Check if the prerequisites list is empty
    if (aCourse.preList.empty()) {
        cout << "none" << endl;
    } else {
        for (unsigned int i = 0; i < aCourse.preList.size(); i++) {
            cout << aCourse.preList.at(i);
            if (aCourse.preList.size() > 1 && i < aCourse.preList.size() - 1) {
                // Add a comma for any elements greater than 1
                cout << ", ";
            }
        }
    }
    cout << endl;
}

/*
Convert a string to uppercase
*/
void convertCase(string& toConvert) {
    for (unsigned int i = 0; i < toConvert.length(); i++) {
        if (isalpha(toConvert[i])) {
            toConvert[i] = toupper(toConvert[i]);
        }
    }
}

/*
Display a goodbye message with a delay
*/
void exitProgram() {
    cout << "Goodbye." << endl;
    Sleep(GLOBAL_SLEEP_TIME);
    return;
}

/*
Main function: display menu and handle user input
*/
int main() {
    BinarySearchTree* courseList;
    courseList = new BinarySearchTree();

    int choice = 0;
    string courseId;
    Course course;

    while (choice != 9) {
        cout << "\n\nWelcome to the course planner.\n\n"
             << "  1. Load Data Structure.\n"
             << "  2. Print Course List.\n"
             << "  3. Print Course.\n"
             << "  9. Exit\n\n"
             << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                // Load the courses from a CSV file
                loadCourses("ABCU_Advising_Program_Input.csv", courseList);
                break
            case 2:
                // Print the course list in order
                courseList->InOrder();
                break;

            case 3:
                // Prompt for the course ID
                cout << "What course do you want to know about? ";
                cin >> courseId;

                // Convert course ID to uppercase
                convertCase(courseId);

                // Search for the course
                course = courseList->Search(courseId);

                // If course is found, display it
                if (!course.courseId.empty()) {
                    displayCourse(course);
                } else {
                    cout << "Course not found." << endl;
                }
                break;

            case 9:
                // Exit the program with a goodbye message
                exitProgram();
                break;

            default:
                // Handle invalid input
                cout << choice << " is not a valid option." << endl;
                break;
        }
    }

    // Clean up the memory
    delete courseList;

    return 0;
}
