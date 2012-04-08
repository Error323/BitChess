# === All *.{cpp,h} files

# === All sources
set(SOURCES
  src/bitchess.cpp
)

# === Test sources
set(TESTS asearchtest)

set(asearchtest_SOURCES
  src/asearch/state.h
  src/asearch/asearch.cpp
  src/asearch/test/asearchtest.cpp
  src/asearch/test/main.cpp
)
