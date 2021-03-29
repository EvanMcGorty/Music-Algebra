#include<array>
#include"notes_and_intervals.h"
#include<cassert>

//twelve tone equally tempered
namespace twt
{

    using int_accidental_count = int;

    using int_interval_length = int;

    using int_exact_distance = int;

}

namespace ma
{
    template<> twt::int_accidental_count zero_accidental<twt::int_accidental_count>() {return twt::int_accidental_count{0};}

    template<> twt::int_interval_length zero_interval<twt::int_interval_length>() {return twt::int_interval_length{0};}

    template<> twt::int_exact_distance zero_distance<twt::int_exact_distance>() {return twt::int_exact_distance{0};}

}

namespace twt
{

    struct standard_7_note_letter
    {

        static constexpr std::array<char,7> ascending_fifths()
        {
            return {'f','c','g','d','a','e','b'};
        }

        char val;

        constexpr standard_7_note_letter(char a) : val(a) {}

        constexpr int_interval_length diatonic_distance_above(standard_7_note_letter a) const
        {
            if(val>=a.val)
            {
                return int_interval_length{val-a.val};
            }
            else
            {
                return int_interval_length{(val-a.val)+7};
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
            abort();
            return int_accidental_count{-5000}; //error
        }

        constexpr standard_7_note_letter shifted_up_by(int_interval_length a) const
        {
            int distance = a;
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

    using basic_interval = ma::basic_interval<int_interval_length>;
    using accidental = ma::accidental<int_accidental_count>;
    using note_letter = ma::note_letter<int_interval_length,int_accidental_count,standard_7_note_letter>;
    using note_name = ma::note_name<int_interval_length,int_accidental_count,standard_7_note_letter>;
    using pure_interval = ma::pure_interval<int_interval_length,int_accidental_count>;
    using ratio_interval = ma::ratio_interval<int_exact_distance>;
    using exact_interval = ma::exact_interval<int_interval_length,int_accidental_count,int_exact_distance,int_exact_distance>;


    template<typename exact_interval_t, char letter_v, int accidental_v, unsigned int freq_num, unsigned int freq_den>
    struct fractional_frequency_anchor
    {

        constexpr float frequency()
        {
            return freq_num/freq_den;
        }

        constexpr standard_7_note_letter get_note_letter() const
        {
            return standard_7_note_letter{letter_v};
        }
        
        constexpr int_accidental_count get_note_accidental() const
        {
            return int_accidental_count{accidental_v};
        }

        constexpr exact_interval_t adjusted_to(fractional_frequency_anchor const& a, exact_interval_t const& b) const
        {
            if(frequency()!=a.frequency())
            {
                abort(); //exact_interval_t with a fractional_frequency_anchor cannot operate with a changing frequency anchor
            }
            return b;
        }

    };

    using anchor = fractional_frequency_anchor<exact_interval,'a',0,440,1>;
    using exact_note = ma::exact_note<int_interval_length,int_accidental_count,standard_7_note_letter,exact_interval,anchor>;

    constexpr accidental flat{-2}, natural{0}, sharp{2};
    //constexpr accidental natural(0);
    //constexpr accidental sharp(2);

    constexpr note_letter a{'a'}, b{'b'}, c{'c'}, d{'d'}, e{'e'}, f{'f'}, g{'g'};

}

void test()
{
    twt::anchor const anchor;

    //standard_7_note_letter la{'a'};
    twt::note_name x{twt::a,twt::natural};
    twt::note_name y{twt::c,twt::natural};
    twt::note_name z{twt::e,twt::natural};
    twt::pure_interval xy = x/y;
    twt::pure_interval yx = y/x;
    twt::pure_interval xz = x/z;
    twt::pure_interval zx = z/x;
    twt::pure_interval yz = y/z;
    twt::pure_interval zy = z/y;

    twt::exact_interval xye{xy,twt::ratio_interval{7},twt::ratio_interval{0}};
    xye.index_ratio<0>();

    twt::exact_note t{x/zy,anchor};

    using test_type = twt::exact_note::interval_type::ratio_interval_type<1>;



    char test;
}