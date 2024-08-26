#include <iostream>
#include <windows.h>
#include <fstream> //  для работы с файлами
#include <chrono> //  для работы с временем
#include <sstream> //  для работы ссо строками
#include <vector>  //  контейнеры для хранения строк
#include <unordered_map> //  контейнеры для групп
#include <map> //  контейнеры для групп
#include <algorithm> //  понадобится для сортировки

#ifdef max //макрос будет нужен для работы встр. фун-ции max() вместе с библиотекой windows.h
#undef max
#endif

using namespace std; //про-во имен


class FileOperation {
private:
    string File_name; //название файла
    vector<vector<string>> lines; //вектор из вектора строк будет содержать данные файла
public:
    FileOperation() {}
    FileOperation(string Filename) { //конструктор где аргумент только название файла
        File_name = Filename;
        Save_in_vector(); //сохраняем данные из файла в наш контейнер
    }

    //ф-ция будет сохранять данные из файла в вектор
    void Save_in_vector() {
        ifstream open_file(File_name); //открываем файл
        if (!open_file) { // Проверяем, удалось ли открыть файл
            cerr << "Не удалось открыть файл." << endl;
            return; // Завершаем программу с ошибкой если файл не открылся
        }
        string line; //для считывания файла построчно
        vector<string> words;//вектора будем добавлять в основной вектор 
        string word;//для считывания слов из строки по одному
        while (getline(open_file, line)) { //файл считывается посторочно
            istringstream iss(line);
            while (iss >> word) { //считывание слов со строк
                words.push_back(word); //добавляем в вектор слова
            }
            lines.push_back(words); // добавляем в основной вектор вектора состоящие из слов
            words.clear(); //очищаем вектор слов, для нового заполения
        }
        open_file.close(); //закрываем файл
    }

    //ф-ция вывода файла. (на самом деле вывода вектора, куда мы скопировали все данные)
    void print() {
        cout <<"\n----Содержимое файла: " << endl;
        for (auto line : lines) {
            for (auto word : line) {
                cout << word << " ";
            }
            cout << endl;
        }
    }

    //перегруженная ф-ция вывода простого вектора. понадобится позже в программе
    void print(vector<string> vec) {
        for (auto word : vec) {
            cout << word << " ";
        }
    }

    //ф-ция добавления объекта
    void add() {
        vector<string> words; //вектор будем добавлять в основной вектор после заполнения
        string word; //этим будем заполнять вектор
        float f; //для проверки ввода координаты
        char c; //для проверки ввода координаты

        cout << "\n----Добавление нового объекта: \n";
        cout << "Введите наименование объекта: ";
        cin >> word; //ввод названия. ограничений нет
        word[0] = toupper(word[0]); //сразу меняем первую букву на большую
        if (word[0] == 'я') //метод выше не работает для буквы я. можно считать эту строчку костылем
            word[0] = 'Я';
        if (word[0] == 'Ё') //группировка не будет определять букву Ё. поэтому тоже выкрутился как смог
            word[0] = 'Е';
        words.push_back(word); //добавляем в вектор название

        //вводим координату Х
        while (true) {
            cout << "Введите координату Х объекта: ";
            cin >> word;
            istringstream iss(word); 
            if ((iss >> f) && !(iss >> c)) { //проверка на ввод. пропустит только формат float
                break;
            }
            else {
                cout << "Ошибка: введено недопустимое значение!" << endl;
            }
        }
        words.push_back(word); //добавляем в вектор координату Х

        //вводим координату У, все тоже самое что для Х
        while (true) {
            cout << "Введите координату Y объекта: ";
            cin >> word;
            istringstream iss(word);
            if ((iss >> f) && !(iss >> c)) {
                break;
            }
            else {
                cout << "Ошибка: введено недопустимое значение!" << endl;
            }
        }
        words.push_back(word);

        //для типа объекту все теже правила что и для названия
        cout << "Введите тип объекта: ";
        cin >> word;
        word[0] = toupper(word[0]);
        if (word[0] == 'я')
            word[0] = 'Я';
        if (word[0] == 'Ё')
            word[0] = 'Е';
        words.push_back(word);

        //определение времени создания объекта
        auto now = chrono::system_clock::now(); //объявление переменной, присваиваем значение текущего времени
        auto seconds = chrono::duration_cast<chrono::seconds>(now.time_since_epoch()).count(); // переводим в секунды
        auto milliseconds = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count() % 1000; //находим остаток, т.е. милисекунды
        ostringstream oss;
        oss << seconds << "." << (milliseconds < 100 ? "0" : "") << (milliseconds < 10 ? "0" : "") << milliseconds; //записываем время в виде секунд и милисекунд в переменную
        words.push_back(oss.str()); //добавляем в вектор значение текущего времени переведенное в строку


        lines.push_back(words); //вектор words готов, добавляем его в осной вектор
    }


    //ф-ция группировки по расстоянию
    void Group_by_distance() {
        long float x, y, distance; //переменные для вычисления и записи координат и расстояния
        map<long float, vector<vector<string>>> grouped_by_distance; // контейнер map для создания групп
        sort(lines.begin(), lines.end(), //метод sort применен к основному вектору
            [](vector<string> a, vector<string> b) { //лямда ф-ция будет проверять результат сравнения расстояния 2 объектов
                return  sqrt(pow(stof(a[1]) - 0, 2) + pow(stof(a[2]) - 0, 2)) < sqrt(pow(stof(b[1]) - 0, 2) + pow(stof(b[2]) - 0, 2));
                // т.1 - коодинаты x = 0 и y = 0 как центр отсчета
                // т.2 - координаты х = stof - перевод из строки в float a[1] элемент вектора с индексом 1 где и хранится координата х и b[1] ссотв. для У.
                // sqrt(pow(stof(a[1]) - 0, 2) + pow(stof(a[2]) - 0, 2)) - формула расстояния одного объекта
                // sqrt(pow(stof(a[1]) - 0, 2) + pow(stof(a[2]) - 0, 2)) - формула расстояния другого объекта
                // расстояния проверяются пообъектно в векторе и сортируются в порядке убывания
            });
        cout << "\n----Результат группировки по расстоянию: " << endl;

        for (auto words : lines) { // проходим по вектору
            x = stof(words[1]); // координата х - элемент с индексом 1 в строке вектора, переводим ее в float
            y = stof(words[2]); // координата y - элемент с индексом 2 в строке вектора, переводим ее в float
            distance = sqrt(pow(x - 0, 2) + pow(y - 0, 2)); // находим расстояние от центра (центр взят 0,0)
            if (distance < 101) { //если дистанция от 0 до 101 не включительно
                grouped_by_distance[100].push_back(words); //тут ниже разделяем элементы по группам (контейнер map)
            }
            else if (distance > 100 && distance < 1001) {
                grouped_by_distance[1000].push_back(words);
            }
            else if (distance > 1000 && distance < 10001) {
                grouped_by_distance[10000].push_back(words);
            }
            else if (distance > 10000) {
                grouped_by_distance[9999].push_back(words);
            }
        }

        for (auto group : grouped_by_distance) { //проходим по контейнеру map где ключ это лимит расстояния а значение это вектор объектов
            //если ключ такой-то то выводим то-то
            if (group.first == 9999) { //тут вывод немного отличается поэтому уловие уникально
                cout << "Группа с расстоянием от '" << group.first + 1 << "':" << endl;
            }
            else 
                cout << "Группа с расстоянием до '" << group.first << "':" << endl;
            for (auto item : group.second) { // проходим по значениям контейнера map. оно является вектором со строками
                print(item); //тут нам и нужен тот самый принт для вектора
                cout << "| Расстояние: " << sqrt(pow(stof(item[1]) - 0, 2) + pow(stof(item[2]) - 0, 2)) << endl; //для удобства выводим также и само расстояние
            }
            cout << endl;
        }
        //очищаем наш главный вектор и записываем в него отсортировнный результат функции
        lines.clear();
        for (auto group : grouped_by_distance) {
            for (auto item : group.second) {
                lines.push_back(item);
            }
        }
    }

    //ф-ция группировки по наименованию
    void Group_by_name() {
        char firstLetter; //переменная для записи первого символа
        map<char, vector<vector<string>>> grouped_by_name; // конейнер для группировки
        sort(lines.begin(), lines.end(),//сортировка где условия сортировки это лямбда ф-ция которая сравнивает 1 элементы векторов (названия)
            [](const vector<string>& a, const vector<string>& b) {
                return  a[0] < b[0];
            });
        cout << "\n----Результат группировки по имени: " << endl;

        for (auto line : lines) {
            firstLetter = line[0][0];//записываем в переменную первый символ названия
            if (firstLetter >= 'А' && firstLetter <= 'Я') { //если переменная между буквами А и Я то группируем так
                                                            //ключ - группа с названием той буквыб с которой начивается название
                                                            //значение - строка с информацией об объекте
                grouped_by_name[firstLetter].push_back(line);
            }
            else //если символ отличается от букв от А до Я то группируем строку в группу с названием #
                grouped_by_name['#'].push_back(line);
        }

        for (auto group : grouped_by_name) {//проходим по контейнеру с группами
            cout << "Группа '" << group.first << "':" << std::endl; // выводим название группы (ключ)
            for (auto item : group.second) { // выводим значения которые находятся в группах (вектора со строками)
                print(item);
                cout << endl;
            }
            cout << endl;
        }
        //очищаем наш главный вектор и записываем в него отсортировнный результат функции
        lines.clear();
        for (auto group : grouped_by_name) {
            for (auto item : group.second) {
                lines.push_back(item);
            }
        }
    }

    //ф-ция группировки по времени создания
    void Group_by_date() {
        map<string, vector<vector<string>>> grouped_by_date;//контецнер для группировки
        sort(lines.begin(), lines.end(),//сортировка где условия сортировки это лямбда ф-ция которая сравнивает 4 элементы векторов (кол-во секунд с 01.01.1970)
            [](const vector<string>& a, const vector<string>& b) {
                return stod(a[4]) < stod(b[4]);
            });
        time_t object_timestamp;//в переменной будет хранится само число со временем (кол-во секунд)
        tm date_from_object_timestamp; // для преобразования object_timestamp в структуру tm далее
        tm start_of_week_object, start_of_week_today; // переменные для определения текущей недели

        // Получаем текущее время
        auto now = chrono::system_clock::now();
        time_t now_c = chrono::system_clock::to_time_t(now);
        tm current_date;
        localtime_s(&current_date, &now_c);

        // Вчерашняя дата
        tm yesterday = current_date;
        yesterday.tm_mday -= 1;
        mktime(&yesterday); // Обновляем структуру tm

        cout << "\n----Результат группировки по дате создания: " << endl;
        for (auto line : lines) {
            object_timestamp = stod(line[4]);//записываем в переменную значение из элемента с 4 индексом и переводим его в double
            localtime_s(&date_from_object_timestamp, &object_timestamp);

            start_of_week_object = date_from_object_timestamp;
            start_of_week_object.tm_mday -= (date_from_object_timestamp.tm_wday + 6) % 7; // Пн = 0, Вс = 6
            start_of_week_today = current_date;
            start_of_week_today.tm_mday -= (current_date.tm_wday + 6) % 7;

            if      (date_from_object_timestamp.tm_year == current_date.tm_year && //если совпадает год у проверяемой даты (date_from_timestamp) и текущей даты (current_date)
                     date_from_object_timestamp.tm_mon == current_date.tm_mon && //и если совпадает месяц
                     date_from_object_timestamp.tm_mday == current_date.tm_mday) {//и если совпадает день
                grouped_by_date["сегодня"].push_back(line); // то записываем объект в соотв. группу (присваиваем ключу значение)
            }
            else if (date_from_object_timestamp.tm_year == yesterday.tm_year && // если совпадает год у проверяемой даты (date_from_timestamp) и вчера (yesterday)
                     date_from_object_timestamp.tm_mon == yesterday.tm_mon && //и если совпадает месяц
                     date_from_object_timestamp.tm_mday == yesterday.tm_mday) { //и если совпадает день
                grouped_by_date["вчера"].push_back(line);
            }
            else if (start_of_week_object.tm_year == start_of_week_today.tm_year &&//если совпадают года у текущей недели start_of_week_today и недели объекта start_of_week_object
                     start_of_week_object.tm_mon == start_of_week_today.tm_mon &&//если совпадают месяца 
                     start_of_week_object.tm_mday == start_of_week_today.tm_mday) {//если совпадают дни 
                grouped_by_date["на этой неделе"].push_back(line);
            }
            else if (date_from_object_timestamp.tm_year == current_date.tm_year && //если совпадает год у проверяемой даты (date_from_timestamp) и текущей даты (current_date)
                     date_from_object_timestamp.tm_mon == current_date.tm_mon) { //и если совпадает месяц
                grouped_by_date["в этом месяце"].push_back(line);
            }
            else if (date_from_object_timestamp.tm_year == current_date.tm_year) { //если совпадает год у проверяемой даты (date_from_timestamp) и текущей даты (current_date)
                grouped_by_date["в этом году"].push_back(line);
            }
            else //остальные результаты попадают в группу более года
                grouped_by_date["ранее"].push_back(line);
        }
        for (auto group : grouped_by_date) {
            cout << "Группа создано " << group.first << ":" << std::endl;
            for (auto item : group.second) {
                print(item);
                cout << endl;
            }
            cout << endl;
        }
        //очищаем наш главный вектор и записываем в него отсортировнный результат функции
        lines.clear();
        for (auto group : grouped_by_date) {
            for (auto item : group.second) {
                lines.push_back(item);
            }
        }
        //сортируем главынй вектор, т.к. 
        sort(lines.begin(), lines.end(),
            [](const vector<string>& a, const vector<string>& b) {
                return stod(a[4]) < stod(b[4]);
            });
    }

    //ф-ция группировки по типу
    void Group_by_type() {
        map<string, vector<vector<string>>> grouped_by_type;//контецнер для группировки
        sort(lines.begin(), lines.end(),//сортировка где условия сортировки это лямбда ф-ция которая сравнивает 3 элементы векторов (строки)
            [](const vector<string>& a, const vector<string>& b) {
                return  a[3] < b[3];
            });
        cout << "\n----Результат группировки по типу: " << endl;
        int count;//переменная для определения кол-ва гркппируемых объектов
        cout << "(Объекты количество которых меньше или равно указанного значения будут помещены в группу \"Разное\")";
        cout << "\nУкажите количество объектов для группировки: ";
        while (true) {
            cin >> count; //вводим кол-во группируемых объектов
            // Проверяем, было ли введено число и не возникла ли ошибка
            if (cin.fail() || count < 0) {
                cout << "Ошибка: введите натуральное число (больше 0): ";
                cin.clear(); // Очищаем состояние потока
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорируем неверный ввод
            }
            else {
                break; // Ввод корректен, выходим из цикла
            }
        }
        unordered_map<string, int> count_map;//создаем контейнер, в котором сначала посчитаем количество каждых элекментов в каждо группе
        for (auto line : lines) {
            count_map[line[3]]++;//при воторном обнаружении ключа добавляем к значению +1
        }

        for (auto line : lines) {
            if (count_map[line[3]] > count) { //добавляем в группу строки объектов которые состоят в группе где больше объектов > count
                grouped_by_type[line[3]].push_back(line);
            }
            else //остальные добвляем в группу разное
                grouped_by_type["Разное"].push_back(line);
        }
        for (auto group : grouped_by_type) {
            cout << "Группа '" << group.first << "':" << std::endl;
            for (auto item : group.second) {
                print(item);
                cout << endl;
            }
            cout << endl;
        }
        //очищаем наш главный вектор и записываем в него отсортировнный результат функции
        lines.clear();
        for (auto group : grouped_by_type) {
            for (auto item : group.second) {
                lines.push_back(item);
            }
        }
    }

    //ф-ция сохранения файла
    void Save_file() {
        ofstream outFile(File_name); // Открываем файл для записи

        if (!outFile) { // Проверяем, удалось ли открыть файл
            cerr << "Не удалось открыть файл." << endl;
            return; // Завершаем программу с ошибкой
        }

        for (auto line : lines) {
            for (auto str : line) {
                outFile << str << " "; // Запись строки в файл
            }
            outFile << endl;
        }

        outFile.close(); // Закрываем файл
        cout << "\n----Данные сохранены в "<< File_name << endl;
    }

    //ф-ция удаления объекта объекта. По заданию не нужна. Посчитал что если можно добавить то должно быть можно и удалить.
    void del() {
        vector<vector<string>> newlines = lines; //создаемвектор для перезаписи для записи удаляемного объекта по названию
        string key_word; //переменная для записи удаляемного объекта по названию
        cout << "\n----Удаление объекта: " << endl;
        cout << "Введите наименование объекта(ов) для удаления: " << endl;
        cin >> key_word;
        for (auto line : lines) { //проходим по главному вектору
            if (key_word == line[0]) { // если указанное слово соотв. названию объекта
                newlines.erase(std::remove_if(newlines.begin(), newlines.end(),
                    [&line](const std::vector<string>& vec) {
                        return vec == line;
                    }), newlines.end());
                //лямбда ф-ция удаляет из вектора все вхождения векторов line из вектора lines, где название соотв. указанному ранее слову
            }
        }
        lines.clear();
        lines = newlines; //перезаписываем вектор на вектор с удаленными объектами
    }

    //ф-ция меню для пользователя
    void menu() {
        int count;
        cout << "Нажмите 1 - Вывод информации из файла: " << endl;
        cout << "Нажмите 2 - Добавление нового объекта в файл." << endl;
        cout << "Нажмите 3 - Группировка объектов по расстоянию. " << endl;
        cout << "Нажмите 4 - Группировка объектов по названию объекта. " << endl;
        cout << "Нажмите 5 - Группировка объектов по времени создания объекта. " << endl;
        cout << "Нажмите 6 - Группировка объектов по типу. " << endl;
        cout << "Нажмите 7 - Сохранение информации в файл. " << endl;
        cout << "Нажмите 8 - Удаление указанного объекта из файла. " << endl;
        cout << "Нажмите 0 - Выход из программы. " << endl;
        while (true) {
            cout << "\n----Выберите опцию: ";
            cin >> count; 
            if (cin.fail() || count < 0 || count > 8) {
                cout << "Ошибка ввода: ";
                cin.clear(); // Очищаем состояние потока
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорируем неверный ввод
            }
            if (count == 1)
                print();
            if (count == 2)
                add();
            if (count == 3)
                Group_by_distance();
            if (count == 4)
                Group_by_name();
            if (count == 5)
                Group_by_date();
            if (count == 6)
                Group_by_type();
            if (count == 7)
                Save_file();
            if (count == 8)
                del();
            if (count == 0) {
                cout << "Выход из программы......";
                exit(1);
            }
        }
    }

};

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    FileOperation file("file.txt");

    file.menu();
}


