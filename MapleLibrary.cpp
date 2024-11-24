#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>

// Admin's Password: adminPass00 //

using namespace std;

struct Member {
  string id;
  string username;
  string password;
  double balance = 0;
  double balanceToPay = 0;

  void display() const {
    cout << "Member ID: " << id
        << ", Username: " << username
        << ", Balance: $" << fixed << setprecision(2) << balance
        << ", Balance to Pay: $" << fixed << setprecision(2) << balanceToPay << endl;
  }
};

struct Book {
  string id;
  string title;
  string author;
  int copies;
  bool isAvailable;

  void display() const {
    cout << "Book ID: " << id
        << ", Title: " << title
        << ", Author: " << author
        << ", Copies: " << copies
        << ", Available: " << (isAvailable ? "Yes" : "No") << endl;
  }
};

void updateBookAvailability(Book& book) {
  book.isAvailable = book.copies > 0;
}

vector<Member> members;
vector<Book> books;
const string adminPassword = "adminPass00";

vector<string> split(const string& str, char delimiter) {
  vector<string> tokens;
  stringstream ss(str);
  string token;
  while (getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

void loadMembers() {
  members.clear(); 
  ifstream file("members.txt");
  string line;
  while (getline(file, line)) {
    vector<string> data = split(line, ',');
    if (data.size() == 5) {
      members.push_back({data[0], data[1], data[2], stod(data[3]), stod(data[4])});
    }
  }
  file.close();
}

void saveMembers() {
  ofstream file("members.txt");
  if (!file.is_open()) {
    cout << "\nError saving members.\n";
    return;
  }
  for (const auto& member : members) {
    file << member.id << "," << member.username << "," << member.password << ","
        << member.balance << "," << member.balanceToPay << endl;
  }
  file.close();
}

void loadBooks() {
  ifstream file("books.txt");
  if (!file.is_open()) {
    cout << "\nBooks file not found. Initializing empty library.\n";
    return;
  }
  string line;
  while (getline(file, line)) {
    vector<string> data = split(line, ',');
    if (data.size() == 5) {
      books.push_back({data[0], data[1], data[2], stoi(data[3]), data[4] == "1"});
    }
  }
  file.close();
}

void saveBooks() {
  ofstream file("books.txt");
  if (!file.is_open()) {
    cout << "\nError saving books.\n";
    return;
  }
  for (const auto& book : books) {
    file << book.id << "," << book.title << "," << book.author << ","
        << book.copies << "," << (book.isAvailable ? "1" : "0") << endl;
  }
  file.close();
}

Member* findMemberById(const string& id) {
  for (auto& member : members) {
    if (member.id == id) return &member;
  }
  return nullptr;
}

Book* findBookById(const string& id) {
  for (auto& book : books) {
    if (book.id == id) return &book;
  }
  return nullptr;
}

bool isValidId(const string& id) {
  return id.size() == 5 && findMemberById(id) == nullptr;
}

bool isValidPassword(const string& password) {
  return password.size() >= 8;
}

struct IssuedBook {
  string bookId;
  string memberId;
  int copies;
  string issueDate;
  string dueDate;
  double lateFee = 0; 

  void display() const {
    cout << "\nBook ID: " << bookId << ", Member ID: " << memberId
        << ", Copies: " << copies
        << ", Issue Date: " << issueDate
        << ", Due Date: " << dueDate;
    if (lateFee > 0) {
      cout << ", Late Fee: $" << fixed << setprecision(2) << lateFee;
    }
    cout << endl;
  }
};

vector<IssuedBook> issuedBooks;

void saveIssuedBooks() {
  ofstream file("issuedBooks.txt");
  if (!file.is_open()) {
    cout << "\nError saving issued books.\n";
    return;
  }
  for (const auto& issuedBook : issuedBooks) {
    file << issuedBook.bookId << "," << issuedBook.memberId << "," << issuedBook.copies << ","
        << issuedBook.issueDate << "," << issuedBook.dueDate << ","
        << fixed << setprecision(2) << issuedBook.lateFee << endl;
  }
  file.close();
}

////// Admin Menu /////
void addBooks() {
  string id, title, author;
  int copies;

  cout << "\n>>> ----- ADD BOOKS ----- <<<\n";
  cout << "\nEnter Book ID: ";
  cin >> id;

  if (findBookById(id)) {
    cout << "\nBook ID already exists.\n";
    return;
  }

  cout << "Enter Book Title: ";
  cin.ignore();
  getline(cin, title);

  cout << "Enter Author: ";
  getline(cin, author);

  cout << "Enter Number of Copies: ";
  cin >> copies;

  books.push_back({id, title, author, copies, copies > 0});
  saveBooks();
  cout << "\nBook added successfully.\n";
}

void updateBooks() {
  string bookId;
  cout << "\n>>> ----- UPDATE BOOKS ----- <<<\n";
  cout << "\nEnter Book ID to Update: ";
  cin >> bookId;

  Book* book = findBookById(bookId);
  if (!book) {
      cout << "\nBook not found.\n";
      return;
  }

  cout << "Enter New Title: ";
  cin.ignore();
  getline(cin, book->title);

  cout << "Enter New Author: ";
  getline(cin, book->author);

  cout << "Enter New Number of Copies: ";
  cin >> book->copies;

  updateBookAvailability(*book);
  saveBooks();
  cout << "\nBook updated successfully.\n";
}

void removeBooks() {
  string bookId;
  cout << "\n>>> ----- REMOVE BOOKS ----- <<<\n";
  cout << "\nEnter book's id to remove: ";
  cin >> bookId;

  Book* book = findBookById(bookId);
  if (!book) {
    cout << "\nBook with ID " << bookId << " not found.\n";
    return;
  }

  auto it = remove_if(books.begin(), books.end(), [&](const Book& b) {
    return b.id == bookId;
  });

  if (it != books.end()) {
    books.erase(it, books.end());
    saveBooks(); 
    cout << "\nBook with ID " << bookId << " removed successfully.\n";
  } else {
    cout << "\nFailed to remove the book.\n";
  }
}

void addMembers() {
  string newId, newUsername, newPassword;

  cout << "\n>>> ----- ADD MEMBERS ----- <<<\n";
  
  cout << "\nEnter new member's id (5 numbers): ";
  cin >> newId;

  if (findMemberById(newId)) {
    cout << "\nMember ID already exists.\n";
    return;
  }

  if (newId.length() < 5 || newId.length() > 5) {
    cout << "\nThe id must be 5 digit.";
    return;
  }

  cout << "Enter new member's username: ";
  cin.ignore(); 
  getline(cin, newUsername);

  cout << "Enter new member's password (minimum 8 characters): ";
  getline(cin, newPassword);
  if (newPassword.length() < 8) {
    cout << "\nPassword must be at least 8 characters long.\n";
    return;
  }

  members.push_back({newId, newUsername, newPassword});
  saveMembers(); 

  cout << "\nNew member added successfully.\n";
}

void updateMembers() {
  string memberId;
  cout << "\n>>> ----- UPDATE MEMBERS ----- <<<\n";
  cout << "\nEnter member's id to update: ";
  cin >> memberId;

  Member* member = findMemberById(memberId);
  if (!member) {
    cout << "\nMember with ID " << memberId << " not found.\n";
    return;
  }

  cout << "\nCurrent details of the member:\n";
  member->display();

  string newId, newUsername, newPassword;

  cout << "\nEnter member's new id: ";
  cin >> newId;

  if (findMemberById(newId) && newId != memberId) {
    cout << "\nNew member ID already exists.\n";
    return;
  }

  cout << "Enter member's new username: ";
  cin.ignore(); 
  getline(cin, newUsername);

  cout << "Enter member's new password: ";
  getline(cin, newPassword);

  member->id = newId;
  member->username = newUsername;
  member->password = newPassword;

  saveMembers();

  cout << "\nMember with ID " << memberId << " updated successfully.\n";
}

void removeMembers() {
  string memberId;
  cout << "\n>>> ----- REMOVE MEMBERS ----- <<<\n";
  cout << "\nEnter member's id to remove: ";
  cin >> memberId;

  Member* member = findMemberById(memberId);
  if (!member) {
    cout << "\nMember with ID " << memberId << " not found.\n";
    return;
  }

  auto it = remove_if(members.begin(), members.end(), [&](const Member& m) {
    return m.id == memberId;
  });

  if (it != members.end()) {
    members.erase(it, members.end());
    saveMembers(); 
    cout << "\nMember with ID " << memberId << " removed successfully.\n";
  } else {
    cout << "\nFailed to remove the member.\n";
  }
}

void viewAllBooks() {
  cout << "\n>>> ----- VIEW ALL BOOKS ----- <<<\n\n";
  
  if (books.empty()) {
    cout << "No books available in the library.\n";
    return;
  }

  for (const auto& book : books) {
    cout << book.id << ", " 
         << book.title << ", " 
         << book.author << ", " 
         << book.copies << " copies, "
         << (book.isAvailable ? "Available" : "Unavailable") << endl;
  }
}

void viewAllMembers() {
  cout << "\n>>> ----- VIEW ALL MEMBERS ----- <<<\n\n";

  if (members.empty()) {
    cout << "\nNo members registered.\n";
    return;
  }

  for (const auto& member : members) {
    cout << member.id << ", "
          << member.username << ", "
          << "Balance to pay: $" << fixed << setprecision(2) << member.balanceToPay << endl;
  }
}

void issueBook() {
  string bookId, memberId;
  int copies;
  string issueDate, dueDate;

  cout << "\n>>> ----- ISSUE BOOKS ----- <<<\n";
  cout << "\nBook ID to issue: ";
  cin >> bookId;

  Book* book = findBookById(bookId);
  if (!book || book->copies <= 0) {
    cout << "\nBook not available.\n";
    return;
  }

  cout << "Issue this to member ID: ";
  cin >> memberId;

  Member* member = findMemberById(memberId);
  if (!member) {
    cout << "Member not found.\n";
    return;
  }

  cout << "Number of copies: ";
  cin >> copies;

  if (copies > book->copies) {
    cout << "\nNot enough copies available.\n";
    return;
  }

  cout << "Enter Issue Date (DD-MM-YYYY): ";
  cin >> issueDate;

  cout << "Enter Due Date (DD-MM-YYYY): ";
  cin >> dueDate;

  issuedBooks.push_back({bookId, memberId, copies, issueDate, dueDate});

  book->copies -= copies;

  if (book->copies == 0) {
    book->isAvailable = false;
  }

  saveBooks();  
  saveIssuedBooks();  
  cout << "\nBook issued successfully.\n";
}

int calculateDaysLate(const string& dueDate, const string& currentDate) {

  int dueDay, dueMonth, dueYear;
  int currDay, currMonth, currYear;

  sscanf(dueDate.c_str(), "%d-%d-%d", &dueDay, &dueMonth, &dueYear);
  sscanf(currentDate.c_str(), "%d-%d-%d", &currDay, &currMonth, &currYear);

  int dueTotalDays = dueYear * 365 + dueMonth * 30 + dueDay;
  int currentTotalDays = currYear * 365 + currMonth * 30 + currDay;

  return currentTotalDays - dueTotalDays;
}

void loadIssuedBooks() {
  ifstream file("issuedBooks.txt");
  string line;
  while (getline(file, line)) {
    vector<string> data = split(line, ',');
    if (data.size() == 6) {
      issuedBooks.push_back({data[0], data[1], stoi(data[2]), data[3], data[4], stod(data[5])});
    }
  }
  file.close();
}

void saveAllMembers(const vector<Member>& members) {
  ofstream outFile("membersData.txt");
  if (!outFile) {
    cout << "\nError saving members data.\n";
    return;
  }
  for (const auto& member : members) {
    outFile << member.id << " " 
            << member.username << " "
            << member.password << " "
            << member.balance << " "
            << member.balanceToPay << "\n";
  }
  outFile.close();
}

void payLateFee(Member& member, vector<Member>& members) {
  if (member.balance >= member.balanceToPay) {
    member.balance -= member.balanceToPay; 
    cout << "\nLate fee of $" << member.balanceToPay << " paid successfully.\n";
    member.balanceToPay = 0;  

    for (auto& issuedBook : issuedBooks) {
      if (issuedBook.memberId == member.id && issuedBook.lateFee > 0) {
        issuedBook.lateFee = 0;  
      }
    }

      saveAllMembers(members);
      saveIssuedBooks();

      cout << "\nPayment and records updated.\n";
  } else {
      cout << "\nInsufficient balance to pay late fees.\n";
  }
}

////////////////////////////////////////

void trackIssuedBooks() {
  cout << "\n>>> ----- TRACK ISSUED BOOKS AND RETURN DATES ----- <<<\n";

  if (issuedBooks.empty()) {
    cout << "\nNo books have been issued.\n";
    return;
  }

  string currentDate;
  cout << "\nEnter current date (DD-MM-YYYY): ";
  cin >> currentDate;

  for (auto& issuedBook : issuedBooks) {
    Member* member = findMemberById(issuedBook.memberId);
    if (!member) {
      continue; 
    }

    if (currentDate > issuedBook.dueDate) {
      int lateDays = calculateDaysLate(issuedBook.dueDate, currentDate);

      double newLateFee = lateDays * 0.50;
      if (member->balanceToPay > 0) {
          newLateFee -= member->balanceToPay; 
          if (newLateFee < 0) newLateFee = 0; 
      }

      issuedBook.lateFee = newLateFee;

      member->balanceToPay += newLateFee;

      saveAllMembers(members);
    }

    issuedBook.display();
  }
}

/////////////// MEMBER MENU /////////////////////

void viewAvailableBooks() {
  cout << "\n>>> ----- VIEW AVAILABLE BOOKS ----- <<<\n\n";

  bool foundAvailable = false;
  for (const auto& book : books) {
    if (book.isAvailable && book.copies > 0) {
      book.display();
      foundAvailable = true;
    }
  }

  if (!foundAvailable) {
    cout << "\nNo available books in the library.\n";
  }
}

void checkBorrowedBooksAndReturnDates(const string& memberId) {
  cout << "\n>>> ----- BORROWED BOOKS AND RETURN DATES ----- <<<\n";

  bool hasRecords = false;
  for (const auto& issuedBook : issuedBooks) {
    if (issuedBook.memberId == memberId) {
      hasRecords = true;
      cout << "\nBook ID: " << issuedBook.bookId
            << "\nCopies Borrowed: " << issuedBook.copies
            << "\nIssue Date: " << issuedBook.issueDate
            << "\nDue Date: " << issuedBook.dueDate
            << "\nLate Fee: $" << fixed << setprecision(2) << issuedBook.lateFee
            << "\n";
    }
  }

  if (!hasRecords) {
      cout << "\nNo borrowed books found for your account.\n";
  }
}

void borrowBooks() {
  string bookId, memberId;
  int copies;
  string issueDate, dueDate;

  cout << "\n>>> ----- BORROW BOOKS ----- <<<\n";

  cout << "\nEnter your Member ID: ";
  cin >> memberId;

  Member* member = findMemberById(memberId);
  if (!member) {
    cout << "\nMember not found.\n";
    return;
  }

  if (member->balanceToPay > 0) {
    cout << "\nYou have a pending balance of $" << fixed << setprecision(2) 
      << member->balanceToPay << ". Please settle this before borrowing books.\n";
    return;
  }

  cout << "Enter Book ID: ";
  cin >> bookId;

  Book* book = findBookById(bookId);
  if (!book || !book->isAvailable) {
    cout << "\nBook not available for borrowing.\n";
    return;
  }

  cout << "Enter the number of copies to borrow: ";
  cin >> copies;

  if (copies <= 0 || copies > book->copies) {
      cout << "\nInvalid number of copies. Only " << book->copies << " copies are available.\n";
      return;
  }

  cout << "Enter Issue Date (DD-MM-YYYY): ";
  cin >> issueDate;

  cout << "Enter Due Date (DD-MM-YYYY): ";
  cin >> dueDate;

  book->copies -= copies;
  updateBookAvailability(*book);

  issuedBooks.push_back({bookId, memberId, copies, issueDate, dueDate});
  saveBooks();     
  saveIssuedBooks(); 

  cout << "\nBook borrowed successfully.\n";
}

void returnBook() {
  string bookId, memberId;

  cout << "\n>>> ----- RETURN BOOKS ----- <<<\n";
  cout << "\nEnter Book ID to return: ";
  cin >> bookId;

  cout << "Enter your Member ID: ";
  cin >> memberId;

  auto it = find_if(issuedBooks.begin(), issuedBooks.end(), [&](const IssuedBook& ib) {
    return ib.bookId == bookId && ib.memberId == memberId;
  });

  if (it != issuedBooks.end()) {
    IssuedBook& issuedBook = *it;

    Book* book = findBookById(bookId);
    if (book) {
      book->copies += issuedBook.copies;
      book->isAvailable = true;
      saveBooks();
    }

    issuedBooks.erase(it); 
    saveIssuedBooks();   

    cout << "\nBook returned successfully.\n";
  } else {
    cout << "\nNo record found for this book return.\n";
  }
}

///////////////////// MEMBER BALANCE //////////////////////

void memberBalanceMenu(Member& member);

void checkBalanceToPay(Member& member) {
  if (member.balanceToPay == 0) {
    cout << "\nYou have no pending balance to pay.\n";
    return;
  }

  cout << "\nBALANCE TO PAY: $" << fixed << setprecision(2) << member.balanceToPay << "\n";
  cout << "\n1. PAY\n2. EXIT\n";
  cout << "\nEnter your choice: ";
  int choice;
  cin >> choice;

  if (choice == 1) {
    cout << "\nYour current wallet balance: $" << fixed << setprecision(2) << member.balance << "\n";
    if (member.balance >= member.balanceToPay) {
      member.balance -= member.balanceToPay;
      member.balanceToPay = 0;
      cout << "\nLate fee fully paid. Thank you!\n";
      saveMembers();
    } else {
    cout << "\nInsufficient balance to pay late fees.\n";
  }
  } else if (choice == 2) {
    cout << "\nExiting balance-to-pay menu.\n";
    memberBalanceMenu(member);
  } else {
    cout << "\nInvalid choice. Please try again.\n";
  }
}

void depositBalance(Member& member) {
  double depositAmount;
  cout << "\nEnter deposit amount: $";
  cin >> depositAmount;

  if (depositAmount > 0) {
    member.balance += depositAmount;
    cout << "\nDeposit successful. Your new wallet balance: $" << fixed << setprecision(2) << member.balance << "\n";
    saveMembers();
  } else {
    cout << "\nInvalid deposit amount.\n";
  }
}

void checkWalletBalance(const Member& member) {
  cout << "\nYour current wallet balance: $" << fixed << setprecision(2) << member.balance << "\n";
}

void memberBalanceMenu(Member& member) {
  int choice;
  do {
    cout << "\n>>> ----- BALANCE MENU ----- <<<\n\n";
    cout << "1. Check balance to pay\n";
    cout << "2. Deposit funds\n";
    cout << "3. Check wallet balance\n";
    cout << "4. Exit\n";
    cout << "\nEnter your choice: ";
    cin >> choice;

    switch (choice) {
      case 1:
        checkBalanceToPay(member);
        break;
      case 2:
        depositBalance(member);
        break;
      case 3:
        checkWalletBalance(member);
        break;
      case 4:
      cout << "\nExiting balance menu. Thank you!\n";
      break;
      default:
          cout << "\nInvalid choice. Please try again.\n";
    }
  } while (choice != 4);
}

///////////////////// MENU //////////////////////////////

void adminMenu() {
  while (true) {
    cout << "\n>>> ----- ADMIN MENU ----- <<<\n\n";
    cout << "1. Add books\n2. Update books\n3. Remove books\n4. Add members\n";
    cout << "5. Update members\n6. Remove members\n7. View all books\n8. View all members\n";
    cout << "9. Track issued books and return dates\n10. Issue book\n11. Exit\n";
    cout << "\nEnter your choice: ";
    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
      addBooks();
      break;
    case 2:
      updateBooks();
      break;
    case 3:
      removeBooks();
      break;
    case 4:
      addMembers();
      break;
    case 5:
      updateMembers();
      break;
    case 6:
      removeMembers();
      break;
    case 7:
      viewAllBooks();
      break;
    case 8:
      viewAllMembers();
      break;
    case 9:
      trackIssuedBooks();
      break;
    case 10:
      issueBook();
      break;
    case 11:
      cout << "\nExiting admin menu.\n";
      return;
    default:
      cout << "\nInvalid choice. Try again.\n";
    }
  }
}

void memberMenu(const string& memberId) {
  Member* member = findMemberById(memberId);
  if (!member) {
    cout << "\nMember not found.\n";
    return;
  }

  while (true) {
    cout << "\n>>> ----- MEMBER MENU ----- <<<\n\n";
    cout << "1. View available books\n";
    cout << "2. Check borrowed books and return dates\n";
    cout << "3. Borrow books\n";
    cout << "4. Return books\n";
    cout << "5. Balance\n";
    cout << "6. Exit\n";
    cout << "\nEnter your choice: ";
    int choice;
    cin >> choice;

    switch (choice) {
      case 1:
        viewAvailableBooks();
        break;
      case 2:
        checkBorrowedBooksAndReturnDates(memberId);
        break;
      case 3:
        borrowBooks();
        break;
      case 4:
        returnBook();
        break;
      case 5:
        memberBalanceMenu(*member);
        break;
      case 6:
        cout << "\nExiting Member Menu.\n";
        saveMembers(); 
        return;
      default:
        cout << "\nInvalid choice. Please try again.\n";
    }
  }
}

/////////////// AUTHENTICATION ////////////////////

void adminAuthentication() {
  string password;
  cout << "\n>>> ----- ADMIN AUTHENTICATION ----- <<<\n";
  cout << "\nEnter admin's password: ";
  cin >> password;

  if (password == adminPassword) {
    cout << "\nAdmin authenticated successfully.\n";
    adminMenu();
  } else {
    cout << "\nInvalid password.\n";
  }
}

void mainMenu();

void memberAuthentication() {
  while (true) {
    cout << "\n>>> ----- MEMBER AUTHENTICATION ----- <<<\n";
    cout << "\n1. Log in\n2. Create new account\n3. Exit\n\nEnter your choice: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
      string id, username, password;
      cout << "\n>>> ----- LOG IN MEMBER ----- <<<\n";
      cout << "\nEnter your id: ";
      cin >> id;
      cout << "Enter username: ";
      cin >> username;
      cout << "Enter password: ";
      cin >> password;

      Member* member = findMemberById(id);
      if (member && member->username == username && member->password == password) {
        cout << "\nLogged in successfully!\n";
        member->display();
        memberMenu(member->id);
      } else {
        cout << "\nInvalid credentials. Returning to Member Authentication.\n";
      }
    } else if (choice == 2) {
      string id, username, password;
      cout << "\n>>> ----- CREATE ACCOUNT ----- <<<\n";
      while (true) {
        cout << "\nEnter your id (5-digit number): ";
        cin >> id;
        if (!isValidId(id) | id.length() > 5 | id.length() < 5) {
          cout << "\nInvalid or duplicate id. Try again.\n";
        } else {
          break;
        }
      }

      cout << "Enter your username: ";
      cin >> username;

      while (true) {
        cout << "Enter a password (minimum 8 characters): ";
        cin >> password;
        if (!isValidPassword(password)) {
          cout << "\nPassword too short. Try again.\n";
        } else {
          break;
        }
    }

    Member newMember = {id, username, password, 0.0, 0.0};
    members.push_back(newMember);

    saveMembers();

    cout << "\nAccount created successfully.\n";
    } else {
      cout << "\nExiting...";
      mainMenu();
    }
  }
}

void mainMenu() {
  loadMembers();

  while (true) {
    cout << "\n>>> -----     MAPLE LIBRARY     ----- <<<\n\n";
    cout << "Log in as:\n\n1. Admin\n2. Member\n3. Exit\n\nEnter your choice: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
      adminAuthentication();
    } else if (choice == 2) {
      memberAuthentication();
    } else if (choice == 3) {
      cout << "\nExiting system. Goodbye!\n";
      break;
    } else {
      cout << "\nInvalid choice. Try again.\n";
    }
  }
}

int main() {
  loadBooks();
  loadMembers();
  loadIssuedBooks();
  
  mainMenu();
  return 0;
}
