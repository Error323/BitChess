# === All *.{cpp,h} files

# === All sources
set(SOURCES
  src/bitchess.cpp
)

# === Test sources
set(TESTS asearch)

set(asearch_SOURCES
  src/asearch/state.h
  src/asearch/asearch.cpp
  src/asearch/test/asearchtest.cpp
  src/asearch/test/main.cpp
)
