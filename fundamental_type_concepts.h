#include<concepts>

namespace ma
{

    //concepts for core types that need to be implemented for a musical system to work with this library


    template<typename t>
    t zero_interval()
    {
        return t; //this function should be specialized for this type
    }

    
    template<typename t>
    t zero_accidental()
    {
        return t; //this function should be specialized for this type
    }

    
    template<typename t>
    t zero_distance()
    {
        return t; //this function should be specialized for this type
    }

    //a number type to represent a basic interval distance
    template<typename t>
    concept interval_count_num =  
    std::copy_constructible<t>
    && requires (t const& a)
    {
        {a+a} -> std::convertible_to<t>;
        {-a} -> std::convertible_to<t>;
        {zero_interval<t>()} -> std::same_as<t>;
    };

    

    //a number type to represent the number and direction of an accidental attached to a note in a given system.
    template<typename t>
    concept accidental_count_num = 
    std::copy_constructible<t>
    && requires (t const& a)
    {
        {a+a} -> std::convertible_to<t>;
        {-a} -> std::convertible_to<t>;
        {zero_accidental<t>()} -> std::same_as<t>;
    };

    //a type that represents a note name, independent of accidentals.
    //can be multiplied by a basic_interval to shift it up by that interval
    template<typename t, typename accidental_t, typename interval_t>
    concept note_letter_holder = 
        interval_count_num<interval_t>
        && accidental_count_num<accidental_t>
        && std::copy_constructible<t>
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
        {zero_distance<t>()} -> std::same_as<t>;
    };


    //an exact note that acts as a relative anchor note for exact_notes (i.e. a 440 hertz or C4)
    template<typename t, typename letter_t, typename accidental_t, typename interval_t, typename exact_interval_t>
    concept anchor_note = 
        interval_count_num<interval_t>
        && accidental_count_num<accidental_t>
        && note_letter_holder<letter_t,accidental_t,interval_t>
        && std::copy_constructible<t>
        && requires (t const& a, exact_interval_t const& b)
    {
        {a.get_note_letter()} -> std::convertible_to<letter_t>;
        {a.get_note_accidental()} -> std::convertible_to<accidental_t>;
        {a.adjusted_to(a,b)} -> std::convertible_to<exact_interval_t>;
    };


}