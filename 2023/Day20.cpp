#include <iostream>
#include <fstream>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <vector>
#include <string_view>
#include <tuple>
#include <map>
#include <queue>
#include <type_traits>
#include <numeric>

using namespace std;

int main()
{
   cout <<  "Part 1: " << invoke([](auto modules) -> int64_t
   {
      int64_t onCount = 0;
      int64_t offCount = 0;

      for ( int32_t i = 0; i < 1000; ++i )
      {
         queue < tuple< string, string, bool > > broadcastQueue;
         broadcastQueue.push( make_tuple( "button", "broadcaster", false ) );

         while ( !broadcastQueue.empty() )
         {
            auto const [source, target, state] = broadcastQueue.front();
            broadcastQueue.pop();

            if ( state ) onCount++; else offCount++;

            auto & module = modules[target];
            auto & [moduleOp, moduleOutputs, moduleState, moduleInputs] = module;

            if ( moduleOp == '%' ) // flip flop
            {
               if ( state == false )
               {
                  moduleState = !moduleState;
                  for ( string const & newTargetName : moduleOutputs )
                  {
                     broadcastQueue.push( make_tuple( target, newTargetName, moduleState ) );
                  }
               }
            }
            else if ( moduleOp == '&' ) // conjunction
            {
               moduleInputs[source] = state;
               bool allHigh = ranges::fold_left( moduleInputs, true, []( bool l, auto const & r ) {
                  return l && r.second;
               } );
               for ( string const & newTargetName : moduleOutputs )
               {
                  broadcastQueue.push( make_tuple( target, newTargetName, !allHigh ) );
               }
            }
            else
            {
               for ( string const & newTargetName : moduleOutputs )
               {
                  broadcastQueue.push( make_tuple( target, newTargetName, state ) );
               }
            }
         }
      }
      return onCount * offCount;
   }, 
   ranges::fold_left(
      forward<ostringstream>( (ostringstream() << ifstream( "input.txt" ).rdbuf()) ).str()
      | views::split( 10 )
      | views::filter( []( auto const & str ) { return !str.empty(); } )
      | views::transform( []( auto && range ) { return string( string_view( range ) ); } )
      | views::transform( []( auto const & str )
   {
      return
         make_pair(
            str[0] == '&' || str[0] == '%' ? str.substr( 1, str.find_first_of( " " ) - 1 ) : str.substr( 0, str.find_first_of( " " ) ),
            string( str.substr( str.find_first_of( "->" ) + 3 ) )
            | views::split( ',' )
            | views::transform( []( auto && range ) { return string_view( range ); } )
            | views::transform( []( auto const & str ) { return string( str[0] == ' ' ? str.substr( 1 ) : str ); } )
            | ranges::to<vector>()
         );
   } ),
   forward<ostringstream>( (ostringstream() << ifstream( "input.txt" ).rdbuf()) ).str()
      | views::split( 10 )
      | views::filter( []( auto const & str ) { return !str.empty(); } )
      | views::transform( []( auto && range ) { return string( string_view( range ) ); } )
      | views::transform( []( auto const & str )
   {
      return
         make_pair(
            str[0] == '&' || str[0] == '%' ? str.substr( 1, str.find_first_of( " " ) - 1 ) : str.substr( 0, str.find_first_of( " " ) ),
            make_tuple(
            str[0], //operation
            string( str.substr( str.find_first_of( "->" ) + 3 ) )
            | views::split( ',' )
            | views::transform( []( auto && range ) { return string_view( range ); } )
            | views::transform( []( auto const & str ) { return string( str[0] == ' ' ? str.substr( 1 ) : str ); } )
            | ranges::to<vector>(), // targets
            false, // state
            map< string, bool >() // inputs
         )
         );
   }
   )
   | ranges::to< map >(), []( auto modules, auto module )
   {
      for ( string const & targetModule : module.second )
      {
         if ( get<0>( modules[targetModule] ) == '&' )
         {
            get<3>( modules[targetModule] )[module.first] = false;
         }
      }
      return modules;
   } )
   ) << endl;


   cout << "Part 2: " << invoke([](auto modules) -> int64_t
   {
      string targetConjuction = ranges::fold_left(
         modules
         | views::filter( []( auto const & module ) 
         {
            auto const [op, targets, state, inputs] = module.second;
            return find_if( targets.begin(), targets.end(), []( auto const & name ) 
            { return name == "rx"; } ) != targets.end();
         } ), string(), []( auto, auto r ) { return r.first; } );

      map< string, int64_t > counts;

      for ( int64_t count = 1; counts.size() < 4; ++count )
      {
         queue < tuple< string, string, bool > > broadcastQueue;
         broadcastQueue.push( make_tuple( "button", "broadcaster", false ) );

         while ( !broadcastQueue.empty() )
         {
            auto const [source, target, state] = broadcastQueue.front();
            broadcastQueue.pop();

            auto & module = modules[target];
            auto & [moduleOp, moduleOutputs, moduleState, moduleInputs] = module;

            if ( target == targetConjuction && state )
            {
               if ( counts.find( source ) == counts.end() )
               {
                  counts[source] = count;
               }
            }

            if ( moduleOp == '%' ) // flip flop
            {
               if ( state == false )
               {
                  moduleState = !moduleState;
                  for ( string const & newTargetName : moduleOutputs )
                  {
                     broadcastQueue.push( make_tuple( target, newTargetName, moduleState ) );
                  }
               }
            }
            else if ( moduleOp == '&' ) // conjunction
            {
               moduleInputs[source] = state;
               bool allHigh = ranges::fold_left( moduleInputs, true, []( bool l, auto const & r ) {
                  return l && r.second;
               } );
               for ( string const & newTargetName : moduleOutputs )
               {
                  broadcastQueue.push( make_tuple( target, newTargetName, !allHigh ) );
               }
            }
            else
            {
               for ( string const & newTargetName : moduleOutputs )
               {
                  broadcastQueue.push( make_tuple( target, newTargetName, state ) );
               }
            }
         }
      }

      return ranges::fold_left( counts | views::values, 1ll, []( auto lhs, auto rhs ) { return lcm( lhs, rhs ); } );
   },
   ranges::fold_left(
      forward<ostringstream>( (ostringstream() << ifstream( "input.txt" ).rdbuf()) ).str()
      | views::split( 10 )
      | views::filter( []( auto const & str ) { return !str.empty(); } )
      | views::transform( []( auto && range ) { return string( string_view( range ) ); } )
      | views::transform( []( auto const & str )
      {
         return
            make_pair(
               str[0] == '&' || str[0] == '%' ? str.substr( 1, str.find_first_of( " " ) - 1 ) : str.substr( 0, str.find_first_of( " " ) ),
               string( str.substr( str.find_first_of( "->" ) + 3 ) )
               | views::split( ',' )
               | views::transform( []( auto && range ) { return string_view( range ); } )
               | views::transform( []( auto const & str ) { return string( str[0] == ' ' ? str.substr( 1 ) : str ); } )
               | ranges::to<vector>()
            );
      }),
      forward<ostringstream>( (ostringstream() << ifstream( "input.txt" ).rdbuf()) ).str()
      | views::split( 10 )
      | views::filter( []( auto const & str ) { return !str.empty(); } )
      | views::transform( []( auto && range ) { return string( string_view( range ) ); } )
      | views::transform( []( auto const & str )
      {
         return
            make_pair(
               str[0] == '&' || str[0] == '%' ? str.substr( 1, str.find_first_of( " " ) - 1 ) : str.substr( 0, str.find_first_of( " " ) ),
               make_tuple(
               str[0], //operation
               string( str.substr( str.find_first_of( "->" ) + 3 ) )
               | views::split( ',' )
               | views::transform( []( auto && range ) { return string_view( range ); } )
               | views::transform( []( auto const & str ) { return string( str[0] == ' ' ? str.substr( 1 ) : str ); } )
               | ranges::to<vector>(), // targets
               false, // state
               map< string, bool >() // inputs

            )
            );
      }
   )
   | ranges::to< map >(), []( auto modules, auto module )
   {
      for ( string const & targetModule : module.second )
      {
         if ( get<0>( modules[targetModule] ) == '&' )
         {
            get<3>( modules[targetModule] )[module.first] = false;
         }
      }
      return modules;
   } )
   ) << endl;
}
