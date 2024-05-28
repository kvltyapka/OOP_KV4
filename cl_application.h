#ifndef __CL_APPLICATION__H
#define __CL_APPLICATION__H

#include "cl_base.h"
#include "cl_1.h"
#include "cl_6.h"
#include "cl_2.h"
#include "cl_3.h"
#include "cl_4.h"
#include "cl_5.h"

class cl_application : public cl_base{
public:
	cl_application(cl_base* p_head_obj);
	void build_tree_objects();
	int exec_app();

	// KV4
	void build_commands();
	void setConnections();
};


#endif
