TARGET = FinalEngineV2
TYPE = ps-exe

SRCS := $(shell find src game -name '*.cpp')

CPPFLAGS += -Isrc

include third_party/nugget/psyqo/psyqo.mk
