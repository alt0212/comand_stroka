
#include <iostream>
#include <vector>
#include <sstream>
#include <filesystem>
#include <fstream>

using namespace std;

#define RESET "\033[0m";    //0BGR
#define BLACK "\33[30m"     //0000
#define RED "\33[31m"       //0001
#define GREEN "\33[32m"     //0010
#define YELLOW "\33[33m"    //0011
#define BLUE "\33[34m"      //0100
#define PURPLE "\33[35m"    //0101
#define CYAN "\33[36m"      //0110
#define WHITE "\33[37m"     //0111
#define BOLD "\33[1m"

#define BG_BLACK "\33[40m"    //0000
#define BG_RED "\33[41m"    //0001
#define BG_GREEN "\33[42m"  //0010
#define BG_BLUE "\33[44m"    //0100

std::vector<std::string> splitCommad(const std::string& command)
{
    std::vector<std::string> words;
    std::string word;
    std::istringstream iss(command);

    while (iss >> word)
    {
        words.push_back(word);
    }

    return words;
}

enum Command
{
    HELP, PWD, EXIT, UNKNOWN, CD, MD, MF, READF, DEL, SEEDIR
};

Command getCommandFromString(const std::string& cmdStr)
{
    if (cmdStr == "help") return Command::HELP;
    if (cmdStr == "pwd") return Command::PWD;
    if (cmdStr == "exit") return Command::EXIT;
    if (cmdStr == "cd") return Command::CD;
    if (cmdStr == "md") return Command::MD;
    if (cmdStr == "del") return Command::DEL;
    if (cmdStr == "mf") return Command::MF;
    if (cmdStr == "readf") return Command::READF;
    if (cmdStr == "seedir") return Command::SEEDIR;

    return Command::UNKNOWN;
}

void PrintCommandList()
{
    cout << "Список доступных команд:\n";
    cout << "help - Вызов справки\n";
    cout << "exit - Завершение работы\n";
    cout << "pwd - Показать текущую папку\n";
    cout << "cd [имя] - Переместиться в другой каталог\n";
    cout << "md [имя] - Создать папку\n";
    cout << "del [имя]/[имя.расширение] - Удалить папку/файл\n";
    cout << "mf [имя.расширение] - Создать файл\n";
    cout << "readf [имя.] - Прочитать файл\n";
    cout << "seedir - Показать данные текущей директории\n";
}

namespace fs = std::filesystem;

int main()
{
    system("chcp 1251");
    fs::path currentPath = fs::current_path();
    string input;

        cout << BOLD << WHITE
            << "=================\n"
            << "ФАЙЛОВЫЙ МЕНЕДЖЕР\n"
            << "---ЗАПУЩЕН---\n"
            << "=================\n\n"
            << RESET;

    while (true) 
    {
        cout << "\n" << ":";
        getline(cin, input);

        if (input.empty()) continue;
        vector<string> cmd = splitCommad(input);

        Command cmdID = getCommandFromString(cmd[0]);
        switch (cmdID)
        {
        case Command::PWD:
            cout << CYAN << currentPath.string() << "\n" << RESET;
            break;
        case Command::EXIT:
            return 0;
        case Command::HELP:
            PrintCommandList();
            break;
        case Command::CD:
            if (cmd.size() < 2)
            {
                cout << BOLD << RED << "Укажите название папки" << RESET;
            }
            else if (cmd.size() > 2)
            {
                cout << BOLD << RED << "Неверное название папки" << RESET;
            }
            else
            {
                if (cmd[1] == "..")
                {
                    currentPath = fs::current_path().root_directory();
                }
                else
                {
                    fs::path newPath = currentPath / cmd[1];
                    if (fs::exists(newPath) && fs::is_directory(newPath))
                        currentPath = newPath;
                    else
                        cout << BOLD << RED << "Имя папка " << cmd[1] << " не найдено" << RESET;
                }
                
            }
            break;
        case Command::SEEDIR:
            try
            {
                for (const auto entry : fs::directory_iterator(currentPath))
                {
                    if (entry.is_directory())
                        cout << BOLD << GREEN << "[ПАПКА] ";
                    else
                        cout << GREEN << "[ФАЙЛ] ";
                    cout << entry.path().filename().string() << "\n" << RESET;
                }
            }
            catch (...)
            {
                cout << BOLD << RED << "Ошибка чтения папки\n" << RESET;
            }
            break;
        case Command::DEL:
            if (cmd.size() < 2)
            {
                cout << BOLD << RED << "Укажите название папки/файла" << RESET;
            }
            else if (cmd.size() > 2)
            {
                cout << BOLD << RED << "Неверное название папки/файла" << RESET;
            }
            else
            {
                fs::remove_all(cmd[1]);
                cout << BOLD << GREEN << "Папка/Файл " << cmd[1] << " со всем содержимом успешно удалина" << RESET;
            }
            break;
        case Command::READF:
            if (cmd.size() < 2)
            {
                cout << BOLD << RED << "Укажите название файла" << RESET;
            }
            else if (cmd.size() > 2)
            {
                cout << BOLD << RED << "Неверное название файла" << RESET;
            }
            else
            {
                ifstream fout(cmd[1]);
                if (fout.is_open())
                {
                    string line;
                    while (getline(fout, line))
                    {
                        cout << BOLD << WHITE << line << "\t";
                    }
                    fout.close();
                }
                else
                    std::cout << "Не удалось открыть " << cmd[1] << ".\n";
            }
            break;
        case Command::MF:
            if (cmd.size() < 2)
            {
                cout << BOLD << RED << "Укажите название файла" << RESET;
            }
            else if (cmd.size() > 2)
            {
                cout << BOLD << RED << "Неверное название файла" << RESET;
            }
            else
            {
                ofstream file(cmd[1]);
                cout << BOLD << GREEN << "Файл " << cmd[1] << " успешно создан" << RESET;
            }
            break;
        case Command::MD:
            if (cmd.size() < 2)
            {
                cout << BOLD << RED << "Укажите название папки" << RESET;
            } 
            else if (cmd.size() > 2)
            {
                cout << BOLD << RED << "Неверное название папки" << RESET;
            }
            else
            {
                fs::create_directory(cmd[1]);
                cout << BOLD << GREEN << "Папка " << cmd[1] << " успешно созадана" << RESET;
            }
            break;
        default:
            cout << BOLD << RED << "Команда " << cmd[0] << " не известна\n" << RESET;
            cout << BOLD << WHITE << "Введите help для вызова списка команд\n" << RESET;
            break;
        }
    }

    //cout << BOLD << YELLOW << "Hello World!\n" << RESET;
    //cout << BOLD << PURPLE << "Hello World!\n" << RESET;
    //cout << BOLD << CYAN << "Hello World!\n" << RESET;
    //cout << BOLD << WHITE << "Hello World!\n" << RESET;
    //cout << BOLD << BG_BLUE << BOLD << RED << "Hello World!\n" << RESET;
}


