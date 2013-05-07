#ifndef MAGICS_HDR
#define MAGICS_HDR

#include "../bitboard/bitboard.h"

using namespace bboard;

/**
 * @headerfile magics
 *
 * @brief
 * Contains rook and bishop magics for fast move lookups. See:
 *  - http://chessprogramming.wikispaces.com/Magic+Bitboards
 *  - http://chessprogramming.wikispaces.com/Looking+for+Magics
 */
namespace magics {

struct Magic {
  U64 *location; ///< location in sAttackTable
  U64 magic;     ///< magic hash
  U64 premask;   ///< to mask relevant squares of both lines (no outer squares)
  U64 postmask;  ///< attacks on the otherwise empty board
} __attribute__((aligned));

static std::vector<U64> sAttackTable;
static Magic sBishopMagics[64];
static Magic sRookMagics[64];

static const U64 rook_magics[64] = {
  0xd280024000822054ull, 0xd540054020011000ull, 0xd5000d0020004030ull,
  0xd500050020700089ull, 0xd5000448000a3100ull, 0xd400820804001020ull,
  0xd50007000408c200ull, 0xd200088100c40062ull, 0xd503801040002281ull,
  0xd821c0100140e001ull, 0xd803802000895000ull, 0xd82a00120020c109ull,
  0xd8408028005c0180ull, 0xd858804200805400ull, 0xd814000402500845ull,
  0xd44e000408822543ull, 0xd408888001400323ull, 0xd890808020084008ull,
  0xd80d420020803204ull, 0xd818a200105a4200ull, 0xd800808004020800ull,
  0xd924808002000c01ull, 0xd8028c001806b021ull, 0xd6a81a0006940151ull,
  0xd48981020020ca00ull, 0xdb00200040015002ull, 0xd8eac5010010a001ull,
  0xd85900a300085000ull, 0xdb2e00920008a044ull, 0xd806000200109409ull,
  0xda1068c400100209ull, 0xd624109200024407ull, 0xd443214010800280ull,
  0xd901018925004000ull, 0xd9c0843002802002ull, 0xd8dae34901001001ull,
  0xda0982c800801400ull, 0xd802004842000c10ull, 0xdb0200080a000904ull,
  0xd400ca8432000141ull, 0xd7a581b140008000ull, 0xd89000402011c002ull,
  0xd860022543010010ull, 0xda10002801818030ull, 0xd882040008008080ull,
  0xd8820070044a0058ull, 0xd90a2846101c0025ull, 0xd4aa28870b460024ull,
  0xd8fffe99fecfaa00ull, 0xdcfffe99fecfaa00ull, 0xdd7fffadff9c2e00ull,
  0xdd3fffddffce9200ull, 0xdfffffe9ffe7ce00ull, 0xdffffff5fff3e600ull,
  0xd822000415084600ull, 0xd90ffff5f63c96a0ull, 0xd5ffff2525125352ull,
  0xd9fffeddfeedaeaeull, 0xdbbfffedffdeb1a2ull, 0xda7fffb9ffdfb5f6ull,
  0xd91fffddffdbf4d6ull, 0xd62a001854101322ull, 0xd540261009089804ull,
  0xd645fffecbfea79eull
};

static const U64 bishop_magics[64] = {
  0xe808913004054080ull, 0xec100508008c8102ull, 0xec11010401060000ull,
  0xec48160042010020ull, 0xec06021040102410ull, 0xec2e080229894003ull,
  0xec02482450082002ull, 0xe821430800902482ull, 0xec06854430240102ull,
  0xec0130304200c040ull, 0xec00080204002208ull, 0xec19090407008269ull,
  0xee81021610040401ull, 0xee81021610040401ull, 0xee81021610040401ull,
  0xec40098248080402ull, 0xee4044125c030424ull, 0xef03289042020c0cull,
  0xe6f00505040284b1ull, 0xe408008b09430007ull, 0xe604104e0202040aull,
  0xe490800140602020ull, 0xec21800044300801ull, 0xec82040120824810ull,
  0xec04200840480300ull, 0xec08044060244082ull, 0xe4c28204500c0012ull,
  0xdd280801008200feull, 0xdec1010033104012ull, 0xe401004042082000ull,
  0xedc1244102084408ull, 0xec14012605069220ull, 0xee02922005702004ull,
  0xec04252008180200ull, 0xe512054044040705ull, 0xdd12a00800c100d1ull,
  0xdd04042400120090ull, 0xe4e5809500920111ull, 0xee01011a02011822ull,
  0xec02120025004c00ull, 0xec1b906420021101ull, 0xed41041062890c02ull,
  0xe60890c028061005ull, 0xe518222218028400ull, 0xe41050201040b208ull,
  0xe410200800400022ull, 0xee20412212000082ull, 0xec180104108c1020ull,
  0xee42084422088001ull, 0xee42084422088001ull, 0xec20904844100000ull,
  0xec04008020980440ull, 0xec00521006020514ull, 0xec0e04200c110201ull,
  0xec1004100461c080ull, 0xed1010611c408489ull, 0xe9c8c404141a0e06ull,
  0xec1a4c211110108dull, 0xece000d106451008ull, 0xec00402000e18804ull,
  0xed09000208210100ull, 0xec0e04200c110201ull, 0xec00411004088080ull,
  0xe8408a1809122080ull
};

static const int sRookSharing[64] = {
   0,  1,  2,  3,  4,  5,  6,  7,
   1,  0,  3,  2,  5,  4,  7,  6,
   8,  9, 10, 11, 12, 13, 14, 15,
   9,  8, 11, 10, 13, 12, 15, 14,
  16, 17, 18, 19, 20, 21, 22, 23,
  17, 16, 19, 18, 21, 20, 23, 22,
  24, 25, 26, 27, 28, 29, 30, 31,
  25, 24, 27, 26, 29, 28, 31, 30
};

static const int sBishopSharing[64] = {
  0,  2,  4,  4,  4,  4, 12, 14,
  0,  2,  5,  5,  5,  5, 12, 14,
  0,  2,  6,  6,  6,  6, 12, 14,
  0,  2,  7,  7,  7,  7, 12, 14,
  1,  3,  8,  8,  8,  8, 13, 15,
  1,  3,  9,  9,  9,  9, 13, 15,
  1,  3, 10, 10, 10, 10, 13, 15,
  1,  3, 11, 11, 11, 11, 13, 15,
};

/// Returns attacks of the rook given occupancy board and square
U64 RookAttacks(U64 board, const int square);

/// Returns attacks of the bishop given occupancy board and square
U64 BishopAttacks(U64 board, const int square);

/// Returns the rook's mask across the bitboard given the square
U64 RookMask(int sq);

/// Returns the bishop's mask across the bitboard given the square
U64 BishopMask(int sq);

/// Returns the rook's attack field given the blocking bitboard
U64 RookAttack(int sq, U64 block);

/// Returns the bishop's attack field given the blocking bitboard
U64 BishopAttack(int sq, U64 block);

/// Returns index to a move table, it's where the magic happens
/// See http://www.rivalchess.com/magic-bitboards/ for details
int Transform(U64 board, const U64 magic);

/// Computes blocking bitboard from index
U64 Index2U64(const int index, const int bits, U64 mask);

void Initialize();

}

#endif // MAGICS_HDR
