# === All *.{cpp,h} files

# === All sources
set(SOURCES
  src/bitchess.cpp
  src/utils/types.h
  src/utils/debugger.h
  src/utils/verbose.cpp
  src/utils/timer.cpp
  src/utils/random.c
)

# === Test sources
set(TESTS asearchtest bboardtest)

set(asearchtest_SOURCES
  src/asearch/state.h
  src/asearch/test/main.cpp
  src/asearch/test/asearchtest.cpp
  src/asearch/ttable.cpp
  src/asearch/asearch.cpp
  src/utils/types.h
  src/utils/debugger.h
  src/utils/verbose.cpp
  src/utils/timer.cpp
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
