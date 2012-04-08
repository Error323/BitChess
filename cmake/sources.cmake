# === All *.{cpp,h} files

# === All sources
set(SOURCES
  src/bitchess.cpp
)

# === Test sources
set(TESTS asearchtest)

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
