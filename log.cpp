/// \file example.cpp
/// 代码示例文件，演示一个简单CGI流程

#include <iostream>
#include "webapplib.h"
#include "json/json.h"
using namespace webapp;

int main() {
	/***************************************************************************
	演示完整的CGI应用程序流程,模拟WEB查询动作	
	1、读取CGI参数和Cookie数据（String、Cgi、Cookie）
	2、使用读取到的参数调用权限检查接口（ConfigFile、HttpClient）
	3、使用读取到的参数查询数据库（MysqlClient）
	4、使用日志文件记录用户请求（FileSystem、Utility、Encode）
	5、更新用户端Cookie（Cookie、DateTime）
	6、显示HTML页面（Template）
	***************************************************************************/	

	////////////////////////////////////////////////////////////////////////////
	// 1、读取CGI参数和Cookie数据（Cgi、Cookie）
	http_head();	
	Cgi cgi;
    
    //jsontest
    Json::Value root;
    Json::FastWriter writer;
    Json::Value person;
    
    person["name"] = "hello world";
    person["age"] = 100;
    root.append(person);
    
    std::string json_file = writer.write(root);
    cout<<json_file.c_str()<<endl;

	////////////////////////////////////////////////////////////////////////////

	MysqlClient mysqlclient;
	MysqlData mysqldata;

	mysqlclient.connect( "127.0.0.1", "root", "jfh_2017", "test" );
	
	if ( mysqlclient.is_connected() ) {
		string s;
		s = "INSERT INTO log (tm, header, content, dfa) VALUES (from_unixtime(";
		s += "1513926245)";
		s+= ",\'os\',\'";
		s+= cgi["content"];
		s+= "\',\'";
		s+=cgi["dfa"];
		s+="\')";
		cout<<s<<endl;
		mysqlclient.query(s);
	}

	return 0;
	
}

