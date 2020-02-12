#ifndef DDL_LOGGING_H
#define DDL_LOGGING_H

#include "messages_internal.hpp"

//#include <glog/logging.h>
#ifndef _LOGGING_H_
#undef LOG
#undef LOG_IF
#define LOG(severity) std::clog << #severity[0] << " " << get_timestamp() << " " << std::setw(5) << getpid() << " " << gettid() << " " \
                                << "DDL"                                                                                               \
                                << ":" << std::setw(4) << std::left << __LINE__ << "] "
#define DLOG(severity) LOG(D)
#define LOG_IF(severity, condition) \
  if (!(condition)) {               \
  } else                            \
    LOG(severity)
#define DLOG_IF(severity, condition) LOG_IF(D, condition)
#endif

#ifdef __GNUC__
#include <iomanip>
#include <iostream>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

inline int gettid() {
  return syscall(SYS_gettid);
}

inline const std::string get_timestamp() {
  struct timeval tmnow;
  struct tm*     tm;
  gettimeofday(&tmnow, NULL);
  tm = localtime(&tmnow.tv_sec);

  char buf[16];
  strftime(buf, sizeof(buf), "%H:%M:%S", tm);

  strcat(buf, ".");
  sprintf(buf + 9, "%03d", (int)tmnow.tv_usec / 1000);

  return buf;
}

inline double get_cur_time() {
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);

  return (time.tv_sec) + (time.tv_nsec) / 1000000000.0;
}

inline const std::string get_hostname() {
  char buf[128];
  gethostname(buf, sizeof(buf));
  return buf;
}
#else
int gettid() {
  return -1;
}

inline const std::string get_timestamp() {
  char   buf[16];
  time_t now = time(0);
  strftime(buf, sizeof(buf), "%H:%M:%S.000", localtime(&now));
  return buf;
}

inline double get_cur_time() {
  return MPI_CHECK(MPI_Wtime());
}

inline const std::string get_hostname() {
  return "localhost";
}
#endif

#ifndef _NO_CUDA_
#include <cublas_v2.h>
#include <cuda.h>
#include <cuda_fp16.h>
#include <cuda_runtime.h>
#include <cusparse.h>
#include <host_defines.h>
#include <nccl.h>
#include <sys/types.h>
#include <unistd.h>
#else
#include "cuda_emul.h"
#endif

#ifndef CUDA_CHECK
#define CUDA_CHECK(res)                                          \
  do {                                                           \
    if ((res) != cudaSuccess) {                                  \
      LOG_ERROR(ddl_message::CUDA_ERROR, get_hostname().c_str(), \
                cudaGetErrorString(cudaGetLastError()));         \
      std::exit(1);                                              \
    }                                                            \
  } while (0)
#endif

#ifndef CUBLAS_CHECK
#define CUBLAS_CHECK(cmd)                                              \
  do {                                                                 \
    int r = cmd;                                                       \
    if (r != 0) {                                                      \
      LOG_ERROR(ddl_message::CUBLAS_ERROR, get_hostname().c_str(), r); \
      std::exit(1);                                                    \
    }                                                                  \
  } while (0)
#endif

#ifndef NCCL_CHECK
#define NCCL_CHECK(cmd)                                          \
  do {                                                           \
    ncclResult_t r = cmd;                                        \
    if (r != ncclSuccess) {                                      \
      LOG_ERROR(ddl_message::NCCL_ERROR, get_hostname().c_str(), \
                ncclGetErrorString(r));                          \
      std::exit(1);                                              \
    }                                                            \
  } while (0)
#endif

#ifndef MPI_CHECK
#define MPI_CHECK(cmd)                                          \
  do {                                                          \
    int r = cmd;                                                \
    if (r != MPI_SUCCESS) {                                     \
      char error_string[MPI_MAX_ERROR_STRING];                  \
      int  error_len = 0;                                       \
      MPI_Error_string(r, error_string, &error_len);            \
      LOG_ERROR(ddl_message::MPI_ERROR, get_hostname().c_str(), \
                r, error_string);                               \
      std::exit(1);                                             \
    }                                                           \
  } while (0)
#endif

#ifndef DDL_CHECK
#define DDL_CHECK(cmd)                                           \
  do {                                                           \
    int r = (cmd);                                               \
    if (r != DDL_SUCCESS) {                                      \
      LOG_ERROR(ddl_message::DDL_ERROR, get_hostname().c_str()); \
      std::exit(1);                                              \
    }                                                            \
  } while (0)
#endif

#endif
