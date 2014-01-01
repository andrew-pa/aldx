#pragma once
#include "helper.h"
#include <map>
#include <string>
#include <vector>
using namespace std;

namespace aldx
{
	//bo_data
	// Data entry in a bo file
	struct bo_data
	{
		void* data;
		uint size;
		uint type;
		bo_data(void* d, uint s, uint t)
			: data(d), size(s), type(t) {}
		bo_data()
			: data(nullptr), size(-1), type(-1) { }
	};


	//bo_map
	// typedef to map<string, bo_data>, is the entry table
	typedef map<string, bo_data> bo_map;

	//bo_file
	// A bo file in memory
	struct bo_file
	{
		bo_map* data;
		uint type;
		bo_file(bo_map* d, uint t)
			: data(d), type(t) { }
	};

	//bo_object
	// Object in the bo header
	struct bo_object
	{
		char name[16];
		uint data_offset;
		uint size;
		uint type;
	};

	const int MAX_BO_OBJECTS = 3;

	//bo_header
	// BO file header
	struct bo_header
	{
		uint number_of_objects;
		uint file_type;
		bo_object objects[MAX_BO_OBJECTS];
	};

	//load_bo : load a bo_file from a datablob
	bo_file* load_bo(datablob<byte>* dbl);

	datablob<byte>* MakeDatablobFromBO_Data(const bo_data& obj);

#define INCLUDE_WRITE_BO
#ifdef INCLUDE_WRITE_BO
	void write_bo(const bo_file& dta, FILE* f);
#endif
}