#include <iostream>
#include <time.h>

#define ARR_SIZE 20

using namespace std;

template<typename T>
void copy_array( const T* from, T* to, size_t count ) {
   std::memcpy( to, from, count * sizeof( T ) );
}

template<typename T>
void print_array( const T* first, size_t count ) {
   for ( int i = 0; i < count; i++ ) {
      std::cout << first[ i ] << std::endl;
   }
}

template<typename T>
int find_min( const T* arr, size_t count ) {
   int result = -1;

   if ( count == 0 )
      return 0;

   int temp = INT_MAX;

   for ( int i = 0; i < count; i++ ) {
      if ( arr[ i ] < temp ) {
         result = i;
         temp = arr[ i ];
      }
   }

   return result;
}

bool main( ) {
   setlocale( LC_ALL, "ru" );

   srand( time( NULL ) );

   int arr[ ARR_SIZE ], arr2[ ARR_SIZE ];

   for ( auto i = 0; i < ARR_SIZE; i++ )
      arr[ i ] = rand( );

   copy_array( arr, arr2, ARR_SIZE );

   printf( "массив 1\n" );
   print_array( arr, ARR_SIZE );

   printf( "массив 2\n" );
   print_array( arr2, ARR_SIZE );

   printf( "мин в 1 %i\n", find_min( arr, ARR_SIZE ) );
   printf( "мин в 2 %i\n", find_min( arr2, ARR_SIZE ) );
}