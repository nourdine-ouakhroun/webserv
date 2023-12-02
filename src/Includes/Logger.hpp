#ifndef	LOGGER_HPP
#define LOGGER_HPP

#include "String.hpp"
#include <ctime>

class	Logger
{
		static String	getTime()
		{
			time_t rawtime;
			time(&rawtime);
			return (String(ctime (&rawtime)).trim(" \r\t\n"));
		}
	public :
		template <class Streamer, typename Value>
		static void info(Streamer& stream , const String& message, const Value& value)
		{
			stream << getTime() << " [INFO]    : " << message << value << std::endl;
		}

		template <class Streamer, typename Value>
		static void error(Streamer& stream , const String& message, const Value& value)
		{
			stream << getTime() << " [ERROR]   : " << message << value << std::endl;
		}

		template <class Streamer, typename Value>
		static void success(Streamer& stream , const String& message, const Value& value)
		{
			stream << getTime() << " [SUCCESS] : " << message << value << std::endl;
		}

		template <class Streamer, typename Value>
		static void warn(Streamer& stream , const String& message, const Value& value)
		{
			stream << getTime() << " [WARN]    : " << message << value << std::endl;
		}

		template <class Streamer, typename Value>
		static void debug(Streamer& stream , const String& message, const Value& value)
		{
			stream << getTime() << " [DEBUG]   : " << message << value << "" << std::endl;
		}
};

#endif
