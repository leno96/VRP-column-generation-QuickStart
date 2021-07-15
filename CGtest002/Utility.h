#pragma once
#pragma warning(disable:4996)
#include <stdlib.h>

#include<time.h>
#include<stdlib.h>
#include<stdarg.h> 
#include<string>
#include<fstream>
#include<iostream>
#include<vector>

#include "Solution.h"

using namespace std;

//#define STR_HELP_ANTS \
//  "  -m, --ants           # number of ants\n"

#define TRUB_MAX_LEN 3
//#define PERTRUB_MAXTIME 10//10//如果strength=4,有的到16次才更新;strength越小，perturb要求的次数越大
//#define MAX_SHAKE_STRENGTH 7//7//6比之前要好，4也不错;4改成了7
//#define RVND_MAX_NONIMPROVE 4 //5
#define SHOW_ROUTE 1

//#define MDVRP 1

//如果是CPTP
#define FIRST_OUT 1
#define FIRST_OUT_2 1
#define FIRST_OUT_3 1

#define CLOSE_SOME

//如果MDVRP
//#define FIRST_OUT 1
//#define FIRST_OUT_2 1
////#define FIRST_OUT_3 1








#ifdef MDVRP
//#define NO_FREQ 1
//这个是基于FREQ的。所以不能在#define NO_FREQ的时候使用。
	#define GUIDE_TRUB 1
	#define MDVRPTEST

	//#define DIFF_DEPOT 1
#else
	#define CVRP 1
	#define SHOW_PACK 1
#endif // MDVRP


//#define NO_FREQREC_LSTEST 1
////如果找到改进就退出，可能这个NO_FREQREC_LS更新会有问题
//#define NO_FREQREC_LS 1




class Utility  
{
public:
	Utility();
	virtual ~Utility();


	static int RVND_MAX_NONIMPROVE;
	static int MAX_SHAKE_STRENGTH;
	static int PERTRUB_MAXTIME;
	static double NUM_DELET_A;
	static double NUM_DELET_B;

    static clock_t m_start;

    static string sLogFileName;
    static ofstream sLogOut;
    static bool sShowLog;

    static string sErrFileName;
    static ofstream sErrOut;
    static double sTimeLimit;//定义静态成员变量
	static void param_in(int argc, char *argv[]) {
		int rvnd_max_num, max_shake_strength, perturb_maxtime;
		int num_delete_a, num_delete_b;

		if (argc != 6) {
			cout << "input wrong!! wrong number of paramter" << endl;
			exit(0);
		}
		sscanf(argv[1], "%d", &rvnd_max_num);
		sscanf(argv[2], "%d", &max_shake_strength);
		sscanf(argv[3], "%d", &perturb_maxtime);
		sscanf(argv[4], "%d", &num_delete_a);
		sscanf(argv[5], "%d", &num_delete_b);

		//cout << "rvnd_max_num=" << rvnd_max_num << endl;
		//cout << "max_shake_strength=" << max_shake_strength << endl;
		//cout << "perturb_maxtime=" << perturb_maxtime << endl;
		//cout << "num_delete_a=" << num_delete_a << endl;

		RVND_MAX_NONIMPROVE = rvnd_max_num;
		MAX_SHAKE_STRENGTH = max_shake_strength;
		PERTRUB_MAXTIME = perturb_maxtime;
		NUM_DELET_A = double(num_delete_a) / 100.0;
		NUM_DELET_B = double(num_delete_b) / 100.0;

	}


    static double Random()//Generate a real [0-1] number
    {
		return 1.0*rand()/RAND_MAX;
    }
    static void SetTimeLimit(double limit)
    {
        sTimeLimit = limit;
    }
    static void SetStartTime(clock_t time)
    {
        m_start = time;//开始计时
    }
	static bool IsTimeLimit()
	{
        return GetPassedTime() > sTimeLimit;
	}
    static double GetPassedTime()
    {
        return 1.0*(clock() - m_start)/CLOCKS_PER_SEC;//从开始计时到“当前”的运行时间，/CLOCKS_PER_SEC是标准格式
    }

    static void SetShowLog(bool showLog)
    {
        sShowLog = showLog;//是否把xx打印出来
    }

    static void SetErrorFileName(string errorFileName)
    {
        if(!sErrFileName.empty())
        {
            sErrOut.close();//为什么要判断是否为空？？
        }
        sErrFileName = errorFileName;
    }
	static string ToString(const char* Msg, ...)//可变参数
	{
        va_list Args;
		static char s[1024];
		va_start(Args, Msg);//指定起始
        vsprintf(s,Msg,Args);//【应该是把Msg的值都放入s】
        va_end(Args);
        return s;
	}
    static void OutPutErrorMsg(const char* Msg, ...)
    {
		if(sErrFileName.empty())
		{
			return;
		}
		if(false == sErrOut.is_open())
		{
			sErrOut.open(sErrFileName.c_str());//如果没打开，就打开
		}
		va_list Args;
		static char s[1024];
		
		va_start(Args, Msg);
        vsprintf(s,Msg,Args);
        va_end(Args);
        sErrOut << s << std::endl;
    }
     static void SetLogFileName(string logFileName)
     {
          if(!sLogFileName.empty())
          {
              sLogOut.close();//关闭之前的，打开这个
          }
          sLogFileName = logFileName;

		  if(sShowLog)
		  {
			  sLogOut.open(sLogFileName.c_str());
		  }
     }
     static void OutPutLogMsg(const char* Msg, ...)
     {
		 //如果 sShowLog为false，则不输出。或者sLogFileName文件名为空，也不输出
          if(false == sShowLog || sLogFileName.empty())
          {
              return;
          }
          va_list Args;
          static char s[1024];

          va_start(Args, Msg);
          vsprintf(s,Msg,Args);
          va_end(Args);
          sLogOut << s << std::endl;
      }
     static void OutPutStampedLogMsg(const char* Msg, ...)
     {
          if(false == sShowLog || sLogFileName.empty())
          {
              return;
          }
          va_list Args;
          static char s[1024];

          va_start(Args, Msg);
		  //依次读入char* ,然后
          vsprintf(s,Msg,Args);
          va_end(Args);
		  //储存到s里，然后再输出
          sLogOut << s << ":\t" << Utility::GetPassedTime() <<endl;
      }


     static string sRouteFileName;
     static ofstream sRouteOut;
     static bool sShowRoute;

     static void SetShowRoute(bool showRoute)
     {
         sShowRoute= showRoute;//是否把route信息打印出来
     }
     static void SetRouteFileName(string routeFileName)
     {
		if(!sRouteFileName.empty())
		{
		  sRouteOut.close();
		}
		sRouteFileName = routeFileName;
		sRouteOut.open(sRouteFileName.c_str());
     }
     static void OutPutRouteMsg(Solution &result, const int m_Nvehicle)
     {
		if(false == sShowRoute || sRouteFileName.empty())
		{
			return;
		}
		
		for(int i = 0; i < m_Nvehicle; ++i)
		{
			if(result[i].Empty())
			{
				continue;
			}
			sRouteOut << "Route_" << i << ": ";
			for(unsigned j = 0; j < result[i].size(); ++j)
			{
				sRouteOut << " " << result[i][j];
			}
			sRouteOut << std::endl;
		}
		sRouteOut << "Total_obj_of_solution: " << result.m_obj << std::endl << std::endl;
	}
	 static void OutPutRouteMsg(Solution &result, const int m_Nvehicle,string string_temp)
	 {
		 if (false == sShowRoute || sRouteFileName.empty())
		 {
			 return;
		 }

		 for (int i = 0; i < m_Nvehicle; ++i)
		 {
			 if (result[i].Empty())
			 {
				 continue;
			 }
			 sRouteOut << "Route_" << i << ": ";
			 for (unsigned j = 0; j < result[i].size(); ++j)
			 {
				 sRouteOut << " " << result[i][j];
			 }
			 sRouteOut << std::endl;
		 }
		 sRouteOut << "Total_cost_of_solution: " << result.m_obj << std::endl;
		 sRouteOut << "above is: " << string_temp << std::endl << std::endl;
	 }
	  
	static void Print1DDoubleArr(vector<double> vec, std::string arrName)
	{
		 std::cout << arrName.c_str() << ":" << std::endl;
		 for (unsigned i = 0; i < vec.size(); ++i)
			 std::cout << vec[i] << "\t";
		 std::cout << std::endl;
	}

	 static void Print1DDoubleArr(vector<double> vec)
	 {
		 for (unsigned i = 0; i < vec.size(); ++i)
			 std::cout << vec[i] << "\t";
		 std::cout << std::endl;
	 }

	 static void Print1DIntArr(vector<int> vec, std::string arrName)
	 {
		 std::cout << arrName.c_str() << ":" << std::endl;
		 for (unsigned i = 0; i < vec.size(); ++i)
			 std::cout << vec[i] << "\t";
		 std::cout << std::endl;
	 }

	 static void Print1DIntArr(vector<int> vec)
	 {
		 for (unsigned i = 0; i < vec.size(); ++i)
			 std::cout << vec[i] << "\t";
		 std::cout << std::endl;
	 }

	 static void Print2DDoubleArr(vector<vector<double> > vec, std::string arrName)
	 {
		 std::cout << arrName.c_str() << ":" << std::endl;
		 for (unsigned i = 0; i < vec.size(); ++i)
		 {
			 for (unsigned j = 0; j < vec[i].size(); ++j)
				std::cout << vec[i][j] << "\t";
			 std::cout << std::endl;
		 }
	 }

	 static void Print2DDoubleArr(vector<vector<double> > vec)
	 {
		 for (unsigned i = 0; i < vec.size(); ++i)
		 {
			 for (unsigned j = 0; j < vec[i].size(); ++j)
				std::cout << vec[i][j] << "\t";
			 std::cout << std::endl;
		 }
	 }
};
