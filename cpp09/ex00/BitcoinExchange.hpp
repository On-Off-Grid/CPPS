#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <map>

class BitcoinExchange {
private:
    std::map<std::string, double> _database;

    static std::string trim(const std::string &str);
    static bool isLeapYear(int year);

public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange &other);
    BitcoinExchange &operator=(const BitcoinExchange &other);
    ~BitcoinExchange();

    bool loadDatabase(const std::string &dbPath);
    void processInput(const std::string &inputPath) const;

    static bool isValidDate(const std::string &dateStr);
    static bool parseValue(const std::string &valStr, double &val, std::string &errorMsg);
};

#endif
