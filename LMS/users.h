#pragma once

const int MAX_FILES_TO_CHECK = 100; // Will be used to check for number of books borrowed, and '100' is the set limit

// Structure for a book
struct Book {
    std::string bookID;
    std::string bookTitle;
    std::string bookPublisher;
    std::string dateBorrowed;

    std::vector<std::string> relevantSubjects;

    int numberOfReleases;
    int yearOfRelease;
    int remainingBooks;
    int quantityBorrowed;
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

    void borrowBook(user individual) // This is dynamic and can be a teacher/student
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

            // Future reference: once overriding has been added, redirect user to the dashboard
        }

        system("CLS"); // Clear the console, should there be books present

        std::cout << "Stepwise University: Borrowing a Book\n";

        // Count the number of books the user has borrowed prior
        int borrowedBooksCount = 0;
        for (int x = 1; x <= MAX_FILES_TO_CHECK; ++x)
        {
            std::ifstream checkExistingFile(individual.getName() + "_" + individual.getSurname() + "_" + std::to_string(x) + ".txt");
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
                borrowBook(individual);
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

                        // Check if the book quantity is '0'
                        if (book.remainingBooks == 0)
                        {
                            std::cout << "\nThis book is currently not available, please try again." << std::endl;
                            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before allowing user to borrow a book
                            individual.clearInputBuffer(); // Clean the input buffer to prevent issues

                            system("CLS"); // Clear console
                            individual.borrowBook(individual); // Recurse, allowing for the user to pick a different book

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
                    strftime(dt, sizeof(dt), "%D-%M-%Y", localtime(&localTime));

                    // Files are created linearly, check if previous linear files exist and create a new borrowing session based on that (i.e. file structure: name + surname + number starting from 1,2,3,4....)

                    for (int x = 1; x <= MAX_FILES_TO_CHECK; ++x)
                    {
                        std::ifstream checkExistingFile(individual.getName() + "_" + individual.getSurname() + "_" + std::to_string(x) + ".txt");

                        if (!checkExistingFile)
                        {
                            // Create a borrow record session, embedding the individual's name and the ID for the book
                            std::ofstream outputFile(individual.getName() + "_" + individual.getSurname() + "_" + std::to_string(x) + ".txt", std::ios::app); // Open a text file for writing 

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
                                    outputFile << "Book ID: " << book.bookID << "\nBook Title: " << book.bookTitle << "\nYear of Release: " << book.yearOfRelease << "\nQuantity Borrowed: "
                                        << book.quantityBorrowed << "\nRemaining Books: " << book.remainingBooks << "\n";
                                }

                                outputFile << "\n";

                            }

                            outputFile << "Date Borrowed: " << dt << std::endl;

                            std::cout << "\nBook borrowed successfully!\n" << std::endl;

                            file.clear(); // Clear the previous errors with the file
                            file.seekg(0); // Reset file pointer

                            std::ifstream fileUpdate(inputBookID + ".csv"); // Open the .CSV file for reading
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
                            }

                            // Open the .CSV file again for writing
                            std::ofstream fileUpdateWrite(inputBookID + ".csv");

                            // Write the updated book information to the CSV file
                            for (const auto& book : booksUpdate)
                            {
                                fileUpdateWrite << book.bookID << "," << book.bookTitle << "," << book.yearOfRelease << ","
                                    << book.bookPublisher << "," << book.numberOfReleases << "," << book.remainingBooks << "\n";
                            }

                            break; // Remove this section of code and replace it with the code below...


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
                        borrowBook(individual);
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
                std::vector<std::string> order = { "Book ID: ", "Book Name: ", "Year of release: ", "Author: ", "Worldwide Releases: ", "Available Copies in the Library: " }; // Contains all relevant information for the book
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
                    book.remainingBooks = std::stoi(fields[5]);

                    // Print book information
                    for (int i = 0; i < order.size(); ++i)
                    {
                        std::cout << order[i] << fields[i] << "\n";
                    }
                    std::cout << std::endl;
                }

            }

            std::cout << "\nPlease enter any key to return to borrowing a book: ";
            system("\npause"); // Register user input

            std::cout << "\nRedirecting user..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before reattempt
            borrowBook(individual); // Recurse
        }
        else
        {
            std::cout << "\nInvalid input, please try again." << std::endl;
            clearInputBuffer(); // Clear the buffer
            system("CLS"); // Clear the console
            borrowBook(individual); // Recurse
        }

         


    }

    std::string getName() const { // Retrieve the individuals name
        return name;
    }

    std::string getSurname() const { // Retrieve the individuals surname 
        return surname;
    }

    void returnBook(user individual) // Should a user want to return a book, then this function will be executed
    {
        std::string userChoice, userChoice2;

        std::cout << "Stepwise University: Returning Book" << std::endl;

        // Count the number of books the user has borrowed 
        int borrowedBooksCount = 0;
        for (int x = 1; x <= MAX_FILES_TO_CHECK; ++x)
        {
            std::ifstream checkExistingFile(individual.getName() + "_" + individual.getSurname() + "_" + std::to_string(x) + ".txt");
            if (checkExistingFile.is_open()) {
                borrowedBooksCount++;
                checkExistingFile.close(); // Close the file
            }
        }

        std::cout << "\nNumber of currently borrowed books: " << borrowedBooksCount << std::endl;

        std::cout << "\nEnter the book ID of the book you'd like to return: ";
        std::cin >> userChoice;

        // Open CSV file to load the books that are available in the library
        std::ifstream file(userChoice + ".txt"); // Open the '.txt' file

        std::string line;

        if (!file.is_open()) // Check if the following file exists
        {
            std::cerr << "\nCannot find book." << std::endl;
            clearInputBuffer(); // Clear the buffer
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before reattempt
            system("CLS"); // Clear the console
            borrowBook(individual);
        }
        else
        {
            while (std::getline(file, line))
            {
                std::vector<std::string> order = { "Book ID: ", "Book Name: ", "Year of release: ", "Author: ", "Worldwide Releases: ", "Available Copies in the Library: ", "Date Borrowed: " };
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
                    book.dateBorrowed = std::stoi(fields[6]); // Additional item appended to the vector as this is to return a book


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

                    std::cout << std::endl; // Spacing
                }


                std::cout << "Would you like to return the following book? (Enter the value '1' for yes and any other key for no: ";
                std::cin >> userChoice2;

                if (userChoice2 == "1")
                {
                    // Add the logic for figuring out the fine, by reading the date borrowed, and then figuring out whether 2 weeks has passed, should this be the case, following the coursework instruction and handle as usual

                }
                else
                {
                    std::cout << "Returning user to menu..." << std::endl;

                    // Overriding user dashboard will be done to allow the indivudal to be redirected correctly...
                }



            }
        }



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

        std::string name, surname, username, password;

        // Request for relevant details of the individual
        std::cout << "Stepwise University: Student Login\n";

        std::cout << "\nInput your name: ";
        std::getline(std::cin, name);

        std::cout << "\nInput your surname: ";
        std::getline(std::cin, surname);




        std::ifstream 

    }

    void studentDashboard(student person)
    {
        std::string userChoice;

        std::vector <std::string> dashboardOptions = { "Borrow a book", "Return a book", "Terminate session" }; // Vector of string options
        // int maxSize = dashboardOptions.max_size(); Determine the maximum value of the 

        int numCount = 1;
        std::cout << "Stepwise University: Student Dashboard\n";
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
            std::cout << "\nBorrowing a book option has been selected. Redirecting user..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before allowing user to borrow a book
            system("CLS"); // Clear console
            person.borrowBook(person); // Redirect user

        }
        else if (userChoice == "2")
        {
            std::cout << "\nReturning a book has been selected. Redirecting user..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before allowing user to borrow a book
            system("CLS"); // Clear console
            person.returnBook(person); // Redirect user
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
            studentDashboard(person); // Recurse 
        }

    }

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
        std::cout << "\nInput your username: ";
        std::cin >> usernameInput;

        std::cout << "\nInput your password: ";
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
            staff.registerBook(staff); // Recurse
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
                staff.registerBook(staff); // Recurse
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
            registerBook(staff); // Recurse
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
        staff.librarianDashboard(staff); // Redirect the user

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

        bookCollection << book.bookID << "," << book.bookTitle << "," << book.yearOfRelease << "," << book.bookPublisher << "," << book.numberOfReleases << "," << book.remainingBooks << "\n"; // Define a structure for the collected books (the book ID and the book name)


    }

    void librarianDashboard(librarian staff)
    {
        std::string userChoice;

        std::vector <std::string> dashboardOptions = { "Register a book", "Borrow a book", "Return a book", "Register a student", "Terminate Session" }; // Vector of string options
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
            std::cout << "\nReturning a book option has been selected. Redirecting user..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait 3 seconds before allowing user to borrow a book
            system("CLS"); // Clear console
            staff.returnBook(staff); // Direct the individual to the return book function
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
            std::ofstream file(name + "_" + surname + ".csv");

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
            staff.librarianDashboard(staff); // Return to dashboard
        }
        else if (userChoice == "5")
        {
            std::cout << "\nTerminating user session...." << std::endl;
            exit(1); // Terminate the console 
        }
        else
        {
            std::cout << "\nInvalid input, please try again." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait three seconds
            system("CLS"); // After 3 seconds, recurse through the program to allow the individual to read the message
            librarianDashboard(staff); // Recurse 
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

protected:
    // Test data for authenticating login
    std::string name = "Khalid";
    std::string username = "Eucladian";
    std::string password = "euclade1";

};