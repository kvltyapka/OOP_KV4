#include "cl_base.h"

cl_base::cl_base(cl_base* p_head_obj, string s_name){
	this -> s_name = s_name;
	this -> p_head_obj = p_head_obj;
	if (this -> p_head_obj)
		p_head_obj -> p_sub_objects.push_back(this);
	
}

bool cl_base::set_name(string s_new_name){
	if (this -> p_head_obj)
		for (int i = 0; p_head_obj -> p_sub_objects.size(); i++)
			if (p_head_obj -> p_sub_objects[i] -> get_name() == s_new_name)
				return false;
	this -> s_name = s_new_name;
	return true;
}

cl_base::~cl_base(){
	for (int i = 0; i < p_sub_objects.size(); i++)
		delete p_sub_objects[i];
}

cl_base* cl_base::get_sub_object(string s_name){
	for (int i = 0; i < p_sub_objects.size(); i++)
		if (p_sub_objects[i] -> get_name() == s_name)
			return p_sub_objects[i];
	return nullptr;
}


string cl_base::get_name()
{
	return s_name;
}

cl_base *cl_base::get_head()
{
	return p_head_obj;
}

cl_base *cl_base::search_current(string s_name)
{
	cl_base *p_found = nullptr;
	queue<cl_base*> q;
	q.push(this);

	while(!q.empty()){
		cl_base *p_front = q.front();
		q.pop();
		if(p_front -> get_name() == s_name)
			if (p_found == nullptr)
				p_found = p_front;
		else
			return nullptr;
		for(auto p_sub : p_front -> p_sub_objects)
			q.push(p_sub);
	}
	return p_found;
}

cl_base *cl_base::search_tree(string s_name)
{
	cl_base *p_root = this;
	while(p_root -> get_head() != nullptr)
		p_root = p_root -> get_head();
	return p_root -> search_current(s_name);
}

void cl_base::set_state(int state)
{
	if (state == 0)
	{
		this -> status = state;
		for(auto p_sub : p_sub_objects)
			p_sub -> set_state(state);
	}
	else
	{
		for(cl_base *temp_head = this -> p_head_obj; temp_head != nullptr; temp_head = temp_head -> p_head_obj)
		{
			if (temp_head -> status == 0)
				return;
		}
		this -> status = state;
	}
}


void cl_base::print_current(int ind)
{
	if (this != nullptr) cout << endl << string(ind, ' ') << this -> s_name;
	for (auto p_sub : this -> p_sub_objects)
		p_sub -> print_current(ind + 4);
}

void cl_base::show_ready_tree(int ind)
{

	for (int i = 0; i < ind; i++) cout << "    ";
	cout << this -> s_name;

	if(this -> status != 0)
		cout << " is ready";
	else
		cout << " is not ready";
	cout << endl;
	for (auto p_sub : this -> p_sub_objects)
		p_sub -> show_ready_tree(ind + 1);
}

///////////////////////////////////////////////////////////////////
cl_base *cl_base::find_object_by_cord(string s_coord)
{
	int i_slash_2 = 0;
	string f_name = s_coord;
	cl_base* p_obj;

	cl_base *p_root = this;

	while(p_root -> get_head() != nullptr)
		p_root = p_root -> get_head();

	if (s_coord == "")
		return nullptr;

	if (s_coord == "/")
		return p_root;

	if (s_coord == ".")
		return this;

	if (s_coord[0] == '/' && s_coord[1] == '/')
	{
		f_name = s_coord.substr(2);
		return this -> search_tree(f_name);
	}

	if (s_coord[0] == '.')
	{
		f_name = s_coord.substr(1);
		return this -> search_current(f_name);
	}

	i_slash_2 = s_coord.find("/", 1);
	if (s_coord[0] == '/')
	{
		if(i_slash_2 != -1)
		{
			f_name = s_coord.substr(1, i_slash_2 -1);
			p_obj = p_root -> get_sub_object(f_name);

			if (p_obj != nullptr)
				return p_obj -> find_object_by_cord(s_coord.substr(i_slash_2 + 1));
			else
				return p_obj;
		}
		else
		{
			f_name = s_coord.substr(1);
			return p_root -> get_sub_object(f_name);
		}
	}
	else
	{
		if (i_slash_2 != -1)
		{
			f_name = s_coord.substr(0, i_slash_2);

			p_obj = this -> get_sub_object(f_name);
			if (p_obj != nullptr)
				return p_obj -> find_object_by_cord(s_coord.substr(i_slash_2 + 1));
			else
				return p_obj;
		}
		else
		{
			f_name = s_coord;
			return this -> get_sub_object(f_name);
		}
	}
}

void cl_base::delete_sub_object(string s_name)
{
	for (int i = 0; i < p_sub_objects.size(); i++)
	{
		if (p_sub_objects[i] -> get_name() == s_name)
		{
			p_sub_objects.erase(p_sub_objects.begin() + i);
			i--;
		}
	}
}

bool cl_base::set_head_obj(cl_base *p_new_head)
{
	cl_base *p_temp = p_new_head;

	if (this -> get_head() == nullptr)
		return false;

	else if(p_new_head == nullptr)
		return false;
	else if(p_new_head -> get_sub_object(this -> get_name()) != nullptr)
		return false;
	else{
		while (p_temp != nullptr)
		if (p_temp == this)
			return false;
		else
			p_temp = p_temp -> get_head();
	}

	this -> get_head() -> delete_sub_object(this -> get_name());
	p_head_obj = p_new_head;
	p_head_obj -> p_sub_objects.push_back(this);

	return true;
}


//////////////////////////////////////
// Определение метода setConnect класса cl_base
void cl_base::setConnect(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler) {
	// Объявление указателя на объект o_sh
	o_sh* p_value;

	// Цикл по вектору подключений
	for (int i = 0; i < connects.size(); i++) {
		// Если подключение с таким же сигналом, целевым объектом и обработчиком уже существует
		if (connects[i]->p_signal == p_signal &&
			connects[i]->p_target == p_target &&
			connects[i]->p_handler == p_handler) {
			// Завершаем выполнение метода
			return;
			}
	}

	// Создание нового объекта o_sh
	p_value = new o_sh();
	// Установка сигнала для нового объекта
	p_value->p_signal = p_signal;
	// Установка целевого объекта для нового объекта
	p_value->p_target = p_target;
	// Установка обработчика для нового объекта
	p_value->p_handler = p_handler;

	// Добавление нового объекта в вектор подключений
	connects.push_back(p_value);
}

// Определение метода deleteConnect класса cl_base
void cl_base::deleteConnect(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler) {
	// Цикл по вектору подключений
	for (int i = 0; i < connects.size(); i++) {
		// Если подключение с таким же сигналом, целевым объектом и обработчиком найдено
		if (connects[i]->p_signal == p_signal &&
			connects[i]->p_target == p_target &&
			connects[i]->p_handler == p_handler) {

			// Удаление объекта подключения
			delete connects[i];
			// Удаление элемента из вектора подключений
			connects.erase(connects.begin() + i);
			// Прекращение цикла после удаления подключения
			break;
			}
	}
}

// Определение метода emitSignal класса cl_base
void cl_base::emitSignal(TYPE_SIGNAL p_signal, string s_message) {
	// Если статус объекта равен 0, прекращаем выполнение метода
	if (status == 0) {
		return;
	}

	// Вызываем метод, указатель на который передан в p_signal, с аргументом s_message
	(this->*p_signal)(s_message);

	// Цикл по всем подключениям в векторе connects
	for (auto connect : connects) {
		// Если сигнал подключения совпадает с переданным сигналом
		if (connect->p_signal == p_signal) {
			// Получаем целевой объект подключения
			cl_base* p_obj = connect->p_target;

			// Если статус целевого объекта не равен 0
			if (p_obj->status != 0) {
				// Получаем обработчик подключения
				TYPE_HANDLER p_handler = connect->p_handler;
				// Вызываем метод, указатель на который передан в p_handler, с аргументом s_message
				(p_obj->*p_handler)(s_message);
			}
		}
	}
}

// Определение метода get_path класса cl_base
string cl_base::get_path(){
	// Получаем головной объект текущего объекта
	cl_base *p_head_obj = this -> get_head();

	// Если головной объект существует
	if (p_head_obj != nullptr){
		// Если у головного объекта нет своего головного объекта
		if (p_head_obj -> get_head() == nullptr){
			// Возвращаем путь головного объекта, добавляя к нему имя текущего объекта
			return p_head_obj -> get_path() + s_name;
		}
		// Если у головного объекта есть свой головной объект
		else{
			// Возвращаем путь головного объекта, добавляя к нему имя текущего объекта, разделенное слэшем
			return p_head_obj -> get_path() + "/" + s_name;
		}
	}

	// Если головного объекта нет, возвращаем корневой путь
	return "/";
}

// Определение метода set_cond класса cl_base
void cl_base::set_cond(int newstate){
	// Устанавливаем новое состояние для текущего объекта
	set_state(newstate);

	// Цикл по всем подобъектам текущего объекта
	for (int i = 0; i < p_sub_objects.size(); i++){
		// Устанавливаем новое состояние для каждого подобъекта
		p_sub_objects[i] -> set_cond(newstate);
	}
}