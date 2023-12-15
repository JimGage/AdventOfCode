#include <iostream>
#include <fstream>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <vector>
#include <string_view>
#include <tuple>
#include <map>
#include <array>

int main()
{
   std::cout << "Part 1: " << 
      std::ranges::fold_left(
      std::forward<std::ostringstream>( (std::ostringstream() << std::ifstream( "input.txt" ).rdbuf()) ).str()
      | std::views::filter( []( char const c ) { return c > 13; } )
      | std::ranges::to<std::string>()
      | std::views::split( ',' )
      | std::views::transform( []( auto && range ) {return std::string_view( range ); } )
      | std::views::transform( []( auto const & str ) 
         { return std::ranges::fold_left( str, 0, []( int code, char ascii ) {return ((code + ascii) * 17) & 255; } );
      } ), 0, std::plus<int64_t>() ) << std::endl;

   std::cout << "Part 2: " <<
      std::ranges::fold_left(
         std::ranges::fold_left(
            std::forward<std::ostringstream>( (std::ostringstream() << std::ifstream( "input.txt" ).rdbuf()) ).str()
            | std::views::filter( []( char const c ) { return c > 13; } )
            | std::ranges::to<std::string>()
            | std::views::split( ',' )
            | std::views::transform( []( auto && range ) {return std::string_view( range ); } )
            | std::views::transform( []( auto const & str )
            { 
               return std::make_tuple(
                  std::ranges::fold_left( str, 0, []( int code, char ascii )
                  { return  std::isalpha( ascii ) ? ((code + ascii) * 17) & 255 : code; } ),
                  str
                  | std::views::filter( []( char const c ) { return std::isalpha( c ); } )
                  | std::ranges::to<std::string>(),
                  str
                  | std::views::filter( []( char const c ) { return !std::isalpha( c ); } )
                  | std::ranges::to<std::string>() );
            } ),
            std::array< std::vector< std::pair< std::string, int32_t > >, 256 >(), 
            []( auto boxState, auto const & entry )
            {
               auto const [hash, name, op ] = entry;
               auto & box = boxState[hash];
               auto const found = std::find_if( box.begin(), box.end(), [&]( auto const & lens ) { return lens.first == name; } );
               if ( op[0] == '-' && found != box.end() ) box.erase(found);
               if ( op[0] == '=' && found != box.end() ) found->second = std::stoi( op.substr( 1 ));
               if ( op[0] == '=' && found == box.end() ) box.push_back({name, std::stoi(op.substr(1))});
               return boxState;
            }
         )
         | std::views::enumerate
         | std::views::transform( []( auto const & lensbox ) 
         {
            auto const [boxIndex, box] = lensbox;
            return std::ranges::fold_left(
                  box
                  | std::views::enumerate
                  | std::views::transform( [&]( auto const & lens )
                  {
                     auto const [lensIndex, nameLength] = lens;
                     return (boxIndex + 1) * (lensIndex + 1) * nameLength.second;
                  } ), 0ll, std::plus<int64_t>() );
         } ), 0ll, std::plus<int64_t>() 
      ) << std::endl;
   return 0;
}
