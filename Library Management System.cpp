// Library Management System 


// Prerequisite libraries
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <regex>
#include <fstream>
#include <sstream>

// Relevant header files
#include "users.h"

void login(int validChances)
{
     std::string userInput;
     int remainingChances = 3;

    if (validChances < 3) // Check if user has less than 3 chances
    {
        if (validChances == 0) // Nested selection statement to check if the chances has reached zero.
        {
            std::cout << "Too many incorrect attempts, console will now terminate." << std::endl;
            exit(0); // Termiante the console
        }

        else
        {
            std::cout << "Remaining chances: " << validChances << std::endl;

        }

    }

    std::cout << "Stepwise University: Identification\n";
    std::cout << "1. Librarian" << std::endl;
    std::cout << "2. Student" << std::endl;
    std::cout << "\nEnter a corresponding value: " << std::endl;
    std::cin >> userInput; // Accept the corresponding value


    if (userInput == "1")
    {
        system("CLS"); // Clear console
        librarian staff; // Create a new session of the class
        staff.librarianLogin(remainingChances, staff); // Engage the method
    }
    else if (userInput == "2")
    {
        system("CLS"); // Clear console
        student person; // Create a new session of the class
        person.studentLogin(remainingChances, person); // Engage the method
    }
    else
    {
        std::cout << "\nIncorrect value, please try again.\n" << std::endl;
        login(validChances - 1); // Recurse
    }

}



// Driver code
int main()
{
    int validChances = 3;
    login(validChances);
}
