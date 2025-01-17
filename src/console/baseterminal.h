
//==============================================================================
//
//  terminal.h - exported_h
//
//  Lightweight and efficient, general purpose terminal to accept commands
//  and manipulate variables.
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================


#ifndef __TERMINAL_H__
#define __TERMINAL_H__

/**
 CONSOLE MANUAL : BUILT IN COMMANDS
 (For API Usage, see the class documentation immediately below)
 -- help : When using the console, type help to get general information, or help <topic> to print the help string on a given topic
 -- listCmd lists the commands bound to the console.
 --listCVars lists all the variables bound to the console.
 --listHelp lists all the topics available for the help command
 --var : You can create new variables in the console itself (in addition to from client code, see below) by using var :
 eg, "var health 100"
 -- echo : eg. echo health - prints the value of the variable to the console
 -- set : eg. set health 25 - sets the value of a variable in the console
 -- runFile <filename> - execute all the commands in a file as if the user typed them in sequence.
 
 --$: Strings prefixed with $ are interpreted as variable names to dereference, and the identifiers will be replaced in the input with the variable value - eg.
 var x listCmd
 help $x # will be processed as "help listCmd"
 
 --Lines starting with # are counted as comments and ignored
 
**/



#include <iostream>
#include <fstream>
#include <unordered_map>
#include <deque>
#include <functional>
#include <sstream>
#include <memory>
#include <string>
#include <cctype>

#include "log.h"
class BaseTerminal
{
public:                                               // the methods in this section are what you should use in your code
	static const unsigned int defaultHistorySize = 10u; ///size of the history file

	typedef std::function<void(std::istream &is, std::ostream &os)> ConsoleFunc;

	typedef std::unordered_map<std::string, ConsoleFunc> CommandTable;
	typedef std::unordered_map<std::string, ConsoleFunc> CVarReadTable;
	typedef std::unordered_map<std::string, ConsoleFunc> CVarPrintTable;
	typedef std::unordered_map<std::string, std::string> HelpTable;

	/// Constructor binds the default commands to the command table & initializes history buffer
	BaseTerminal(std::size_t maxHistory = defaultHistorySize);

	// --------------------------------------//
	/* --------- COMMAND EXECUTION --------- */
	// --------------------------------------//
	// call one of these functions to make the console do things - eg. the first one when you press enter
	// and your GUI console widget gives you a string
	// You can execute all commands in an input stream until EOF with executeUntilEOF(),
	// or run every line in a file (eg. a startup or debug playback file) with executeFile()

	/// Execute command line passed in as string.  Console output goes to "output"
	void commandExecute(const std::string &str, std::ostream &output);

	/// Get a command line from the input stream and execute it.   Console output goes to "output"
	void commandExecute(std::istream &input, std::ostream &output);

	/// execute commands from an istream until EOF
	void executeUntilEOF(std::istream &f, std::ostream &output);

	/// execute commands from a file (named by the input string 'f') until EOF
	void executeFile(const std::string &f, std::ostream &output);

	//------------------------------------//
	/*----------- ADDING CVARS -----------*/
	//-------------------------------------//
	// Call one of these to add variables (eg, Player.health) from your c++ code to the console.
	// This makes them available to built in commands using the given "varname" - dereference them with $, print them with echo, and set them with "set"

	/// function which takes in a string and a variable from client code we want to associate with it in the console.  Takes in an optional help string to describe the variable to the user.
	template <class T>
	void bindCVar(const std::string &varname, T &var, const std::string &help = "");

	// ------------------------------------//
	/* --------- ADDING COMMANDS ----------*/
	// ------------------------------------//
	// You should call console.bindCommand() with the command name you want and a C++ function from your code with an arbitrary signature.  Optionally provide a help string for the command as a third argument
	// You shouldn't need to worry about which overload or template arguments to use - it should resolve automatically.
	// If your function has arguments, so long as there is an istream operator to read them from the input,
	// the console will automatically parse them with variadic template and lambda magic, so you never have to write a parser
	// IF YOU ARE USING A MEMBER FUNCTION - we need to wrap the "this" pointer too, so call bindMemberCommand() which takes the object reference as the first argument after the command name

	/// add a command to the console using a function pointer that takes no arguments.  set the associated help string to the "help" argument
	void bindCommand(const std::string &commandName, void(*fptr)(void), const std::string &help = "");

	///add a command to the console using a function pointer that takes arbitrary arguments, then set the associated help string to the "help" argument
	template <typename... Args>
	void bindCommand(const std::string &commandName, void(*fptr)(Args...), const std::string &help = "");

	///add a command to the console using a function object with arbitrary arguments, then set the associated help string to the optional "help" argument
	template <typename... Args>
	void bindCommand(const std::string &commandName, std::function<void(Args...)> fun, const std::string &help = "");

	/// same as bindCommand, but for a member function of an object.  Object instance is first argument after the command name
	template <typename O, typename... Args>
	void bindMemberCommand(const std::string &commandName, O &obj, void (O::*fptr)(Args...), const std::string &help = "");

	/// the bindCommand that actually does the work of adding commands to the table AFTER they've been coerced to a ConsoleFunc that takes the input/output from executeCommand.  Takes optional help string.
	void bindCommand(const std::string &commandName, ConsoleFunc f, const std::string &help = "");

	// ------------------------------------//
	/* --------- HISTORY FILES ----------- */
	// ------------------------------------//
	/* Saving and loading the history buffer of previously executed commands to file  */

	/// populate the command buffer from an input file named by string inFile
	bool loadHistoryBuffer(const std::string &inFile);

	/// write the history buffer to file named by string outFile
	void saveHistoryBuffer(const std::string &outFile);

	/// populate the command buffer from an input file named by istream inFile
	void loadHistoryBuffer(std::istream &inFile);

	/// write the history buffer to file named by ostream outFile
	void saveHistoryBuffer(std::ofstream &outfile);

	// ------------------------------------//
	/* ----------OUTPUT STYLING------------*/
	// ------------------------------------//

	/// begin / end pairs that wrap output of a certain type
	struct ConsoleStyling
	{
		std::pair<std::string, std::string> error;
		std::pair<std::string, std::string> warning;
		std::pair<std::string, std::string> echo;
	};

	/// ANSI color codes wrap the output in the scope.
	inline static ConsoleStyling ConsoleStylingColor() { return{ { "\u001b[37;41;1m[error]: ", "\u001b[0m" },{ "\u001b[33;1m[warning]: ", "\u001b[0m" },{ "\u001b[32;1m> ", "\u001b[0m" } }; }

	/// Styling parameters with no colors, just tags
	inline static ConsoleStyling ConsoleStylingPlain() { return{ { "[error]: ", "" },{ "[warning]: ", "" },{ "> ", "" } }; }

	ConsoleStyling style = ConsoleStylingPlain(); ///< public so user can modify.  Initialized using uncolored output

												  // - sugared getters for console implementation -

	const std::pair<std::string, std::string> &error() { return style.error; }
	const std::pair<std::string, std::string> &warning() { return style.warning; }
	const std::pair<std::string, std::string> &echo() { return style.echo; }

	// ------------------------------------//
	/* ----OTHER GETTERS AND SETTERS-------*/
	// ------------------------------------//

	/// sets the help string (see built in 'help' command) for a given topic
	void setHelpTopic(const std::string &topic, const std::string &data);

	const std::deque<std::string> &historyBuffer() const;
	inline const CommandTable &getCommandTable() const { return commandTable; }
	inline const CVarReadTable &getCVarReadTable() const { return cvarReadFTable; }
	inline const CVarPrintTable &getCVarPrintTable() const { return cvarPrintFTable; }
	inline const HelpTable &getHelpTable() const { return helpTable; }

protected:
	/// WindowedQueue - We implement a ring buffer for the command history as a queue
	template <class T>
	class WindowedQueue : public std::deque<T>
	{
		void fix_size()
		{
			while (std::deque<T>::size() > m_capacity)
			{
				std::deque<T>::pop_front();
			}
		}

		std::size_t m_capacity;

	public:
		WindowedQueue(std::size_t maxCapacity) : m_capacity(maxCapacity)
		{
		}

		void capacity(std::size_t newCapacity)
		{
			m_capacity = newCapacity;
			fix_size();
		}

		std::size_t capacity() const
		{
			return m_capacity;
		}
		using std::deque<T>::operator[];

		template <class... Args>
		void emplace(Args &&... args)
		{
			std::deque<T>::emplace_back(std::forward<Args>(args)...);
			fix_size();
		}

		void push(const T &value)
		{
			std::deque<T>::push_back(value);
			fix_size();
		}

		void push(T &&value)
		{
			std::deque<T>::push_back(value);
			fix_size();
		}
		void pop()
		{
			std::deque<T>::pop_front();
		}
	};

	typedef WindowedQueue<std::string> ConsoleHistoryBuffer;

	ConsoleHistoryBuffer history_buffer; ///< history buffer of previous commands

										 /// maps strings naming cVars to functions which read them from a std::istream.
										 /// This allows the console to parse variables of any type representable as text without modifying the console code or adding custom parsing code.
	CVarReadTable cvarReadFTable;

	/// maps strings naming cVars to functions which write the variables of arbitrary type out to a std::ostream
	CVarPrintTable cvarPrintFTable;

	///maps strings to std function objects, representing the available commands to the user.  eg, quit, set, etc
	CommandTable commandTable;

	/// maps names of functions or cvars to string literals containing helpful information on their use
	HelpTable helpTable;

	///function which simply sets the value of an arbitrary type based on what's in the input stream
	///the arguments are "eaten" by std bind, allowing it to be stored as type void (*x)(void) in the cvarReadFTable
	template <class T>
	void setCvar(std::istream &is, std::ostream &os, T *var);

	///function which simply prints the value of a variable to an output stream
	///the arguments are "eaten" by std bind, allowing it to be stored as type void (*x)(void) in the cvarPrintFTable
	template <class T>
	void printCvar(std::ostream &os, T *var);

	///dumps a list of available commands to the output stream
	void listCmd(std::ostream &os) const;

	///dumps a list of bound cvars to the output stream
	void listCVars(std::ostream &os) const;

	///dumps a list of available help topics to the output stream
	void listHelp(std::ostream &os) const;

	///The function associated with the built in command "set" which parses the name of a cvar, and if it is bound, sets the value based on
	///the value in the input stream
	void commandSet(std::istream &is, std::ostream &os);

	///the function associated with built in command "echo", which prints the value of a cvar if it is bound. if not, reports an error.
	void commandEcho(std::istream &is, std::ostream &os);

	///prints help on a topic if the user types help < topic >, or a generic help message if the user just types help
	void commandHelp(std::istream &, std::ostream &);

	///creates a string variable from the console
	void commandVar();

	///wrapper function which parses arguments to a function object of arbitrary type from the console's input stream then executes the function if the parsing was successful
	template <typename... Args>
	void parse(std::istream &is, std::ostream &os, std::function<void(Args...)> f);

	///This function is called by populateAndExecute, and only executes the bound function if the parsing succeeds
	///if parsing failed we do not want to pass in uninitialized garbage to the c++ function we bound
	template <typename... Args>
	void conditionalExecute(std::istream &is, std::ostream &os, std::function<void(Args...)> f, const Args &... args);

	/// adds the built-in commands to the command table
	void bindBasicCommands();

	/// helper function to return a default-constructed temp variable of a particular type.  Used in parsing arguments for C++ functions bound to the console
	template <class T>
	T makeTemp();

	/// for parsing arguments to C++ functions bound to the console.  variadic template that recursively parses our function arguments in order
	template <typename FirstType, typename... Args>
	void populateTemps(std::istream &is, FirstType &in, Args &... Temps);

	/// for parsing arguments to C++ functions bound to the console. variadic template that recursively parses our function arguments in order.  base case
	template <typename FirstType>
	void populateTemps(std::istream &is, FirstType &in);

	/// for parsing arguments to C++ functions bound to the console. variadic template that recursively parses our function arguments in order.  base case
	void populateTemps(std::istream &is) {}

	/// for parsing arguments to C++ functions bound to the console.  call starts populating temp variables
	template <typename... Args>
	void goPopulateTemps(std::istream &is, Args &... temps);

	/// for parsing arguments to C++ functions bound to the console.  Populates the temp variables using the istream, then calls the function with them
	template <typename... Args>
	void populateAndExecute(std::istream &is, std::ostream &os, std::function<void(Args...)> f,
		typename std::remove_const<typename std::remove_reference<Args>::type>::type... temps);

	/// helper function that takes an input line containing commands and dereferences any console variables whose names appear prefixed by the $ symbol
	void dereferenceVariables(std::istream &is, std::ostream &os, std::string &str);

	/// assigns the value of a dynamically created console variable using the console's input stream
	template <class T>
	void assignDynamicVariable(std::istream &is, std::shared_ptr<T> var);

	/// writes a dynamically created console variable to the console output
	template <class T>
	void writeDynamicVariable(std::ostream &os, std::shared_ptr<T> var);

	/// bind dynamically created console variable to the variable table and set the associated help string
	template <class T>
	void bindDynamicCVar(const std::string &var, const T &value, const std::string &help);

	/// bind dynamically created console variable to the variable table.
	template <class T>
	void bindDynamicCVar(const std::string &var, const T &valueIn);

public:
	/// Dynamic variables are string-based variables that can be created, assigned, and dereferenced from the console itself rather than C++ code
	struct DynamicVariable : public std::string
	{
	};

	// -----------------------------------------------------------------------------
	// EndOfLineEscapeStreamScope : by Steve132
	// For wrapping the output of an ostream push sequence << in a begin / end tag
	// Used for console output formatting
	// -----------------------------------------------------------------------------
	typedef std::pair<std::string, std::string> EndOfLineEscapeTag;

	struct BaseEndOfLineEscapeStreamScope
	{
	protected:
		EndOfLineEscapeTag tag;
		std::ostream &os;

		BaseEndOfLineEscapeStreamScope(const EndOfLineEscapeTag &ttag, std::ostream &tout) : tag(ttag),
			os(tout)
		{
			os << tag.first; //you can overload this for custom ostream types with a different color interface
							 //this might also have a stack interface for if you need multiple resets
		}

		friend BaseEndOfLineEscapeStreamScope operator<<(std::ostream &out, const EndOfLineEscapeTag &tg);

	public:
		template <class T>
		BaseEndOfLineEscapeStreamScope &operator<<(T &&t)
		{
			os << std::forward<T>(t);
			return *this;
		}

		BaseEndOfLineEscapeStreamScope &operator<<(std::ostream &(&M)(std::ostream &))
		{
			M(os);
			return *this;
		}

		~BaseEndOfLineEscapeStreamScope()
		{
			os << tag.second;
		}
	};
};

inline BaseTerminal::BaseEndOfLineEscapeStreamScope operator<<(std::ostream &os, const BaseTerminal::EndOfLineEscapeTag &tg)
{
	return BaseTerminal::BaseEndOfLineEscapeStreamScope(tg, os);
}


template <class T>
inline T BaseTerminal::makeTemp()
{
	T temp{};
	return temp;
}

template <typename... Args>
inline void BaseTerminal::conditionalExecute(std::istream &is, std::ostream &os, std::function<void(Args...)> f, const Args &... args)
{
	if (is.fail())
	{
		os << error() << "Syntax error in function arguments." << std::endl;
		is.clear();
	}
	else
	{
		f(args...);
	}
}

template <typename FirstType>
inline void BaseTerminal::populateTemps(std::istream &is, FirstType &in)
{
	is >> in;
}

//variadic template that recursively parses our function arguments in order
template <typename FirstType, typename... Args>
inline void BaseTerminal::populateTemps(std::istream &is, FirstType &in, Args &... Temps)
{
	is >> in;
	populateTemps(is, Temps...);
}

template <typename... Args>
inline void BaseTerminal::goPopulateTemps(std::istream &is, Args &... temps)
{
	populateTemps(is, temps...);
}

template <typename... Args>
inline void BaseTerminal::populateAndExecute(std::istream &is, std::ostream &os, std::function<void(Args...)> f,
	typename std::remove_const<typename std::remove_reference<Args>::type>::type... temps)
{
	goPopulateTemps<typename std::remove_const<typename std::remove_reference<Args>::type>::type...>(is, temps...);

	conditionalExecute<Args...>(is, os, f, temps...);
}

//function that gets bound as type void to the console with a second function object that may have multiple arguments of various types
//this function gets called when the user enters the function name into the console.
template <typename... Args>
inline void BaseTerminal::parse(std::istream &is, std::ostream &os, std::function<void(Args...)> f)
{

	//first we have to create a bunch of temp variables and pass them into the populateAndExecute function
	//the temp variables are needed to store the result.  There's no guarantee in c++ for argument evaluation order, so we can't just
	//skip the intermediate step of passing constructed temps into a second function
	populateAndExecute<Args...>(is, os, f, (makeTemp<typename std::remove_const<typename std::remove_reference<Args>::type>::type>())...);
}


inline void BaseTerminal::bindCommand(const std::string &str, void(*fptr)(void), const std::string &help)
{
	commandTable[str] = [fptr](std::istream &, std::ostream &) { fptr(); };

	if (help.length())
		setHelpTopic(str, help);
}

template <typename... Args>
inline void BaseTerminal::bindCommand(const std::string &str, void(*fptr)(Args...), const std::string &help)
{
	commandTable[str] =
		[this, fptr](std::istream &is, std::ostream &os) {
		auto fo = std::function<void(Args...)>(fptr);
		this->parse<Args...>(is, os, fo);
	};

	if (help.length())
		setHelpTopic(str, help);
}

template <typename... Args>
inline void BaseTerminal::bindCommand(const std::string &str, std::function<void(Args...)> fun, const std::string &help)
{
	commandTable[str] =
		[this, fun](std::istream &is, std::ostream &os) {
		this->parse<Args...>(is, os, fun);
	};

	if (help.length())
		setHelpTopic(str, help);
}

inline void BaseTerminal::bindCommand(const std::string &str, ConsoleFunc fun, const std::string &help)
{
	if (help.length())
		setHelpTopic(str, help);

	commandTable[str] = fun;
}

inline void BaseTerminal::setHelpTopic(const std::string &str, const std::string &data)
{
	helpTable[str] = data;
}

template <class T>
inline void BaseTerminal::bindCVar(const std::string &str, T &var, const std::string &help)
{
	cvarReadFTable[str] =
		[this, &var](std::istream &is, std::ostream &os) {
		this->setCvar<T>(is, os, &var);
	};

	cvarPrintFTable[str] =
		[this, &var](std::istream &is, std::ostream &os) {
		this->printCvar<T>(os, &var);
	};

	if (help.length())
		setHelpTopic(str, help);
}

template <class T>
void BaseTerminal::setCvar(std::istream &is, std::ostream &os, T *var)
{
	T tmp; ///temp argument is a necessity; without it we risk corruption of our variable value if there is a parse error.  Should be no issue unless someone is using this to parse a ginormous structure or copy construction invokes a state change.

	is >> tmp;

	if (is.fail())
	{
		os << error() << "SYNTAX ERROR IN VARIABLE PARSER" << std::endl;
		is.clear();
	}
	else
	{
		*var = tmp;
	}
}

template <class T>
inline void BaseTerminal::printCvar(std::ostream &os, T *var)
{
	os << *var << std::endl;
}

template <class T>
inline void BaseTerminal::assignDynamicVariable(std::istream &is, std::shared_ptr<T> var)
{
	is >> (*var);
}

template <class T>
inline void BaseTerminal::writeDynamicVariable(std::ostream &os, std::shared_ptr<T> var)
{
	const T &deref = *(var);
	os << deref;
}

template <class T>
inline void BaseTerminal::bindDynamicCVar(const std::string &var, const T &value, const std::string &help)
{
	helpTable[var] = help;
	bindDynamicCVar(var, value);
}

template <class T>
inline void BaseTerminal::bindDynamicCVar(const std::string &var, const T &valueIn)
{
	std::shared_ptr<T> ptr(new T(valueIn));

	cvarReadFTable[var] =
		[this, ptr](std::istream &is, std::ostream &os) {
		this->assignDynamicVariable<T>(is, ptr);
	};

	cvarPrintFTable[var] =
		[this, ptr](std::istream &is, std::ostream &os) {
		this->writeDynamicVariable<T>(os, ptr);
	};
}

inline void BaseTerminal::executeUntilEOF(std::istream &f, std::ostream &output)
{
	while (!f.eof())
	{
		commandExecute(f, output);
	}
}

inline void BaseTerminal::executeFile(const std::string &x, std::ostream &output)
{
	std::ifstream f(x);

	if (!f.is_open())
	{
		output << error() << "Unable to open file : " << x << std::endl;
	}
	else
	{
		executeUntilEOF(f, output);
	}

	f.close();
}

inline void BaseTerminal::commandHelp(std::istream &is, std::ostream &os)
{
	const char *genericHelp = "Type 'help' followed by the name of a command or variable to get help on that topic if available."
		"\nType listCmd, listCVars, and listHelp to print lists of the available commands, variables, and help topics."
		"\nUse $<varname> to dereference a variable in a command argument list and use # to comment the rest of a line";

	std::string x;

	if (is >> x)
	{
		HelpTable::const_iterator it = helpTable.find(x);

		if (it != helpTable.end())
		{
			os << (it->second) << std::endl;
		}
		else
		{
			os << error() << "No help available for topic: " << x << std::endl;
		}
	}
	else
	{
		os << genericHelp << std::endl;
	}
}

inline void BaseTerminal::listHelp(std::ostream &os) const
{

	COUTC("Available help topics:");
	for (HelpTable::const_iterator it = helpTable.begin(); it != helpTable.end(); it++)
	{
		COUTY("  %s", it->first.c_str());
	}

	os << std::endl;
}

inline void BaseTerminal::listCmd(std::ostream &os) const
{
	os << "\nAvailable commands:";

	for (CommandTable::const_iterator it = commandTable.begin(); it != commandTable.end(); it++)
	{
		os << "\n"
			<< it->first;
	}

	os << std::endl;
}

inline void BaseTerminal::listCVars(std::ostream &os) const
{
	os << "\nBound console variables:";

	for (CVarReadTable::const_iterator it = cvarReadFTable.begin(); it != cvarReadFTable.end(); it++)
	{
		os << "\n"
			<< it->first;
	}

	os << std::endl;
}

inline void BaseTerminal::commandSet(std::istream &is, std::ostream &os)
{
	std::string x;

	if (!(is >> x))
	{
		os << error() << "Syntax error parsing argument" << std::endl;
		return;
	}

	CVarReadTable::iterator it = cvarReadFTable.find(x);

	if (it != cvarReadFTable.end())
	{
		it->second(is, os);
	}
	else
	{
		os << error() << "Variable " << x << " unknown." << std::endl;
	}
}

/// the function associated with built in command "echo", which prints the value of a cvar if it is bound. if not, reports an error.
inline void BaseTerminal::commandEcho(std::istream &is, std::ostream &os)
{
	std::string x;

	if (!(is >> x))
	{
		os << error() << "Syntax error parsing argument." << std::endl;
		return;
	}

	CVarPrintTable::iterator it = cvarPrintFTable.find(x);

	if (it != cvarPrintFTable.end())
	{
		(it->second)(is, os);
	}
	else
	{
		os << error() << "Variable " << x << " unknown." << std::endl;
	}
}

inline void BaseTerminal::commandExecute(const std::string &str, std::ostream &output)
{
	std::stringstream lineStream;
	lineStream.str(str);
	commandExecute(lineStream, output);
}

///reads a string from the input stream and executes the command associated with it, if there is one.  if not, reports an error.
inline void BaseTerminal::commandExecute(std::istream &is, std::ostream &os)
{
	char ch;
	while (!is.eof())
	{
		ch = is.peek();

		if (ch == std::char_traits<char>::eof())
		{
			is.get(ch);
			return;
		}
		else if (ch == '#')
		{
			std::string tmp;
			getline(is, tmp);
			return;
		} //if newline we will not parse anything else
		else if (std::isspace(ch))
		{
			is.get(ch);
			continue;
		}
		else
		{
			break;
		}
	}

	std::stringstream lineStream;
	{
		std::string lineTemp;

		getline(is, lineTemp);

		history_buffer.push(lineTemp);

		os << echo() << lineTemp << std::endl;

		dereferenceVariables(is, os, lineTemp);

		lineStream.str(lineTemp); ///\todo this constrains us to a single line.  way to go later might be to require user or
								  ///generated command parser to return string that was parsed
	}

	std::string x;

	while (lineStream >> x)
	{
		CommandTable::const_iterator it = commandTable.find(x);

		if (it == commandTable.end())
		{
			os << error() << "Command " << x << " unknown" << std::endl;
		}
		else
		{
			(it->second)(lineStream, os); //execute the command
		}

		os << '\n';
	}
}

inline void BaseTerminal::bindBasicCommands()
{
	std::function<void(const std::string &, const DynamicVariable &)> f1 =

		std::bind(
			static_cast<void (BaseTerminal::*)(const std::string &, const DynamicVariable &)>(&BaseTerminal::bindDynamicCVar<DynamicVariable>),
			this, std::placeholders::_1, std::placeholders::_2);

	bindCommand("var", f1,
		"Type var <varname> <value> to declare a dynamic variable with name <varname> and value <value>."
		"\nVariable names are any space delimited string and variable value is set to the remainder of the line.");

	bindCommand("get-commands", [this](std::istream &is, std::ostream &os) { this->listCmd(os); }, "lists the available console commands");

	bindCommand("set", [this](std::istream &is, std::ostream &os) { this->commandSet(is, os); }, "type set <identifier> <val> to change the value of a cvar");

	bindCommand("echo", [this](std::istream &is, std::ostream &os) { this->commandEcho(is, os); }, "type echo <identifier> to print the value of a cvar");

	bindCommand("get-variables", [this](std::istream &is, std::ostream &os) { listCVars(os); }, "lists the bound cvars");

	bindCommand("help", [this](std::istream &is, std::ostream &os) { this->commandHelp(is, os); }, "you're a smartass");

	bindCommand("get-helptopics", [this](std::istream &is, std::ostream &os) { this->listHelp(os); }, "lists the available help topics");

	bindCommand("run-file", [this](std::istream &is, std::ostream &os) {
		std::string f;
		is >> f;
		this->executeFile(f, os);
	},
		"runs the commands in a text file named by the argument");
}

inline bool BaseTerminal::loadHistoryBuffer(const std::string &inFile)
{
	std::ifstream hfi(inFile);

	if (hfi.is_open())
	{
		loadHistoryBuffer(hfi);
		hfi.close();
		return true;
	}

	return false;
}

inline void BaseTerminal::saveHistoryBuffer(const std::string &outFile)
{
	if (history_buffer.size())
	{
		std::ofstream hfo(outFile);
		saveHistoryBuffer(hfo);
		hfo.close();
	}
}

inline void BaseTerminal::loadHistoryBuffer(std::istream &inFile)
{
	while (!inFile.eof())
	{

		std::string tmp;
		std::getline(inFile, tmp);

		if (tmp.length())
		{
			history_buffer.push(tmp);
		}
	}
}

inline void BaseTerminal::saveHistoryBuffer(std::ofstream &outfile)
{
	for (unsigned int i = 0; i < history_buffer.size(); i++)
	{
		outfile << history_buffer[i] << std::endl;
	}
}

inline const std::deque<std::string> &BaseTerminal::historyBuffer() const
{
	return history_buffer;
}

inline void BaseTerminal::dereferenceVariables(std::istream &is, std::ostream &os, std::string &str)
{
	size_t varBase = 0;
	int n = 0;

	while ((varBase = str.find('$', varBase)) != str.npos)
	{
		size_t substrEnd = varBase;
		size_t dollar = varBase;
		varBase++;

		n++;

		for (; ((substrEnd < str.size()) && (!isspace(str[substrEnd]))); substrEnd++)
			;

		if (substrEnd == varBase)
		{
			os << error() << "EXPECTED IDENTIFIER AT $" << std::endl;
		}
		else
		{
			std::string substr(str.substr(varBase, substrEnd - varBase));

			std::stringstream sstr;

			CVarPrintTable::iterator it = cvarPrintFTable.find(substr);

			// check that variable exists
			if (it != cvarPrintFTable.end())
			{
				it->second(is, sstr);
				str.replace(dollar, substrEnd, sstr.str());
			}
			else
			{
				os << error() << "Variable " << substr << " not found" << std::endl;
			}
		}
	}
}

inline BaseTerminal::BaseTerminal(size_t maxCapacity)
	: history_buffer(maxCapacity)
{
	bindBasicCommands();
}

template <typename O, typename... Args>
void BaseTerminal::bindMemberCommand(const std::string &commandName, O &obj, void (O::*fptr)(Args...), const std::string &help)
{
	auto mf = std::mem_fn(fptr);

	std::function<void(const Args...)> fp =
		[mf, &obj](const Args &... args) {
		mf(obj, args...);
	};

	bindCommand(commandName, fp);

	if (help.length())
	{
		setHelpTopic(commandName, help);
	}
}

/// here we just overload the iostream operators.  The only real difference is that on input a dynamic variable takes a full line from input
inline std::ostream &operator<<(std::ostream &instream, const BaseTerminal::DynamicVariable &var)
{
	const std::string &str = var;
	return instream << str;
}

/// dynamic variable definitions take a full line from the input stream
inline std::istream &operator >> (std::istream &istream, BaseTerminal::DynamicVariable &var)
{
	return getline(istream, var);
}

#endif //__SERVICE_TERMINAL_H__