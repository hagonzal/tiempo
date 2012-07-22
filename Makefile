# Copyright (c) 2011 The Tiempo Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

# Directory with the gtest source code
GTEST_DIR=../gtest-1.6.0

CC := g++
# Optimized flags
CFLAGS := -Wall -g -O2 -I. -I$(GTEST_DIR) -I$(GTEST_DIR)/include
#cprof profiling flags
#CFLAGS := -Wall -g -O3 -pg -I. -I$(GTEST_DIR) -I$(GTEST_DIR)/include
#Debug flags.
#CFLAGS := -Wall -g -I. -I$(GTEST_DIR) -I$(GTEST_DIR)/include
PROTOC := protoc
LIBS := -lprotobuf -lgflags -lglog -lboost_regex -larmadillo -lprofiler -lboost_iostreams
LFLAGS :=
#Link flags for cprof
#LFLAGS := -pg

OBJ_PROTO = \
  proto/model.pb.o \
  proto/tick.pb.o

OBJ_UTIL = \
  util/bit_utils.o \
  util/calendar.o \
  util/env.o \
  util/timestamp.o \
  util/util.o

OBJ_PARSER = \
  parser/byte_stream.o \
  parser/csv_parser.o \
  parser/file_stream.o \
  parser/external_mergesort.o \
  parser/splitter.o

OBJ_SHARED = \
  shared/database.o \
  shared/dataset.o
 
OBJ_REGRESSION = \
  regression/regression.o \
  regression/regression_result.o \
  regression/regression_set.o

TEST_OBJ = \
  util/calendar_test.o \
  util/timestamp_test.o
  
OBJ_LIB := \
  $(OBJ_PROTO) \
  $(OBJ_UTIL) \
  $(OBJ_PARSER) \
  $(OBJ_SHARED) \
  $(OBJ_REGRESSION)

ALL_OBJ := $(OBJ_LIB) $(MAIN_OBJ) $(TEST_OBJ)

PROGRAMS = \
  main.out

A = $(notdir $(TEST_OBJ))
TESTS = $(A:.o=.out)

all: $(PROGRAMS) $(TESTS)

test: $(TESTS)
	for t in $(TESTS); do echo "Running test $$t"; ./$$t || exit 1; done

# include all dependency rules created by the compiler
# unless we are executing "make clean"
# don't complain for missing includes
DEPS = $(ALL_OBJ:.o=.d)
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

# rule to build dependency files
# we use fix_dep.sh to add correct directories to .d files.
%.d: %.cc
	g++ -MM -MG $(CFLAGS) $*.cc > $@
	./dev_scripts/fix_dep.sh $@

%.d: %.c
	g++ -MM -MG $(CFLAGS) $*.c > $@
	./dev_scripts/fix_dep.sh $@

# rule to build object files
%.o: %.cc
	$(CC) $(CFLAGS) -c $*.cc -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $*.c -o $@

# rule to compile protocol buffers
%.pb.cc %.pb.h: %.proto
	protoc -I. --cpp_out=. $*.proto

# rules for flex/bison
%.yy.c: %.l
	flex -o $@ $*.l

%.tab.c %.tab.h: %.y
	bison -d $*.y -o $@

# Build the gtest library
gtest-all.o: $(GTEST_DIR)/src/gtest-all.cc
	$(CC) -I$(GTEST_DIR) -I$(GTEST_DIR)/include -c $(GTEST_DIR)/src/gtest-all.cc

libgtest.a: gtest-all.o
	ar -rv libgtest.a gtest-all.o

main.out: parser/main.o $(OBJ_LIB)
	$(CC) $(LFLAGS) -o $@ parser/main.o $(OBJ_LIB) $(LIBS) -pthread

# Start of test targets

timestamp_test.out: util/timestamp_test.o $(OBJ_LIB) libgtest.a
	$(CC) $(CFLAGS) -o $@ util/timestamp_test.o $(OBJ_LIB) libgtest.a $(LIBS) -pthread

calendar_test.out: util/calendar_test.o $(OBJ_LIB) libgtest.a
	$(CC) $(CFLAGS) -o $@ util/calendar_test.o $(OBJ_LIB) libgtest.a $(LIBS) -pthread

# End of test targets

clean:
	@rm -f *.out
	@rm -f *.o
	@rm -f *.a
	@rm -f $(ALL_OBJ)
	@rm -f $(DEPS)
	@rm -f proto/*.pb.h
	@rm -f proto/*.pb.cc
