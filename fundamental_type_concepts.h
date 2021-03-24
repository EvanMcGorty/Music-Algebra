
#include<concepts>


//concepts for core types that need to be implemented for a musical system to work with this library


//a number type to represent a basic interval distance
template<typename t>
concept interval_count_num =  
std::copy_constructible<t>
&& requires (t const& a)
{
    {a+a} -> std::same_as<t>;
    {-a} -> std::same_as<t>;
};



//a number type to represent the number and direction of an accidental attached to a note in a given system.
//must be able to be added with itself and be negated
template<typename t>
concept accidental_count_num = 
std::copy_constructible<t>
&& requires (t const& a)
{
    {t{a}};
    {a+a} -> std::same_as<t>;
    {-a} -> std::same_as<t>;
};

//a type that represents a note name, independent of accidentals.
//can be multiplied by a basic_interval to shift it up by that interval
template<typename t, typename accidental_t, typename interval_length_t>
concept note_letter_holder = 
    accidental_count_num<accidental_t> 
    && interval_count_num<interval_length_t>
    && requires (t const& a, basic_interval<interval_length_t> const& b)
{
    {a.diatonic_distance_above(a)} -> std::convertible_to<basic_interval<interval_length_t>>; //should preferably only return positive intervals
    {a.shifted_up_by(b)} -> std::same_as<t>; //naturally shifts down if b is a negative interval
	{a.accidental_difference_from(a)} -> std::convertible_to<accidental_t>;
};

