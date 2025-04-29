#include <stdexcept>
 
struct incorrect_json_format : public std::exception
{
	using std::exception::exception;
	incorrect_json_format(const std::string &msg) : message(msg) {}
	char const *what() const noexcept override
	{
		return message.c_str();
	}
private:
	std::string message;
};
