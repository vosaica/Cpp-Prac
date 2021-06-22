#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class InRange
{
public:
    InRange(std::string::size_type low, std::string::size_type high) : low(low), high(high)
    {
    }

    bool operator()(const std::string& str) const
    {
        return str.length() >= low && str.length() <= high;
    }

private:
    std::string::size_type low;
    std::string::size_type high;
};

int main()
{
    InRange range(1, 9);
    std::vector<size_t> result(10, 0);
    std::ifstream ifs("14.39_In.txt");
    std::string word;

    while (ifs >> word)
    {
        if (range(word))
        {
            ++result[word.length()];
        }
        else
        {
            ++result[0];
        }
    }

    int count = 1;
    for (auto iter = result.begin() + 1; iter != result.end(); ++iter)
    {
        std::cout << count++ << ": " << *iter << '\n';
    }
    std::cout << "10 & above 10: " << result[0] << std::endl;

    return 0;
}
