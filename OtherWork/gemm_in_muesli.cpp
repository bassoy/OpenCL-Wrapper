#include <utl_matrix.h>

#include <Atomic.h>
#include <DistributedArray.h>
#include <Final.h>
#include <Initial.h>
#include <Muesli.h>
#include <Pipe.h>

typedef float Type;
typedef utl::Matrix< Type, utl::column_major_tag > Matrix;
typedef msl::DistributedArray< Type > Vector;

template< typename T > T mul( T a, T b ) { return a * b; }

/**
 * Indices into the result matrix C.
 */
struct Indices
{
  size_t row, col;
};

/**
 * Result of the dot product of one row of A and one column of B.
 */
struct Result
{
  Type value;
  size_t row, col;
  
  friend std::ostream& operator <<( std::ostream& o, Result const& r )
  {
    return o << r.value << r.row << r.col;
  }
};

/**
 * Generate indices from (0,0) to (N-1,M-1).
 */
Indices* generateIndices( size_t N, size_t M, msl::Empty )
{
  static size_t n = 0u, m = 0u;
  
  if ( n == 0u && m == M ) return nullptr;
  
  auto p = new Indices{ n , m };
  
  if ( ++n == N )
  {
    ++m;
    n = 0u;
  }
  
  return p;
}

/**
 * Used to extract a row of a matrix.
 */
inline static Type extractRow( Matrix const& mat, size_t rowIndex, int colIndex )
{
  return mat.at( rowIndex, colIndex );
}

/**
 * Used to extract a column of a matrix.
 */
inline static Type extractCol( Matrix const& mat, size_t colIndex, int rowIndex )
{
  return mat.at( rowIndex, colIndex );
}

/**
 * Compute the dot product of one row of @c lhs and one
 * column of @c rhs at the indices stored in @c idx.
 */
static Result* dotProduct( Matrix const& lhs, Matrix const& rhs, Indices* idx )
{  
  Vector row( lhs.cols(), msl::curry( extractRow )( lhs )( idx->row ) );
  Vector col( rhs.rows(), msl::curry( extractCol )( rhs )( idx->col ) );
  
  Vector dot = row.zipWith( col, mul< Type > );
  
  return new Result{ dot.fold( msl::add< Type > ), idx->row, idx->col };
}

/**
 * Populate @c mat with the results.
 */
static void collectResults( Matrix const& mat, Result* r )
{
  // Cast away constness as Muesli does not support non-const references as arguments ...
  const_cast< Matrix& >( mat ).at( r->row, r->col ) = r->value;
  
  // If all results arrived, print them.
  if ( mat.rows() == r->row + 1u && mat.cols() == r->col + 1u )
    std::cout << mat;
}

int main( int argc, char** argv )
{
  try
  { 
    constexpr auto N = 16u, M = 16u, L = 4u;
    
    Matrix A( N, L ), B( L, M ), C( N, M );
    
    for ( size_t i = 0u; i < N * L; ++i ) A[i] = i / N;
    for ( size_t i = 0u; i < L * M; ++i ) B[i] = i % L;
    
    msl::InitSkeletons( argc, argv /*, msl::MSL_SERIALIZED */ );
    
    // Only one process should print the operands.
    if ( msl::Muesli::MSL_myId == 0 )
      std::cout << A << '\n' << B << '\n';
    
    // Build a pipe that computes all dot products.
    msl::Initial< Indices > source( msl::curry( generateIndices )( C.rows() )( C.cols() ) );
    msl::Atomic< Indices, Result > executeDotProduct( msl::curry( dotProduct )( A )( B ), 1 );
    msl::Final< Result > sink( msl::curry( collectResults )( C ) );
    
    msl::Pipe p( source, executeDotProduct, sink );
    
    p.start();
    
    msl::TerminateSkeletons();
  }
  catch ( msl::Exception& e )
  {
    std::cerr << e.toString() << std::endl;
  }
  catch ( std::exception& e )
  {
    std::cerr << e.what() << std::endl;
  }
  catch ( ... )
  {
    std::cerr << "Unknown exception" << std::endl;
  }
  
  return EXIT_SUCCESS;
}
