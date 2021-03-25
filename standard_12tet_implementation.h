#include<array>
#include"notes_and_intervals.h"
#include<cassert>


using int_accidental_count = int;

using int_interval_length = int;

using int_exact_distance = int;

class standard_7_note_letter
{
private:

    static constexpr std::array<char,7> ascending_fifths()
    {
        return {'f','c','g','d','a','e','b'};
    }

    char val;

public:

    constexpr standard_7_note_letter(char a) : val(a) {}

    constexpr basic_interval<int_interval_length> diatonic_distance_above(standard_7_note_letter a) const
    {
        if(val>=a.val)
        {
            return basic_interval{int_interval_length{val-a.val}};
        }
        else
        {
            return basic_interval{int_interval_length{(val-a.val)+7}};
        }
    }

    constexpr int_accidental_count accidental_difference_from(standard_7_note_letter tar) const
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
        return int_accidental_count{-5000}; //error
    }

    constexpr standard_7_note_letter shifted_up_by(basic_interval<int_interval_length> a) const
    {
        int distance = int(a.length);
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

using ttet_accidental = accidental<int_accidental_count>;
using ttet_note_letter = note_letter<int_interval_length,int_accidental_count,standard_7_note_letter>;
using ttet_note_name = note_name<int_interval_length,int_accidental_count,standard_7_note_letter>;
using ttet_pure_interval = pure_interval<int_interval_length,int_accidental_count>;
using ttet_ratio_interval = ratio_interval<int_exact_distance>;
using ttet_exact_interval = exact_interval<int_interval_length,int_accidental_count,int_exact_distance>;

constexpr ttet_accidental flat(-2);
constexpr ttet_accidental natural(0);
constexpr ttet_accidental sharp(2);



void test()
{

    //standard_7_note_letter la{'a'};
    ttet_note_name x{ttet_note_letter{'a'},natural};
    ttet_note_name y{ttet_note_letter{'c'},natural};
    ttet_note_name z{ttet_note_letter{'e'},natural};
    ttet_pure_interval xy = x/y;
    ttet_pure_interval yx = y/x;
    ttet_pure_interval xz = x/z;
    ttet_pure_interval zx = z/x;
    ttet_pure_interval yz = y/z;
    ttet_pure_interval zy = z/y;

    ttet_exact_interval xye{xy,ttet_ratio_interval{7}};
    xye.index_ratio<0>();

    ttet_note_name t = x/zy;




    char test;
}