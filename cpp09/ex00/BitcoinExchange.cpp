#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cctype>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : _database(other._database) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
    if (this != &other) {
        _database = other._database;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

std::string BitcoinExchange::trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool BitcoinExchange::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool BitcoinExchange::isValidDate(const std::string &dateStr) {
    if (dateStr.length() != 10)
        return false;
    if (dateStr[4] != '-' || dateStr[7] != '-')
        return false;

    for (size_t i = 0; i < 10; ++i) {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(dateStr[i]))
            return false;
    }

    int year = std::atoi(dateStr.substr(0, 4).c_str());
    int month = std::atoi(dateStr.substr(5, 2).c_str());
    int day = std::atoi(dateStr.substr(8, 2).c_str());

    if (month < 1 || month > 12)
        return false;

    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) {
        daysInMonth[2] = 29;
    }

    if (day < 1 || day > daysInMonth[month])
        return false;

    return true;
}

bool BitcoinExchange::parseValue(const std::string &valStr, double &val, std::string &errorMsg) {
    if (valStr.empty()) {
        errorMsg = "bad input => " + valStr;
        return false;
    }

    // Reject leading '-' before parsing so that "-0" is caught as not positive
    if (valStr[0] == '-') {
        errorMsg = "not a positive number.";
        return false;
    }

    std::stringstream ss(valStr);
    ss >> val;

    if (ss.fail()) {
        errorMsg = "bad input => " + valStr;
        return false;
    }

    std::string remaining;
    ss >> remaining;
    if (!remaining.empty()) {
        errorMsg = "bad input => " + valStr;
        return false;
    }

    if (val < 0) {
        errorMsg = "not a positive number.";
        return false;
    }

    if (val > 1000) {
        errorMsg = "too large a number.";
        return false;
    }

    return true;
}

bool BitcoinExchange::loadDatabase(const std::string &dbPath) {
    std::ifstream dbFile(dbPath.c_str());
    if (!dbFile.is_open()) {
        std::cerr << "Error: could not open database file." << std::endl;
        return false;
    }

    std::string line;
    bool isHeader = true;
    while (std::getline(dbFile, line)) {
        line = trim(line);
        if (line.empty())
            continue;
        if (isHeader) {
            isHeader = false;
            if (line == "date,exchange_rate")
                continue;
        }

        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos)
            continue;

        std::string dateStr = trim(line.substr(0, commaPos));
        std::string rateStr = trim(line.substr(commaPos + 1));

        if (!isValidDate(dateStr))
            continue;

        std::stringstream ss(rateStr);
        double rate;
        if (ss >> rate) {
            _database[dateStr] = rate;
        }
    }

    dbFile.close();
    if (_database.empty()) {
        std::cerr << "Error: database is empty or invalid." << std::endl;
        return false;
    }
    return true;
}

void BitcoinExchange::processInput(const std::string &inputPath) const {
    std::ifstream inputFile(inputPath.c_str());
    if (!inputFile.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::string trimmedLine = trim(line);
        if (trimmedLine.empty())
            continue;

        if (trimmedLine == "date | value")
            continue;

        size_t pipePos = line.find('|');
        if (pipePos == std::string::npos) {
            std::cout << "Error: bad input => " << trimmedLine << std::endl;
            continue;
        }

        std::string dateStr = trim(line.substr(0, pipePos));
        std::string valStr = trim(line.substr(pipePos + 1));

        if (!isValidDate(dateStr)) {
            std::cout << "Error: bad input => " << dateStr << std::endl;
            continue;
        }

        double val;
        std::string errorMsg;
        if (!parseValue(valStr, val, errorMsg)) {
            std::cout << "Error: " << errorMsg << std::endl;
            continue;
        }

        std::map<std::string, double>::const_iterator it = _database.upper_bound(dateStr);
        if (it == _database.begin()) {
            std::cout << "Error: bad input => " << dateStr << std::endl;
        } else {
            --it;
            std::cout << dateStr << " => " << valStr << " = " << (val * it->second) << std::endl;
        }
    }

    inputFile.close();
}
