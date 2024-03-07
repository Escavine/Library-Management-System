// Library Management System 


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



class librarian : user // Librarian inherits properties of a user
{

    void librarianLogin()
    {


    }

    void registerBook()
    {
        // Insert relevant logic for registering a book
    }

    void librarianDashboard()
    {
        // Insert logic for student dashboard
    }




};

class student : user // Student inherits properties of a user
{
    void studentLogin()
    {
        // Insert logic for student login

    }

    void studentDashboard()
    {
        // Insert logic for student dashboard
    }


};



void login(int validChances)
{
     std::string userInput;

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

    std::cout << "Stepwise University - Login\n";
    std::cout << "1. Librarian" << std::endl;
    std::cout << "2. Student" << std::endl;
    std::cout << "\nEnter a corresponding value: " << std::endl;
    std::cin >> userInput; // Accept the corresponding value


    if (userInput == "1")
    {



    }
    else if (userInput == "2")
    {


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
