/******************************************************************
*
* Licensed Materials - Property of IBM
*
* (C) Copyright IBM Corp. 2018, 2018. All Rights Reserved.
*
* US Government Users Restricted Rights - Use, duplication or
* disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*
*******************************************************************
*/
#ifndef DDL_LIBS_DDL_MDR_MESSAGES_HPP
#define DDL_LIBS_DDL_MDR_MESSAGES_HPP

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>
#include <unordered_map>

namespace ddl_message {
constexpr std::uint16_t MESSAGE_CATEGORY = 1;

enum Message_Severity {
  ERROR   = 1,
  WARNING = 2
};

const std::unordered_map<std::uint32_t, std::string> Severity =
  {
    {ERROR, "ERROR"},
    {WARNING, "WARN"}};

enum Message_Id {
  UNKNOWN_ERROR_MSG         = 1,
  CUDA_ERROR                = 2,
  CUBLAS_ERROR              = 3,
  NCCL_ERROR                = 4,
  MPI_ERROR                 = 5,
  DDL_ERROR                 = 6,
  DEVICE_NOT_SEQ            = 7,
  DDL_NO_ENTERPRISE_LICENSE = 8
};

const std::unordered_map<std::uint32_t, std::string> Messages =
  {
    {UNKNOWN_ERROR_MSG, "An unknown error has occurred."},
    {CUDA_ERROR, "A CUDA error has occurred on host %1$s:\n%2$s"},
    {CUBLAS_ERROR, "A cuBLAS error has occurred on host %1$s. Return code is %2$d."},
    {NCCL_ERROR, "A NCCL error has occurred on host %1$s:\n%2$s"},
    {MPI_ERROR, "A MPI error has occurred on host %1$s with return code '%2$d':\n%3$s"},
    {DDL_ERROR, "A DDL error has occurred on host %1$s."},
    {DEVICE_NOT_SEQ, "[MPI: %1$d] Device to PCI mapping is not sequential."},
    {DDL_NO_ENTERPRISE_LICENSE, "Attempting to use more than %2$d nodes without a PowerAI Enterprise License on host %1$s."}};

inline std::string get_error_prefix(Message_Severity sev, Message_Id id) {
  return std::string("[") + Severity.at(sev) + " DDL-" +
         std::to_string(MESSAGE_CATEGORY) + "-" + std::to_string(id) +
         "] ";
}

inline void log_message_(std::string file, int line, Message_Severity sev, Message_Id id, ...) {
  char* env = std::getenv("DDL_DEBUG");
  if (env != nullptr && strcmp(env, "1") == 0) {
    std::fprintf(stderr, "[DDL DEBUG] %s:%d", file.c_str(), line);
  }
  auto message_iter = Messages.find(id);
  if (message_iter == Messages.end()) {
    std::fprintf(stderr, "%s", (get_error_prefix(ERROR, UNKNOWN_ERROR_MSG) + Messages.at(UNKNOWN_ERROR_MSG) + "\n").c_str());
  } else {
    va_list args;
    va_start(args, id);
    std::vfprintf(stderr, (get_error_prefix(sev, id) + message_iter->second + "\n").c_str(), args);
    va_end(args);
  }
}

#define LOG_ERROR(id, args...) log_message_(__FILE__, __LINE__, ddl_message::ERROR, id, ##args)
#define LOG_WARN(id, args...) log_message_(__FILE__, __LINE__, ddl_message::WARNING, id, ##args)
} // namespace ddl_message
#endif /* DDL_LIBS_DDL_MDR_MESSAGES_HPP */
