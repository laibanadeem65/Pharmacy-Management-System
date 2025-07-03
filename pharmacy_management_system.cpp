#include <iostream>
#include <fstream>
using namespace std;

// User data (parallel arrays, hardcoded sizes)
char usernames[100][100];
char passwords[100][100];
char roles[100][100];
int user_count = 0;

// Medicine data (parallel arrays, hardcoded sizes)
int med_ids[1000];
char med_names[1000][100];
int med_qty[1000];
float med_price[1000];
char med_expiry[1000][100];
int med_count = 0;

// --- Function Declarations ---
void mainMenu();
void signup();
void login();
void adminMenu(const char* username);
void customerMenu(const char* username);
void addMedicine();
void viewMedicines();
void updateMedicineStock();
void deleteMedicine();
void searchMedicine();
void buyMedicine(const char* username);
void viewPurchaseHistory(const char* username);
void loadUsers();
void saveUser(const char* username, const char* password, const char* role);
int checkUser(const char* username, const char* password, char* role);
int userExists(const char* username);
void loadMedicines();
void saveMedicines();
int findMedicineById(int id);
int findMedicineByName(const char* name);
void recordSale(const char* username, const char* medname, int qty);
void viewAllMedicines();
void clearInput();
void copystr(char* dest, const char* src);
int strsame(const char* a, const char* b);
int strlength(const char* s);

int main() {
    loadUsers();
    loadMedicines();
    mainMenu();
    return 0;
}

void mainMenu() {
    int choice;
    while (1) {
        cout << "\n==== Pharmacy Management System ====" << endl;
        cout << "1. Login" << endl;
        cout << "2. Signup" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        clearInput();
        if (choice == 1) login();
        else if (choice == 2) signup();
        else if (choice == 3) { cout << "Goodbye!\n"; break; }
        else cout << "Invalid choice. Try again.\n";
    }
}

void signup() {
    char username[100], password[100], role[100];
    cout << "\n--- Signup ---" << endl;
    cout << "Enter username: ";
    cin.get();
    cin.getline(username, 100);
    if (userExists(username)) {
        cout << "Username already exists. Try another.\n";
        return;
    }
    cout << "Enter password: ";
    cin.getline(password, 100);
    cout << "Role (admin/customer): ";
    cin.getline(role, 100);
    if (!(strsame(role, "admin") || strsame(role, "customer"))) {
        cout << "Invalid role. Must be 'admin' or 'customer'.\n";
        return;
    }
    saveUser(username, password, role);
    cout << "Signup successful! You can now login.\n";
}

void login() {
    char username[100], password[100], role[100];
    cout << "\n--- Login ---" << endl;
    cout << "Enter username: ";
    cin.get();
    cin.getline(username, 100);
    cout << "Enter password: ";
    cin.getline(password, 100);
    if (checkUser(username, password, role)) {
        cout << "Login successful!\n";
        if (strsame(role, "admin")) adminMenu(username);
        else customerMenu(username);
    } else {
        cout << "Invalid credentials.\n";
    }
}

void adminMenu(const char* username) {
    int choice;
    while (1) {
        cout << "\n==== Admin Menu ====" << endl;
        cout << "1. Add New Medicine" << endl;
        cout << "2. View All Medicines" << endl;
        cout << "3. Update Medicine Stock" << endl;
        cout << "4. Delete Medicine" << endl;
        cout << "5. Logout" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        clearInput();
        if (choice == 1) addMedicine();
        else if (choice == 2) viewMedicines();
        else if (choice == 3) updateMedicineStock();
        else if (choice == 4) deleteMedicine();
        else if (choice == 5) break;
        else cout << "Invalid choice. Try again.\n";
    }
}

void customerMenu(const char* username) {
    int choice;
    while (1) {
        cout << "\n==== Customer Menu ====" << endl;
        cout << "1. Search Medicine" << endl;
        cout << "2. Buy Medicine" << endl;
        cout << "3. View All Available Medicines" << endl;
        cout << "4. View Purchase History" << endl;
        cout << "5. Logout" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        clearInput();
        if (choice == 1) searchMedicine();
        else if (choice == 2) buyMedicine(username);
        else if (choice == 3) viewAllMedicines();
        else if (choice == 4) viewPurchaseHistory(username);
        else if (choice == 5) break;
        else cout << "Invalid choice. Try again.\n";
    }
}

void addMedicine() {
    int id, qty;
    float price;
    char name[100], expiry[100];
    cout << "\n--- Add New Medicine ---" << endl;
    cout << "Enter ID: "; cin >> id; clearInput();
    if (findMedicineById(id) != -1) {
        cout << "Medicine with this ID already exists.\n";
        return;
    }
    cout << "Enter name: "; cin.get(); cin.getline(name, 100);
    cout << "Enter quantity: "; cin >> qty; clearInput();
    cout << "Enter price: "; cin >> price; clearInput();
    cout << "Enter expiry date: "; cin.get(); cin.getline(expiry, 100);
    med_ids[med_count] = id;
    copystr(med_names[med_count], name);
    med_qty[med_count] = qty;
    med_price[med_count] = price;
    copystr(med_expiry[med_count], expiry);
    med_count++;
    saveMedicines();
    cout << "Medicine added successfully!\n";
}

void viewMedicines() {
    cout << "\n--- All Medicines ---" << endl;
    if (med_count == 0) { cout << "No medicines found.\n"; return; }
    cout << "ID\tName\tQty\tPrice\tExpiry" << endl;
    for (int i = 0; i < med_count; ++i) {
        cout << med_ids[i] << "\t" << med_names[i] << "\t" << med_qty[i] << "\t" << med_price[i] << "\t" << med_expiry[i] << endl;
    }
}

void updateMedicineStock() {
    int id, idx, qty;
    cout << "\n--- Update Medicine Stock ---" << endl;
    cout << "Enter Medicine ID: "; cin >> id; clearInput();
    idx = findMedicineById(id);
    if (idx == -1) { cout << "Medicine not found.\n"; return; }
    cout << "Current quantity: " << med_qty[idx] << endl;
    cout << "Enter new quantity: "; cin >> qty; clearInput();
    med_qty[idx] = qty;
    saveMedicines();
    cout << "Stock updated!\n";
}

void deleteMedicine() {
    int id, idx;
    cout << "\n--- Delete Medicine ---" << endl;
    cout << "Enter Medicine ID: "; cin >> id; clearInput();
    idx = findMedicineById(id);
    if (idx == -1) { cout << "Medicine not found.\n"; return; }
    for (int i = idx; i < med_count - 1; ++i) {
        med_ids[i] = med_ids[i + 1];
        copystr(med_names[i], med_names[i + 1]);
        med_qty[i] = med_qty[i + 1];
        med_price[i] = med_price[i + 1];
        copystr(med_expiry[i], med_expiry[i + 1]);
    }
    med_count--;
    saveMedicines();
    cout << "Medicine deleted!\n";
}

void searchMedicine() {
    char name[100];
    cout << "\n--- Search Medicine ---" << endl;
    cout << "Enter medicine name: ";
    cin.get(); cin.getline(name, 100);
    int idx = findMedicineByName(name);
    if (idx == -1) { cout << "Medicine not found.\n"; return; }
    cout << "ID: " << med_ids[idx] << endl;
    cout << "Name: " << med_names[idx] << endl;
    cout << "Quantity: " << med_qty[idx] << endl;
    cout << "Price: " << med_price[idx] << endl;
    cout << "Expiry: " << med_expiry[idx] << endl;
}

void buyMedicine(const char* username) {
    int id, qty, idx;
    cout << "\n--- Buy Medicine ---" << endl;
    cout << "Enter Medicine ID: "; cin >> id; clearInput();
    idx = findMedicineById(id);
    if (idx == -1) { cout << "Medicine not found.\n"; return; }
    cout << "Available quantity: " << med_qty[idx] << endl;
    cout << "Enter quantity to buy: "; cin >> qty; clearInput();
    if (qty <= 0 || qty > med_qty[idx]) {
        cout << "Invalid quantity.\n";
        return;
    }
    med_qty[idx] -= qty;
    saveMedicines();
    recordSale(username, med_names[idx], qty);
    cout << "Purchase successful!\n";
}

void viewPurchaseHistory(const char* username) {
    ifstream fin("sales.txt");
    char line[256];
    cout << "\n--- Purchase History ---" << endl;
    int found = 0;
    while (fin.getline(line, 256)) {
        char user[100], med[100], date[100];
        int qty = 0, i = 0, j = 0, k = 0, l = 0, comma = 0;
        for (i = 0, j = 0, comma = 0; line[i] != '\0'; ++i) {
            if (line[i] == ',') { user[j] = '\0'; break; }
            user[j++] = line[i];
        }
        for (++i, k = 0; line[i] != '\0'; ++i) {
            if (line[i] == ',') { med[k] = '\0'; break; }
            med[k++] = line[i];
        }
        for (++i, l = 0; line[i] != '\0'; ++i) {
            if (line[i] == ',') { break; }
            if (line[i] >= '0' && line[i] <= '9') qty = qty * 10 + (line[i] - '0');
        }
        int m = 0;
        for (++i; line[i] != '\0' && line[i] != '\n'; ++i) {
            date[m++] = line[i];
        }
        date[m] = '\0';
        if (strsame(user, username)) {
            cout << "Medicine: " << med << ", Qty: " << qty << ", Date: " << date << endl;
            found = 1;
        }
    }
    if (!found) cout << "No purchase history found.\n";
    fin.close();
}

void viewAllMedicines() {
    viewMedicines();
}

void loadUsers() {
    ifstream fin("users.txt");
    user_count = 0;
    char line[300];
    while (fin.getline(line, 300)) {
        int i = 0, j = 0, k = 0, l = 0, comma = 0;
        for (i = 0, j = 0; line[i] != '\0'; ++i) {
            if (line[i] == ',') { usernames[user_count][j] = '\0'; break; }
            usernames[user_count][j++] = line[i];
        }
        for (++i, k = 0; line[i] != '\0'; ++i) {
            if (line[i] == ',') { passwords[user_count][k] = '\0'; break; }
            passwords[user_count][k++] = line[i];
        }
        for (++i, l = 0; line[i] != '\0' && line[i] != '\n'; ++i) {
            roles[user_count][l++] = line[i];
        }
        roles[user_count][l] = '\0';
        user_count++;
    }
    fin.close();
}

void saveUser(const char* username, const char* password, const char* role) {
    ofstream fout("users.txt", ios::app);
    fout << username << ',' << password << ',' << role << '\n';
    fout.close();
    copystr(usernames[user_count], username);
    copystr(passwords[user_count], password);
    copystr(roles[user_count], role);
    user_count++;
}

int checkUser(const char* username, const char* password, char* role) {
    for (int i = 0; i < user_count; ++i) {
        if (strsame(usernames[i], username) && strsame(passwords[i], password)) {
            copystr(role, roles[i]);
            return 1;
        }
    }
    return 0;
}

int userExists(const char* username) {
    for (int i = 0; i < user_count; ++i) {
        if (strsame(usernames[i], username)) return 1;
    }
    return 0;
}

void loadMedicines() {
    ifstream fin("medicines.txt");
    med_count = 0;
    char line[400];
    while (fin.getline(line, 400)) {
        int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0, comma = 0;
        int id = 0;
        for (i = 0; line[i] != '\0'; ++i) {
            if (line[i] == '\n' || line[i] == ',') break;
            if (line[i] >= '0' && line[i] <= '9') id = id * 10 + (line[i] - '0');
        }
        med_ids[med_count] = id;
        for (++i, j = 0; line[i] != '\0'; ++i) {
            if (line[i] == ',') { med_names[med_count][j] = '\0'; break; }
            med_names[med_count][j++] = line[i];
        }
        int qty = 0;
        for (++i, k = 0; line[i] != '\0'; ++i) {
            if (line[i] == ',') break;
            if (line[i] >= '0' && line[i] <= '9') qty = qty * 10 + (line[i] - '0');
        }
        med_qty[med_count] = qty;
        float price = 0, frac = 0.1f;
        int seen_dot = 0;
        for (++i, l = 0; line[i] != '\0'; ++i) {
            if (line[i] == ',') break;
            if (line[i] == '.') { seen_dot = 1; continue; }
            if (line[i] >= '0' && line[i] <= '9') {
                if (!seen_dot) price = price * 10 + (line[i] - '0');
                else { price = price + (line[i] - '0') * frac; frac /= 10; }
            }
        }
        med_price[med_count] = price;
        for (++i, m = 0; line[i] != '\0' && line[i] != '\n'; ++i) {
            med_expiry[med_count][m++] = line[i];
        }
        med_expiry[med_count][m] = '\0';
        med_count++;
    }
    fin.close();
}

void saveMedicines() {
    ofstream fout("medicines.txt");
    for (int i = 0; i < med_count; ++i) {
        fout << med_ids[i] << ',' << med_names[i] << ',' << med_qty[i] << ',' << med_price[i] << ',' << med_expiry[i] << '\n';
    }
    fout.close();
}

int findMedicineById(int id) {
    for (int i = 0; i < med_count; ++i) if (med_ids[i] == id) return i;
    return -1;
}

int findMedicineByName(const char* name) {
    for (int i = 0; i < med_count; ++i) if (strsame(med_names[i], name)) return i;
    return -1;
}

void recordSale(const char* username, const char* medname, int qty) {
    ofstream fout("sales.txt", ios::app);
    fout << username << ',' << medname << ',' << qty << ",today\n";
    fout.close();
}

void clearInput() {
    cin.ignore(10000, '\n');
}

void copystr(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') { dest[i] = src[i]; i++; }
    dest[i] = '\0';
}

int strsame(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

int strlength(const char* s) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}
