#ifndef TYPES_HDR
#define TYPES_HDR

#include <vector>
#include <string>

#include <stdint.h>

#define DECLARE_HUNGARIAN(TYPE)                        \
  typedef       TYPE*                     p##TYPE;     \
  typedef       TYPE&                     r##TYPE;     \
  typedef const TYPE                      c##TYPE;     \
  typedef const TYPE*                     pc##TYPE;    \
  typedef const TYPE&                     rc##TYPE;    \
  typedef       std::vector<TYPE>         v##TYPE;     \
  typedef       std::vector<const TYPE>   vc##TYPE;    \
  typedef       std::vector<TYPE*>        vp##TYPE;    \
  typedef       std::vector<const TYPE*>  vpc##TYPE;   \
  typedef       std::vector<TYPE>&        rv##TYPE;    \
  typedef       std::vector<const TYPE>&  rvc##TYPE;   \
  typedef       std::vector<TYPE*>&       rvp##TYPE;   \
  typedef       std::vector<const TYPE*>& rvpc##TYPE;  \
  typedef const std::vector<TYPE>         cv##TYPE;    \
  typedef const std::vector<const TYPE>   cvc##TYPE;   \
  typedef const std::vector<TYPE*>        cvp##TYPE;   \
  typedef const std::vector<const TYPE*>  cvpc##TYPE;  \
  typedef const std::vector<TYPE>&        rcv##TYPE;   \
  typedef const std::vector<const TYPE>&  rcvc##TYPE;  \
  typedef const std::vector<TYPE*>&       rcvp##TYPE;  \
  typedef const std::vector<const TYPE*>& rcvpc##TYPE;

#define DECLARE_CLASS(CLS)                             \
  class CLS;                                           \
  DECLARE_HUNGARIAN(CLS)

#define DECLARE_STRUCT(STR)                            \
  struct STR;                                          \
  DECLARE_HUNGARIAN(STR)

typedef float            Float;
DECLARE_HUNGARIAN(Float)
typedef int              Int;
DECLARE_HUNGARIAN(Int)
typedef bool             Bool;
DECLARE_HUNGARIAN(Bool)
typedef double           Double;
DECLARE_HUNGARIAN(Double)
typedef char             Char;
DECLARE_HUNGARIAN(Char)
typedef std::string      String;
DECLARE_HUNGARIAN(String)

typedef int64_t          I64;
DECLARE_HUNGARIAN(I64)
typedef uint64_t         U64;
DECLARE_HUNGARIAN(U64)
typedef int32_t          I32;
DECLARE_HUNGARIAN(I32)
typedef uint32_t         U32;
DECLARE_HUNGARIAN(U32)
typedef int16_t          I16;
DECLARE_HUNGARIAN(I16)
typedef uint16_t         U16;
DECLARE_HUNGARIAN(U16)
typedef char             I8;
DECLARE_HUNGARIAN(I8)
typedef unsigned char    U8;
DECLARE_HUNGARIAN(U8)

#endif // TYPES_HDR
