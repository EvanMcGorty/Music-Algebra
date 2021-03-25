#include<concepts>

namespace ma
{

    //concepts for core types that need to be implemented for a musical system to work with this library



    //a number type to represent a basic interval distance
    template<typename t>
    concept interval_count_num =  
    std::copy_constructible<t>
    && requires (t const& a)
    {
        {a+a} -> std::convertible_to<t>;
        {-a} -> std::convertible_to<t>;
    };


    //a number type to represent the number and direction of an accidental attached to a note in a given system.
    template<typename t>
    concept accidental_count_num = 
    std::copy_constructible<t>
    && requires (t const& a)
    {
        {a+a} -> std::convertible_to<t>;
        {-a} -> std::convertible_to<t>;
    };

    //a type that represents a note name, independent of accidentals.
    //can be multiplied by a basic_interval to shift it up by that interval
    template<typename t, typename accidental_t, typename interval_t>
    concept note_letter_holder = 
        std::copy_constructible<t>
        && accidental_count_num<accidental_t>
        && requires (t const& a, interval_t const& b)
    {
        {a.diatonic_distance_above(a)} -> std::convertible_to<interval_t>; //should preferably only return positive intervals
        {a.shifted_up_by(b)} -> std::same_as<t>; //naturally shifts down if b is a negative interval
        {a.accidental_difference_from(a)} -> std::convertible_to<accidental_t>;
    };


    //an exact distance between two frequencies
    template<typename t>
    concept exact_distance_num = 
    std::copy_constructible<t>
    && requires (t const& a)
    {
        {a+a} -> std::convertible_to<t>;
        {-a} -> std::convertible_to<t>;
    };


    //an exact frequency
    template<typename t, typename exact_distance_t>
    concept exact_frequency_num = 
        std::copy_constructible<t>
        && exact_distance_num<exact_distance_t>
        && requires (t const& a, exact_distance_t const& b)
    {
        {a.exact_distance_above(a)} -> std::convertible_to<exact_distance_t>;
        {a.shifted_up_by(b)} -> std::convertible_to<t>;
    };


}