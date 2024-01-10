#include"ManageServers.hpp"
ManageServers::PollException::PollException()
{
    this->exception_msg = "POLL EXCEPTION";
}
ManageServers::PollException::PollException(const std::string &exception_msg)
{
    this->exception_msg = exception_msg;
}
const char *ManageServers::PollException::what() const throw()
{
    return this->exception_msg.c_str();
}
ManageServers::PollException::~PollException() throw()
{
}