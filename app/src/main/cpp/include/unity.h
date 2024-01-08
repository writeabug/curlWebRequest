//
// Created by Administrator on 2024/1/5.
//

#ifndef MY_APPLICATION_UNITY_H
#define MY_APPLICATION_UNITY_H
std::string intToStr(int code);
void Log(const std::string &tag, const std::string &msg);
std::string removeSpaces(std::string str);
std::tuple<int, std::string> analysisRes(CURLcode res, std::string response, long code);
jstring toAnalysisResTuple(JNIEnv *env,jstring path,std::tuple<int, std::string> tuple);
#endif //MY_APPLICATION_UNITY_H
