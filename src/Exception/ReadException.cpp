#include"ReadRequest.hpp"
ReadRequest::ReadException::ReadException()
{
	this->exception_msg = "POLL EXCEPTION";
}
ReadRequest::ReadException::ReadException(const string &exception_msg)
{
	this->exception_msg = exception_msg;
}
const char *ReadRequest::ReadException::what() const throw()
{
	return this->exception_msg.c_str();
}
ReadRequest::ReadException::~ReadException() throw()
{
}