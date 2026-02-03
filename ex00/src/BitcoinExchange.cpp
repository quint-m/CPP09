#include "BitcoinExchange.hpp"
#include <array>
#include <ctime>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>

BitcoinExchange::BitcoinExchange()
{
    loadData("data.csv");
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : _data(other._data)
{
}

BitcoinExchange::BitcoinExchange(BitcoinExchange &&other) noexcept : _data(std::move(other._data))
{
}

BitcoinExchange::BitcoinExchange(BitcoinExchange &&other) noexcept : data(std::move(other.data))
{
}

BitcoinExchange::~BitcoinExchange()
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (this != &other)
    {
        _data = other._data;
    }
    return *this;
}

BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange &&other) noexcept
{
    if (this != &other)
    {
        _data = std::move(other._data);
    }
    return *this;
}

BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }
    data = std::move(other.data);
    return *this;
}

void BitcoinExchange::processInput(const std::string &input) const
{
    std::ifstream file(input);
    std::string line;
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file");
    }
    std::getline(file, line);
    if (line != "date | value")
    {
        throw std::runtime_error("Invalid input file format");
    }
    while (std::getline(file, line))
    {
        process(line);
    }
}

void BitcoinExchange::process(const std::string &line) const
{
    size_t pipePos = line.find('|');
    if (pipePos == std::string::npos)
    {
        std::cerr << "Error: bad input => " << line << "\n";
        return;
    }

    try
    {
        std::string dateStr = trim(line.substr(0, pipePos));
        std::string valueStr = trim(line.substr(pipePos + 1));
        std::string date = parseDate(dateStr);
        double value = std::stod(valueStr);
        if (value < 0)
        {
            throw std::runtime_error("not a positive number.");
        }
        if (value > 1000)
        {
            throw std::runtime_error("too large a number.");
        }
        std::cout << date.data() << " => " << value << " = ";
        double rate = getRate(date);
        std::cout << value * rate << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return;
    }
}

void BitcoinExchange::loadData(const std::string &input)
{
    std::ifstream file(input);
    std::string line;

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open data file");
    }
    std::getline(file, line);
    if (line != "date,exchange_rate")
    {
        throw std::runtime_error("Invalid file format");
    }
    while (std::getline(file, line))
    {
        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos)
        {
            continue;
        }
        try
        {
            std::string date = parseDate(line.substr(0, commaPos));
            double rate = std::stod(line.substr(commaPos + 1));
            _data[date] = rate;
        }
        catch (const std::exception &e)
        {
            continue;
        }
    }
}

double BitcoinExchange::getRate(const std::string &date) const
{
    auto it = _data.find(date);
    if (it != _data.end())
    {
        return it->second;
    }

    auto lower = _data.lower_bound(date);
    if (lower != _data.begin())
    {
        --lower;
        return lower->second;
    }
    throw std::runtime_error("No data available for the given date");
}

std::string BitcoinExchange::parseDate(const std::string &date) const
{
    if (date.length() != 10)
    {
        throw std::runtime_error("Invalid date format");
    }

    std::tm tm = {};
    char *res = strptime(date.c_str(), "%Y-%m-%d", &tm);
    if (res == nullptr || *res != '\0')
    {
        throw std::runtime_error("Invalid date format");
    }

    std::time_t time = std::mktime(&tm);
    if (time == -1)
    {
        throw std::runtime_error("Invalid date");
    }

    std::array<char, 11> buffer{}; // YYYY-MM-DD + null terminator
    size_t written = std::strftime(buffer.data(), buffer.size(), "%Y-%m-%d", &tm);
    if (date != buffer.data() || written != 10)
    {
        throw std::runtime_error("Date formatting error");
    }

    return {buffer.data()};
}

std::string BitcoinExchange::trim(const std::string &str) const
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
    {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}