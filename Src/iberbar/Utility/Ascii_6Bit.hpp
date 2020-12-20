#pragma once

#include <iberbar/Utility/Platform.h>

namespace iberbar
{
    class Ascii_6Bit
    {
    public:
        // encodes ASCII characters to 6bit encoding
        static constexpr unsigned char encoding_table[] = {
            /*     ..0 ..1 ..2 ..3 ..4 ..5 ..6 ..7 ..8 ..9 ..A ..B ..C ..D ..E ..F  */
            /* 0.. */  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            /* 1.. */  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            /* 2.. */  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            /* 3.. */  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 0, 0, 0, 0, 0,
            /* 4.. */  0, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
            /* 5.. */ 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 0, 0, 0, 0, 37,
            /* 6.. */  0, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
            /* 7.. */ 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 0, 0, 0, 0 };

        // decodes 6bit characters to ASCII
        static constexpr char decoding_table[] = " 0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ_"
            "abcdefghijklmnopqrstuvwxyz";

        static constexpr uint64 next_interim( uint64 current, size_t char_code ) {
            return (current << 6) | encoding_table[ (char_code <= 0x7F) ? char_code : 0 ];
        }

        static constexpr uint64 atom_val( const char* cstr, uint64 interim = 0xF ) {
            return (*cstr == '\0') ?
                interim :
                atom_val( cstr + 1,
                    next_interim( interim, static_cast<size_t>(*cstr) ) );
        }


        /// The value type of atoms.
        enum class atom_value : uint64_t {
            /// @cond PRIVATE
            dirty_little_hack = 31337
            /// @endcond
        };

        ///// @relates atom_value
        //static std::string to_string( atom_value x );

        ///// @relates atom_value
        //static std::ostream& operator<<( std::ostream& out, atom_value x );

        ///// @relates atom_value
        //static atom_value to_lowercase( atom_value x );

        ///// @relates atom_value
        ////atom_value atom_from_string( string_view x );

        ///// @relates atom_value
        //static int compare( atom_value x, atom_value y );

        /// Creates an atom from given string literal.
        template <size_t Size>
        static constexpr atom_value atom( char const (&str)[ Size ] ) {
            // last character is the NULL terminator
            static_assert(Size <= 11, "only 10 characters are allowed");
            return static_cast<atom_value>(atom_val( str ));
        }

        /// Creates an atom from given string literal and return an integer
        /// representation of the atom..
        template <size_t Size>
        static constexpr uint64 atom_uint( char const (&str)[ Size ] ) {
            static_assert(Size <= 11, "only 10 characters are allowed");
            return atom_val( str );
        }

        ///// Converts an atom to its integer representation.
        //static constexpr uint64_t atom_uint( atom_value x ) {
        //    return static_cast<uint64_t>(x);
        //}

        ///// Lifts an `atom_value` to a compile-time constant.
        //template <atom_value V>
        //struct atom_constant {
        //    constexpr atom_constant() {
        //        // nop
        //    }

        //    /// Returns the wrapped value.
        //    constexpr operator atom_value() const {
        //        return V;
        //    }

        //    /// Returns the wrapped value as its base type.
        //    static constexpr uint64_t uint_value() {
        //        return static_cast<uint64_t>(V);
        //    }

        //    /// Returns the wrapped value.
        //    static constexpr atom_value get_value() {
        //        return V;
        //    }

        //    /// Returns an instance *of this constant* (*not* an `atom_value`).
        //    static const atom_constant value;
        //};

        //template <class T>
        //struct is_atom_constant {
        //    static constexpr bool value = false;
        //};

        //template <atom_value X>
        //struct is_atom_constant<atom_constant<X>> {
        //    static constexpr bool value = true;
        //};

        //template <atom_value V>
        //static std::string to_string( const atom_constant<V>& ) {
        //    return to_string( V );
        //}

        //template <atom_value V>
        //const atom_constant<V> atom_constant<V>::value = atom_constant<V>{};
    };
    

    //} // namespace caf


}
//
//namespace std {
//
//    template <>
//    struct hash<iberbar::atom_value> {
//        size_t operator()( iberbar::atom_value x ) const {
//            hash<uint64_t> f;
//            return f( static_cast<uint64_t>(x) );
//        }
//    };
//
//} // namespace std
