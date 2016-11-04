#======================================================================#
#   Android.mk for CGgreen C++ unit test framework
#
#   (Suggest building as .so and only including lib for debug builds
#    when running e.g. android instrumented tests)
#
#   Steve Madsen, 10 Aug 2016
#======================================================================#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cgreen

MY_CGREEN_BASE_PATH := ../..
MY_CGREEN_BASE_PATH_ABS := $(LOCAL_PATH)/$(MY_CGREEN_BASE_PATH)
MY_CGREEN_SRC_PATH := $(MY_CGREEN_BASE_PATH)/src

LOCAL_SRC_FILES := \
	$(MY_CGREEN_SRC_PATH)/cpp_assertions.cpp \
	$(MY_CGREEN_SRC_PATH)/cpp_constraint.cpp \
	$(MY_CGREEN_SRC_PATH)/local_messaging.cpp

LOCAL_SRC_FILES += \
	$(MY_CGREEN_SRC_PATH)/assertions.c \
	$(MY_CGREEN_SRC_PATH)/boxed_double.c \
	$(MY_CGREEN_SRC_PATH)/breadcrumb.c \
	$(MY_CGREEN_SRC_PATH)/cdash_reporter.c \
	$(MY_CGREEN_SRC_PATH)/cgreen_time.c \
	$(MY_CGREEN_SRC_PATH)/constraint.c \
	$(MY_CGREEN_SRC_PATH)/constraint_syntax_helpers.c \
	$(MY_CGREEN_SRC_PATH)/cute_reporter.c \
	$(MY_CGREEN_SRC_PATH)/memory.c \
	$(MY_CGREEN_SRC_PATH)/message_formatting.c \
	$(MY_CGREEN_SRC_PATH)/mocks.c \
	$(MY_CGREEN_SRC_PATH)/parameters.c \
	$(MY_CGREEN_SRC_PATH)/posix_cgreen_pipe.c \
	$(MY_CGREEN_SRC_PATH)/posix_cgreen_time.c \
    $(MY_CGREEN_SRC_PATH)/posix_runner_platform.c \
	$(MY_CGREEN_SRC_PATH)/reporter.c \
	$(MY_CGREEN_SRC_PATH)/runner.c \
	$(MY_CGREEN_SRC_PATH)/string_comparison.c \
	$(MY_CGREEN_SRC_PATH)/suite.c \
	$(MY_CGREEN_SRC_PATH)/text_reporter.c \
	$(MY_CGREEN_SRC_PATH)/utils.c \
	$(MY_CGREEN_SRC_PATH)/vector.c \
	$(MY_CGREEN_SRC_PATH)/xml_reporter.c

LOCAL_C_INCLUDES := \
        $(MY_CGREEN_BASE_PATH_ABS)/include \
        $(MY_CGREEN_BASE_PATH_ABS)/src

LOCAL_EXPORT_C_INCLUDES := \
        $(MY_CGREEN_BASE_PATH_ABS) \
        $(MY_CGREEN_BASE_PATH_ABS)/include \
        $(MY_CGREEN_BASE_PATH_ABS)/src

LOCAL_CFLAGS += -O3 -DVERSION="\"OnAndroid\""

# DO_MAKE_DYNAMIC_LIB: boolean-as-int
#   0: build .a (static) lib
#   1: build .so ("dynamic"/"shared") lib
DO_MAKE_DYNAMIC_LIB := 0

ifeq ($(DO_MAKE_DYNAMIC_LIB),1)
    LOCAL_LDLIBS := -llog
    include $(BUILD_SHARED_LIBRARY)
else
    include $(BUILD_STATIC_LIBRARY)
endif

