
#include<concepts>

//interval between note names, e.g. a third, a fifth, a second, not something like a major third or a minor seventh.
//simply the number of diatonic steps required to get from one note to another
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

//a number type to represent the number and direction of an accidental attached to a note in a given system.
//must be able to be added and subtracted with itself and be .inverted
template<typename t>
concept accidental_count_num = requires (t a)
{
    {a+a} -> std::same_as<t>;
    {a-a} -> std::same_as<t>;
    {-a} -> std::same_as<t>;
};

//a type that represents a note name, independent of accidentals.
//can be multiplied by a basic_interval to shift it up by that interval
template<typename t, accidental_count_num accidental_t>
concept note_letter_holder = requires (t a, basic_interval b)
{
    {a.distance_above(a)} -> std::same_as<basic_interval>;
    {a.shift_up(b)} -> std::same_as<t>;
	{a.accidental_difference_from(a)} -> std::convertible_to<accidental_t>;
};



template<accidental_count_num num_t>
struct pure_interval
{
	basic_interval count_distance;
	num_t accidental_distance;
}


template<accidental_count_num num_t, note_letter_holder<num_t> letter_t>
struct note_name
{
    letter_t letter;
    num_t accidental_count;
};

