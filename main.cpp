#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>


/*//### INTERFACE ###/*/
int MenuTask()
{
	std::string command;
	std::cout << "Select task:\n";
	std::cout << "1. Remove substring from strings;\n";
	std::cout << "2. Remove Non-Digits strings;\n";
	std::cout << "3. Remove strings with selected number of vovwels\n";
	std::cout << "4. Remove first and last symbols from strings.\n";
	std::cout << "5. Read file.\n";
	std::cout << "<-1> Exit\n";
	while (true)
	{
		std::cin >> command;
		if (command == "1" || command == "2"
			|| command == "3" || command == "4"
			|| command == "5" || command == "-1")
			return std::stoi(command);
		std::cout << "Wrong, repeat\n";
	}

}

bool H_YESNO(std::string& input)
{
	if (input == "1")
		return true;
	return false;
}

char SetDelimiter()
{
	char c = '0';
	std::string d;
	std::cout << "Which symbol will be used as delimiter?\n";
	while (true)
	{
		std::cin >> d;
		if (d.length() == 1)
			return d[0];
		std::cout << "Wrong, repeat\n";
	}

	return c;
}

std::string SetFileName(bool reading)
{
	std::string handle;
	std::string filename;

	std::cout << "Type filename you would like to ";
	handle = (reading ? "read" : "write");
	std::cout << handle << std::endl;
	while (true)
	{
		std::cout << ">>";
		std::cin >> filename;
		std::cout << "You would like to use filename <" << filename << ">? (1 - yes, any other - no)\n";
		std::cin >> handle;
		if (H_YESNO(handle))
			return filename;
	}
}

template<typename Container>
void CheckData(Container& data)
{
	if (!data.size())
		std::cout << "### WARINING! EMPTY DATA ###\n";
}

/*//### READ - WRITE ###/*/
template<typename Container>
void ReadData(const char* file_name, Container& data) try
{

	std::ifstream file(file_name);
		copy(std::istream_iterator<std::string>(file),
			std::istream_iterator<std::string>(),
			inserter(data, data.begin()));

		// Если с циклом
		/*if (file.is_open())
		{
			std::string values;
			while (!file.eof())
			{
				getline(file, values);
				data.push_back(values);
			}
		}*/
		file.close();
}
catch (std::ifstream::failure& exc)
{
	std::cerr << "Some error while reading\n" << exc.what();
	throw(1);
}


template<typename Container>
void WriteData(const char* file_name, const Container& data) try
{
	CheckData(data);
	std::ofstream file(file_name);

	std::ostream_iterator<int> out(file, "\n");
	std::copy(data.begin(), data.end(), std::ostream_iterator<std::string>(file,"\n"));

	/* // Если с циклом
	for (const auto& item : data)
		file << item << std::endl;
	*/

	file.close();
} 
catch (std::ofstream::failure& exc)
{
	std::cerr << "Some error while writing\n" << exc.what();
	throw(2);
}



/*//### HANDLERS ###/*/
/*//### 1 ###/*/
template<typename Container>
void CutStrings(Container& data)
{
	CheckData(data);
	char delimiter = SetDelimiter();
	std::for_each(data.begin(), data.end(), 
		[&delimiter](std::string &s)
		{
			int f = s.find_first_of(delimiter);
			int  l = s.find_last_of(delimiter);
			std::string s1;
			if (f != -1)
				s.erase(f, l + 1 - f);
		});

	std::string new_file = SetFileName(false);
	WriteData(new_file.c_str(), data);
}

/*//### 2 ###/*/
template<typename Container>
void RemoveAlphabetStrings(Container& data)
{
	CheckData(data);
	data.erase(std::remove_if(data.begin(), data.end(),
		[](std::string& str)
		{
			bool res = std::all_of(str.begin(), str.end(), 
				[](char s){ return (std::isdigit(s));});
			return (!res);
		}), data.end()
	);
	std::string new_file = SetFileName(false);
	WriteData(new_file.c_str(), data);
}

/*//### 3 ###/*/
template<typename Container>
void FilterStrings(Container & data)
{
	CheckData(data);
	std::string vowels = "AEIOUYaeiouy";
	int vowels_count = 2;

	data.erase(std::remove_if(data.begin(), data.end(),
		[&vowels_count, &vowels](std::string& str)
		{
			int res = std::count_if(str.begin(), str.end(),
				[&vowels](char c) { return (vowels.find(c) != std::string::npos); });
			return (res != vowels_count);
		}), data.end()
	);
	std::string new_file = SetFileName(false);
	WriteData(new_file.c_str(), data);
}

/*//### 4 ###/*/
template<typename Container>
void TrimStrings(const Container& data)
{
	CheckData(data);
	Container new_data(data.size());

	std::copy_if(data.begin(), data.end(), new_data.begin(),
		[](const std::string& s) { return(s.length() >= 3);});
	std::for_each(new_data.begin(), new_data.end(), [](std::string& s)
		{if (s.length() >= 3) s = s.substr(1, s.length() - 2); });

	std::string new_file = SetFileName(false);
	WriteData(new_file.c_str(), new_data);
}


/*//### MAIN ###/*/
int main() try
{
	bool loop = true;
	int num = 0;

	std::string new_name;
	std::vector<std::string> content;

	while (loop)
	{
		CheckData(content);
		num = MenuTask();

		switch (num)
		{
		case 1:
			CutStrings(content);
			break;
		case 2:
			RemoveAlphabetStrings(content);
			break;
		case 3:
			FilterStrings(content);
			break;
		case 4:
			TrimStrings(content);
			break;
		case 5:
			content.clear();
			new_name = SetFileName(true);
			ReadData(new_name.c_str(), content);
			break;
		case -1:
			std::cout << "Would you like to exit? (1 - yes, any - no)\n";
			std::cin >> new_name;
			loop = !(H_YESNO(new_name));
			break;
		default:
			std::cout << "Unknown command. Retry\n";
		}

	}

	return 0;
}
catch (int err)
{
	std::cout << "main() error #" << err;
	return err;
}