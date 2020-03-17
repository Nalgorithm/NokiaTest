#pragma once
#include<exception>

class FileFormatException : public std::exception {
private:
	const char* message;
public:
	FileFormatException(const char* mes) : message(mes) { }
	const char* what() const noexcept {
		return message;
	}
};

class RepeatingNamesException : public std::exception {
private:
	const char* message;
public:
	RepeatingNamesException(const char* mes) : message(mes) { }
	const char* what() const noexcept {
		return message;
	}
};

class NonexistentCellException : public std::exception {
private:
	const char* message;
public:
	NonexistentCellException(const char* mes) : message(mes) { }
	const char* what() const noexcept {
		return message;
	}
};

class CycleException : public std::exception {
private:
	const char* message;
public:
	CycleException(const char* mes) : message(mes) { }
	const char* what() const noexcept {
		return message;
	}
};