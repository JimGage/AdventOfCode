#include <iostream>
#include <fstream>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <vector>
#include <string_view>
#include <tuple>

int main()
{
   std::cout << "Part 1: " << std::ranges::fold_left(
      std::forward<std::ostringstream>( (std::ostringstream() << std::ifstream( "input.txt" ).rdbuf()) ).str()
      | std::views::split( 10 )
      | std::views::filter( []( auto const & str ) { return !str.empty(); } )
      | std::views::transform( []( auto && range ) { return std::string( std::string_view( range ) ); } )
      | std::views::filter( []( auto const & str ) -> bool
      {
         return std::ranges::fold_left(
            str.substr( str.find_first_of( ':' ) + 2 )
            | std::views::filter( []( char c ) { return c != ';' && c != ','; } )
            | std::ranges::to< std::string >()
            | std::views::split( ' ' )
            | std::views::transform( []( auto && range ) {return std::string( std::string_view( range ) ); } )
            | std::views::chunk( 2 )
            | std::views::transform( []( auto const & values ) 
            { auto a = std::ranges::to<std::vector>( values ); return std::pair{ std::stoi( a[0] ), a[1] }; } )
            | std::views::transform( []( auto const & values ) -> bool
            {
               auto const & [value, str] = values;
               return 
                  (str == "red"   && value <= 12) ||
                  (str == "green" && value <= 13) ||
                  (str == "blue"  && value <= 14);
            } ), true, []( bool const l, bool const r ) { return l && r; } );
      } )
      | std::views::transform( []( std::string const & str ) -> int64_t 
           { return std::stoi( str.substr( str.find_first_of( ' ' ) + 1, str.find_first_of( ':' ) ) );  } ),
         0, std::plus<int64_t>() ) << std::endl;

   std::cout << "Part 2: " << std::ranges::fold_left(
      std::forward<std::ostringstream>( (std::ostringstream() << std::ifstream( "input.txt" ).rdbuf()) ).str()
      | std::views::split( 10 )
      | std::views::filter( []( auto const & str ) { return !str.empty(); } )
      | std::views::transform( []( auto && range ) { return std::string( std::string_view( range ) ); } )
      | std::views::transform( []( auto const & str ) 
      {
         using tvalues = std::tuple<int, int, int>;
         return std::ranges::fold_left(
            str.substr( str.find_first_of( ':' ) + 2 )
            | std::views::filter( []( char c ) { return c != ';' && c != ','; } )
            | std::ranges::to< std::string >()
            | std::views::split( ' ' )
            | std::views::transform( []( auto && range ) {return std::string( std::string_view( range ) ); } )
            | std::views::chunk( 2 )
            | std::views::transform( []( auto const & values ) 
               { auto a = std::ranges::to<std::vector>( values ); return std::pair{ std::stoi( a[0] ), a[1] }; } )
            | std::views::transform( []( auto const & values) -> tvalues
            {
               auto [value, str] = values;
               return { str == "red" ? value : 0,str == "green" ? value : 0,str == "blue" ? value : 0 };
            } ), std::make_tuple(0,0,0), [](auto const & l, auto const r) -> tvalues
               { return { std::max( std::get<0>( l ), std::get<0>( r ) ),
                          std::max( std::get<1>( l ), std::get<1>( r ) ),
                          std::max( std::get<2>( l ), std::get<2>( r ) ) }; } );
      } )
      | std::views::transform( []( auto const & values ) -> int64_t
                               { auto [r, g, b] = values;  return r * g * b; } ),
         0, std::plus<int64_t>() ) << std::endl;
   return 0;
}