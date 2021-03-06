﻿#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <wrl.h>
#include <ppl.h>
#include <ppltasks.h>

#include "vector_math.h"

#include <d3d11_2.h>
#include <d2d1_2.h>
#include <dwrite_2.h>

#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <iostream>
#include <fstream>
#include <functional>
#include <exception>
#include <algorithm>
using namespace std;


using Microsoft::WRL::ComPtr;
typedef unsigned int uint;

#ifdef LINK_DIRECTX
/*d2d1.lib
d3d11.lib
dxgi.lib
ole32.lib
windowscodecs.lib
dwrite.lib
xinput.lib
*/
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "xinput.lib")
#endif

#define proprw(t, n, gc) inline t& n() gc
#define propr(t, n, gc) inline t n() const gc

namespace aldx
{

	inline float randf() { return ((float)rand() / (float)RAND_MAX); }
	inline float randfn() { return ((randf() * 2) - 1); }

	template <typename T>
	inline vector<T> make_vector_of_one(T& it)
	{
		vector<T> v;
		v.push_back(it);
		return v;
	}


	//HRexception
	// exception that resulted from a failed HRESULT, which is passed along
	struct hresult_exception : public exception
	{
	public:
		HRESULT hr;
		hresult_exception(HRESULT h, const char* m = "") : hr(h), exception(m) { }
	};

	//ErrorCodeexception
	// exception the resulted from a error code that is failing
	struct error_code_exception : public exception
	{
	public:
		uint errorcode;
		error_code_exception(uint ec, const char* m = "") : errorcode(ec), exception(m) { }
	};


	//chr : check if FAILED(hr) and if so, throw a HRexception
	inline void chr(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw new hresult_exception(hr);
		}
	}

	//check_flag : check if flag value c is "on" in v
	template<typename E>
	inline bool check_flag(E v, E c)
	{
		return (v & c) == c;
	}

	//shader_stage
	// Enum that contains all the different shader stages
	enum class shader_stage
	{
		Vertex, Pixel, Geometry, Domain, Hull, Compute,
	};

	typedef UINT32 uint32;
	typedef UINT16 uint16;
	typedef UINT8 uint8;

	//datablob<T>
	// pointer to a T along with the T's size, usually file data
	template<typename T>
	struct datablob
	{
		datablob() : data(nullptr), length(-1) { }
		datablob(T* d, uint32 l) : data(d), length(l) { }
		datablob(uint32 l) : data(new T[l]), length(l) { }

		~datablob()
		{
			delete[] data;
		}

		T* data;
		uint32 length;
	};

	//datablob that leaks memory. only for weird stuff, like files
	template<typename T>
	struct _strange_datablob
	{
		_strange_datablob() : data(nullptr), length(-1) { }
		_strange_datablob(T* d, uint32 l) : data(d), length(l) { }
		_strange_datablob(uint32 l) : data(new T[l]), length(l) { }

		T* data;
		uint32 length;
	};

#ifndef _DX_READ_DATA
#define _DX_READ_DATA
	//Read in the data contained in filename, put it in to a datablob
	inline datablob<byte>* read_data(
		const wstring& filename	//_In_ const wchar_t* filename
		)
	{
		CREATEFILE2_EXTENDED_PARAMETERS extendedParams = { 0 };
		extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
		extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
		extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
		extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
		extendedParams.lpSecurityAttributes = nullptr;
		extendedParams.hTemplateFile = nullptr;

		HANDLE file = CreateFile2(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &extendedParams);

		if (file == INVALID_HANDLE_VALUE)
		{
			throw exception("DX::ReadData > Invalid File");
		}

		FILE_STANDARD_INFO fileInfo = { 0 };
		if (!GetFileInformationByHandleEx(
			file,
			FileStandardInfo,
			&fileInfo,
			sizeof(fileInfo)
			))
		{
			throw exception("DX::ReadData > GetFileInfo failed");
		}

		if (fileInfo.EndOfFile.HighPart != 0)
		{
			throw exception();
		}

		//datablob<byte> fileData = ref new Platform::Array<byte>(fileInfo.EndOfFile.LowPart);

		datablob<byte>* fileData = new datablob<byte>(fileInfo.EndOfFile.LowPart);

		if (!ReadFile(
			file,
			fileData->data,
			fileData->length,
			nullptr,
			nullptr
			))
		{
			throw exception("DX::ReadData > Couldn't read file");
		}

		CloseHandle(file);
		return fileData;
	}

	//Wrapper for read_data, but adds the executable path on to the file name
	inline datablob<byte>* read_data_from_package(_In_ const wstring& filename)
	{
		static std::wstring fpath = L"";
		if (fpath.length() == 0)
		{
			wchar_t* modfn = new wchar_t[MAX_PATH];
			GetModuleFileName(NULL, modfn, MAX_PATH);
			fpath = std::wstring(modfn);
			int pl = -1;
			for (int i = fpath.length(); i > 0; i--)
			{
				if (fpath[i] == '\\')
				{
					pl = i + 1;
					break;
				}
			}
			fpath = fpath.substr(0, pl);
		}
		std::wstring fpn = fpath;
		fpn += filename;
		return read_data(fpn.data());
	}
#endif
}