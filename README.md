基于curl的网络请求模块

暂时只支持arm64 armv8-7a这两种架构


使用方法：
        val web = WebUnity()//初始化
        web.setHeader("Cookie:123123=123123")//设置header
        web.setHeaderContentType("application/json")//设置header的content-type
        web.setTimeOut(30)//设置超时时间
        web.setIsShowLog(true, true)//是否展示curl请求过程中的日志
    
        lifecycleScope.launch(Dispatchers.IO) {//post请求
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

        lifecycleScope.launch(Dispatchers.IO) {//get请求
            try {
                web.getRequest("https://www.baidu.com/").apply {
                    Log.d("android curl get:", this)
                }
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }

    curl网络请求在遇到错误的时候会抛出一个自定义的错误NetWorkException
