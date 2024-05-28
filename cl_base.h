#ifndef __CL_BASE__H
#define __CL_BASE__H

#include <iostream>
#include <vector>
#include <queue>
#include <string>

// Это макросы препроцессора в C++. Они используются для создания алиасов для типов функций.
#define SIGNAL_D(signal_f) (TYPE_SIGNAL)(&signal_f) //  этот макрос принимает имя функции signal_f и приводит её к типу TYPE_SIGNAL. В результате получается указатель на функцию типа TYPE_SIGNAL
#define HANDLER_D(handler_f) (TYPE_HANDLER)(&handler_f) // этот макрос принимает имя функции handler_f и приводит её к типу TYPE_HANDLER. В результате получается указатель на функцию типа TYPE_HANDLER.

using namespace std;

class cl_base;

typedef void(cl_base::* TYPE_SIGNAL) (string& msg); // cl_base::* TYPE_SIGNAL - указатель на объект класса cl_base; (string& msg) -  по ссылке
typedef void (cl_base::* TYPE_HANDLER) (string msg);


struct o_sh // структура, которая организовывает связь
{
	TYPE_SIGNAL p_signal; // указатель на сигнал
	TYPE_HANDLER p_handler; // указатель на обработчик
	cl_base *p_target; // целевой объект
};

class cl_base{
	string s_name;
	cl_base* p_head_obj;
	vector<cl_base*> p_sub_objects;
	int status;
	int p_ready = 1;
	vector<o_sh*> connects; // вектор, в котором будут храниться указатели на связи
	int comand_state = 1;
public:
	cl_base(cl_base* p_head_obj, string s_name = "Base object");
	bool set_name(string s_new_name);
	string get_name();
	cl_base* get_sub_object(string s_name);
	cl_base* get_head();
	~cl_base();

	// KV2
	cl_base *search_current(string);
	cl_base *search_tree(string);
	void set_state(int);
	void print_current(int ind = 0);
	void show_ready_tree(int ind = 0);

	// KV3
	cl_base *find_object_by_cord(string);
	void delete_sub_object(string);
	bool set_head_obj(cl_base*);

	// KV4
	int number = 1;
	void set_cond(int);
	void setConnect(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler);
	void deleteConnect(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler);
	void emitSignal(TYPE_SIGNAL p_signal, string message);
	string get_path();
};



#endif
