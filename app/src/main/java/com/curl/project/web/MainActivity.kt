package com.curl.project.web

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.TextView
import androidx.lifecycle.lifecycleScope
import com.curl.project.web.databinding.ActivityMainBinding
import com.google.gson.Gson
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)


        binding.sampleText.text = "stringFromJNI()"
        val web = WebUnity()
        web.setHeader("Cookie:123123=123123")//设置header
        web.setHeaderContentType("application/json")//设置header的content-type
        web.setTimeOut(30)//设置超时时间
        web.setIsShowLog(true, true)//是否展示curl请求过程中的日志
        lifecycleScope.launch(Dispatchers.IO) {
            try {
                web.baseRequest(
                    "http://111.11.111.111:22222/aaaaaaaa",
                    Gson().toJson(BaseData())//post的数据
                ).let {
                    Log.d("android curl post:", "获取到的数据 ${it}")
                }
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }

        lifecycleScope.launch(Dispatchers.IO) {
            try {
                web.getRequest("https://www.baidu.com/").apply {
                    Log.d("android curl get:", this)
                }
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }
}