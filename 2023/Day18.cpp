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
   std::cout << "Part 1: " <<
      std::ranges::fold_left(
         std::ranges::fold_left(
            std::forward<std::ostringstream>( (std::ostringstream() << std::ifstream( "input.txt" ).rdbuf()) ).str()
            | std::views::split( 10 )
            | std::views::filter( []( auto const & str ) { return !str.empty(); } )
            | std::views::transform( []( auto && range ) { return std::string( std::string_view( range ) ); } )
            | std::views::transform( []( auto const & str ) -> std::pair<int64_t, int64_t>
            {
               int32_t const length = std::stoi( str.substr( 2, 2 - str.find_first_of( ' ', 2 ) ) );
               switch ( str[0] )
               {
               case 'U': return { 0, -length };
               case 'D': return { 0, length };
               case 'L': return { -length, 0 };
               case 'R': return { length, 0 };
               }
               return { 0,0 };
            } 
         ), std::vector< std::pair< std::pair< int64_t, int64_t >, int64_t > >( 1, { {0,0},0 } ), []( auto vec, auto nums )
         {
            vec.push_back( { { vec.back().first.first + nums.first, vec.back().first.second + nums.second}, nums.first + nums.second } );
            return vec;
         } )
         | std::views::slide( 2 )
         | std::views::transform( []( auto a )
         {
            return (a[0].first.first * a[1].first.second - a[0].first.second * a[1].first.first)
               + std::abs( a[1].second );
         }
      ) , 2ll, std::plus<int64_t>() ) / 2 << std::endl;

   std::cout << "Part 2: " <<
      std::ranges::fold_left(
         std::ranges::fold_left(
            std::forward<std::ostringstream>( (std::ostringstream() << std::ifstream( "input.txt" ).rdbuf()) ).str()
            | std::views::split( 10 )
            | std::views::filter( []( auto const & str ) { return !str.empty(); } )
            | std::views::transform( []( auto && range ) { return std::string( std::string_view( range ) ); } )
            | std::views::transform( []( auto const & str ) -> std::pair<int64_t, int64_t>
            {
               std::string instruction = str.substr( str.find_first_of( "#" ) + 1 );
               int32_t const length = std::stoi( instruction.substr( 0, 5 ), nullptr, 16 );
               switch ( instruction[5] )
               {
               case '3': return { 0, -length };
               case '1': return { 0, length };
               case '2': return { -length, 0 };
               case '0': return { length, 0 };
               }
               return { 0,0 };
            }
         ), std::vector< std::pair< std::pair< int64_t, int64_t >, int64_t > >( 1, { {0,0},0 } ), []( auto vec, auto nums )
         {
            vec.push_back( { { vec.back().first.first + nums.first, vec.back().first.second + nums.second}, nums.first + nums.second } );
            return vec;
         } )
         | std::views::slide( 2 )
         | std::views::transform( []( auto a )
         {
            return (a[0].first.first * a[1].first.second - a[0].first.second * a[1].first.first)
               + std::abs( a[1].second );
         }
      ), 2ll, std::plus<int64_t>() ) / 2 << std::endl;

   return 0;
}
