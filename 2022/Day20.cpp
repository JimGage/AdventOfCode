#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

void solve( int32_t const iterationCount, int64_t const multiplier )
{
   auto const intValues = std::forward<std::ostringstream>( (std::ostringstream() << std::ifstream( "input.txt" ).rdbuf()) ).str()
         | std::views::split( '\n' )
         | std::views::filter( []( auto const& str ) { return !str.empty(); } )
         | std::views::transform( []( auto&& range ) {return std::string( std::string_view( range ) ); } )
         | std::views::transform( [&]( auto const& str ) -> int64_t { return std::stoll( str ) * multiplier; } )
         | std::ranges::to< std::vector >();

   // used for lookup
   std::vector< size_t > indices;
   for (size_t i = 0; i < intValues.size(); ++i) indices.push_back( i );

   auto puzzleValues = intValues;
   size_t const modValue = puzzleValues.size() - 1;

   for (int32_t it = 0; it < iterationCount; ++it)
   {
      for (size_t index = 0; index < intValues.size(); ++index)
      {
         int64_t const value = intValues[index];
         size_t const offset = value < 0 ? (modValue - (std::abs( value ) % modValue)) : (value % modValue);

         auto found = std::find( indices.begin(), indices.end(), index );
         size_t const eraseOffset = std::distance( indices.begin(), found );
         size_t const insertOffset = ((eraseOffset + offset) % (modValue));
         puzzleValues.erase( puzzleValues.begin() + eraseOffset );
         puzzleValues.insert( puzzleValues.begin() + insertOffset, value );
         indices.erase( indices.begin() + eraseOffset );
         indices.insert( indices.begin() + insertOffset, index );
      }
   }

   auto const findZero = std::find( puzzleValues.begin(), puzzleValues.end(), 0 );
   size_t const baseOffset = std::distance( puzzleValues.begin(), findZero );
   std::cout <<
      *(puzzleValues.begin() + (baseOffset + 1000) % puzzleValues.size()) +
      *(puzzleValues.begin() + (baseOffset + 2000) % puzzleValues.size()) +
      *(puzzleValues.begin() + (baseOffset + 3000) % puzzleValues.size()) << std::endl;
}

int main( int argc, char* argv[] )
{
   solve( 1, 1 );
   solve( 10, 811589153ll );
   return 0;
}
