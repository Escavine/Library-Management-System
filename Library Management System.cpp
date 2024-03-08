// Library Management System 


// Prerequisite libraries
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>


// Defines all users (i.e. students and librarians)
class user
{
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

    protected:
        // Key attributes of a user
        std::string name;
        std::string surname;
        int age;
        std::string gender;
        std::string email;
};



class librarian : user // Librarian inherits properties of a user
{

    public:
        void librarianLogin(int remainingChances)
        {

            if (remainingChances < 3) // Check if user has less than 3 chances
            {
                if (remainingChances == 0) // Nested selection statement to check if the chances has reached zero.
                {
                    std::cout << "Too many incorrect attempts, console will now terminate." << std::endl;
                    exit(0); // Termiante the console
                }

                else
                {
                    std::cout << "Remaining chances: " << remainingChances << std::endl;

                }

            }

            std::string usernameInput;
            std::string passwordInput;

            // Request for relevant details of the individual
            std::cout << "Stepwise University - Staff Login\n";
            std::cout << "\nInput your username: " << std::endl;
            std::cin >> usernameInput;

            std::cout << "\nInput your password: " << std::endl;
            std::cin >> passwordInput;

            if (usernameInput == username && passwordInput == password)
            {
                std::cout << "\nIndividual authenticated." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait three seconds
                system("CLS"); // After 3 seconds, recurse through the program to allow the individual to read the message
                librarianDashboard();
                
            }
            else
            {
                std::cout << "\nIncorrect login details, please try again.\n" << std::endl;
                librarianLogin(remainingChances - 1); // Reduce number of chances per recursion to avoid brute force
            }
        }

        void registerBook()
        {
            // Insert relevant logic for registering a book
        }

        void librarianDashboard()
        {
            std::string userChoice;

            std::vector <std::string> dashboardOptions = { "Register a book", "Borrow a book", "Return a book", "Log out" }; // Vector of string options
            // int maxSize = dashboardOptions.max_size(); Determine the maximum value of the 

            int numCount = 1;
            std::cout << "Stepwise University: Staff/Librarian Dashboard\n";
            std::cout << "\n"; // Spacing
            std::cout << "Welcome " << name << std::endl;
            std::cout << "\n"; // Spacing

            for (int i = 0; i < dashboardOptions.size(); i++) // Iterate through the vector values, displaying them to the staff member
            {
                std::cout << numCount << ". " << dashboardOptions[i] << std::endl;
                numCount++;
            }

            std::cout << "\n"; // Spacing
            std::cout << "Enter a corresponding value: " << std::endl;
            std::getline(std::cin, userChoice);

            // Selection case as switch case will break the program since string isn't supported (average C# user)

            if (userChoice == "1")
            {
                // Relevant parameters for book registering
                std::string bookID;
                std::string bookTitle;
                int yearOfBook;
                std::string bookPublisher;
                int numberOfBooks;
                int numberOfAvailableBooks;
                std::vector <std::string> relevantSubjects = {}; // A vector that will contain all necessary subjects of the books registered
                std::cout << "Stepwise University: Registering a book\n";

                std::cout << "Enter the book ID: " << std::endl;
                std::getline(std::cin, bookID);

                std::cout << "Enter the book title: " << std::endl;
                std::getline(std::cin, bookTitle);

                std::cout << "Enter the year of book published: " 





            }
            else if (userChoice == "2")
            {



            }
            else if (userChoice == "3")
            {



            }
            else if (userChoice == "4")
            {
                // int remainingChances = 3;
                // std::cout << "Logging user out of session." << std::endl;
                // login(remainingChances);  
            }
            else
            {
                std::cout << "\nInvalid input, please try again." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait three seconds
                system("CLS"); // After 3 seconds, recurse through the program to allow the individual to read the message
                librarianDashboard(); // Recurse 
            }

        }

    protected:
        // Test data for authenticating login
        std::string name = "Khalid";
        std::string username = "Eucladian";
        std::string password = "euclade1";


};

class student : user // Student inherits properties of a user
{
    public:

        void studentLogin(int remainingChances)
        {
            if (remainingChances < 3) // Check if user has less than 3 chances
            {
                if (remainingChances == 0) // Nested selection statement to check if the chances has reached zero.
                {
                    std::cout << "Too many incorrect attempts, console will now terminate." << std::endl;
                    exit(0); // Termiante the console
                }

                else
                {
                    std::cout << "Remaining chances: " << remainingChances << std::endl;

                }

            }

            std::string usernameInput;
            std::string passwordInput;

            // Request for relevant details of the individual
            std::cout << "Stepwise University - Student Login\n";
            std::cout << "\nInput your username: " << std::endl;
            std::cin >> usernameInput;

            std::cout << "\nInput your password: " << std::endl;
            std::cin >> passwordInput;

            if (usernameInput == username && passwordInput == password)
            {
                std::cout << "\nStudent authenticated." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait three seconds
                system("CLS"); // After 3 seconds, recurse through the program to allow the individual to read the message
                studentDashboard(); // Lead user to their dashboard
            }
            else
            {
                std::cout << "\nIncorrect login details, please try again.\n" << std::endl;
                studentLogin(remainingChances - 1); // Reduce number of chances per recursion to avoid brute force
            }
        }

        void studentDashboard()
        {
            // Insert logic for student dashboard
        }

    private:
        // Test data for authenticating login
        std::string username = "Eucladian";
        std::string password = "euclade1";

};



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

    std::cout << "Stepwise University - Identification\n";
    std::cout << "1. Librarian" << std::endl;
    std::cout << "2. Student" << std::endl;
    std::cout << "\nEnter a corresponding value: " << std::endl;
    std::cin >> userInput; // Accept the corresponding value


    if (userInput == "1")
    {
        system("CLS"); // Clear console
        librarian staff; // Create a new session of the class
        staff.librarianLogin(remainingChances); // Engage the method
    }
    else if (userInput == "2")
    {
        system("CLS"); // Clear console
        student person; // Create a new session of the class
        person.studentLogin(remainingChances); // Engage the method
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
