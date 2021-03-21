#include<iostream>
#include"note_name.h"

class int_accidental_count
{
    int count;
public:
    explicit constexpr int_accidental_count(int a) : count(a) {}

    constexpr int_accidental_count operator+(int_accidental_count rhs) const
    {
        return int_accidental_count{count+rhs.count};
    }
    constexpr int_accidental_count operator-(int_accidental_count rhs) const
    {
        return int_accidental_count{count-rhs.count};
    }

    constexpr int_accidental_count operator-() const
    {
        return int_accidental_count{-count};
    }
};



int main()
{
    std::cout << "test" << std::flush;
    char c;
    std::cin>>c;
    return 0;
}