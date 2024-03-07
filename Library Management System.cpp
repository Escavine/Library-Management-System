// Library Management System.cpp : This file contains the 'main' function. Program execution begins and ends there.


// Prerequisite libraries
#include <iostream>
#include <string>

// Defines all users (i.e. students and librarians)
class user
{
    // Key attributes of a user
    std::string name;
    std::string surname;
    int age;
    std::string gender;
    std::string email;

    void borrowBook()
    {
        // Insert logic for borrowing a book


    }


    void returnBook()
    {
        // Insert logic for returning a book

    }

    void calculatingFine()
    {
        // Insert logic for calculating a fine based on how long the book as taken for it to be returned


    }
};



class librarian : user // Usage of inheritance for the librarian as it is a user
{


};



// Engaging the start of the program
int main()
{
    
    std::cout << "Stepwise University - Login\n";
    std::cout << "1. Librarian" << std::endl;
    std::cout << "2. Student" << std::endl;
    std::cout << "\nEnter a corresponding value: " << std::endl;
}
