#include "bofile.h"

namespace aldx
{
	bo_file* load_bo(datablob<byte>* dbl)
	{
		bo_header* he = (bo_header*)dbl->data;
		bo_map* bmap = new bo_map();

		for (uint i = 0; i < he->number_of_objects; ++i)
		{
			string n = he->objects[i].name;
			(*bmap)[n] = bo_data(dbl->data + he->objects[i].data_offset,
				he->objects[i].size, he->objects[i].type);
		}

		return new bo_file(bmap, he->file_type);
	}

	datablob<byte>* MakeDatablobFromBO_Data(const bo_data& obj)
	{
		return new datablob<byte>((byte*)obj.data, obj.size);
	}

#ifdef INCLUDE_WRITE_BO
	void write_bo(const bo_file& dta, FILE* f)
	{
		size_t acsiz = sizeof(uint)* 2 + sizeof(bo_object)*dta.data->size(); //calulate actual size of the bo header vs. what the compiler thinks the size is
		char* actual_header_data = new char[acsiz];
		bo_header* boh = (bo_header*)actual_header_data;
		boh->file_type = dta.type;
		//if (dta.data->size() > MAX_BO_OBJECTS)
		//	throw exception("Number of objects in dta exceed the maximum number of objects in a BO file");
		boh->number_of_objects = dta.data->size();
		int bo_objidx = 0;
		const uint dt_size = dta.data->size() * sizeof(bo_object)+sizeof(uint32)* 2;
		fwrite(boh, dt_size, 1, f);
		for (auto o = dta.data->begin(); o != dta.data->end(); ++o)
		{
			memcpy(boh->objects[bo_objidx].name, o->first.c_str(), 16);
			boh->objects[bo_objidx].size = o->second.size;
			boh->objects[bo_objidx].type = o->second.type;
			boh->objects[bo_objidx].data_offset = ftell(f);
			fwrite(o->second.data, o->second.size, 1, f);
			bo_objidx++;
		}
		fseek(f, 0, SEEK_SET);
		fwrite(boh, dt_size, 1, f);
		fclose(f);
	}
#endif
}