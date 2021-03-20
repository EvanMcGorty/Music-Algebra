#include<iostream>
#include<concepts>

//interval between note names, e.g. a third, a fifth, a second, not a major third or a minor 7th
class basic_interval
{
    using interval_length_t = int;

    interval_length_t length;

    explicit constexpr basic_interval(interval_length_t a) : length(a){}

    constexpr basic_interval operator*(basic_interval rhs) const
    {
        return basic_interval{length+rhs.length};
    }

    constexpr basic_interval operator/(basic_interval rhs) const
    {
        return basic_interval{length-rhs.length};
    }

    constexpr basic_interval inverted() const
    {
        return basic_interval{-length};
    }

};

template<typename t>
concept note_letter_holder = requires (t a, basic_interval b)
{
    {a/a} -> std::same_as<basic_interval>;
    {a*b} -> std::same_as<t>;
    {a/b} -> std::same_as<t>;
};

template<typename t>
concept accidental_count_num = requires (t a)
{
    {a+a} -> std::same_as<t>;
    {a-a} -> std::same_as<t>;
    {a.inverted()} -> std::same_as<t>;
};

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

    constexpr int_accidental_count inverted() const
    {
        return int_accidental_count(-count);
    }

    
};


template<accidental_count_num num_t, note_letter_holder letter_t>
struct note_name
{
    letter_t letter;
    num_t accidental_count;
};




int main()
{
    std::cout << "test" << std::flush;
    char c;
    std::cin>>c;
    return 0;
}