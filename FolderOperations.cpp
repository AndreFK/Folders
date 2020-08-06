// FolderOperations.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <tchar.h> 
#include <iostream>
#include <string>
#include <vector>

#define VH(h) (h != INVALID_HANDLE_VALUE)

using namespace std;

void createFolder(string path, string name) {
	path = path + "\\" + name;
	wstring ws = wstring(path.begin(), path.end());
	if (!CreateDirectory(ws.c_str(), NULL))
	{
		cout << " -- Error Creating Directory -- " << endl;
	}
	else
	{
		cout << " -- Directory Created -- " << endl;
	}
}


void deleteFolder(string path) {
	vector<wstring::value_type> doubleNullTerminatedPath;
	copy(path.begin(), path.end(), back_inserter(doubleNullTerminatedPath));
	doubleNullTerminatedPath.push_back(L'\0');
	doubleNullTerminatedPath.push_back(L'\0');

	SHFILEOPSTRUCTW fileOperation;
	fileOperation.wFunc = FO_DELETE;
	fileOperation.pFrom = &doubleNullTerminatedPath[0];
	fileOperation.fFlags = FOF_NO_UI | FOF_NOCONFIRMATION;

	int result = ::SHFileOperationW(&fileOperation);
	if (result != 0) {
		cout << " -- Error Deleting Directory -- " << endl;
	}
	else {
		cout << " -- Directory Deleted -- " << endl;
	}
}

void listFolder(string path) {
	path = path + "\\*";
	
	LARGE_INTEGER filesize;
	int count = 2;

	wstring ws = wstring(path.begin(), path.end());
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(ws.c_str(), &data);
	
	if (VH(hFind)) {
		do {
			if (count <= 0) {
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					_tprintf(TEXT("  %s   [ DIRECTORY ]\n"), data.cFileName);
				}
				else
				{
					filesize.LowPart = data.nFileSizeLow;
					filesize.HighPart = data.nFileSizeHigh;
					_tprintf(TEXT("  %s  [ FILE ]  %ld bytes\n"), data.cFileName, filesize.QuadPart);
				}
			}
			count--;
		}while(FindNextFile(hFind, &data));
		FindClose(hFind);
	}
}

bool isCommand(string command) {
	if (command == "create_folder" || command == "delete_folder" || command == "list_folder" || command == "exit") {
		return true;
	}
	else {
		cout << " -- Invalid Command -- " << endl;
		return false;
	}
}

void parseCommand(string command) {
	string delim = " ";
	string commandType;
	size_t pos;
	
	pos = command.find(delim);

	commandType = command.substr(0, pos);

	if (isCommand(commandType)) {
		if (commandType == "create_folder") {
			string arg1, arg2;
			size_t pos2 = command.find(delim, pos + 1);
			arg1 = command.substr(pos + 1, pos2 - pos - 1);
			arg2 = command.substr(pos2 + 1);
			createFolder(arg1, arg2);
		}
		else if (commandType == "delete_folder") {
			string arg = command.substr(pos + 1);
			deleteFolder(arg);
		}
		else if (commandType == "list_folder") {
			string arg = command.substr(pos + 1);
			listFolder(arg);
		}
		cout << "\n";
	}
	else {
		return;
		cout << "\n";
	}
	
}

int main()
{
	string command;
	while (command != "exit") {
		getline(cin, command);
		parseCommand(command);
	}
}
