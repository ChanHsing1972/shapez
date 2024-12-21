//#include <iostream>
//#include <fstream>
//using namespace std;
//#define FILENAME "save.txt"
//
//void load() // 读档
//{
//	cout << "正在读取存档……";
//	freopen("jisuan.txt", "r", stdin);
//	// 这里各种输入（像平常那样读入，cin,scanf()等）
//	fclose(stdin);
//}
//
//void save() // 存档
//{
//	cout << "正在为你存档……" << endl;
//	freopen("jisuan.txt", "w", stdout);
//	cout << "ok";
//	fclose(stdout);
//}
//
//int main()
//{
//	fstream _file;
//	_file.open(FILENAME, ios::in); // 打开文件
//	if (_file)                     // 打开失败，通常默认为没有存档
//	{
//		save();
//	}
//}