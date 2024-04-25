//============================================================================
// Name        : CS 300 Project Two.cpp
// Author      : Charles Morris, Jr.
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <vector>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string title;
    vector<string> prerequisites;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
            Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;   
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    while (root != nullptr) {
        Remove(root->course.courseId);
    }
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if root equarl to null ptr
    if (root == nullptr) {
        // root is equal to new node course
        root = new Node(course);
    // else
    } else {
        // add Node root and course
        addNode(root, course);
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseId) {
    // remove node root courseID
    root = removeNode(root, courseId);
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
    // set current node equal to root
    Node* current = root;
    
    // keep looping downwards until bottom reached or matching courseId found
    while (current != nullptr) {
        // if match found, return current course
        if (current->course.courseId == courseId) {
            return current->course;
        // if course is smaller than current node then traverse left
        } else if (courseId < current->course.courseId) {
            current = current->left;
        // else larger so traverse right
        } else {
            current = current->right;
        }
    }

    Course course;
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (course.courseId < node->course.courseId) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        // else recurse down the left node
        } else {
            addNode(node->left, course);
        }
    // else
    } else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        //else
        } else {
            // recurse down the right node
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr    
    if (node != nullptr) {
        //inOrder left
        inOrder(node->left);
        //output courseID, title
        cout << node->course.courseId << ", " << node->course.title << endl;
        //InOrder right
        inOrder(node->right);
    }
}

Node* BinarySearchTree::removeNode(Node* node, string courseId) {
    // If node is null, return the node
    if (node == nullptr) {
        return node;
    }

    // If courseId is less than the current node's courseId
    if (courseId < node->course.courseId) {
        // Recursively call removeNode on the left subtree
        node->left = removeNode(node->left, courseId);
    } 
    // If courseId is greater than the current node's courseId
    else if (courseId > node->course.courseId) {
        // Recursively call removeNode on the right subtree
        node->right = removeNode(node->right, courseId);
    } 
    // If courseId matches the current node's courseId
    else {
        // If the left child is null, replace the node with its right child
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        } 
        // If the right child is null, replace the node with its left child
        else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }

        // Find the leftmost node in the right subtree
        Node* temp = node->right;
        while (temp->left != nullptr) {
            temp = temp->left;
        }

        // Replace the node's course with the leftmost node's course
        node->course = temp->course;
        // Remove the leftmost node
        node->right = removeNode(node->right, temp->course.courseId);
    }

    return node;
}    

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    // Output course information
    cout << course.courseId << ", " << course.title << endl;
    cout << "Prerequisites: ";

    // Output prerequisites if they exist
    for (auto it = course.prerequisites.begin(); it != course.prerequisites.end(); ++it) {
        // If prerequisites are empty, output "None"
        if (it->empty()) {
            cout << "None";
        // else output the prerequisite
        } else {
            cout << *it;
        }

        // If it is not the last element, add a comma and space
        if (next(it) != course.prerequisites.end()) {
            cout << ", ";
        }
    }
    
    cout << endl;
    return;
}

/**
 * Load the courses from a text file into the binary search tree
 *
 * @param bst Binary search tree to load the courses into
 */
void loadCourses(BinarySearchTree* bst) {
   string filePath;
   
   // Ask user for filename of text file to load
   while (true) {
      cout << "Enter the filename of the course list: ";
      cin >> filePath;

      // Check if file exists
      ifstream file(filePath);
      if (file.good()) {
         break;
      } else {
         cout << "File not found. Please try again." << endl;
      }
   }
   
   cout << "Loading text file " << filePath << endl;

   // Open the file
   ifstream file(filePath);

   string line;
   string temp;
   Course course;

   // Check if file is open
   if (file.is_open()) {
      // Clear input buffer by ignoring first three characters
      file.ignore(3, '\n');
      
      // Read each line of the file
      while (getline(file, line)) {
         // Create a string stream from the line
         istringstream ss(line);
         // Clear the prerequisites vector
         course.prerequisites.clear();

         // Get courseId and title
         getline(ss, course.courseId, ',');
         getline(ss, course.title, ',');

         // Check if a prerequisite is provided
         if (getline(ss, temp, ',')) {
            // Add the prerequisite to the prerequisites vector
            course.prerequisites.push_back(temp);
            
            // Check if there is another prerequisite
            if (getline(ss, temp, ',')) {
                // Add the second prerequisite to the prerequisites vector
                course.prerequisites.push_back(temp);
            }
         // Else if no prerequisites are provided
         } else {
            course.prerequisites.push_back(""); // Set prerequisites to empty if not provided
         }

         // Insert the course into the binary search tree
         bst->Insert(course);
      }
      // Close the file
      file.close();
   }
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {
    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;
    string courseKey;

    cout << "Welcome to the course planner." << endl;

    int choice = 0;
    while (true) {
        cout << endl << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl << endl;
        cout << "What would you like to do? ";

        // Check if input is valid
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            
            continue;
        }

        switch (choice) {

        case 1:
            // Complete the method call to load the courses
            loadCourses(bst);

            break;

        case 2:
            cout << "Here is a sample schedule:" << endl << endl;

            // Print the course list
            bst->InOrder();
            break;

        case 3:
            cout << "What course do you want to know about? ";
            cin >> courseKey;

            // Convert courseKey to uppercase
            transform(courseKey.begin(), courseKey.end(), courseKey.begin(), ::toupper);
            
            // Search for the course
            course = bst->Search(courseKey);

            // If the course is found, display the course
            if (!course.courseId.empty()) {
                displayCourse(course);
            // Else, output that the course was not found
            } else {
            	cout << "Course Id " << courseKey << " not found." << endl;
            }

            break;
        case 9:
            // Exit the program
            cout << "Thank you for using the course planner!" << endl;
            return 0;
        default:
            // Output that the choice is invalid
            cout << choice << " is not a valid option." << endl;
        }
    }
}
