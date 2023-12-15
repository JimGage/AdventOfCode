#include <iostream>
#include <fstream>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <vector>
#include <string_view>
#include <tuple>
#include <set>
#include <map>

int main()
{
   std::cout << "Part 1: " << std::ranges::fold_left(
      std::forward<std::ostringstream>( (std::ostringstream() << std::ifstream( "input.txt" ).rdbuf()) ).str()
      | std::views::split( 10 )
      | std::views::filter( []( auto const & str ) { return !str.empty(); } )
      | std::views::transform( []( auto && range ) { return std::string( std::string_view( range ) ); } )
      | std::views::transform( []( auto const & str ) { return str.substr( str.find_first_of( ":" ) + 2 ); } )
      | std::views::transform( []( auto const & str ) { return std::pair{ str.substr( 0, str.find_first_of( "|" ) ),
                                 str.substr( str.find_first_of( "|" ) + 1 ) }; } )
      | std::views::transform( []( auto const & input )
      {
         auto [in_nums, in_wins] = input;
         std::set<int32_t> wins =
            in_wins
            | std::views::split( 32 )
            | std::views::filter( []( auto const & str ) { return !str.empty(); } )
            | std::views::transform( []( auto && range ) { return std::string( std::string_view( range ) ); } )
            | std::views::transform( []( auto const & str ) { return std::stoi( str ); } )
            | std::ranges::to<std::set>();

         return std::ranges::fold_left(
            in_nums
            | std::views::split( 32 )
            | std::views::filter( []( auto const & str ) { return !str.empty(); } )
            | std::views::transform( []( auto && range ) { return std::string( std::string_view( range ) ); } )
            | std::views::transform( []( auto const & str ) { return std::stoi( str ); } )
            | std::views::transform( [&]( auto const & num ) { return wins.find( num ) != wins.end() ? 1 : 0; } ),
            0ll, std::plus<int64_t>() );
      } )
      | std::views::filter( []( auto const & num ) { return num != 0; } )
      , 0ll, []( auto const & l, auto const r ) { return l + (1ll << (r - 1)); } ) << std::endl;

   std::map< int32_t, int64_t > card_counts;
   std::cout << "Part 2: " << std::ranges::fold_left(
      std::forward<std::ostringstream>( (std::ostringstream() << std::ifstream( "input.txt" ).rdbuf()) ).str()
      | std::views::split( 10 )
      | std::views::filter( []( auto const & str ) { return !str.empty(); } )
      | std::views::transform( []( auto && range ) { return std::string( std::string_view( range ) ); } )
      | std::ranges::to<std::vector >()
      | std::views::reverse
      | std::views::transform( []( auto const & str ) 
      { 
         return std::tuple
         {
               std::stoi( str.substr( str.find_first_of( " " ), 4 ) ),
               str.substr( str.find_first_of( ":" ) + 2, str.find_first_of( "|" ) - 11 ),
               str.substr( str.find_first_of( "|" ) + 1 ) 
         }; 
      } )
      | std::views::transform( [&]( auto const & input )
      {
         auto [index, in_nums, in_wins] = input;
         std::set<int32_t> wins =
            in_wins
            | std::views::split( 32 )
            | std::views::filter( []( auto const & str ) { return !str.empty(); } )
            | std::views::transform( []( auto && range ) { return std::string( std::string_view( range ) ); } )
            | std::views::transform( []( auto const & str ) { return std::stoi( str ); } )
            | std::ranges::to<std::set>();
         int64_t const win_count = std::ranges::fold_left(
            in_nums
            | std::views::split( 32 )
            | std::views::filter( []( auto const & str ) { return !str.empty(); } )
            | std::views::transform( []( auto && range ) { return std::string( std::string_view( range ) ); } )
            | std::views::transform( []( auto const & str ) { return std::stoi( str ); } )
            | std::views::transform( [&]( auto const & num ) { return wins.find( num ) != wins.end() ? 1 : 0; } ),
            0ll, std::plus<int64_t>() );
         int64_t const card_count = std::ranges::fold_left(
            std::ranges::iota_view( index, index + win_count )
            | std::views::transform( [&]( auto const & value )
            { return card_counts.find( value + 1 ) == card_counts.end() ? 0 : card_counts.find( value + 1 )->second; } ),
            win_count, std::plus<int64_t>() );
         card_counts.insert( { index,card_count } );
         return card_count + 1;
      } )
      , 0ll, std::plus<int64_t>() ) << std::endl;

   return 0;
}