A Pharmacy Management System designed to help administrators and customers manage pharmacy operations efficiently. The system enables admins to add, update, view, and delete medicines, while customers can search, buy, and view purchase history.
The Single Responsibility Principle was followed
mainMenu() → entry point, calls login or signup.
Code Organization:
login(), signup() → authentication & registration.

adminMenu() → admin options.

customerMenu() → customer options.

File handling done by loadUsers(), saveUser(), loadMedicines(), saveMedicines().

String operations handled by custom helper functions.
The project is a practical demonstration of basic file handling, arrays, modular design, and adherence to SRP, showcasing foundational programming skills.
