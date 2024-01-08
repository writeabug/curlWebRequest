package com.curl.project.web

import com.google.gson.Gson
import com.google.gson.reflect.TypeToken

class WebUnity {

    companion object {
        init {
            System.loadLibrary("web")
        }
    }


    external fun baseRequest(path:String,postData:String):String
    external fun setTimeOut(time:Int)
    external fun setIsShowLog(isShowLoad:Boolean,isShowHeaderInfo:Boolean)
    external fun setHeader(info:String)
    external fun setHeaderContentType(info:String)
    external fun getRequest(path:String):String

    inline fun <reified T>postRequest(url: String, postData: String):T?{
        return baseRequest(url,postData).toJson<T>()
    }

    inline fun <reified T> String.toJson(): T {
        return Gson().fromJson<T>(this, object :
            TypeToken<T>() {}.type)
    }
}