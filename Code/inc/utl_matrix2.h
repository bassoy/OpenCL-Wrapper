#ifndef UTL_MATRIX_2_H
#define UTL_MATRIX_2_H

#include <iterator>

#include <utl_matrix.h>

namespace utl {

/**
 * @tparam T Type of value stored in the matrix.
 * @tparam OuterLayout Layout of the inner blocks stored in the matrix. Can be either row_major_tag or column_major_tag.
 * @tparam InnerLayout Layout of the elements in the inner blocks of the matrix. Can be one of row_major_tag or column_major_tag.
 */
template< typename T, typename OuterLayout, typename InnerLayout >
class Matrix2 { ~Matrix2() = delete; };



template< typename T, typename OuterLayout, typename InnerLayout >
Matrix2< T, OuterLayout, InnerLayout > operator *( 
  Matrix2< T, OuterLayout, InnerLayout > const& m,
  typename Matrix2< T, OuterLayout, InnerLayout >::const_reference value )
{
  return static_cast< __MatrixBase< T > const& >( m ) * value;
}



template< typename T, typename OuterLayout, typename InnerLayout >
Matrix2< T, OuterLayout, InnerLayout > operator /( 
  Matrix2< T, OuterLayout, InnerLayout > const& m,
  typename Matrix2< T, OuterLayout, InnerLayout >::const_reference value )
{
  return static_cast< __MatrixBase< T > const& >( m ) / value;
}



template< typename T, typename OuterLayout, typename InnerLayout >
Matrix2< T, OuterLayout, InnerLayout > operator +( 
  Matrix2< T, OuterLayout, InnerLayout > const& m,
  typename Matrix2< T, OuterLayout, InnerLayout >::const_reference value )
{
  return static_cast< __MatrixBase< T > const& >( m ) + value;
}



template< typename T, typename OuterLayout, typename InnerLayout >
Matrix2< T, OuterLayout, InnerLayout > operator +( 
  Matrix2< T, OuterLayout, InnerLayout > const& m1,
  Matrix2< T, OuterLayout, InnerLayout > const& m2 )
{
  return static_cast< __MatrixBase< T > const& >( m1 ) + m2;
}



template< typename T, typename OuterLayout, typename InnerLayout >
Matrix2< T, OuterLayout, InnerLayout > operator -( 
  Matrix2< T, OuterLayout, InnerLayout > const& m,
  typename Matrix2< T, OuterLayout, InnerLayout >::const_reference value )
{
  return static_cast< __MatrixBase< T > const& >( m ) - value;
}



template< typename T, typename OuterLayout, typename InnerLayout >
Matrix2< T, OuterLayout, InnerLayout > operator -( 
  Matrix2< T, OuterLayout, InnerLayout > const& m1,
  Matrix2< T, OuterLayout, InnerLayout > const& m2 )
{
  return static_cast< __MatrixBase< T > const& >( m1 ) - m2;
}



template< typename T, typename OuterLayout, typename InnerLayout >
Matrix2< T, OuterLayout, InnerLayout > operator *( 
  Matrix2< T, OuterLayout, InnerLayout > const& lhs,
  Matrix2< T, OuterLayout, InnerLayout > const& rhs )
{
  TRUE_ASSERT( lhs.cols() == rhs.rows(), "Inner dimension have to match for matrix multiplication." );
  
  Matrix2< T, OuterLayout, InnerLayout > result(
    lhs.rows(), rhs.cols(), lhs.innerRows(), rhs.innerCols()
  );
  
  for ( size_t n = 0; n < result.rows(); ++n )
    for ( size_t m = 0; m < result.cols(); ++m )
    {
      typename Matrix2< T, OuterLayout, InnerLayout >::value_type element( 0 );
      
      for ( size_t l = 0; l < lhs.cols(); ++l )
        element += lhs.at( n, l ) * rhs.at( l, m );
      
      result.at( n, m ) = element;
    }
  
  return result;
}



template< typename T, typename OuterLayout, typename InnerLayout >
std::ostream& operator <<( std::ostream& out, Matrix2< T, OuterLayout, InnerLayout > const& mat )
{
  out << '[';
  
  if ( mat.is_scalar() )
  {
    out << mat.front();
  }
  else if ( mat.is_vector() )
  {
    char const* const delimiter = mat.rows() > mat.cols() ? "; " : ", ";
    
    std::ostream_iterator< typename Matrix2< T, OuterLayout, InnerLayout >::value_type > it( out, delimiter );
    
    std::copy( std::begin( mat ), std::end( mat ), it );
  }
  else
  {
    out << "...\n";
    
    for ( size_t n = 0; n < mat.rows(); ++n )
    {     
      /*if ( n % mat.innerRows() == 0 )
      {
        for ( size_t m = 0; m < mat.cols() - 1u; ++m )
          out << '-';
        out << '\n';
      }*/
      
      size_t m = 0;
      
      for ( ; m < mat.cols() - 1u; ++m )
      {
        /*if ( m % mat.innerCols() == 0 )
          out << "| ";*/
        
        out << mat.at( n, m ) << ' ';
        
      }
      
      /*if ( m % mat.innerCols() == 0 )
          out << "| ";*/
      
      out << mat.at( n, m ) << ";...\n";

    }
  }
  
  return out << "];\n";
}

template< typename T >
class Matrix2< T, column_major_tag, column_major_tag > : public __MatrixBase< T >
{
public :
  typedef typename __MatrixBase< T >::pointer         pointer;
  typedef typename __MatrixBase< T >::const_pointer   const_pointer;
  typedef typename __MatrixBase< T >::reference       reference;
  typedef typename __MatrixBase< T >::const_reference const_reference;
  typedef typename __MatrixBase< T >::value_type      value_type;
  typedef typename __MatrixBase< T >::iterator        iterator;
  typedef typename __MatrixBase< T >::const_iterator  const_iterator;
  
  reference at( size_t row, size_t col )
  {
    return const_cast< reference >(
      const_cast< Matrix2 const& >( *this ).at( row, col )
    );
  }
  
  Matrix2( size_t rows, size_t cols, size_t innerRows, size_t innerCols, const_reference value ):
    __MatrixBase< T >( rows, cols, value ), innerRows_( innerRows ), innerCols_( innerCols )
  { 
    TRUE_ASSERT( innerCols <= cols, "Inner blocks larger than matrix" );
    TRUE_ASSERT( innerRows <= rows, "Inner blocks larger than matrix" );
  }
  
  Matrix2( size_t rows, size_t cols, size_t innerRows, size_t innerCols ):
    __MatrixBase< T >( rows, cols ), innerRows_( innerRows ), innerCols_( innerCols )
  { 
    TRUE_ASSERT( innerCols <= cols, "Inner blocks larger than matrix" );
    TRUE_ASSERT( innerRows <= rows, "Inner blocks larger than matrix" );
  }
  
  Matrix2( Matrix2 const& ) = default;
  Matrix2( Matrix2&& ) = default;
  Matrix2() = default;
  
  const_reference at( size_t row, size_t col ) const
  {
    return this->_vector.at( computeIndex( row, col ) );
  }
  
//   size_t innerRows() const { return innerRows_; }
//   size_t innerCols() const { return innerCols_; }
  
private :
  size_t innerRows_, innerCols_;
  
  Matrix2( __MatrixBase< T >&& base ): 
    __MatrixBase< T >( std::move( base ) ), innerRows_( 0 ), innerCols_( 0 )
  { }
  
  size_t computeIndex( size_t row, size_t col ) const
  {
    // Indices of the inner blocks.
    auto const innerBlockRow( row / innerRows_ );
    auto const innerBlockCol( col / innerCols_ );
    
    auto const colsOfBlock(
      (innerBlockCol + 1u) * innerCols_ >= this->cols() ? 
        this->cols() - (innerBlockCol * innerCols_) : innerCols_
    );
    
    // Indices of the elements of the inner block.
    auto const innerElementRow( row % innerRows_ );
    auto const innerElementCol( col % innerCols_ );
    
    // Number of rows of the block we are indexing into.
    auto const rowsOfBlock(
      (innerBlockRow + 1u) * innerRows_ >= this->rows() ?
        this->rows() - (innerBlockRow * innerRows_) : innerRows_
    );

    // Offset of the block.
    auto const blockIndex(
      innerBlockCol * innerCols_ * this->rows() +
      innerBlockRow * innerRows_ * colsOfBlock
    );
    
    // Offset of the element of the inner block.
    auto const elementIndex(
      innerElementCol * rowsOfBlock + innerElementRow
      
    );

    return blockIndex + elementIndex;
  }
};



template< typename T >
class Matrix2< T, column_major_tag, row_major_tag > : public __MatrixBase< T >
{  
public :
  typedef typename __MatrixBase< T >::pointer         pointer;
  typedef typename __MatrixBase< T >::const_pointer   const_pointer;
  typedef typename __MatrixBase< T >::reference       reference;
  typedef typename __MatrixBase< T >::const_reference const_reference;
  typedef typename __MatrixBase< T >::value_type      value_type;
  typedef typename __MatrixBase< T >::iterator        iterator;
  typedef typename __MatrixBase< T >::const_iterator  const_iterator;
  
  reference at( size_t row, size_t col )
  {
    return const_cast< reference >(
      const_cast< Matrix2 const& >( *this ).at( row, col )
    );
  }
  
  Matrix2( size_t rows, size_t cols, size_t innerRows, size_t innerCols, const_reference value ):
    __MatrixBase< T >( rows, cols, value ), innerRows_( innerRows ), innerCols_( innerCols )
  { 
    TRUE_ASSERT( innerCols <= cols, "Inner blocks larger than matrix" );
    TRUE_ASSERT( innerRows <= rows, "Inner blocks larger than matrix" );
  }
  
  Matrix2( size_t rows, size_t cols, size_t innerRows, size_t innerCols ):
    __MatrixBase< T >( rows, cols ), innerRows_( innerRows ), innerCols_( innerCols )
  { 
    TRUE_ASSERT( innerCols <= cols, "Inner blocks larger than matrix" );
    TRUE_ASSERT( innerRows <= rows, "Inner blocks larger than matrix" );
  }
  
  Matrix2( Matrix2 const& ) = default;
  Matrix2( Matrix2&& ) = default;
  Matrix2() = default;
  
  const_reference at( size_t row, size_t col ) const
  {
    return this->_vector.at( computeIndex( row, col ) );
  }
  
//   size_t innerRows() const { return innerRows_; }
//   size_t innerCols() const { return innerCols_; }
  
private :
  size_t innerRows_, innerCols_;
  
  Matrix2( __MatrixBase< T >&& base ): __MatrixBase< T >( std::move( base ) )
  { }
  
  size_t computeIndex( size_t row, size_t col ) const
  {
    // Indices of the inner blocks.
    auto const innerBlockRow( row / innerRows_ );
    auto const innerBlockCol( col / innerCols_ );
    
    auto const colsOfBlock(
      (innerBlockCol + 1u) * innerCols_ >= this->cols() ? 
        this->cols() - (innerBlockCol * innerCols_) : innerCols_
    );

    
    // Indices of the elements of the inner block.
    auto const innerElementRow( row % innerRows_ );
    auto const innerElementCol( col % innerCols_ );
    
    /*// Number of rows of the block we are indexing into.
    auto const rowsOfBlock(
      (innerBlockRow + 1u) * innerRows_ >= this->rows() ?
        this->rows() - (innerBlockRow * innerRows_) : innerRows_
    );*/
    
    // Offset of the block.
    auto const blockIndex(
      innerBlockCol * innerCols_ * this->rows() +
      innerBlockRow * innerRows_ * colsOfBlock
    );
    
    // Offset of the element of the inner block.
    auto const elementIndex(
      innerElementRow * colsOfBlock + innerElementCol
    );

    return blockIndex + elementIndex;
  }
};



template< typename T >
class Matrix2< T, row_major_tag, column_major_tag > : public __MatrixBase< T >
{
public :
  typedef typename __MatrixBase< T >::pointer         pointer;
  typedef typename __MatrixBase< T >::const_pointer   const_pointer;
  typedef typename __MatrixBase< T >::reference       reference;
  typedef typename __MatrixBase< T >::const_reference const_reference;
  typedef typename __MatrixBase< T >::value_type      value_type;
  typedef typename __MatrixBase< T >::iterator        iterator;
  typedef typename __MatrixBase< T >::const_iterator  const_iterator;
  
  reference at( size_t row, size_t col )
  {
    return const_cast< reference >(
      const_cast< Matrix2 const& >( *this ).at( row, col )
    );
  }
  
  Matrix2( size_t rows, size_t cols, size_t innerRows, size_t innerCols, const_reference value ):
    __MatrixBase< T >( rows, cols, value ), innerRows_( innerRows ), innerCols_( innerCols )
  { 
    TRUE_ASSERT( innerCols <= cols, "Inner blocks larger than matrix" );
    TRUE_ASSERT( innerRows <= rows, "Inner blocks larger than matrix" );
  }
  
  Matrix2( size_t rows, size_t cols, size_t innerRows, size_t innerCols ):
    __MatrixBase< T >( rows, cols ), innerRows_( innerRows ), innerCols_( innerCols )
  { 
    TRUE_ASSERT( innerCols <= cols, "Inner blocks larger than matrix" );
    TRUE_ASSERT( innerRows <= rows, "Inner blocks larger than matrix" );
  }
  
  Matrix2( Matrix2 const& ) = default;
  Matrix2( Matrix2&& ) = default;
  Matrix2() = default;
  
  const_reference at( size_t row, size_t col ) const
  {
    return this->_vector.at( computeIndex( row, col ) );
  }
  
//   size_t innerRows() const { return innerRows_; }
//   size_t innerCols() const { return innerCols_; }
  
private :
  size_t innerRows_, innerCols_;
  
  Matrix2( __MatrixBase< T >&& base ): __MatrixBase< T >( std::move( base ) )
  { }
  
  size_t computeIndex( size_t row, size_t col ) const
  {
    // Indices of the inner blocks.
    auto const innerBlockRow( row / innerRows_ );
    auto const innerBlockCol( col / innerCols_ );
    
    /*auto const colsOfBlock(
      (innerBlockCol + 1u) * innerCols_ >= this->cols() ? 
        this->cols() - (innerBlockCol * innerCols_) : innerCols_
    );*/
    
    // Indices of the elements of the inner block.
    auto const innerElementRow( row % innerRows_ );
    auto const innerElementCol( col % innerCols_ );
    
    // Number of rows of the block we are indexing into.
    auto const rowsOfBlock(
      (innerBlockRow + 1u) * innerRows_ >= this->rows() ?
        this->rows() - (innerBlockRow * innerRows_) : innerRows_
    );
    
    // Offset of the block.
    auto const blockIndex(
      innerBlockRow * innerRows_ * this->cols() +
      innerBlockCol * innerCols_ * rowsOfBlock
    );
    
    // Offset of the element of the inner block.
    auto const elementIndex(
      innerElementCol * rowsOfBlock + innerElementRow
    );

    return blockIndex + elementIndex;
  }
};



template< typename T >
class Matrix2< T, row_major_tag, row_major_tag > : public __MatrixBase< T >
{
public :
  typedef typename __MatrixBase< T >::pointer         pointer;
  typedef typename __MatrixBase< T >::const_pointer   const_pointer;
  typedef typename __MatrixBase< T >::reference       reference;
  typedef typename __MatrixBase< T >::const_reference const_reference;
  typedef typename __MatrixBase< T >::value_type      value_type;
  typedef typename __MatrixBase< T >::iterator        iterator;
  typedef typename __MatrixBase< T >::const_iterator  const_iterator;
  
  reference at( size_t row, size_t col )
  {
    return const_cast< reference >(
      const_cast< Matrix2 const& >( *this ).at( row, col )
    );
  }
  
  Matrix2( size_t rows, size_t cols, size_t innerRows, size_t innerCols, const_reference value ):
    __MatrixBase< T >( rows, cols, value ), innerRows_( innerRows ), innerCols_( innerCols )
  { 
    TRUE_ASSERT( innerCols <= cols, "Inner blocks larger than matrix" );
    TRUE_ASSERT( innerRows <= rows, "Inner blocks larger than matrix" );
  }
  
  Matrix2( size_t rows, size_t cols, size_t innerRows, size_t innerCols ):
    __MatrixBase< T >( rows, cols ), innerRows_( innerRows ), innerCols_( innerCols )
  { 
    TRUE_ASSERT( innerCols <= cols, "Inner blocks larger than matrix" );
    TRUE_ASSERT( innerRows <= rows, "Inner blocks larger than matrix" );
  }
  
  Matrix2( Matrix2 const& ) = default;
  Matrix2( Matrix2&& ) = default;
  Matrix2() = default;
  
  const_reference at( size_t row, size_t col ) const
  {
    return this->_vector.at( computeIndex( row, col ) );
  }
  
//   size_t innerRows() const { return innerRows_; }
//   size_t innerCols() const { return innerCols_; }
  
private :
  size_t innerRows_, innerCols_;
  
  Matrix2( __MatrixBase< T >&& base ): __MatrixBase< T >( std::move( base ) )
  { }
  
  size_t computeIndex( size_t row, size_t col ) const
  {
    // Indices of the inner blocks.
    auto const innerBlockRow( row / innerRows_ );
    auto const innerBlockCol( col / innerCols_ );
    
    auto const colsOfBlock(
      (innerBlockCol + 1u) * innerCols_ >= this->cols() ? 
        this->cols() - (innerBlockCol * innerCols_) : innerCols_
    );
    
    // Indices of the elements of the inner block.
    auto const innerElementRow( row % innerRows_ );
    auto const innerElementCol( col % innerCols_ );
    
    // Number of rows of the block we are indexing into.
    auto const rowsOfBlock(
      (innerBlockRow + 1u) * innerRows_ >= this->rows() ?
        this->rows() - (innerBlockRow * innerRows_) : innerRows_
    );
    
    // Offset of the block.
    auto const blockIndex(
      innerBlockRow * innerRows_ * this->cols() +
      innerBlockCol * innerCols_ * rowsOfBlock
    );
    
    // Offset of the element of the inner block.
    auto const elementIndex(
      innerElementRow * colsOfBlock + innerElementCol
    );

    return blockIndex + elementIndex;
  }
};

}

#endif
