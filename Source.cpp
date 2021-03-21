#include<iostream>
#include<array>
#include"note_name.h"


class int_accidental_count
{
    //every 2 count is 1 sharp or flat.
    //1 count is the accidental interval between two notes in the same key, sort of like a half sharp, but only for intervals.
    int count;
public:
    explicit constexpr int_accidental_count(int a) : count(a) {}

    constexpr int_accidental_count operator+(int_accidental_count rhs) const
    {
        return int_accidental_count{count+rhs.count};
    }

    constexpr int_accidental_count operator-() const
    {
        return int_accidental_count{-count};
    }
};

class standard_7_note_letter
{


private:

    static constexpr std::array<char,7> ascending_fifths()
    {
        return {'f','c','g','d','a','e','b'};
    }

    char val;

public:

    standard_7_note_letter(char a) : val(a) {}

    basic_interval diatonic_distance_above(standard_7_note_letter a) const
    {
        if(val>=a.val)
        {
            return basic_interval{val-a.val};
        }
        else
        {
            return basic_interval{(val-a.val)+7};
        }
    }

    int_accidental_count accidental_difference_from(standard_7_note_letter tar) const
    {
        for(size_t i=0;i!=ascending_fifths().size();++i)
        {
            if(tar.val==ascending_fifths()[i]&&(val==ascending_fifths()[i]))
            {
                return int_accidental_count(0);
            }
            else if(tar.val==ascending_fifths()[i]) 
            {
                return int_accidental_count(1);
            }
            else if(val==ascending_fifths()[i])
            {
                return int_accidental_count(-1);
            }
        }
    }

    standard_7_note_letter shifted_up_by(basic_interval a) const
    {
        int distance = a.length;
        while(distance<0)
        {
            distance+=7;
        }
        while(distance>=7)
        {
            distance-=7;
        }
        char newval=val+distance;
        if(newval>'g')
        {
            newval-=7;
        }
        return standard_7_note_letter{newval};
    }

};

using basic_note_name = note_name<int_accidental_count,standard_7_note_letter>;
using basic_pure_interval = pure_interval<int_accidental_count>;

constexpr int_accidental_count flat(-2);
constexpr int_accidental_count natural(0);
constexpr int_accidental_count sharp(2);


int main()
{
    basic_note_name x{'a',natural};
    basic_note_name y{'c',natural};
    basic_note_name z{'e',natural};
    basic_pure_interval xy = x/y;
    basic_pure_interval yx = y/x;
    basic_pure_interval xz = x/z;
    basic_pure_interval zx = z/x;
    basic_pure_interval yz = y/z;
    basic_pure_interval zy = z/y;

    basic_note_name t = x/zy;
    

    std::cout << "test" << std::flush;
    char c;
    std::cin>>c;
    return 0;
}