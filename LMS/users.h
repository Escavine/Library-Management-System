#pragma once

// All relevant libraries
#include <chrono>
#include <iomanip>
#include <cstdio>
#include <fstream>

const int MAX_FILES_TO_CHECK = 100; // Will be used to check for number of books borrowed, and '100' is the set limit
const int MAX_BORROWING_DAYS = 14; // Max borrowing days is set to '14' (2 weeks)

// Structure for a book
struct Book {
    std::string bookID, bookTitle, bookPublisher, dateBorrowed;

    std::vector<std::string> relevantSubjects;

    int numberOfReleases, yearOfRelease, remainingBooks, quantityBorrowed;
};

struct date
{
    int day;
    int month;
    int year;
};

struct studentlogin {
    std::string name, surname, username, password;
};

struct stafflogin {
    std::string name, surname, username, password;
};


// Defines all users (i.e. students and librarians)
class user
{
public:
    void clearInputBuffer()
    {
        std::cin.clear(); // Clear the input
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard any input buffers
    }

    void borrowBook(user individual, std::string name, std::string surname) // This is dynamic and can be a teacher/student
    {
        std::string userInput;
        std::string inputBookID;

        // Check if there's any books registered in the system, should there not be any books, return user to the menu

        std::cout << "Stepwise University: Checking for existence of books\n";

        std::ifstream checkExist("RegisteredBooks.csv"); // Read from the file

        if (!checkExist)
        {
            std::cout << "\nNo books are currently registered on the system..." << std::endl;
            exit(1); // Temporary measure to deal with non-existing books
        }

        system("CLS"); // Clear the console, should there be books present

        std::cout << "Stepwise University: Borrowing a Book\n";

        // Count the number of books the user has borrowed prior
        int borrowedBooksCount = 0;
        for (int x = 1; x <= MAX_FILES_TO_CHECK; ++x)
        {
            std::ifstream checkExistingFile(name + "_" + surname + "_" + std::to_string(x) + ".csv");
            if (checkExistingFile.is_open()) {
                borrowedBooksCount++;
                checkExistingFile.close(); // Close the file
            }
        }

        std::cout << "\nNumber of currently borrowed books: " << borrowedBooksCount << "\n";

        std::cout << "\n1. Borrow a book via Book ID" << std::endl;
        std::cout << "\n2. Check the current books registered on the system" << std::endl;

        std::cout << "\nSelect an option: ";
        std::cin >> userInput; // Register user input

        if (userInput == "1")
        {
            std::cout << "\nRedirecting user..." << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds
            clearInputBuffer();
            system("CLS");

            std::cout << "Stepwise University: Borrowing a Book\n";

            std::cout << "\nEnter the book ID for the book you'd like to borrow (format xxx-xxx-xxx): ";
            std::cin >> inputBookID; // Register the input

            // Open CSV file to load the books that are available in the library
            std::ifstream file(inputBookID + ".csv"); // Open the .CSV file, which contains the given book based on the book ID

            if (!file.is_open())
            {
                std::cerr << "\nCannot find book." << std::endl;
                clearInputBuffer(); // Clear the buffer
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before reattempt
                system("CLS"); // Clear the console
                borrowBook(individual, name, surname);
            }
            else
            {
                std::string confirmUserBorrow;

                // Logic for displaying book information

                std::string line; // string for iterating file
                std::vector<Book> bookQuantityCheck; // Vector for checking the quantity of the current books availiable


                while (std::getline(file, line))
                {
                    std::vector<std::string> order = { "Book ID: ", "Book Name: ", "Year of release: ", "Author: ", "Worldwide Releases: ", "Available Copies in the Library: " };
                    // Create stringstream from line
                    std::stringstream ss(line);
                    std::string field; // Defining a single field for the '.csv'
                    std::vector<std::string> fields; // Defining fields for the '.csv' 

                    while (std::getline(ss, field, ',')) // Parse each field of the line
                    {
                        fields.push_back(field); // Append per field to the vector
                    }

                    if (fields.size() >= 6)
                    {
                        Book book;
                        book.quantityBorrowed = 1; // Set the quantity to '1' book borrowed
                        book.bookID = fields[0];
                        book.bookTitle = fields[1];
                        book.yearOfRelease = std::stoi(fields[2]);
                        book.bookPublisher = fields[3];
                        book.remainingBooks = std::stoi(fields[5]);

                        // Check if the book quantity is '0'
                        if (book.remainingBooks == 0)
                        {
                            std::cout << "\nThis book is currently not available, please try again." << std::endl;
                            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before allowing user to borrow a book
                            individual.clearInputBuffer(); // Clean the input buffer to prevent issues

                            system("CLS"); // Clear console
                            individual.borrowBook(individual, name, surname); // Recurse, allowing for the user to pick a different book

                        }
                        else
                        {
                            std::cout << "\nFound book!\n" << std::endl;

                            for (int i = 0; i < order.size();)
                            {
                                for (int f = 0; f < fields.size();)
                                {
                                    std::cout << order[i] << fields[f] << " ";
                                    i += 1;
                                    f += 1;
                                    std::cout << "\n";
                                }
                            }
                            std::cout << std::endl;
                        }

                        bookQuantityCheck.push_back(book);
                    }

                }

                std::cout << "Would you like to borrow this book? (1 for 'yes' and any other key for 'no'): ";
                std::cin >> confirmUserBorrow;


                if (confirmUserBorrow == "1")
                {
                    file.clear(); // Clear the previous errors with the file
                    file.seekg(0); // Reset file pointer to the beginnning


                    time_t localTime; // Retrieve local time
                    time(&localTime);

                    char dt[100]; // Store the following information from 'strftime' into 'dt'

                    // Format the date and time into string format using strftime
                    strftime(dt, sizeof(dt), "%d/%m/%Y", localtime(&localTime));

                    // Files are created linearly, check if previous linear files exist and create a new borrowing session based on that (i.e. file structure: name + surname + number starting from 1,2,3,4....)

                    for (int x = 1; x <= MAX_FILES_TO_CHECK; ++x)
                    {
                        std::ifstream checkExistingFile(name + "_" + surname + "_" + std::to_string(x) + ".csv");

                        if (!checkExistingFile)
                        {
                            // Create a borrow record session, embedding the individual's name and the ID for the book
                            std::ofstream outputFile(name + "_" + surname + "_" + std::to_string(x) + ".csv", std::ios::app); // Open a text file for writing 

                            std::string line;
                            std::vector<std::string> fields; // Defining fields for the CSV 
                            std::vector<Book> books; // Vector for updating the remaining book value with the 'book' structure as the data type


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


                                if (fields.size() >= 6)
                                {
                                    Book book;

                                    book.quantityBorrowed = 1; // Set the quantity to '1' book borrowed

                                    book.bookID = fields[0];
                                    book.bookTitle = fields[1];
                                    book.yearOfRelease = std::stoi(fields[2]);
                                    book.bookPublisher = fields[3];
                                    book.remainingBooks = std::stoi(fields[5]);

                                    // Deduct the book linearly
                                    if (book.remainingBooks > 0)
                                    {
                                        book.remainingBooks -= 1;
                                    }

                                    books.push_back(book);
                                }


                                for (const auto& book : books)
                                {
                                    outputFile << book.bookID << "," << book.bookTitle << "," << book.yearOfRelease << ","
                                        << book.quantityBorrowed << "," << book.remainingBooks << "," << dt << "\n"; // Adhere strictly to '.csv' format
                                }

                            }

                            file.clear(); // Clear the previous errors with the file
                            file.seekg(0); // Reset file pointer

                            std::ifstream fileUpdate(inputBookID + ".csv"); // Open the .CSV file for writing
                            std::vector<Book> booksUpdate; // Vector for updating book information

                            std::string lineUpdate;
                            while (std::getline(fileUpdate, lineUpdate))
                            {
                                // Create stringstream from line
                                std::stringstream ss(lineUpdate);
                                std::string field; // Defining a single field for the CSV
                                std::vector<std::string> fields; // Defining fields for the CSV 

                                // Parse each field of the line
                                while (std::getline(ss, field, ','))
                                {
                                    fields.push_back(field); // Append per field to the vector
                                }

                                // Check if there are enough fields to process
                                if (fields.size() >= 6)
                                {
                                    Book book;
                                    book.quantityBorrowed = 1; // Set the quantity to '1' book borrowed
                                    book.bookID = fields[0];
                                    book.bookTitle = fields[1];
                                    book.yearOfRelease = std::stoi(fields[2]);
                                    book.bookPublisher = fields[3];
                                    book.numberOfReleases = std::stoi(fields[4]);
                                    book.remainingBooks = std::stoi(fields[5]);

                                    // Deduct the book linearly from the '.csv' file itself
                                    if (book.remainingBooks > 0)
                                    {
                                        book.remainingBooks -= 1;
                                    }

                                    booksUpdate.push_back(book);
                                }

                                fileUpdate.close(); // Close the instance 

                                std::ofstream updatedFile(inputBookID + ".csv"); // For writing

                                if (!updatedFile)
                                {
                                    std::cout << "\nUnable to open file for writing..." << std::endl;
                                    exit(1);
                                }

                                // Write the updated book information back to the file
                                for (const auto& book : booksUpdate)
                                {
                                    updatedFile << book.bookID << "," << book.bookTitle << "," << book.yearOfRelease << ","
                                        << book.bookPublisher << "," << book.numberOfReleases << "," << book.remainingBooks << "\n";
                                }
                            }

                            exit(1); // Terminate to mitigate future issues (temp solution)

                        }
                    }

                }
                else
                {
                    std::string userChoice;
                    std::cout << "\nWould you like to borrow another book or return to the dashboard? (1 for 'to borrow' and any other key for the dashboard): ";
                    std::getline(std::cin, userChoice); // Retrieve user input

                    if (userChoice == "1")
                    {
                        std::cout << "\nRedirecting user to borrow another book..." << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(3));
                        system("CLS");
                        clearInputBuffer();
                        borrowBook(individual, name, surname);
                    }
                    else
                    {
                        std::cout << "\nRedirecting user to the dashboard..." << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(3));
                        system("CLS");
                        clearInputBuffer();

                        // Check if the user is a 'librarian' or 'student'
                    }
                }

            }

            file.close();

        }
        else if (userInput == "2")
        {
            std::cout << "\nRedirecting user..." << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds
            clearInputBuffer();
            system("CLS");

            std::cout << "Current books registered on the system: " << "\n" << std::endl; // Numerous books can be registered on the system

            std::string line;
            std::vector<Book> bookRegistering; // Vector for displaying the number of books available

            while (std::getline(checkExist, line))
            {
                std::vector<std::string> order = { "Book ID: ", "Book Name: ", "Year of release: ", "Author: ", "Worldwide Releases: " }; // Contains all relevant information for the book
                std::stringstream ss(line);
                std::string field;
                std::vector<std::string> fields; // Will contain the appended information for the book

                while (std::getline(ss, field, ',')) // ',' is the delimiter
                {
                    fields.push_back(field); // Append the following information to the vector
                }

                if (fields.size() == order.size()) // Ensure both vectors have the same size            
                {
                    Book book;
                    book.bookID = fields[0];
                    book.bookTitle = fields[1];
                    book.yearOfRelease = std::stoi(fields[2]);
                    book.bookPublisher = fields[3];
                    book.numberOfReleases = std::stoi(fields[4]);

                    // Print book information
                    for (int i = 0; i < order.size(); ++i)
                    {
                        std::cout << order[i] << fields[i] << "\n";
                    }
                    std::cout << std::endl;
                }

            }

            std::cout << "\nFor any information regarding the quantity of remaining books in the library, kindly return to the borrowing menu, click the option to find the book ID and input the given book." << std::endl;

            std::cout << "\nPlease enter any key to return to borrowing a book: ";
            system("\npause"); // Register user input

            std::cout << "\nRedirecting user..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before reattempt
            borrowBook(individual, name, surname); // Recurse
        }
        else
        {
            std::cout << "\nInvalid input, please try again." << std::endl;
            clearInputBuffer(); // Clear the buffer
            system("CLS"); // Clear the console
            borrowBook(individual, name, surname); // Recurse
        }




    }



    void returnBook(user individual, std::string name, std::string surname) // Function that calculates the fine, if a book has been borrowed for over 2 weeks
    {
        std::string userChoice, userChoice2;

        std::cout << "Stepwise University: Returning Book" << std::endl;

        // Count the number of books the user has borrowed 
        int borrowedBooksCount = 0;
        int x = 1; // This has intentionally designed to be placed out of the loop, that way it can be used to find the given file and determine if the user has borrowed a book for over 2 weeks

        for (x; x <= MAX_FILES_TO_CHECK; ++x) {
            std::ifstream checkExistingFile(name + "_" + surname + "_" + std::to_string(x) + ".csv");
            if (checkExistingFile.is_open()) 
            {
                borrowedBooksCount++;
                checkExistingFile.close();
            }
        }

        std::cout << "\nNumber of currently borrowed books: " << borrowedBooksCount << std::endl;

        std::cout << "\nEnter the number of the book you'd like to return (i.e. the first book, then 1, the second book 2 etc...): ";
        std::cin >> userChoice;

        std::ifstream file(name + "_" + surname + "_" + userChoice + ".csv");
        if (file.is_open())
        {
            std::string line;

            std::cout << "\nBorrowing session has been found, now displaying information...\n" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));

            while (std::getline(file, line))
            {
                std::vector<std::string> fields;
                std::stringstream ss(line);
                std::vector<std::string> order = { "Book ID: ", "Book Name: ", "Year of Release: ", "Quantity Borrowed: ", "Remaining Quantity: ", "Borrow Date: " };
                std::string field;
                while (std::getline(ss, field, ',')) {
                    fields.push_back(field);
                }

                for (int y = 0; y < order.size();)
                {
                    for (int i = 0; i < fields.size();) {
                        std::cout << order[y] << fields[i] << " ";
                        y += 1;
                        i += 1;
                        std::cout << "\n";
                    }
                }

                std::cout << "\n"; // Spacing

                calculatingFine(name, surname, std::stoi(userChoice)); // Execute the given function to see whether the user has borrowed the book for over 2 weeks or not, and continue with the program...
            }

            file.close();
        }
        else
        {
            std::cout << "\nBorrowing session has not been found." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds
            system("CLS"); // Clear console
            returnBook(individual, name, surname); // Recurse
        }
    }

    void calculatingFine(std::string name, std::string surname, int x)
    {
        std::string userChoice; // Will be used to give user a choice between how to pay (i.e. cash/card)

        std::cout << "Checking if borrowing duration is over 2 weeks...\n" << std::endl;

        // Retrieve local time
        time_t currentTime;
        time(&currentTime);

        // Convert current time to string format
        char currentDate[100];
        strftime(currentDate, sizeof(currentDate), "%d/%m/%Y", localtime(&currentTime));

        std::ifstream dateSearchForBook(name + "_" + surname + "_" + std::to_string(x) + ".csv"); // Open the following file

        if (!dateSearchForBook.is_open()) {
            std::cerr << "Error: Unable to open file for reading." << std::endl;
            // Handle the error, possibly by returning from the function or taking appropriate action
            return; // For example, return from the function to avoid further processing
        }
        else
        {
            std::cout << "Successfully opened file for reading (CHECK 1)" << std::endl;
        }


        std::ifstream currentFileDate("CurrentDate.csv"); // File that contains the current date

        std::string field;

        std::string currentDateStr;

        if (std::getline(currentFileDate, currentDateStr))
        {
            std::istringstream iss(currentDateStr);
            date currentDateInfo;
            char delimiter;

            if (iss >> currentDateInfo.day >> delimiter >> currentDateInfo.month >> delimiter >> currentDateInfo.year)
            {
                std::cout << "Current date: " << currentDateStr << " (CHECK 2)" << std::endl; // Display the current date to the user
            }
            else
            {
                std::cout << "\nInvalid date format contained within the CurrentDate.csv file." << std::endl;
            }

        }
        else
        {
            std::cout << "Error: CurrentDate.csv does not exist." << std::endl;
        }


        // Logic to check if the following book has been borrowed for over 2 weeks (borrowing limit is 2 weeks, each day that is exceeded is an added 0.20p charge)

        while (std::getline(dateSearchForBook, field))
        {
            std::vector<std::string> fields;
            std::istringstream iss(field);

            while (std::getline(iss, field, ','))
            {
                fields.push_back(field); // Append the fields in the file
            }

            if (fields.size() >= 6)
            {
                Book book;
                book.bookID = fields[0];
                book.bookTitle = fields[1];
                book.dateBorrowed = fields[5]; // Retrieve the date borrowed for the book

                std::istringstream dateStream(book.dateBorrowed);
                date borrowDate;
                char delimiter;

                if (!(dateStream >> borrowDate.day >> delimiter >> borrowDate.month >> delimiter >> borrowDate.year))
                {
                    std::cout << "\nError: Invalid date format in the borrowed book file." << std::endl;
                    continue;
                }

                // Calculate the difference between the two dates

                std::tm borrowTime = { 0 };
                borrowTime.tm_mday = borrowDate.day;
                borrowTime.tm_mon = borrowDate.month - 1; // Months are zero based
                borrowTime.tm_year = borrowDate.year - 1900; // Year is years since 1900
                
                time_t borrowTimestamp = mktime(&borrowTime);

                double secondsElapsed = difftime(currentTime, borrowTimestamp);
                int daysElapsed = static_cast<int>(secondsElapsed / (60 * 60 * 24));

                // Check if borrowing duration exceeds two weeks
                if (daysElapsed > MAX_BORROWING_DAYS)
                {
                    double fine = 0.20 * (daysElapsed - MAX_BORROWING_DAYS); // Calculate the fine

                    int actualDays = daysElapsed - MAX_BORROWING_DAYS; // Figure out the difference in the days to give the user a proper measure of the actual days the book is overdue for...

                    std::cout << "\nBook overdue for: " << actualDays << " day/days" << std::endl; // Display the number of days that the book was overdue for...

                    std::cout << "\nFine for book \"" << book.bookTitle << "\" (ID: " << book.bookID << "): " << fine << "p" << std::endl;

                    std::cout << "\nHow would you like to pay your fine? ('1' for 'cash' and '2' for 'card')" << std::endl;

                    std::cout << "\n1. Cash" << std::endl;
                    std::cout << "2. Card" << std::endl;

                    std::cout << "\nEnter a corresponding value (CHECK 3): "; // Allow user to register their choice for the options displayed
                    std::cin >> userChoice; // Register user input

                    std::ifstream changeQuantity(book.bookID + ".csv"); // Find the '.csv' file for the given book that was borrowed by the user


                    if (userChoice == "1" || userChoice == "2")
                    {
                        // Returning book logic + creation of returned books

                        std::cout << "\nPayment is being processed..." << std::endl; // This section remains static as no mention of an actual payment process

                        std::cout << "\nReturning book..." << std::endl; // Delete the file that keeps the record of the book borrowing session

                        // Create a book return record and update the quantity of the following book by '+1'
                        std::string line;
                        std::vector<std::string> updatedLines; // Store updated lines

                        while (std::getline(changeQuantity, field))
                        {
                            std::vector<std::string> fields;
                            std::stringstream ss(field);

                            while (std::getline(ss, field, ','))
                            {
                                fields.push_back(field); // Append the fields in the file
                            }

                            if (fields.size() >= 6)
                            {
                                int remainingBooks = std::stoi(fields[5]); // Convert remaining books to integer
                                remainingBooks += 1; // Increment the value by '+1' as the book is being returned

                                fields[5] = std::to_string(remainingBooks); // Convert back to string and update

                                std::stringstream updatedLine;
                                for (size_t i = 0; i < fields.size(); ++i)
                                {
                                    updatedLine << fields[i];
                                    if (i < fields.size() - 1)
                                    {
                                        updatedLine << ','; // Add delimiter if it's not the last field
                                    }
                                }

                                updatedLines.push_back(updatedLine.str()); // Store the updated line
                            }
                        }

                        std::ofstream pushQuantityChange(book.bookID + ".csv"); // Open the output file

                        // Write all updated lines to the output file
                        for (const auto& line : updatedLines)
                        {
                            pushQuantityChange << line << std::endl;
                        }
                    
                        // Open the file in append mode
                        std::ofstream returnRecords("ReturnRecords.csv", std::ios::app);

                        // Check if the file is opened successfully
                        if (!returnRecords.is_open()) {
                            std::cerr << "Error: Unable to open ReturnRecords.csv for writing." << std::endl;
                            // Handle the error appropriately, such as returning from the function
                        }
                        else {
                            // DEBUGGING MEASURES
                            std::cout << "\nDebugging Measures (CHECK 4)\n" << std::endl;
                            std::cout << "Book ID: " << book.bookID << "\n";
                            std::cout << "Book Title: " << book.bookTitle << "\n";
                            std::cout << "Days Overdue: " << actualDays << "\n";
                            std::cout << "Fine: " << fine << "\n";

                            // Write data to the file
                            returnRecords << book.bookID << "," << book.bookTitle << "," << actualDays << "," << fine << std::endl;

                            // Check for any errors during write operation
                            if (!returnRecords) {
                                std::cerr << "Error: Failed to write data to ReturnRecords.csv." << std::endl;
                                // Handle the error appropriately, such as returning from the function
                            }

                            // Close the file
                            returnRecords.close();
                        }
                         
                        // Delete the file that retains the book borrowing session of the users
                        std::string filename = name + "_" + surname + "_" + std::to_string(x) + ".csv";

                        if (remove(filename.c_str()) != 0)
                        {
                            std::perror("\nError deleting borrow session file (CHECK 5: FAILED)");
                            std::cerr << "\nFailed to delete " << filename << std::endl;
                        }
                        else
                        {
                            std::cout << "Successfully deleted borrow session file! (CHECK 5: SUCCESS)" << std::endl; // Let the user know that their borrow session has been removed after payment
                        }

                        pushQuantityChange.close(); // Close the output file
                        changeQuantity.close(); // Close the input file
                        dateSearchForBook.close(); // Close the file once done used
                    }
                    else
                    {
                        std::cout << "\nInvalid input, please try again" << std::endl;

                        std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds (aesthetic purposes)
                        system("CLS"); // Clear the console

                        calculatingFine(name, surname, x); // Recurse
                    }
                }
                else
                {
                    // Should the book be returned in 2 weeks or less...
                    std::cout << "\nBook has not been borrowed for over 2 weeks, so no fines will be charged." << std::endl;
                    std::cout << "\nProcessing return..." << std::endl;

                    std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds...

                    // Process the return 
                    
                    std::ifstream changeQuantity(book.bookID + ".csv");
                    std::vector<std::string> linesToUpdate; // To store all lines with updates

                    // Create a book return record and update the quantity of the following book by '+1'
                    std::string line;
                    std::vector<std::string> updatedLines; // Store updated lines

                    while (std::getline(changeQuantity, field))
                    {
                        std::vector<std::string> fields;
                        std::stringstream ss(field);

                        while (std::getline(ss, field, ','))
                        {
                            fields.push_back(field); // Append the fields in the file
                        }

                        if (fields.size() >= 6)
                        {
                            int remainingBooks = std::stoi(fields[5]); // Convert remaining books to integer
                            remainingBooks += 1; // Increment the value by '+1' as the book is being returned

                            fields[5] = std::to_string(remainingBooks); // Convert back to string and update

                            std::stringstream updatedLine;
                            for (size_t i = 0; i < fields.size(); ++i)
                            {
                                updatedLine << fields[i];
                                if (i < fields.size() - 1)
                                {
                                    updatedLine << ','; // Add delimiter if it's not the last field
                                }
                            }

                            updatedLines.push_back(updatedLine.str()); // Store the updated line
                        }
                    }

                    std::ofstream pushQuantityChange(book.bookID + ".csv"); // Open the output file

                    // Write all updated lines to the output file
                    for (const auto& line : updatedLines)
                    {
                        pushQuantityChange << line << std::endl;
                    }

                    changeQuantity.close();
                    pushQuantityChange.close(); // Close the output file

                    std::cout << "\nBook successfully returned!" << std::endl; // Tell the user that the changes have been made

                    // Creation/Writing (if it already exists) of a '.csv' file named returned records, which will contain the book that's returned along with the fine cost, and overdue date for the book returned.
                    std::ofstream returnRecords("ReturnRecords.csv", std::ios::app);

                    returnRecords << book.bookID << "," << book.bookTitle << "," << daysElapsed << "," << 0 << "\n"; // Append the following information to the file (writing)

                    // Delete the file that retains the book borrowing session of the users
                    std::string filename = name + "_" + surname + "_" + std::to_string(x) + ".csv";

                    if (remove(filename.c_str()) != 0)
                    {
                        std::perror("Error deleting borrow session file");
                    }
                    else
                    {
                        std::cout << "Successfully deleted borrow session file!" << std::endl; // Let the user know that their borrow session has been removed after payment
                    }

                    break;
                }
            }
        }

    }

};





class student : user // Student inherits properties of a user
{
    public:

        void studentLogin(int remainingChances, student person)
        {
            std::string name, surname, username, password;
            person.clearInputBuffer(); // Clear buffer prior to information collection

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

            // Request for relevant details of the individual
            std::cout << "Stepwise University: Student Identification\n";

            std::cout << "\nInput your name: ";
            std::cin >> name;

            clearInputBuffer();

            std::cout << "\nInput your surname: ";
            std::cin >> surname;

            std::ifstream findingUserOnSystem("student_" + name + "_" + surname + ".csv"); // Concatenate the following information to locate the given '.csv' file

            if (findingUserOnSystem.is_open())
            {
                std::string field;
                std::vector<std::string> fields; // Vector that will append the given values
                std::stringstream ss(field);
                std::vector<studentlogin> student; // Will retain the following fields for the user (i.e. name, surname, username and password) via a vector

                std::cout << "\nStudent identified." << std::endl;

                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds
                system("CLS"); // Clear the console

                // Proceed with logging in the user
                std::cout << "Stepwise University: Student Login\n";

                std::cout << "\nEnter your username: ";
                std::cin >> username;

                std::cout << "\nEnter your password: ";
                std::cin >> password;

                std::string line;

                while (std::getline(findingUserOnSystem, line))
                {
                    std::istringstream iss(line);

                    // Conditions to authenticate the user, by retrieving the stored username and password from the file and comparing it to the users input
                    std::string storedFirstName, storedLastName, storedUsername, storedPassword;

                    if (std::getline(iss, storedFirstName, ',') &&
                        std::getline(iss, storedLastName, ',') &&
                        std::getline(iss, storedUsername, ',') &&
                        std::getline(iss, storedPassword, ',')) // Ensure all fields are properly read
                    {
                        if (username == storedUsername && password == storedPassword)
                        {
                            std::cout << "\nStudent authenticated." << std::endl;

                            std::cout << "\nClick any key to go to the student dashboard.\n" << std::endl;

                            system("pause");

                            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds
                            system("CLS"); // Clear the console
                            person.studentDashboard(person, name, surname); // Recurse 

                        }
                        else
                        {
                            std::cout << "\nUsername or password not recognized, please try again." << std::endl;

                            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds
                            system("CLS"); // Clear the console

                            person.studentLogin(remainingChances - 1, person); // Reduce chances, and recurse
                        }
                    }
                }
            }
            else
            {
                std::cout << "\nUnidentified individual, please try again." << std::endl;

                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds
                system("CLS"); // Clear the console

                studentLogin(remainingChances - 1, person);

            }
        }

        void studentDashboard(student person, std::string name, std::string surname)
        {
            std::string userChoice;

            std::vector <std::string> dashboardOptions = { "Borrow a book", "Return a book", "Terminate session" }; // Vector of string options
            // int maxSize = dashboardOptions.max_size(); Determine the maximum value of the 

            int numCount = 1;
            std::cout << "Stepwise University: Student Dashboard\n";
            std::cout << "\n"; // Spacing

            std::cout << "Welcome " + name << "\n" << std::endl;

            for (int i = 0; i < dashboardOptions.size(); i++) // Iterate through the vector values, displaying them to the staff member
            {
                std::cout << numCount << ". " << dashboardOptions[i] << std::endl;
                numCount++;
            }

            std::cout << "\n"; // Spacing
            std::cout << "Enter a corresponding value: ";
            std::cin >> userChoice;


            // Selection case as switch case will break the program since string isn't supported (average C# user)

            if (userChoice == "1")
            {
                std::cout << "\nBorrowing a book option has been selected. Redirecting user..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before allowing user to borrow a book
                system("CLS"); // Clear console
                person.borrowBook(person, name, surname); // Redirect user

            }
            else if (userChoice == "2")
            {
                std::cout << "\nReturning a book has been selected. Redirecting user..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before allowing user to borrow a book
                system("CLS"); // Clear console
                person.returnBook(person, name, surname); // Redirect user
            }
            else if (userChoice == "3") // Allows for the creation of a student account
            {
                std::cout << "\nTerminating user session..." << std::endl;
                exit(1); // Successfully terminate the users session
            }
            else
            {
                std::cout << "\nInvalid input, please try again." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait three seconds
                system("CLS"); // After 3 seconds, recurse through the program to allow the individual to read the message
                studentDashboard(person, name, surname); // Recurse 
            }

        }

};


class librarian : user // Librarian inherits properties of a user
{
    public:
        void librarianLogin(int remainingChances, librarian staff)
        {
            std::string staffName, staffSurname, staffUsername, staffPassword; // Will store vital details for the staff member

            if (remainingChances < 3) // Check if user has less than 3 chances
            {
                if (remainingChances == 0) // Nested selection statement to check if the chances have reached zero.
                {
                    std::cout << "Too many incorrect attempts, console will now terminate." << std::endl;
                    exit(0); // Terminate the console
                }
                else
                {
                    std::cout << "Remaining chances: " << remainingChances << std::endl;
                }
            }

            // Request for relevant details of the individual
            std::cout << "Stepwise University: Staff/Librarian Identification\n";

            std::cout << "\nInput your name: ";
            std::cin >> staffName;
            clearInputBuffer();

            std::cout << "\nInput your surname: ";
            std::cin >> staffSurname;

            // Construct the filename using the provided name and surname
            std::string fileName = "staff_" + staffName + "_" + staffSurname + ".csv";

            std::ifstream findingUserOnSystem(fileName); // Open the file using the constructed filename

            if (findingUserOnSystem.is_open())
            {
                std::cout << "\nStaff member identified" << std::endl;

                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds
                system("CLS"); // Clear the console

                // Proceed with logging in the user
                std::cout << "Stepwise University: Staff/Librarian Login\n";

                std::cout << "\nEnter your username: ";
                std::cin >> staffUsername;

                std::cout << "\nEnter your password: ";
                std::cin >> staffPassword;

                std::string line;

                while (std::getline(findingUserOnSystem, line))
                {
                    std::istringstream iss(line);

                    std::string storedName, storedSurname, storedUsername, storedPassword;

                    if (std::getline(iss, storedName, ',') &&
                        std::getline(iss, storedSurname, ',') &&
                        std::getline(iss, storedUsername, ',') &&
                        std::getline(iss, storedPassword, ',')) // Ensure all fields are properly read
                    {
                        if (staffUsername == storedUsername && staffPassword == storedPassword)
                        {
                            std::cout << "\nStaff member authenticated." << std::endl;
                            std::cout << "\nClick any key to go to the staff dashboard.\n" << std::endl;

                            system("pause");
                            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds
                            system("CLS"); // Clear the console
                            staff.librarianDashboard(staff, staffName, staffSurname); // Redirect to the dashboard
                        }
                        else
                        {
                            std::cout << "\nUsername or password not recognized, please try again." << std::endl;
                            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds
                            system("CLS"); // Clear the console
                            librarianLogin(remainingChances - 1, staff); // Reduce chances and recurse
                        }
                    }
                }
            }
            else
            {
                std::cout << "\nStaff member not recognized, please try again..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds
                clearInputBuffer();
                system("CLS"); // Clear the console
                librarianLogin(remainingChances - 1, staff); // Recurse
            }
        }

        void registerBook(librarian staff, std::string name, std::string surname)
        {
            Book book; // Create an instance of a book
            int numCount = 1; // Will keep track of the loop
            std::string subjectRegisterer;
            int numOfRelevantSubjects;

            // Add input validation measures to prevent trash input
            std::cout << "Stepwise University: Registering a book\n";

            std::cout << "\nEnter the book ID (format: xxx-xxx-xxx): ";
            std::cin >> book.bookID; // Add a structure for the book ID's that need to be followed

            if (!bookIDPattern(book.bookID))
            {
                std::cout << "\nInvalid book ID, you'll be sent back to re-input your information." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds to allow user to read information
                system("CLS");
                staff.clearInputBuffer();
                staff.registerBook(staff, name, surname); // Recurse
            }
            else
            {
                checkExistingBookID(book.bookID); // Check if there's an already existing book ID for that given ID inputted by the librarian

                if (!checkExistingBookID(book.bookID))
                {
                    std::cout << "\nBook ID already exists in the library system, you'll be sent back to re-input your information." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds to allow user to read information
                    system("CLS"); // Clear the console
                    staff.clearInputBuffer(); // Clear the input buffer
                    staff.registerBook(staff, name, surname); // Recurse
                }
                staff.clearInputBuffer();
            }


            // Add all relevant input validation mediums
            std::cout << "\nEnter the book title: ";
            std::getline(std::cin, book.bookTitle);
            staff.clearInputBuffer();


            std::cout << "\nEnter the year of book published: ";
            std::cin >> book.yearOfRelease;

            if (!yearValidator(book.yearOfRelease)) // Check if the year format is valid
            {
                std::cout << "\nInvalid year registered, you'll be sent back to re-input your information." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait three seconds
                system("CLS"); // After 3 seconds, recurse through the program to allow the individual to read the message
                registerBook(staff, name, surname); // Recurse
            }
            else
            {
                staff.clearInputBuffer();
            }

            std::cout << "\nEnter the name of the book publisher: ";
            std::getline(std::cin, book.bookPublisher);
            staff.clearInputBuffer();

            std::cout << "\nEnter the number of books that have been released: ";
            std::cin >> book.numberOfReleases;
            staff.clearInputBuffer();

            std::cout << "\nEnter the number of available books in the library: ";
            std::cin >> book.remainingBooks;
            staff.clearInputBuffer();

            std::cout << "\nHow many relevant subjects are there for this given book?: ";
            std::cin >> numOfRelevantSubjects; // Register this value
            staff.clearInputBuffer();

            for (int i = 0; i < numOfRelevantSubjects; ++i)
            {
                std::cout << "\nEnter the relevant subject " << "(" << "Subject " << numCount << "): ";
                std::cin >> subjectRegisterer; // Register user input
                book.relevantSubjects.push_back(subjectRegisterer); // Register the subject
                numCount++;
            }

            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before displaying the details
            system("CLS");

            // Output all relevant information regarding the registered book
            std::cout << "Stepwise University: Book Registered\n";
            std::cout << "Book Details\n";
            std::cout << "\nBook ID: " << book.bookID << std::endl;
            std::cout << "\nBook Title: " << book.bookTitle << std::endl;
            std::cout << "\nYear of release: " << book.yearOfRelease << std::endl;
            std::cout << "\nBook Publisher: " << book.bookPublisher << std::endl;
            std::cout << "\nNumber of worldwide books released: " << book.numberOfReleases << std::endl;
            std::cout << "\nNumber of available copies of the book in the library: " << book.remainingBooks << std::endl;

            std::cout << "\nNumber of relevant subjects: \n" << std::endl;
            int size = book.relevantSubjects.size();

            for (int i = 0; i < size; i++) // Iterate through the relevant subjects to display to user
            {
                std::cout << "* " << book.relevantSubjects[i] << "\n" << std::endl;

            }

            // Create a CSV file to store the books
            createCSV(book);

            // Redirect user back to dashboard
            std::cout << "\nYou will now be redirected to the dashboard..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(10)); // Wait five seconds
            system("CLS"); // Clear the console
            staff.librarianDashboard(staff, name, surname); // Redirect the user

        }

        void createCSV(const Book& book) // Use const reference to avoid unnecessary copying
        {
            std::ofstream file(book.bookID + ".csv"); // Create a file named after the book's ID

            // Write book details as comma-separated values
            file << book.bookID << ","
                << book.bookTitle << ","
                << book.yearOfRelease << ","
                << book.bookPublisher << ","
                << book.numberOfReleases << ","
                << book.remainingBooks << std::endl;

            file.close(); // Close the file

            std::ofstream bookCollection("RegisteredBooks.csv", std::ios::app); // Create a file/write on a file to register the book

            bookCollection << book.bookID << "," << book.bookTitle << "," << book.yearOfRelease << "," << book.bookPublisher << "," << book.numberOfReleases << "," << "\n"; // Define a structure for the collected books (the book ID and the book name)


        }

        void librarianDashboard(librarian staff, std::string name, std::string surname)
        {
            std::string userChoice;

            std::vector <std::string> dashboardOptions = { "Register a book", "Borrow a book", "Return a book", "Student registering", "Check return records", "Terminate Session" }; // Vector of string options
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
            std::cout << "Enter a corresponding value: ";
            std::cin >> userChoice;


            // Selection case as switch case will break the program since string isn't supported (average C# user)

            if (userChoice == "1")
            {
                std::cout << "\nYou will now be lead to register a book on the system" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before registering a book
                system("CLS"); // Clear the console for cleanliness
                registerBook(staff, name, surname); // Lead user to the function
            }
            else if (userChoice == "2")
            {
                std::cout << "\nBorrowing a book option has been selected. Redirecting user..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before allowing user to borrow a book
                system("CLS"); // Clear console
                staff.borrowBook(staff, name, surname);

            }
            else if (userChoice == "3")
            {
                std::cout << "\nReturning a book option has been selected. Redirecting user..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before allowing user to borrow a book
                system("CLS"); // Clear console
                staff.returnBook(staff, name, surname); // Direct the individual to the return book function
            }
            else if (userChoice == "4")
            {
                std::cout << "\nRegistering a student option has been chosen. Redirecting user..." << std::endl;
                std::string name, surname, username, password;
                staff.clearInputBuffer();
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait three seconds
                system("CLS");

                std::cout << "Stepwise University: Student Registering" << std::endl;

                std::cout << "\nEnter the student's name: ";
                std::getline(std::cin, name); // Allowing spaces in name

                std::cout << "\nEnter the student's surname: ";
                std::getline(std::cin, surname); // Allowing spaces in surname

                std::cout << "\nEnter the student's username: ";
                std::cin >> username;

                std::cout << "\nEnter the student's password: ";
                std::cin >> password;

                // Create a '.csv' file for the student
                std::ofstream file("student_" + name + "_" + surname + ".csv");

                if (file.is_open()) {
                    file << name << "," << surname << "," << username << "," << password;
                    file.close();
                    std::cout << "\nStudent successfully registered!" << std::endl;
                }
                else {
                    std::cerr << "Unable to open file for writing." << std::endl;
                }

                std::cout << "\nPress any key to return to the dashboard.\n" << std::endl;

                system("pause"); // Register user input

                std::cout << "\nRedirecting user..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds for transfer
                system("CLS"); // Clear the console
                staff.librarianDashboard(staff, name, surname); // Return to dashboard
            }
            else if (userChoice == "5")
            {
                std::cout << "\nRedirecting user..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds for transfer
                system("CLS"); // Clear the console

                std::cout << "Stepwise University: Returned Books\n" << std::endl;

                std::ifstream checkReturnRecords("ReturnRecords.csv");

                if (!checkReturnRecords) // Check if the file actually exists within the directory
                {
                    std::cerr << "\nReturn records doesn't exist, no books have been returned." << std::endl;
                }

                // Otherwise display all return information for the returned books

                std::string field;
                std::vector<std::string> fields;

                while (std::getline(checkReturnRecords, field))
                {
                    std::stringstream ss(field);

                    while (std::getline(ss, field, ','))
                    {
                        fields.push_back(field);
                    }

                    std::vector<std::string> order = { "Book ID: ", "Book Name: ", "Day/Days Overdue: ", "Amount Charged: " }; // Contains all relevant information for returned book information

                    if (fields.size() == order.size())
                    {
                        Book book;
                        book.bookID = fields[0];
                        book.bookTitle = fields[1];
                        std::string daysOverdue = fields[2];
                        std::string amountCharged = fields[3] + "p";

                        for (int i = 0; i < order.size(); ++i)
                        {
                            std::cout << order[i] << fields[i] << "\n"; // Display all return books information
                        }

                        std::cout << std::endl;

                    }
                }

                std::cout << "If that is all clear, press any key to return to the dashboard." << std::endl;

                system("\npause"); // Register user input

                std::cout << "\nRedirecting user..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before sending user to dashboard       
                system("CLS"); // Clear console

                staff.librarianDashboard(staff, name, surname); // Return
            }
            else if (userChoice == "6")
            {
                std::cout << "\nTerminating user session...." << std::endl;
                exit(1); // Terminate the console 
            }
            else
            {
                std::cout << "\nInvalid input, please try again." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait three seconds
                system("CLS"); // After 3 seconds, recurse through the program to allow the individual to read the message
                librarianDashboard(staff, name, surname); // Recurse 
            }

        }

        bool yearValidator(int y)
        {
            return (y <= 2024); // Pre-set limit for the year, in this case 2024
        }

        bool bookIDPattern(const std::string& bookID)
        {
            std::regex pattern("\\d{3}-\\d{3}-\\d{3}"); // Defining pattern
            return std::regex_match(bookID, pattern); // Return true, if there's match in design
        }

        bool checkExistingBookID(std::string bookID)
        {
            std::ifstream check(bookID + ".csv"); // Will check if the following book exists within the system, if so, error will be outputted
        
            if (!check.is_open()) // Should it not open, essentially meaning it doesn't exist, then return 'true'
            {
                return true;
            }
            else // Otherwise, false
            {
                return false;
            }
        }

        // Will retrieve the individuals role (i.e. a librarian)
        std::string getRole() const
        {
            return "librarian";
        }

};