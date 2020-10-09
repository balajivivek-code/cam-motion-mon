#pragma once

#include <fstream>
#include <algorithm>
#include "tracing/Logging.h"
#include <syscall.h>

#include <string>

#define LOGINFO(fmt, ...) do { fprintf(stderr, "[%d] INFO [%s:%d] %s: " fmt "\n", (int)syscall(SYS_gettid), Core::FileNameOnly(__FILE__), __LINE__, __FUNCTION__, ##__VA_ARGS__); fflush(stderr); } while (0)
#define LOGWARN(fmt, ...) do { fprintf(stderr, "[%d] WARN [%s:%d] %s: " fmt "\n", (int)syscall(SYS_gettid), Core::FileNameOnly(__FILE__), __LINE__, __FUNCTION__, ##__VA_ARGS__); fflush(stderr); } while (0)
#define LOGERR(fmt, ...) do { fprintf(stderr, "[%d] ERROR [%s:%d] %s: " fmt "\n", (int)syscall(SYS_gettid), Core::FileNameOnly(__FILE__), __LINE__, __FUNCTION__, ##__VA_ARGS__); fflush(stderr); } while (0)

#define LOGINFOMETHOD() { std::string json; parameters.ToString(json); LOGINFO( "params=%s", json.c_str() );  }
#define LOGTRACEMETHODFIN() do { std::string json; response.ToString(json); LOGINFO( "response=%s", json.c_str() );  } while (0)

#define returnResponse(success) \
    response["success"] = success; \
    LOGTRACEMETHODFIN(); \
    return (Core::ERROR_NONE);

#define sendNotify(event,params)\
    std::string json;\
    params.ToString(json);\
    LOGINFO("Notify %s %s", event, json.c_str());\
    Notify(event,params);
