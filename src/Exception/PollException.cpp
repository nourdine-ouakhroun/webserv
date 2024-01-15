#include"Servers.hpp"
Servers::PollException::PollException()
{
    this->exception_msg = "POLL EXCEPTION";
}
Servers::PollException::PollException(const string &exception_msg)
{
    this->exception_msg = exception_msg;
}
const char *Servers::PollException::what() const throw()
{
    return this->exception_msg.c_str();
}
Servers::PollException::~PollException() throw()
{
}