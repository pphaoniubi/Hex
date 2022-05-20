#include "Inject.h"


bool ManualMap(HANDLE hProc, const char * szDllFile){




	BYTE* pSrcData = nullptr;
	IMAGE_NT_HEADERS * pOldNtHeader = nullptr;
	IMAGE_OPTIONAL_HEADER * pOldOptHeader = nullptr;
	IMAGE_FILE_HEADER * pOldFileHeader = nullptr;
	BYTE* pTargetBase = nullptr;

	//checking if the file exists
	DWORD dwCheck = 0;

	if (!GetFileAttributesA(szDllFile)) {

		printf("File does not exist");
		return false;
	}

	std::ifstream File(szDllFile, std::ios::binary | std::ios::ate);

	if (File.fail()) {
		printf("Opening the file failed: %X\n", (DWORD)File.rdstate());
		return false;
	}

	auto FileSize = File.tellg();
	if (FileSize < 0x1000) {
		printf("fileSize is invalid\n");
		File.close();
		return false;
	}

	//reading the file into memory

	pSrcData = new BYTE[static_cast<UINT_PTR>(FileSize)];

	if (!pSrcData){

		printf("Memory allocating failed\n");
		File.close();
		return false;
	}

	File.seekg(0, std::ios::beg);
	File.read(reinterpret_cast<char*>(pSrcData), FileSize);
	File.close();

	//check whether the file is dll

	if(reinterpret_cast<IMAGE_DOS_HEADER*> (pSrcData)->e_magic != 0x5A4D) //MZ
	{

		printf("invalid file");

		delete[] pSrcData;
		return false;
	}

	pOldNtHeader = reinterpret_cast<IMAGE_NT_HEADERS*> (pSrcData + reinterpret_cast<IMAGE_DOS_HEADER*>(pSrcData)->e_lfanew);

	pOldOptHeader = &pOldNtHeader->OptionalHeader;
	pOldFileHeader = &pOldNtHeader->FileHeader;

	//this project should work for both x86 and x64

	#ifdef _WIN64
	
	if(pOldFileHeader->Machine != IMAGE_FILE_MACHINE_AMD64){
				printf("Invalid platform\n");
				delete[] pSrcData;
				return false;
	}


	#else

	if(pOldFileHeader->Machine != IMAGE_FILE_MACHINE_I386){
				printf("Invalid platform\n");
				delete[] pSrcData;
				return false;
	}


	#endif

	pTargetBase = reinterpret_cast<BYTE*>(VirtualAllocEx(hProc, reinterpret_cast<void*>(pOldOptHeader->ImageBase), pOldOptHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

	if (!pTargetBase) {

		printf("Memory allocation failed (ex) %X\n", GetLastError());
		delete[] pSrcData;
		return false;

	}

	MANUAL_MAPPING_DATA data{ 0 };
	data.pLoadLibraryA = LoadLibraryA;
	data.pGetProcAddress = reinterpret_cast<f_GetProcAddress>(GetProcAddress);


	//start mapping here
	auto* pSectionHeader = IMAGE_FIRST_SECTION(pOldNtHeader);


}