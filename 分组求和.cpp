#include <map>
#include <string>
#include <iostream>
#include <vector>
int main()
{
    std::vector<std::pair<std::string, int>> sales = {
        {"Electronics", 100}, {"Clothing", 50}, {"Electronics", 150},
        {"Clothing", 80}, {"Groceries", 40}, {"Electronics", 200}};

    std::map<std::string, std::string> category_details = {
        {"Electronics", "Consumer devices like phones and laptops"},
        {"Clothing", "Apparel and accessories"},
        {"Groceries", "Daily household items"}};

    std::map<std::string,std::pair<int,std::string>> category_totals;
    // 分组聚合，采用叠加方式计算
    for(const auto& sale:sales)
    {
        // std::cout<<sale.first<<" "<<sale.second <<std::endl;
        category_totals[sale.first].first+=sale.second;
        category_totals[sale.first].second=category_details[sale.first];

    }

    // 输出结果
    for(const auto&[category,detail]:category_totals)
    {
        std::cout<<"Category:" << category<< ",Total: "<< detail.first<< "  details :"<< detail.second << std::endl;
    }
    return 0;
}
