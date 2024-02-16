/*
* You are given series of dates on which trading happened and the price of the stock on each date. 
*  Write a C++ program which will find out the date on which the stock must be bought and 
*  the date on which the stock must be sold for the maximum profit 
*/

#include <iostream>
#include <format>
#include <string>
#include <vector>

// Function to find the buy and sell dates for maximum profit
void static findMaxProftDates(std::vector<std::pair<std::string,double>>& prices) {
    std::string buyDate = prices[0].first, sellDate = prices[0].first;
    double maxProfit = 0;
    double minPrice = prices[0].second;

    for (int i = 1; i < prices.size(); ++i) {
        if (prices[i].second < minPrice) {
            minPrice = prices[i].second;
            buyDate = prices[i].first;
        }
        else if (prices[i].second - minPrice > maxProfit) {
            maxProfit = prices[i].second - minPrice;
            sellDate = prices[i].first;
        }
    }
    std::cout << std::format("The buy date is {}, the sell date is {} and the max profit is {}\n", buyDate, sellDate, maxProfit);
}
int main() {

    std::vector<std::pair<std::string, double>> prices = { 
        {"01/01/2024",696},
        {"02/01/2024",100},
        {"03/01/2024",180},
        {"04/01/2024",260},
        {"05/01/2024",310},
        {"08/01/2024",40},
        {"09/01/2024",795},
        {"10/01/2024",535},
        {"11/01/2024",695} };
    findMaxProftDates(prices);

    return 0;
}
