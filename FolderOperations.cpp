// FolderOperations.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <sys/types.h> 
#include <sys/stat.h> 
#include <filesystem>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

void createFolder(string path, string name) {
	path += "/" + name;
	wstring sw = wstring(path.begin(), path.end());
	const wchar_t* pathw = sw.c_str();
	int check = _wmkdir(pathw);
	if (check == 0) {
		cout << " -- Folder Created -- " << endl;
	}
	else {
		cout << " -- Error Creating Folder -- " << endl;
	}
}

void deleteFolder(string dir) {
	int check = fs::remove_all(dir);
	if (check > 0) {
		cout << " -- Folder Deleted -- " << endl;
	}
	else {
		cout << " -- Error Deleting Folder -- " << endl;
	}
}

void listFolder(string path) {
	for (const auto& entry : fs::directory_iterator(path)) {
		string type = entry.is_directory() == 1 ? " -- [ Directory ]" : " -- [ File ]";
		cout << entry.path().filename() << type << endl;
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
