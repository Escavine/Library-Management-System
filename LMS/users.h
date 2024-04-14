#pragma once

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

        std::cout << "Stepwise University: Borrowing a Book\n";
        std::cout << "Current Books" << std::endl;



        std::cout << "\nEnter the Book ID for the book you'd like to borrow (FORMAT: xxx-xxx-xxx): ";
        std::cin >> userInput; // Register user input


        // std::string embeddingCSV = userInput + ".csv"; // Used as comparision marker
        // std::cout << "\n" << embeddingCSV << std::endl; // Debugging

        // Open CSV file to load the books that are available in the library
        std::ifstream file(userInput + ".csv"); // Open the .CSV file, which contains the given book based on the book ID

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
            int confirmUserBorrow;

            std::cout << "\nFound book!\n" << std::endl;

            // Logic for displaying book information

            std::string line; // string for iterating file

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

            std::cout << "\nWould you like to borrow this book? (1 for 'yes' and any other key for 'no'): ";
            std::cin >> confirmUserBorrow;


            if (confirmUserBorrow == 1)
            {
                file.clear(); // Clear the previous errors with the file
                file.seekg(0); // Reset file pointer to the beginnning


                time_t localTime; // Retrieve local time
                time(&localTime);

                char dt[100]; // Store the following information from 'strftime' into 'dt'

                // Format the date and time into string format using strftime
                strftime(dt, sizeof(dt), "%D-%M-%Y %H:%M-%S", localtime(&localTime));


                // Create a borrow record session, embedding the individauls name and the ID for the book
                std::ofstream outputFile("BorrowingBook.txt"); // Open a text file for writing (TESTING)

                std::string line;
                std::vector<std::string> fields; // Defining fields for the CSV 
                std::vector<Book> books; // Vector for updating the remaining book value with the 'book' structure as the data typ


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

                    if (fields.size() >= 5)
                    {
                        Book book;
                        book.bookID = fields[0];
                        book.bookTitle = fields[1];
                        book.yearOfRelease = std::stoi(fields[2]);
                        book.quantityBorrowed = std::stoi(fields[3]);
                        book.dateBorrowed = fields[4];
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
                            << book.quantityBorrowed << "," << book.dateBorrowed << "," << book.remainingBooks << "\n";
                    }

                    outputFile << "\n";

                }

                outputFile << "\nDate Borrowed: " << dt << std::endl;

                std::cout << "\nBook borrowed successfully!" << std::endl;


            }
            else
            {
                std::string userChoice;
                std::cout << "Would you like to borrow another book or return to the dashboard? (1 for 'to borrow' and any other key for the dashboard):";

                if (userChoice == "1")
                {
                    std::cout << "Redirecting user to borrow another book..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    system("CLS");
                    clearInputBuffer();
                    borrowBook(individual);
                }
                else
                {
                    std::cout << "Redirecting user to the dashboard..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    system("CLS");
                    clearInputBuffer();

                    // Check if the user is a 'librarian' or 'student'



                }

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
        std::cout << "\nInput your username: ";
        std::cin >> usernameInput;

        std::cout << "\nInput your password: ";
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
        std::cout << "Welcome " << person.username << std::endl;
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


    // Will retrieve the individuals role (i.e. a librarian)
    std::string getRole() const
    {
        return "student";
    }

private:
    // Test data for authenticating login
    std::string username = "Eucladian";
    std::string password = "euclade1";

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

        bool checkPattern = bookIDPattern(book.bookID);

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
        std::cout << "\nEnter the book title: ";
        std::getline(std::cin, book.bookTitle);
        staff.clearInputBuffer();


        std::cout << "\nEnter the year of book published: ";
        std::cin >> book.yearOfRelease;
        staff.clearInputBuffer();

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



        }
        else if (userChoice == "4")
        {
            // int remainingChances = 3;
            // std::cout << "Logging user out of session." << std::endl;


            // Future reference: Reference the login function as a header file, that way it can be called whenever without an issue

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