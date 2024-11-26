# Maple Library - Library Management System

Maple Library is a C++ terminal-based system project for a school project. This project aims to manage books and members within a library, handling operations such as book issuing, returning, tracking late fees, and managing member balances.

## Key Features

- **Admin Authentication**: Admins can log in using a secure password (`adminPass00`) to manage the library.

- **Member Authentication**: Members can log in with their username and password to access the library services or create new accounts.

- **Book Management**:
  - Add new books to the library.
  - Update book details such as title, author, and availability.
  - Remove books from the library collection.

- **Member Management**:
  - Add, update, and remove members.
  - View a list of all members.

- **Track Issued Books**: Admins can track books issued to members, including the due date and any applicable late fees.

- **Borrowing and Returning Books**:
  - Members can borrow books if they have no outstanding balance to pay.
  - Members can return books and, if returned late, pay a late fee.
  - Books that are returned late will have a fee added to the member’s balance.

- **Balance Management**:
  - Members can check their balance to pay (including late fees).
  - Members can make deposits to add funds to their balance.
  - Once a late fee is paid, the member's balance to pay is updated and reflected in the system.
  - Admins are notified when late fees are paid and the late fees are updated in the system.

- **Availability of Books**:
  - The system keeps track of the number of copies available for each book. If a book is borrowed, its availability is updated accordingly.
