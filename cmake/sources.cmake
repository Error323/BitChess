# === All *.{cpp,h} files

# === All sources
set(SOURCES
  src/bitchess.cpp
  src/utils/types.h
  src/utils/debugger.h
  src/utils/verbose.cpp
  src/utils/random.c
)

# === Test sources
set(TESTS asearchtest bboardtest cstatetest cmovetest perftest)

set(asearchtest_SOURCES
  src/asearch/state.h
  src/asearch/test/main.cpp
  src/asearch/test/asearchtest.cpp
  src/asearch/ttable.cpp
  src/asearch/asearch.cpp
  src/utils/types.h
  src/utils/debugger.h
  src/utils/verbose.cpp
  src/utils/random.c
)

set(bboardtest_SOURCES
  src/bitboard/test/main.cpp
  src/bitboard/test/bboardtest.cpp
  src/bitboard/bitboard.cpp
  src/utils/types.h
  src/utils/verbose.cpp
  src/utils/random.c
)

set(cstatetest_SOURCES
  src/chess/test/main.cpp
  src/chess/test/cstatetest.cpp
	src/chess/cstate.cpp
	src/chess/magics.h
  src/chess/magics.cpp
  src/bitboard/bitboard.cpp
	src/asearch/ttable.cpp
	src/asearch/state.h
  src/utils/types.h
  src/utils/verbose.cpp
  src/utils/random.c
)

set(cmovetest_SOURCES
  src/chess/test/main.cpp
  src/chess/test/cmovetest.cpp
  src/chess/cmove.h
  src/utils/random.c
)

set(perftest_SOURCES
  src/chess/test/main.cpp
  src/chess/test/perftest.cpp
  src/chess/cmove.h
  src/chess/magics.cpp
  src/chess/cstate.cpp
  src/asearch/ttable.cpp
  src/asearch/state.h
  src/utils/types.h
  src/utils/verbose.cpp
)
