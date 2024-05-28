#include "cl_application.h"

cl_application::cl_application(cl_base* p_head_object):cl_base(p_head_object){
	this->number = 1;
}

void cl_application::build_tree_objects() {
	cl_base* p_head = this;
	cl_base* p_sub = nullptr;
	string s_head, s_sub;
	int i_class;

	cin >> s_head;
	set_name(s_head);

	while (true) {
		cin >> s_head;
		if (s_head == "endtree") break;
		cin >> s_sub >> i_class;
		p_head = find_object_by_cord(s_head);

		if (p_head == nullptr) {
			cout << "Object tree";
			print_current();
			cout << endl << "The head object " << s_head << " is not found";
			exit(1);
		}

		if (p_head->get_sub_object(s_head) == nullptr) {
			switch (i_class){
				case 2:
					p_sub = new cl_2(p_head, s_sub);
					break;
				case 3:
					p_sub = new cl_3(p_head, s_sub);
					break;
				case 4:
					p_sub = new cl_4(p_head, s_sub);
					break;
				case 5:
					p_sub = new cl_5(p_head, s_sub);
					break;
				case 6:
					p_sub = new cl_6(p_head, s_sub);
					break;
			}
		}
	}
}

int cl_application::exec_app() {
	cout << "Object tree";
	print_current();

	string s_send_coord_set;
	string s_recive_coord_set;
	cl_base* p_send_coord_set;
	cl_base* p_recive_coord_set;

	vector <TYPE_SIGNAL> SIGNALS_INDEXES = {
		SIGNAL_D(cl_1::signal_f),
		SIGNAL_D(cl_2::signal_f),
		SIGNAL_D(cl_3::signal_f),
		SIGNAL_D(cl_4::signal_f),
		SIGNAL_D(cl_5::signal_f),
		SIGNAL_D(cl_6::signal_f)
	};

	vector <TYPE_HANDLER> HANDLER_INDEXES = {
		HANDLER_D(cl_1::handler_f),
		HANDLER_D(cl_2::handler_f),
		HANDLER_D(cl_3::handler_f),
		HANDLER_D(cl_4::handler_f),
		HANDLER_D(cl_5::handler_f),
		HANDLER_D(cl_6::handler_f)
	};

	while (true) {
		cin >> s_send_coord_set;
		if (s_send_coord_set == "end_of_connections") break;
		cin >> s_recive_coord_set;
		p_send_coord_set = this->find_object_by_cord(s_send_coord_set);
		p_recive_coord_set = this->find_object_by_cord(s_recive_coord_set);
		TYPE_SIGNAL send_signal_set = SIGNALS_INDEXES[p_send_coord_set->number - 1];
		TYPE_HANDLER recive_handler_set = HANDLER_INDEXES[p_recive_coord_set->number - 1];
		p_send_coord_set->setConnect(send_signal_set, p_recive_coord_set, recive_handler_set);
	}

	set_cond(1);
	string s_command, s_data;
	string s_coord, s_msg;

	while (true) {
		cin >> s_command;
		if (s_command == "END") break;
		getline(cin, s_data);
		s_data = s_data.substr(1);
		s_coord = s_data.substr(0, s_data.find(' '));
		s_msg = s_data.substr(s_data.find(' ') + 1);
		cl_base* p_sender = this->find_object_by_cord(s_coord);

		if (p_sender == nullptr) {
			cout << endl << "Object " << s_coord << " not found";
			continue;
		}

		if (s_command == "EMIT") {
			TYPE_SIGNAL send_sig = SIGNALS_INDEXES[p_sender->number - 1];
			p_sender->emitSignal(send_sig, s_msg);
			continue;
		}

		if (s_command == "SET_CONNECT") {
			cl_base* p_recive = this->find_object_by_cord(s_msg);
			if (p_recive == nullptr) {
				cout << endl << "Handler object "<< s_msg <<" not found";
				continue;
			} else {
				TYPE_SIGNAL send_sig = SIGNALS_INDEXES[p_sender->number - 1];
				TYPE_HANDLER recive_hand = HANDLER_INDEXES[p_recive->number - 1];
				p_sender->setConnect(send_sig, p_recive, recive_hand);
			}
		}

		if (s_command == "DELETE_CONNECT") {
			cl_base* p_recive = this->find_object_by_cord(s_msg);
			if (p_recive == nullptr) {
				cout << endl << "Handler object "<< s_msg <<" not found";
				continue;
			} else {
				TYPE_SIGNAL send_sig = SIGNALS_INDEXES[p_sender->number - 1];
				TYPE_HANDLER recive_hand = HANDLER_INDEXES[p_recive->number - 1];
				p_sender->deleteConnect(send_sig, p_recive, recive_hand);
			}
		}

		if (s_command == "SET_CONDITION") {
			int new_state = stoi(s_msg);
			p_sender->set_cond(new_state);
		}
	}

	return 0;
}
