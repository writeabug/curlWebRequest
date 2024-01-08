//
// Created by Administrator on 2024/1/5.
//
#include <string>
#include <android/log.h>
#include <curl/curl.h>
#include <jni.h>
#include "include/json/json.h"


extern bool isShowLog;
std::string intToStr(int code) {
    std::stringstream ss;
    ss << code;
    return ss.str();
}

std::string longToStr(long code) {
    std::stringstream ss;
    ss << code;
    return ss.str();
}

void Log(const std::string &tag, const std::string &msg) {
    if (isShowLog) {
        __android_log_print(ANDROID_LOG_ERROR, "CURL", "%s%s", tag.c_str(), msg.c_str());
    }
}

std::string removeSpaces(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}
std::tuple<int, std::string> analysisRes(CURLcode res, std::string response, long code) {
    if (res != CURLE_OK) {
        Log("curl error res:", curl_easy_strerror(res));
        std::tuple<int, std::string> tuple(res, curl_easy_strerror(res));
        return tuple;
    } else {
        if (code != 200) {
            std::tuple<int, std::string> tuple(code, "http error code:" + longToStr(code));
            return tuple;
        } else {
            std::tuple<int, std::string> tuple(code, response);
            return tuple;
        }
    }
}


jstring toAnalysisResTuple(JNIEnv *env,jstring path,std::tuple<int, std::string> tuple){
    int code = std::get<0>(tuple);
    std::string response = std::get<1>(tuple);
    if (code == 200) {
        Log("response data:\n", (std::string(
                env->GetStringUTFChars(path, nullptr)) + "\n" + response));
        return env->NewStringUTF(response.c_str());
    } else {
        if (code == 999) {
            jclass exceptionClass = env->FindClass("java/lang/Exception");
            env->ThrowNew(exceptionClass, (response + "\nerror code:" + intToStr(code)).c_str());
            return nullptr;
        } else {
            jclass exceptionClass = env->FindClass("com/curl/project/web/NetWorkException");
            env->ThrowNew(exceptionClass, (response + "\nerror code:" + intToStr(code)).c_str());
            return nullptr;
        }
    }
}