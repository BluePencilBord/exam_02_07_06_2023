#include <iostream>
#include <string>
#include <cstdlib>
#include <list>
#include <vector>

using namespace std;

struct FileInformation 
{
	explicit FileInformation(string name_ = "", string extension_ = "", 
		string datetime_ = "", string attributes_ = "", 
		unsigned long long int size_ = 0) : name(name_), extension(extension_),
		datetime(datetime_), attributes(attributes_), size(size_)
	{}

	bool operator==(FileInformation const& fi) const
	{
		if ((name == fi.name) && (extension == fi.extension)
			&& (datetime == fi.datetime) && (attributes == fi.attributes)
			&& (size == fi.size)) return true;
		else return false;
	}

	void Read()
	{
		cout << "Введите дату и время создания(ДД.ММ.ГГГГ ЧАС:МИН): "; getline(cin, datetime);
		cout << "Введите название файла: "; getline(cin, name);
		cout << "Введите расширение: "; cin >> extension;
		cout << "Введите аттрибуты: "; cin >> attributes;
		cout << "Введите размер файла: "; cin >> size; cout << endl;
	}

	void Display()
	{
		cout << "Название файла: " << name
		<< "\nРасширение: " << extension
		<< "\nДата и время создания(ДД:ММ:ГГ ЧАС:МИН): " << datetime
		<< "\nАттрибуты: " << attributes
		<< "\nРазмер файла: " << size << endl;
	}

	string get_name()
	{
		return name;
	}

	string get_extension()
	{
		return extension;
	}

	string get_datetime()
	{
		return datetime;
	}

	string get_attributes()
	{
		return attributes;
	}

	unsigned long long int get_size()
	{
		return size;
	}

private:
	string name;
	string extension;
	string datetime;
	string attributes;
	unsigned long long int size;
};

struct File
{
	File() = default;
	explicit File(FileInformation file_info_, string del_sign_ = "", string alloc_sign_ = "") :
		file_info(file_info_), del_sign(del_sign_), alloc_sign(alloc_sign_)
	{}

	bool operator==(File const& file) const
	{
		if ((del_sign == file.del_sign) && (alloc_sign == file.alloc_sign) && (file_info == file.file_info))
		{
			return true;
		}
		else return false;
	}

	string get_del_sign()
	{
		return del_sign;
	}

	string get_alloc_sign()
	{
		return alloc_sign;
	}

	FileInformation get_file_info()
	{
		return file_info;
	}

	void Read()
	{
		file_info.Read();
		cout << "Введите признак выделения: "; cin >> alloc_sign;
		cout << "\nВведите признак удаления: "; cin >> del_sign; cout << endl;
	}

	void Display()
	{
		file_info.Display();
		cout << "Признак выделения: " << alloc_sign
		<< "\nПризнак удаления: " << del_sign << endl;
	}

private:
	FileInformation file_info;
	string del_sign = "";
	string alloc_sign = "";
};

struct Directory
{
	Directory(string name_) : name(name_) 
	{}

	File& operator[](unsigned int index)
	{
		return file_list[index];
	}

	vector<File>& get_file_list()
	{
		return file_list;
	}

	unsigned int get_file_count()
	{
		return file_count;
	}

	void AddFile()
	{
		File file;
		file.Read();
		file_list.push_back(file);
		++file_count;
	}

	void AddFile(File file)
	{
		file_list.push_back(file);
		++file_count;
	}

	void DeleteFile()
	{
		string del_file_name;
		cout << endl << "Введите название удаляемого файла: "; cin >> del_file_name;
		for (vector<File>::iterator i = file_list.begin(); i != file_list.end(); ++i)
		{
			if (i->get_file_info().get_name() == del_file_name)
			{
				file_list.erase(i);
				--file_count;
				break;
			}
		}
		cout << "Файл с введенным именем не найден" << endl;
	}

	unsigned int file_name_search(string file_name)
	{
		for (int i = 0; i != file_count; ++i)
		{
			if (file_list[i].get_file_info().get_name() == file_name)
			{
				return i;
			}
		}
		cout << "Файл с введенным именем не найден" << endl;
	}

	unsigned int file_extension_search(string file_extension)
	{
		for (int i = 0; i != file_count; ++i)
		{
			if (file_list[i].get_file_info().get_extension() == file_extension)
			{
				return i;
			}
		}
		cout << "Файл с введенным расширением не найден" << endl;
	}

	unsigned int file_datetime_search(string file_datetime)
	{
		for (int i = 0; i != file_count; ++i)
		{
			if (file_list[i].get_file_info().get_datetime() == file_datetime)
			{
				return i;
			}
		}
		cout << "Файл с введенной датой не найден" << endl;
	}

	unsigned long long int directory_full_size()
	{
		unsigned long long int full_size = 0;
		for (vector<File>::iterator i = file_list.begin(); i != file_list.end(); ++i)
		{
			full_size += i->get_file_info().get_size();
		}
		return full_size;
	}

	void directory_union(Directory& dir1, Directory& dir2)
	{
		file_list.clear();
		file_count = dir1.get_file_count();
		file_list = dir1.get_file_list();
		bool addfile = 1;
		for (vector<File>::iterator i = dir2.get_file_list().begin(); i != dir2.get_file_list().end(); ++i)
		{
			for (vector<File>::iterator j = dir1.get_file_list().begin(); j != dir1.get_file_list().end(); ++j)
			{
				if (*i == *j) addfile = 0;
			}
			if (addfile)
			{
				file_list.push_back(*i);
				++file_count; 
			}
			else addfile = 1;
		}
	}

	void directory_intersection(Directory& dir1, Directory& dir2)
	{
		file_list.clear();
		file_count = 0;
		bool addfile = 0;
		for (vector<File>::iterator i = dir2.get_file_list().begin(); i != dir2.get_file_list().end(); ++i)
		{
			for (vector<File>::iterator j = dir1.get_file_list().begin(); j != dir1.get_file_list().end(); ++j)
			{
				if (*i == *j) addfile = 1;
			}
			if (addfile)
			{
				file_list.push_back(*i);
				++file_count;
				addfile = 0;
			}
		}
	}

private:
	string name = "";
	unsigned int file_count = 0;
	vector<File> file_list;
};

struct Group
{
	void group_by_date_less_than(Directory& dir, string date)
	{
		file_group.clear();
		string day_str = "";
		day_str = day_str + date[0] + date[1];
		string month_str = "";
		month_str = month_str + date[3] + date[4];
		string year_str = "";
		year_str = year_str + date[6] + date[7] + date[8] + date[9];
		int year = stoi(year_str);
		int month = stoi(month_str);
		int day = stoi(day_str);
		string dir_date = "";
		string dir_month = "";
		string dir_day = "";
		for (vector<File>::iterator i = dir.get_file_list().begin(); i != dir.get_file_list().end(); ++i)
		{
			for (int j = 6; j != 10; ++j) dir_date = dir_date + i->get_file_info().get_datetime()[j];

			for (int j = 3; j != 5; ++j) dir_month = dir_month + i->get_file_info().get_datetime()[j];

			for (int j = 0; j != 2; ++j) dir_day = dir_day + i->get_file_info().get_datetime()[j];

			if (year > stoi(dir_date))
			{
				file_group.push_back(*i);
			}
			else if ((year = stoi(dir_date)) && (month > stoi(dir_month)))
			{
				file_group.push_back(*i);
			}
			else if ((month = stoi(dir_month)) && (day > stoi(dir_day)))
			{
				file_group.push_back(*i);
			}

			dir_date = "";
			dir_month = "";
			dir_day = "";
		}
	}

	void group_by_date_greater_than(Directory& dir, string date)
	{
		file_group.clear();
		string day_str = "";
		day_str = day_str + date[0] + date[1];
		string month_str = "";
		month_str = month_str + date[3] + date[4];
		string year_str = "";
		year_str = year_str + date[6] + date[7] + date[8] + date[9];
		int year = stoi(year_str);
		int month = stoi(month_str);
		int day = stoi(day_str);
		string dir_date = "";
		string dir_month = "";
		string dir_day = "";
		for (vector<File>::iterator i = dir.get_file_list().begin(); i != dir.get_file_list().end(); ++i)
		{
			for (int j = 6; j != 10; ++j) dir_date = dir_date + i->get_file_info().get_datetime()[j];

			for (int j = 3; j != 5; ++j) dir_month = dir_month + i->get_file_info().get_datetime()[j];

			for (int j = 0; j != 2; ++j) dir_day = dir_day + i->get_file_info().get_datetime()[j];

			if (year < stoi(dir_date))
			{
				file_group.push_back(*i);
			}
			else if ((year = stoi(dir_date)) && (month < stoi(dir_month)))
			{
				file_group.push_back(*i);
			}
			else if ((month = stoi(dir_month)) && (day < stoi(dir_day)))
			{
				file_group.push_back(*i);
			}

			dir_date = "";
			dir_month = "";
			dir_day = "";
		}
	}

	void group_by_attribute(Directory& dir, string attribute)
	{
		file_group.clear();
		for (vector<File>::iterator i = dir.get_file_list().begin(); i != dir.get_file_list().end(); ++i)
		{
			if (i->get_file_info().get_attributes().find(attribute) != string::npos) file_group.push_back(*i);
		}
	}

	void group_by_size_less_than(Directory& dir, unsigned int size_)
	{
		file_group.clear();
		for (vector<File>::iterator i = dir.get_file_list().begin(); i != dir.get_file_list().end(); ++i)
		{
			if (i->get_file_info().get_size() < size_) file_group.push_back(*i);
		}
	}

	void group_by_size_greater_than(Directory& dir, unsigned int size_)
	{
		file_group.clear();
		for (vector<File>::iterator i = dir.get_file_list().begin(); i != dir.get_file_list().end(); ++i)
		{
			if (i->get_file_info().get_size() > size_) file_group.push_back(*i);
		}
	}

	void group_by_del_sign(Directory& dir, string del_sign_)
	{
		file_group.clear();
		for (vector<File>::iterator i = dir.get_file_list().begin(); i != dir.get_file_list().end(); ++i)
		{
			if (i->get_del_sign() == del_sign_) file_group.push_back(*i);
		}
	}

	vector<File>& get_file_group()
	{
		return file_group;
	}

private:
	vector<File> file_group;
};

int main()
{
	// создание и доавление файла
	FileInformation file_inf("file_1", "txt", "10.01.2000 18:24", "read_only", 1024);
	File file_1(file_inf, "ds1", "as1");
	Directory dir1("dir1");
	dir1.AddFile(file_1);

	// создание и доавление файла
	FileInformation file_inf2("file_2", "txt", "11.01.2000 18:35", "read_only system", 1024);
	File file_2(file_inf2, "ds1", "as1");
	Directory dir2("dir2");
	dir2.AddFile(file_1);
	dir2.AddFile(file_2);

	// тест метода объединения directory_union
	Directory dir3("dir3");
	dir3.directory_union(dir1, dir2);

	// тест метода пересечения directory_intersection
	Directory dir4("dir4");
	dir4.directory_intersection(dir1, dir2);

	// тест класса группа и метода группировки файлов по размеру меньше чем group_by_size_less_than
	Group grp;
	grp.group_by_size_less_than(dir2, 1500);

	cout << "Тестирование метода группировки файлов по размеру меньше чем(правильный вывод 2 и file_1, file_2):\n";
	cout << grp.get_file_group().size() << endl;
	cout << grp.get_file_group()[0].get_file_info().get_name() << endl;
	cout << grp.get_file_group()[1].get_file_info().get_name() << endl << endl;

	// тест метода группировки файлов по дате меньше чем group_by_date_less_than
	grp.group_by_date_less_than(dir2, "11.01.2000");

	cout << "Тестирование метода группировки файлов по дате меньше чем и подсчета кол-ва файлов(правильный вывод 1 и file_1):\n";
	cout << grp.get_file_group().size() << endl;
	cout << grp.get_file_group()[0].get_file_info().get_name() << endl << endl;

	cout << "Тестирование метода вычисления полного объема директории(правильный вывод 2048):\n";
	cout << dir2.directory_full_size() << endl;

	cout << "Тестирование метода добавления файла в директорию и подсчета кол-ва файлов(правильный вывод 1 и file_1):\n";
	cout << dir1.get_file_count() << endl;
	cout << dir1.get_file_list()[0].get_file_info().get_name() << endl << endl;

	cout << "Тестирование метода добавления файла в директорию(правильный вывод file_1 и file_2):\n";
	cout << dir2.get_file_list()[0].get_file_info().get_name() << endl;
	cout << dir2.get_file_list()[1].get_file_info().get_name() << endl << endl;

	cout << "Тестирование метода объединения directory_union и подсчета кол-ва файлов(правильный вывод 2 и file_1, file_2):\n";
	cout << dir3.get_file_count() << endl;
	cout << dir3.get_file_list()[0].get_file_info().get_name() << endl;
	cout << dir3.get_file_list()[1].get_file_info().get_name() << endl << endl;

	cout << "Тестирование метода пересечения directory_intersection и подсчета кол-ва файлов(правильный вывод 1 и file_1):\n";
	cout << dir4.get_file_count() << endl;
	cout << dir4.get_file_list()[0].get_file_info().get_name() << endl << endl;

	return 0;
}