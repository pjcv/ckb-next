#ifndef INCLUDES_H
#define INCLUDES_H

#include "os.h"

#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <iconv.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/time.h>

// Unsigned char/short definition
typedef unsigned char uchar;
typedef unsigned short ushort;
// Gets the index of an object within an array
#define INDEX_OF(entry, array) (int)(entry - array)

// Compile with -DCKB_OUTPUT_TO_STDERR if you want to separate error messages from normal status updates.
// (probably not useful because the errors don't mean much without context)
#ifdef CKB_OUTPUT_TO_STDERR
#define ckb_s_out   stdout
#define ckb_s_err   stderr
#else
#define ckb_s_out   stdout
#define ckb_s_err   stdout
#endif

// Better __FILE__ macro
#define __FILE_NOPATH__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// Output helpers
// Use ckb_* to output info or ckb_*_fn to override the file/line numbers (useful when describing where a function was invoked from)
#define INFO  0
#define WARN  1
#define ERR   2
#define FATAL 3
#define LEVEL0 "I"
#define LEVEL1 "W"
#define LEVEL2 "E"
#define LEVEL3 "F"
#define __log(lvl, fmt, args...)                 do { \
                                                   time_t rawtime; \
                                                   struct tm* timeinfo; \
                                                   time(&rawtime); \
                                                   timeinfo = localtime(&rawtime); \
                                                   fprintf(lvl >= 2 ? ckb_s_err : ckb_s_out, \
						     "[%s %02d:%02d:%02d] " fmt, \
						     LEVEL ## lvl, \
						     timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, \
						     ## args);	\
                                                  } while(0)

#define ckb_log_nofile(lvl, fmt, args...)         __log(lvl, fmt, ## args)
#define ckb_log_fn(lvl, fmt, file, line, args...) __log(lvl, "%s (via %s:%d): " fmt, __func__, file, line, ## args)
#define ckb_log(lvl, fmt, args...)                __log(lvl, "%s (%s:%d): " fmt, __func__, __FILE_NOPATH__, __LINE__, ## args)

#define ckb_fatal_nofile(fmt, args...)            ckb_log_nofile(FATAL, fmt, ## args)
#define ckb_fatal_fn(fmt, file, line, args...)    ckb_log_fn(FATAL, fmt, file, line, ## args)
#define ckb_fatal(fmt, args...)                   ckb_log(FATAL, fmt, ## args)
#define ckb_err_nofile(fmt, args...)              ckb_log_nofile(ERR, fmt, ## args)
#define ckb_err_fn(fmt, file, line, args...)      ckb_log_fn(ERR, fmt, file, line, ## args)
#define ckb_err(fmt, args...)                     ckb_log(ERR, fmt, ## args)
#define ckb_warn_nofile(fmt, args...)             ckb_log_nofile(WARN, fmt, ## args)
#define ckb_warn_fn(fmt, file, line, args...)     ckb_log_fn(WARN, fmt, file, line, ## args)
#define ckb_warn(fmt, args...)                    ckb_log(WARN, fmt, ## args)
#define ckb_info_nofile(fmt, args...)             ckb_log_nofile(INFO, fmt, ## args)
#define ckb_info_fn(fmt, file, line, args...)     ckb_log_fn(INFO, fmt, file, line, ## args)
#define ckb_info(fmt, args...)                    ckb_log(INFO, fmt, ## args)

// Timespec utilities
void timespec_add(struct timespec* timespec, long nanoseconds);
#define timespec_gt(left, right)    ((left).tv_sec > (right).tv_sec || ((left).tv_sec == (right).tv_sec && (left).tv_nsec > (right).tv_nsec))
#define timespec_eq(left, right)    ((left).tv_sec == (right).tv_sec && (left).tv_nsec == (right).tv_nsec)
#define timespec_ge(left, right)    ((left).tv_sec > (right).tv_sec || ((left).tv_sec == (right).tv_sec && (left).tv_nsec >= (right).tv_nsec))
#define timespec_lt(left, right)    (!timespec_ge(left, right))
#define timespec_le(left, right)    (!timespec_gt(left, right))

// Common structs
#include "structures.h"

#endif  // INCLUDES_H
