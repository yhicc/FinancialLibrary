
#ifndef FINLIBEXCEPTION_H_INCLUDED_
#define FINLIBEXCEPTION_H_INCLUDED_

#include <string>
#include <stdexcept>

namespace FinLib{

class FinLibException : public std::exception{

public:
	
	FinLibException();
	FinLibException(const std::string &message);
	void SetMessage(const std::string &message);
	
	std::string What();
	
private:
	
	std::string m_message;
	
};

}

#endif

