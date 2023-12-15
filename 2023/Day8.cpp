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
#include <numeric>

using namespace std::string_literals;

int main()
{
   std::cout << "Part 1: " <<
      std::ranges::fold_left(
         std::forward<std::ostringstream>( (std::ostringstream() << std::ifstream( "input.txt" ).rdbuf()) ).str()
         | std::views::split( "\n\n"s )
         | std::views::transform( []( auto && range ) {return std::string_view( range ); } )
         | std::views::chunk( 2 )
         | std::views::transform( []( auto const strings )
         {
            return std::pair( (strings
                              | std::ranges::to<std::vector>())[0],
                              (strings
                              | std::ranges::to<std::vector>())[1]
                              | std::views::split( '\n' )
                              | std::views::transform( []( auto && range ) {return std::string_view( range ); } )
                              | std::views::filter( []( auto const & str ) {return !str.empty(); } )
                              | std::views::transform( []( auto const & str )
                              {
                                 return std::pair( str.substr( 0, 3 ),
                                 std::pair( str.substr( 7, 3 ), str.substr( 12, 3 ) ) );
                              } )
                              | std::ranges::to< std::map >() );
         } )
         | std::views::transform( []( auto const input )
         {
            auto [dirs, dirmap] = input;
            std::string start( "AAA" );
            return
               std::ranges::fold_left(
                  std::views::iota( 0ll )
                  | std::views::take_while( [&]( int64_t const index )
                  {
                     if ( dirs[index % dirs.size()] == 'L' )
                        start = dirmap[start].first;
                     else
                        start = dirmap[start].second;
                     return start != "ZZZ";
                  } ), 0, []( auto, auto r ) { return r + 2; } );
         } ), 0, std::plus<int64_t>() ) << std::endl;

   std::cout << "Part 2: " <<
      std::ranges::fold_left(
         std::forward<std::ostringstream>( (std::ostringstream() << std::ifstream( "input.txt" ).rdbuf()) ).str()
         | std::views::split( "\n\n"s )
         | std::views::transform( []( auto && range ) {return std::string_view( range ); } )
         | std::views::chunk( 2 )
         | std::views::transform( []( auto const strings )
         {
            return std::make_tuple((strings
                                    | std::ranges::to<std::vector>())[0],
                                    (strings
                                    | std::ranges::to<std::vector>())[1]
                                    | std::views::split( '\n' )
                                    | std::views::transform( []( auto && range ) {return std::string_view( range ); } )
                                    | std::views::filter( []( auto const & str ) {return !str.empty(); } )
                                    | std::views::transform( []( auto const & str )
                                    {
                                       return std::pair( str.substr( 0, 3 ),
                                       std::pair( str.substr( 7, 3 ), str.substr( 12, 3 ) ) );
                                    } )
                                    | std::ranges::to< std::map >(),
                                    (strings
                                    | std::ranges::to<std::vector>())[1]
                                    | std::views::split( '\n' )
                                    | std::views::transform( []( auto && range ) {return std::string_view( range ); } )
                                    | std::views::filter( []( auto const & str ) {return !str.empty(); } )
                                    | std::views::filter( []( auto const & str ) {return str[2] == 'A'; } )
                                    | std::views::transform( []( auto const & str ) { return str.substr( 0, 3 ); } )
                                    | std::ranges::to< std::vector >() );
         } )
         | std::views::transform( []( auto const input )
         {
            auto [dirs, dirmap, startNodes] = input;
            return
               std::ranges::fold_left(
                  startNodes
                  | std::views::transform( [&]( std::string_view const & startStr )
                  {
                     std::string start{ startStr };
                     return std::ranges::fold_left(
                        std::views::iota( 0ll )
                        | std::views::take_while( [&]( int64_t const index )
                        {
                           if ( dirs[index % dirs.size()] == 'L' )
                              start = dirmap[start].first;
                           else
                              start = dirmap[start].second;
                           return start[2] != 'Z';
                        } ), 0, []( auto, auto r ) { return r + 2; } );
                  } ),
                  1ll,
                  []( int64_t const l, int64_t const r )
                  { return std::lcm( l, r ); } );
         } ),
            0, std::plus<int64_t>() ) << std::endl;

   return 0;
}