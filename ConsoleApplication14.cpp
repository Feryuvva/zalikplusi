#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct Phone {
    string brand;
    string model;
    double price;

    void saveToFile(ofstream& outFile) const {
        size_t brandSize = brand.size();
        size_t modelSize = model.size();
        outFile.write(reinterpret_cast<const char*>(&brandSize), sizeof(brandSize));
        outFile.write(brand.c_str(), brandSize);
        outFile.write(reinterpret_cast<const char*>(&modelSize), sizeof(modelSize));
        outFile.write(model.c_str(), modelSize);
        outFile.write(reinterpret_cast<const char*>(&price), sizeof(price));
    }

    void loadFromFile(ifstream& inFile) {
        size_t brandSize, modelSize;
        inFile.read(reinterpret_cast<char*>(&brandSize), sizeof(brandSize));
        brand.resize(brandSize);
        inFile.read(&brand[0], brandSize);
        inFile.read(reinterpret_cast<char*>(&modelSize), sizeof(modelSize));
        model.resize(modelSize);
        inFile.read(&model[0], modelSize);
        inFile.read(reinterpret_cast<char*>(&price), sizeof(price));
    }
};

class PhoneStore {
private:
    vector<Phone> inventory;
    const string filename = "phonestore.dat";

public:
    PhoneStore() {
        loadFromFile();
    }

    ~PhoneStore() {
        saveToFile();
    }

    void addPhone(const string& brand, const string& model, double price) {
        inventory.push_back({ brand, model, price });
        cout << "Телефон добавлен успешно.\n";
    }

    void listPhones() const {
        if (inventory.empty()) {
            cout << "Магазин пуст.\n";
            return;
        }

        for (size_t i = 0; i < inventory.size(); ++i) {
            cout << i + 1 << ". " << inventory[i].brand << " " << inventory[i].model << " - " << inventory[i].price << " грн.\n";
        }
    }

    void buyPhone(int index) {
        if (index < 1 || index > inventory.size()) {
            cout << "Неверный индекс.\n";
            return;
        }

        cout << "Вы купили " << inventory[index - 1].brand << " " << inventory[index - 1].model << " за " << inventory[index - 1].price << " грн.\n";
        inventory.erase(inventory.begin() + index - 1);
    }

    void adminMenu() {
        int choice;
        do {
            cout << "\n--- Админ Меню ---\n";
            cout << "1. Добавить телефон\n";
            cout << "2. Показать все телефоны\n";
            cout << "3. Выйти\n";
            cout << "Выберите опцию: ";
            cin >> choice;

            switch (choice) {
            case 1: {
                string brand, model;
                double price;
                cout << "Введите бренд: ";
                cin >> brand;
                cout << "Введите модель: ";
                cin >> model;
                cout << "Введите цену: ";
                cin >> price;
                addPhone(brand, model, price);
                break;
            }
            case 2:
                listPhones();
                break;
            case 3:
                cout << "Выход из режима админа.\n";
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
            }
        } while (choice != 3);
    }

    void customerMenu() {
        int choice;
        do {
            cout << "\n--- Меню Покупателя ---\n";
            cout << "1. Показать все телефоны\n";
            cout << "2. Купить телефон\n";
            cout << "3. Выйти\n";
            cout << "Выберите опцию: ";
            cin >> choice;

            switch (choice) {
            case 1:
                listPhones();
                break;
            case 2: {
                int index;
                cout << "Введите номер телефона, который хотите купить: ";
                cin >> index;
                buyPhone(index);
                break;
            }
            case 3:
                cout << "Выход из режима покупателя.\n";
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
            }
        } while (choice != 3);
    }

    void saveToFile() const {
        ofstream outFile(filename, ios::binary);
        if (!outFile) {
            cerr << "Ошибка открытия файла для сохранения.\n";
            return;
        }

        size_t inventorySize = inventory.size();
        outFile.write(reinterpret_cast<const char*>(&inventorySize), sizeof(inventorySize));

        for (const auto& phone : inventory) {
            phone.saveToFile(outFile);
        }
    }

    void loadFromFile() {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cerr << "Ошибка открытия файла для загрузки.\n";
            return;
        }

        size_t inventorySize;
        inFile.read(reinterpret_cast<char*>(&inventorySize), sizeof(inventorySize));
        inventory.resize(inventorySize);

        for (auto& phone : inventory) {
            phone.loadFromFile(inFile);
        }
    }
};

int main() {
    setlocale(0, "");
    PhoneStore store;
    int roleChoice;

    do {
        cout << "\n--- Магазин Телефонов ---\n";
        cout << "1. Админ\n";
        cout << "2. Покупатель\n";
        cout << "3. Выйти\n";
        cout << "Выберите свою роль: ";
        cin >> roleChoice;

        switch (roleChoice) {
        case 1:
            store.adminMenu();
            break;
        case 2:
            store.customerMenu();
            break;
        case 3:
            cout << "Выход из магазина. Спасибо!\n";
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while (roleChoice != 3);

    return 0;
}
