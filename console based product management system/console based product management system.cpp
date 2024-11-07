#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <ctime>

struct ProductData {
    double price;
    int sales;
    double rating;
};

// Function to check if a year is a leap year
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

class Company {
public:
    std::string name;
    std::vector<ProductData> dailyData;
    Company(const std::string& name) : name(name) {}

    void generateRandomData(int daysInYear, double minPrice, double maxPrice, int minSales, int maxSales, double minRating, double maxRating) {
        for (int i = 0; i < daysInYear; ++i) {
            ProductData data;
            data.price = minPrice + (rand() / (RAND_MAX / (maxPrice - minPrice)));
            data.sales = minSales + rand() % (maxSales - minSales + 1);
            data.rating = minRating + (rand() / (RAND_MAX / (maxRating - minRating)));
            dailyData.push_back(data);
        }
    }

    double averagePrice() const {
        double total = 0;
        for (const auto& data : dailyData) total += data.price;
        return total / dailyData.size();
    }

    double averageSales() const {
        double total = 0;
        for (const auto& data : dailyData) total += data.sales;
        return total / dailyData.size();
    }

    double averageRating() const {
        double total = 0;
        for (const auto& data : dailyData) total += data.rating;
        return total / dailyData.size();
    }

    int bestDayToBuy() const {
        auto minPriceIt = std::min_element(dailyData.begin(), dailyData.end(), [](const ProductData& a, const ProductData& b) {
            return a.price < b.price;
            });
        return std::distance(dailyData.begin(), minPriceIt) + 1;  // +1 for 1-based index
    }
};

void plotDailyData(const std::vector<Company>& companies, int daysInYear) {
    std::cout << "\n--- Daily Price, Sales, and Rating Data ---\n";
    for (int day = 0; day < daysInYear; ++day) {
        std::cout << "Day " << std::setw(3) << day + 1 << " | ";
        for (const auto& company : companies) {
            std::cout << company.name << " (Price: $" << std::fixed << std::setprecision(2) << company.dailyData[day].price
                << ", Sales: " << company.dailyData[day].sales
                << ", Rating: " << company.dailyData[day].rating << ")   ";
        }
        std::cout << "\n";
    }
}

void summarizeAndExport(const std::vector<Company>& companies, const std::string& productName, const std::string& filename) {
    std::ofstream outFile(filename);
    outFile << "Product,Company,Average Price,Average Sales,Average Rating,Best Day to Buy\n";
    std::cout << "\n--- Summary for " << productName << " ---\n";

    for (const auto& company : companies) {
        double avgPrice = company.averagePrice();
        double avgSales = company.averageSales();
        double avgRating = company.averageRating();
        int bestDay = company.bestDayToBuy();

        std::cout << "Company: " << company.name << "\n";
        std::cout << "  Average Price: $" << std::fixed << std::setprecision(2) << avgPrice << "\n";
        std::cout << "  Average Sales: " << avgSales << "\n";
        std::cout << "  Average Rating: " << avgRating << "\n";
        std::cout << "  Best Day to Buy: Day " << bestDay << " (Price: $" << company.dailyData[bestDay - 1].price << ")\n\n";

        outFile << productName << "," << company.name << "," << avgPrice << "," << avgSales << "," << avgRating << "," << bestDay << "\n";
    }

    outFile.close();
    std::cout << "Summary saved to " << filename << "\n";
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::string productName;
    std::cout << "Enter the product you are searching for: ";
    std::cin >> productName;

    int year;
    std::cout << "Enter the year for analysis: ";
    std::cin >> year;

    // Determine the number of days in the specified year
    int daysInYear = isLeapYear(year) ? 366 : 365;
    std::cout << "Analyzing data for " << daysInYear << " days.\n";

    int numCompanies;
    std::cout << "Enter the number of companies: ";
    std::cin >> numCompanies;

    std::vector<Company> companies;
    for (int i = 0; i < numCompanies; ++i) {
        std::string name;
        std::cout << "Enter name for company " << i + 1 << ": ";
        std::cin >> name;
        Company company(name);
        company.generateRandomData(daysInYear, 50.0, 150.0, 100, 1000, 1.0, 5.0);  // Example ranges for price, sales, and rating
        companies.push_back(company);
    }

    plotDailyData(companies, daysInYear);
    summarizeAndExport(companies, productName, "summary.csv");

    return 0;
}
