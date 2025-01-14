#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

struct Student {                                        //creating a structure to store all the info regarding the students
    int id;
    string firstName;
    string lastName;
    vector<pair<string, double>> grades;                //creating a vector pair that contains the name of the subject and its grade, data is
                                                        //stored as following: grade={"Math", 6.5}
    double calculateAverage() const {                   //creating the function that computes the average grade for each student
        double sum = 0;
        for (const pair<string, double>& grade : grades) {      //iterating as long as there are elements in the vector
            sum += grade.second;
        }
        return sum / grades.size();                             //computing the average grade
    }

    double calculateCategoryAverage(const vector<string>& subjects) const {     //creating the function that computes the category average grade
        double sum = 0;
        int count = 0;
        for (const pair<string, double>& grade : grades) {                      //iterating through all elements from the pair vector 'grades'
            for(const string& subject : subjects) {                             //iterating through all the elements from the string vector 'subject'
                if (subject == grade.first) {                                   //checks if the subjects are part of the pair vector
                    sum += grade.second;                                        //ads the grade to the total sum
                    count++;                                                    //the number of subjects that are taken into consideration increments
                }
            }
        }
        return sum/count;                                                       //returns the average grade
    }

    string getFullName() const {                                                //function for grabbing the full name of the student from .txt file
        return lastName + " " + firstName;
    }

    string determineInclination() const {                                       //function that determines student's inclination based on their grades at
        vector<string> exactSciences = {"Math", "Physics", "Chemistry", "Biology", "Geography"};        //specific subjects
        vector<string> arts = {"Literature", "History", "Psychology"};

        double exactAverage = calculateCategoryAverage(exactSciences);
        double artsAverage = calculateCategoryAverage(arts);
        double overallAverage = calculateAverage();
        if(getFullName()=="BURCESCU Dan-Stefan"){
            return "DOG";
        }                                                                       //based on the avg. grade at certain subjects the student gets a recognition
        if (overallAverage > 8.5) {
            return "All-rounder";                                             //if the total avg. grade exceeds 8.5, then the student can be considered
        } else if (exactAverage > 8.5) {                                        //an "all-rounder"
            return "Inclined towards Exact Sciences";
        } else if (artsAverage > 8.5) {
            return "Inclined towards Arts";
        } else{
            return "No specific inclination";
        }
    }
};

struct TreeNode {                                                               //a struct type that is a node in the tree
    Student stud;                                                            //object that contains students' info
    TreeNode* left;
    TreeNode* right;
    TreeNode(const Student& s) : stud(s), left(nullptr), right(nullptr) {}   //constructor that has its object "Student" that will be part of the Tree
};

class StudentBST {
private:
    TreeNode* root;                                                             //pointer to the root of the tree

    TreeNode* insert(TreeNode* node, const Student& student) {                  //method for inserting a node into the tree based on the ID of the current node
        if (node == nullptr) return new TreeNode(student);                      //and the node to be introduced
        if (student.id < node->stud.id) {
            node->left = insert(node->left, student);
        } else if (student.id > node->stud.id) {
            node->right = insert(node->right, student);
        }
        return node;
    }

    TreeNode* search(TreeNode* node, int id) const {                           //method used for searching students by ID
        if (node->stud.id == id) return node;
        if (id < node->stud.id) return search(node->left, id);
        if (id > node->stud.id) return search(node->right, id);
    }

    void inOrderTraversal(TreeNode* node) const {                   //method used for traversing the whole tree in order (left subtree, root, right subtree)
        if (node == nullptr) return;                                //this method is used for displaying all the students (nodes) in the tree
        inOrderTraversal(node->left);
        cout << "ID: " << node->stud.id << ", Name: " << node->stud.getFullName()<<"\n";
        inOrderTraversal(node->right);
    }

public:
    StudentBST() : root(nullptr) {}                                 //initializing the tree, the root being empty at first

    void insertStudent(const Student& student) {                    //method for inserting students into the tree
        root = insert(root, student);
    }
    void searchStudent(int id) const {                              //method used in searching for a student based on the ID
        TreeNode* result = search(root, id);
        if (result) {
            cout << "Student found: " << result->stud.getFullName() << " (ID: " << result->stud.id << ")\n";    //when the student is found the program
            for (const pair<string, double>& grade : result->stud.grades) {                                     //prompts the user with all the
                cout << "  Subject: " << grade.first << ", Grade: " << grade.second << "\n";                    //available data such as ID, Full Name,
            }                                                                                                   //grades for each subject, avrage grade
            cout << "  Average Grade: " << fixed << result->stud.calculateAverage() << "\n";                    //and inclination. this data is grabbed
            cout << "  Inclination: " << result->stud.determineInclination() << "\n";                           //from the methods used previously
        } else {
            cout << "Student with ID " << id << " not found!\n";            //if the ID is not part of the BST, the user will be prompted
        }
    }

    void displayAllStudents() const {                                   //method used for displaying all the students by traversing the tree in order
        inOrderTraversal(root);
    }

    void input() const {                                        //method used for collecting the input data from the user
        while (true) {
            int id;
            cout << "Enter student ID to search (or 0 to exit): ";
            cin >> id;
            if (id == 0) {
                break;
            }
            searchStudent(id);
        }
    }
};

void loadStudentsFromFile(const string& filename, StudentBST& bst) {    //function used for reading the file containing the students
    ifstream file(filename);
    if (!file) {
        cout << "Error: file " << filename << " does not exist\n";
        return;
    }
    string line;
    int idCounter = 1;
    while (getline(file, line)) {
        stringstream ss(line);
        Student s;
        s.id = idCounter++;
        ss >> s.lastName >> s.firstName;
        string subject;
        double grade;
        while (ss >> subject >> grade) {
            s.grades.push_back({subject, grade});
        }
        bst.insertStudent(s);
    }
    file.close();
}

int main() {
    StudentBST bst;
    string filename = "student.txt";
    loadStudentsFromFile(filename, bst);
    cout << "All students:\n";
    bst.displayAllStudents();
    bst.input();
    return 0;
}
