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


// Defines all users (i.e. students and librarians)
class user
{
    public:
        void clearInputBuffer()
        {
            std::cin.clear(); // Clear the input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard any input buffers
        }

        void borrowBook(user individual) // This is dynamic and can be a teacher/student
        {
            std::string userInput;

            std::cout << "Stepwise University: Borrowing a Book\n";
            std::cout << "Current Books" << std::endl;



            std::cout << "\nEnter the Book ID for the book you'd like to borrow: " << std::endl;
            std::cin >> userInput; // Register user input


            // std::string embeddingCSV = userInput + ".csv"; // Used as comparision marker
            // std::cout << "\n" << embeddingCSV << std::endl; // Debugging

            // Open CSV file to load the books that are available in the library
            std::ifstream file(userInput + ".csv"); // Open the .CSV file, which contains the given book based on the book ID

            if (!file.is_open())
            {
                std::cerr << "\nCannot find book." << std::endl;
            }
            
            else
            {
                int confirmUserBorrow;

                std::cout << "\nFound book!" << std::endl;

                // Logic for displaying book information

                std::string line; // string for iterating file

                while (std::getline(file, line))
                {
                    // Create stringstream from line
                    std::stringstream ss(line);
                    std::string field; // Defining a single field for the CSV
                    std::vector<std::string> fields; // Defining fields for the CSV 

                    while (std::getline(ss, field, ',')) // Parse each field of the line
                    {
                        fields.push_back(field); // Append per field to the vector
                    }

                    for (const auto& f : fields)
                    {
                        std::cout << f << " ";
                    }
                    std::cout << std::endl;
                }

                std::cout << "\nWould you like to borrow this book? (1 for 'yes' and any other key for 'no')" << std::endl;
                std::cin >> confirmUserBorrow;


                if (confirmUserBorrow == 1)
                {
                    time_t localTime; // Retrieve local time
                    time(&localTime);

                    char dt[100]; // Store the following information from 'strftime' into 'dt'
                    
                    // Format the date and time into string format using strftime
                    strftime(dt, sizeof(dt), "%Y-%M-%D %H:%M-%S", localtime(&localTime));


                    file.clear(); // Clear the previous errors with the file
                    file.seekg(0); // Reset file pointer to the beginnning

                    // Create a borrow record session, embedding the individauls name and the ID for the book
                    std::ofstream outputFile(name + userInput + ".txt"); // Open a text file for writing


                    std::string line;
                    while (std::getline(file, line))
                    {
                        // Create stringstream from line
                        std::stringstream ss(line);
                        std::string field; // Defining a single field for the CSV
                        std::vector<std::string> fields; // Defining fields for the CSV 

                        while (std::getline(ss, field, ',')) // Parse each field of the line
                        {
                            fields.push_back(field); // Append per field to the vector
                        }
                        
                        // Append the following data values into the '.txt' file (TESTING)
                        
                        for (const auto& f : fields)
                        {
                            outputFile << f << " ";
                        }
                       
                        outputFile << std::endl;
                    }
                    
                    // Extra, relevant information
                    outputFile << "\nDate Borrowed: " << dt << std::endl;

                    std::cout << "\nBook borrowed successfully!" << std::endl;

                }
                else
                {
                    std::cout << "Would you like to perhaps borrow another book, or return to the dashboard? (1 for 'to borrow' and any other key for the dashboard)" << std::endl;
                    
                }

            }

            file.close();
        }

        void returnBook()
        {
            // Insert logic for returning a book

            // Based on the users information, see which books they've borrowed for how long etc, if its been over 2 weeks, then charge 0.20p per day extra from that point.

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
        void librarianLogin(int remainingChances, librarian staff)
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
            std::cout << "Stepwise University: Staff Login\n";
            std::cout << "\nInput your username: " << std::endl;
            std::cin >> usernameInput;

            std::cout << "\nInput your password: " << std::endl;
            std::cin >> passwordInput;

            if (usernameInput == username && passwordInput == password)
            {
                std::cout << "\nIndividual authenticated." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait three seconds
                system("CLS"); // After 3 seconds, recurse through the program to allow the individual to read the message
                librarianDashboard(staff);
                
            }
            else
            {
                std::cout << "\nIncorrect login details, please try again.\n" << std::endl;
                librarianLogin(remainingChances - 1, staff); // Reduce number of chances per recursion to avoid brute force
            }
        }

        void registerBook(librarian staff)
        {
            // Relevant parameters for book registering
            std::string subjectRegisterer; // Will register each subject that will then be appended to the vector
            std::string bookID;
            std::string bookTitle;
            std::vector <std::string> relevantSubjects; // A vector that will contain all necessary subjects of the books registered
            std::string bookPublisher;

            int numOfRelevantSubjects;
            int numCount = 1; // Will keep track of the loop
            int yearOfBook;
            int numberOfBooks;
            int numberOfAvailableBooks;

            bool relevantSubjectInput = false;

            // Add input validation measures to prevent trash input
            std::cout << "Stepwise University: Registering a book\n";

            std::cout << "\nEnter the book ID (format: xxx-xxx-xxx): ";
            std::cin >> bookID; // Add a structure for the book ID's that need to be followed

            bool checkPattern = bookIDPattern(bookID);

            if (!checkPattern)
            {
                std::cout << "\nIncorrect book ID structure, please try again." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds to allow user to read
                system("CLS");
                staff.clearInputBuffer();
                registerBook(staff); // Recurse
            }
            else
            {
                staff.clearInputBuffer();
            }


            // Add all relevant input validation mediums
            std::cout << "\nEnter the book title: " << std::endl;
            std::getline(std::cin, bookTitle);
            staff.clearInputBuffer();


            std::cout << "\nEnter the year of book published: " << std::endl;
            std::cin >> yearOfBook;
            staff.clearInputBuffer();

            std::cout << "\nEnter the name of the book publisher: " << std::endl;
            std::getline(std::cin, bookPublisher);
            staff.clearInputBuffer();

            std::cout << "\nEnter the number of books that have been released: " << std::endl;
            std::cin >> numberOfBooks;
            staff.clearInputBuffer();

            std::cout << "\nEnter the number of available books in the library: " << std::endl;
            std::cin >> numberOfAvailableBooks;
            staff.clearInputBuffer();

            std::cout << "\nHow many relevant subjects are there for this given book?" << std::endl;
            std::cin >> numOfRelevantSubjects; // Register this value
            staff.clearInputBuffer();



            // Do-while loop for appending relevant subjects for the book
            do
            {
                for (int i = 0; i < numOfRelevantSubjects; i++)
                {
                    std::cout << "\nEnter the relevant subject " << "(" << "Subject " << numCount << "): " << std::endl;
                    std::cin >> subjectRegisterer; // Register user input
                    relevantSubjects.push_back(subjectRegisterer); // Register the subject
                    numCount++;
                }



            } while (relevantSubjectInput = false);

            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before displaying the details
            system("CLS");

            // Output all relevant information regarding the registered book
            std::cout << "Stepwise University: Book Registered\n";
            std::cout << "Book Details\n";
            std::cout << "\nBook ID: " << bookID << std::endl;
            std::cout << "\nBook Title: " << bookTitle << std::endl;
            std::cout << "\nYear of release: " << yearOfBook << std::endl;
            std::cout << "\nBook Publisher: " << bookPublisher << std::endl;
            std::cout << "\nNumber of worldwide books released: " << numberOfBooks << std::endl;
            std::cout << "\nNumber of available copies of the book in the library: " << numberOfAvailableBooks << std::endl;
            
            std::cout << "\nNumber of relevant subjects: \n" << std::endl;

            int size = relevantSubjects.size();

            for (int i = 0; i < size; i++) // Iterate through the relevant subjects to display to user
            {
                std::cout << "* " << relevantSubjects[i] << "\n" << std::endl;

            }
            
            // Create a CSV file to store the books
            createCSV(bookID, bookTitle, yearOfBook, bookPublisher, numberOfBooks, numberOfAvailableBooks);

            // Redirect user back to dashboard
            std::cout << "\nYou will now be redirected to the dashboard..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(10)); // Wait five seconds
            system("CLS"); // Clear the console
            staff.librarianDashboard(staff); // Redirect the user

        }

        void createCSV(std::string bookID, std::string bookTitle, int yearOfBook, std::string bookPublisher, int numberOfBooks, int numberOfAvailableBooks) // This function will create a .CSV file for the registered book
        {
            std::string fileName = bookID; 
            std::ofstream file(fileName + ".csv"); // Create a file called books.csv

            // Store the necessary details of the book into the file
            file << "\nBook ID: " << bookID << std::endl;
            file << "\nBook Title: " << bookTitle << std::endl;
            file << "\nYear of release: " << yearOfBook << std::endl;
            file << "\nBook Publisher: " << bookPublisher << std::endl;
            file << "\nNumber of worldwide books released: " << numberOfBooks << std::endl;
            file << "\nNumber of available copies of the book in the library: " << numberOfAvailableBooks << std::endl;
            file.close(); // Close the file

        }



        void librarianDashboard(librarian staff)
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
            std::cin >> userChoice;


            // Selection case as switch case will break the program since string isn't supported (average C# user)

            if (userChoice == "1")
            {
                std::cout << "\nYou will now be lead to register a book on the system" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before registering a book
                system("CLS"); // Clear the console for cleanliness
                registerBook(staff); // Lead user to the function
            }
            else if (userChoice == "2")
            {
                std::cout << "\nBorrowing a book option has been selected. Redirecting user..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before allowing user to borrow a book
                system("CLS"); // Clear console
                staff.borrowBook(staff);

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
                librarianDashboard(staff); // Recurse 
            }

        }

        bool bookIDPattern(const std::string& bookID)
        {
            std::regex pattern("\\d{3}-\\d{3}-\\d{3}"); // Defining pattern
            return std::regex_match(bookID, pattern); // Return true, if there's match in design
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

        void studentLogin(int remainingChances, student person)
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
                studentDashboard(person); // Lead user to their dashboard
            }
            else
            {
                std::cout << "\nIncorrect login details, please try again.\n" << std::endl;
                studentLogin(remainingChances - 1, person); // Reduce number of chances per recursion to avoid brute force
            }
        }

        void studentDashboard(student person)
        {
            std::string userChoice;

            std::vector <std::string> dashboardOptions = { "Borrow a book", "Return a book", "Log out" }; // Vector of string options
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
            std::cin >> userChoice;


            // Selection case as switch case will break the program since string isn't supported (average C# user)

            if (userChoice == "1")
            {
                std::cout << "\nBorrowing a book option has been selected. Redirecting user..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before allowing user to borrow a book
                system("CLS"); // Clear console
                person.borrowBook(person);

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
                studentDashboard(person); // Recurse 
            }

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
