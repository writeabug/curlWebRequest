#include <jni.h>
#include <string>
#include <utility>
#include <curl/curl.h>
#include <android/log.h>
#include "include/json/json.h"
#include "include/unity.h"
#include <list>

std::int32_t outTime;
bool isShowLog = false;
bool isShowHeaderInfo = false;

curl_slist *headers = nullptr;

std::tuple<int, std::string>
baseRequest(const std::string &path, const std::string &data, bool isPost);


size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char *>(contents), total_size);
    return total_size;
}


std::tuple<int, std::string>
baseRequest(const std::string &path, const std::string &data, bool isPost) {
    CURL *curl;
    curl = curl_easy_init();
    if (!curl) {
        std::tuple<int, std::string> tuple(999, "curl not init");
        return tuple;
    }
    std::string response;
    Log("request url:", path);
    Log("post data:", data);


    curl_easy_setopt(curl, CURLOPT_URL, removeSpaces(path).c_str());//需要去除空字符串
    if (isPost){
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());//post的数据
    }
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, outTime);//设置超时时间
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);//不对CA证书进行验证
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);//不对CA证书进行验证
    if (isPost) {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);//指定使用post请求
    } else {
        curl_easy_setopt(curl, CURLOPT_POST, 0L);//指定使用get请求
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);//请求返回的数据
    CURLcode res = curl_easy_perform(curl);//发起请求
    if (isShowHeaderInfo) {
        while (headers) {
            Log("header Info:", headers->data);
            headers = headers->next;
        }
    }
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_cleanup(curl);//清除curl
    return analysisRes(res, response, http_code);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_curl_project_web_WebUnity_setTimeOut(JNIEnv *env, jobject thiz, jint time) {
    outTime = time;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_curl_project_web_WebUnity_setIsShowLog(JNIEnv *env, jobject thiz,
                                                jboolean is_show,
                                                jboolean is_show_header_info) {
    isShowLog = is_show;
    isShowHeaderInfo = is_show_header_info;
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_curl_project_web_WebUnity_baseRequest(JNIEnv *env, jobject thiz, jstring path,
                                               jstring post_data) {
    std::tuple<int, std::string> tuple = baseRequest(env->GetStringUTFChars(path, nullptr),
                                                     env->GetStringUTFChars(post_data,
                                                                            nullptr), true);
    return toAnalysisResTuple(env, path, tuple);
}



extern "C"
JNIEXPORT jboolean JNICALL
Java_com_curl_project_web_WebUnity_initCurl(JNIEnv *env, jobject thiz) {
    return false;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_curl_project_web_WebUnity_setHeader(JNIEnv *env, jobject thiz, jstring info) {
    headers = curl_slist_append(headers,
                                env->GetStringUTFChars(info, nullptr));
}
extern "C"
JNIEXPORT void JNICALL
Java_com_curl_project_web_WebUnity_setHeaderContentType(JNIEnv *env, jobject thiz, jstring info) {
    char type[] = "Content-Type:";
    headers = curl_slist_append(headers, (type + std::string(
            env->GetStringUTFChars(info, nullptr))).c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_curl_project_web_WebUnity_getRequest(JNIEnv *env, jobject thiz, jstring path) {
    std::tuple<int, std::string> tuple = baseRequest(env->GetStringUTFChars(path, nullptr),
                                                     "", false);
    return toAnalysisResTuple(env, path, tuple);
}