#include "cl_application.h"

// Конструктор класса cl_application, принимает указатель на головной объект
cl_application::cl_application(cl_base* p_head_object):cl_base(p_head_object){
    this->number = 1; // Устанавливаем номер объекта равным 1
}

// Метод для построения дерева объектов
void cl_application::build_tree_objects() {
    cl_base* p_head = this; // Указатель на головной объект
    cl_base* p_sub = nullptr; // Указатель на подобъект
    string s_head, s_sub; // Строки для хранения имен головного объекта и подобъекта
    int i_class; // Переменная для хранения класса объекта

    cin >> s_head; // Считываем имя головного объекта
    set_name(s_head); // Устанавливаем имя головного объекта

    while (true) { // Цикл для построения дерева объектов
        cin >> s_head; // Считываем имя головного объекта
        if (s_head == "endtree") break; // Если имя равно "endtree", прерываем цикл
        cin >> s_sub >> i_class; // Считываем имя подобъекта и класс объекта
        p_head = find_object_by_cord(s_head); // Находим головной объект по его имени

        if (p_head == nullptr) { // Если головной объект не найден
            cout << "Object tree"; // Выводим сообщение об ошибке
            print_current(); // Выводим текущее состояние дерева объектов
            cout << endl << "The head object " << s_head << " is not found"; // Выводим сообщение об ошибке
            exit(1); // Завершаем программу с кодом ошибки 1
        }

        if (p_head->get_sub_object(s_head) == nullptr) { // Если подобъект не найден
            switch (i_class){ // В зависимости от класса объекта
                case 2:
                    p_sub = new cl_2(p_head, s_sub); // Создаем новый объект класса cl_2
                    break;
                case 3:
                    p_sub = new cl_3(p_head, s_sub); // Создаем новый объект класса cl_3
                    break;
                case 4:
                    p_sub = new cl_4(p_head, s_sub); // Создаем новый объект класса cl_4
                    break;
                case 5:
                    p_sub = new cl_5(p_head, s_sub); // Создаем новый объект класса cl_5
                    break;
                case 6:
                    p_sub = new cl_6(p_head, s_sub); // Создаем новый объект класса cl_6
                    break;
            }
        }
    }
}

// Метод для выполнения приложения
int cl_application::exec_app() {
    cout << "Object tree"; // Выводим сообщение "Object tree"
    print_current(); // Выводим текущее состояние дерева объектов

    string s_send_coord_set; // Строка для хранения координат отправителя
    string s_recive_coord_set; // Строка для хранения координат получателя
    cl_base* p_send_coord_set; // Указатель на объект-отправитель
    cl_base* p_recive_coord_set; // Указатель на объект-получатель

    vector <TYPE_SIGNAL> SIGNALS_INDEXES = { // Вектор с индексами сигналов
        SIGNAL_D(cl_1::signal_f),
        SIGNAL_D(cl_2::signal_f),
        SIGNAL_D(cl_3::signal_f),
        SIGNAL_D(cl_4::signal_f),
        SIGNAL_D(cl_5::signal_f),
        SIGNAL_D(cl_6::signal_f)
    };

    vector <TYPE_HANDLER> HANDLER_INDEXES = { // Вектор с индексами обработчиков
        HANDLER_D(cl_1::handler_f),
        HANDLER_D(cl_2::handler_f),
        HANDLER_D(cl_3::handler_f),
        HANDLER_D(cl_4::handler_f),
        HANDLER_D(cl_5::handler_f),
        HANDLER_D(cl_6::handler_f)
    };

    while (true) { // Цикл для установки соединений
        cin >> s_send_coord_set; // Считываем координаты отправителя
        if (s_send_coord_set == "end_of_connections") break; // Если координаты равны "end_of_connections", прерываем цикл
        cin >> s_recive_coord_set; // Считываем координаты получателя
        p_send_coord_set = this->find_object_by_cord(s_send_coord_set); // Находим объект-отправитель по его координатам
        p_recive_coord_set = this->find_object_by_cord(s_recive_coord_set); // Находим объект-получатель по его координатам
        TYPE_SIGNAL send_signal_set = SIGNALS_INDEXES[p_send_coord_set->number - 1]; // Получаем сигнал отправителя
        TYPE_HANDLER recive_handler_set = HANDLER_INDEXES[p_recive_coord_set->number - 1]; // Получаем обработчик получателя
        p_send_coord_set->setConnect(send_signal_set, p_recive_coord_set, recive_handler_set); // Устанавливаем соединение
    }

    set_cond(1); // Устанавливаем состояние равное 1
    string s_command, s_data; // Строки для хранения команды и данных
    string s_coord, s_msg; // Строки для хранения координат и сообщения

    while (true) { // Цикл для обработки команд
        cin >> s_command; // Считываем команду
        if (s_command == "END") break; // Если команда равна "END", прерываем цикл
        getline(cin, s_data); // Считываем данные
        s_data = s_data.substr(1); // Удаляем первый символ из данных
        s_coord = s_data.substr(0, s_data.find(' ')); // Получаем координаты из данных
        s_msg = s_data.substr(s_data.find(' ') + 1); // Получаем сообщение из данных
        cl_base* p_sender = this->find_object_by_cord(s_coord); // Находим объект-отправитель по его координатам

        if (p_sender == nullptr) { // Если объект-отправитель не найден
            cout << endl << "Object " << s_coord << " not found"; // Выводим сообщение об ошибке
            continue; // Продолжаем цикл
        }

        if (s_command == "EMIT") { // Если команда равна "EMIT"
            TYPE_SIGNAL send_sig = SIGNALS_INDEXES[p_sender->number - 1]; // Получаем сигнал отправителя
            p_sender->emitSignal(send_sig, s_msg); // Отправляем сигнал
            continue; // Продолжаем цикл
        }

        if (s_command == "SET_CONNECT") { // Если команда равна "SET_CONNECT"
            cl_base* p_recive = this->find_object_by_cord(s_msg); // Находим объект-получатель по его координатам
            if (p_recive == nullptr) { // Если объект-получатель не найден
                cout << endl << "Handler object "<< s_msg <<" not found"; // Выводим сообщение об ошибке
                continue; // Продолжаем цикл
            } else {
                TYPE_SIGNAL send_sig = SIGNALS_INDEXES[p_sender->number - 1]; // Получаем сигнал отправителя
                TYPE_HANDLER recive_hand = HANDLER_INDEXES[p_recive->number - 1]; // Получаем обработчик получателя
                p_sender->setConnect(send_sig, p_recive, recive_hand); // Устанавливаем соединение
            }
        }

        if (s_command == "DELETE_CONNECT") { // Если команда равна "DELETE_CONNECT"
            cl_base* p_recive = this->find_object_by_cord(s_msg); // Находим объект-получатель по его координатам
            if (p_recive == nullptr) { // Если объект-получатель не найден
                cout << endl << "Handler object "<< s_msg <<" not found"; // Выводим сообщение об ошибке
                continue; // Продолжаем цикл
            } else {
                TYPE_SIGNAL send_sig = SIGNALS_INDEXES[p_sender->number - 1]; // Получаем сигнал отправителя
                TYPE_HANDLER recive_hand = HANDLER_INDEXES[p_recive->number - 1]; // Получаем обработчик получателя
                p_sender->deleteConnect(send_sig, p_recive, recive_hand); // Удаляем соединение
            }
        }

        if (s_command == "SET_CONDITION") { // Если команда равна "SET_CONDITION"
            int new_state = stoi(s_msg); // Преобразуем сообщение в целое число
            p_sender->set_cond(new_state); // Устанавливаем новое состояние
        }
    }

    return 0; // Возвращаем 0, что означает успешное выполнение программы
}