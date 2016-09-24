
#include "FinLibException.h"
#include <string>
#include <vector>
#include <stdexcept>

namespace FinLib{

//Constructor
//FinLibException::FinLibException();
FinLibException::FinLibException(const std::string &message){
	m_message = message;
}

void FinLibException::SetMessage(const std::string &message){
	m_message = message;
}

std::string FinLibException::What(){
	return m_message;
}

}

