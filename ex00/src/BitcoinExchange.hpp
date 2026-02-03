#pragma once

#include <map>
#include <string>

class BitcoinExchange
{
  public:
    BitcoinExchange();

    BitcoinExchange(const BitcoinExchange &other);

    BitcoinExchange(BitcoinExchange &&other) noexcept;

    ~BitcoinExchange();

    BitcoinExchange &operator=(const BitcoinExchange &other);

    BitcoinExchange &operator=(BitcoinExchange &&other) noexcept;

    void processInput(const std::string &input) const;

    [[nodiscard]] double getRate(const std::string &date) const;

  private:
    std::map<std::string, double> _data;

    void loadData(const std::string &input);
    void process(const std::string &line) const;

    [[nodiscard]] std::string parseDate(const std::string &date) const;
    [[nodiscard]] std::string trim(const std::string &str) const;
};