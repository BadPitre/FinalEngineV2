TARGET = FinalEngineV2
TYPE = ps-exe

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS := $(call rwildcard,src,*.cpp) $(call rwildcard,game,*.cpp)
SRCS += third_party/nugget/modplayer/modplayer.c

CPPFLAGS += -Isrc

include third_party/nugget/psyqo-paths/psyqo-paths.mk
