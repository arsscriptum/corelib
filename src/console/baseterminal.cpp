
//==============================================================================
//
//  terminal.cpp
//
//  Lightweight and efficient, general purpose terminal to accept commands
//  and manipulate variables.
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================

#include "baseterminal.h"

/*
#include "log.h"
 BaseTerminal::EndOfLineEscapeStreamScope operator<<(std::ostream &os, const BaseTerminal::EndOfLineEscapeTag &tg)
{
    return BaseTerminal::EndOfLineEscapeStreamScope(tg, os);
}

template <class T>
 T BaseTerminal::makeTemp()
{
    T temp{};
    return temp;
}

template <typename... Args>
 void BaseTerminal::conditionalExecute(std::istream &is, std::ostream &os, std::function<void(Args...)> f, const Args &... args)
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
 void BaseTerminal::populateTemps(std::istream &is, FirstType &in)
{
    is >> in;
}

//variadic template that recursively parses our function arguments in order
template <typename FirstType, typename... Args>
 void BaseTerminal::populateTemps(std::istream &is, FirstType &in, Args &... Temps)
{
    is >> in;
    populateTemps(is, Temps...);
}

template <typename... Args>
 void BaseTerminal::goPopulateTemps(std::istream &is, Args &... temps)
{
    populateTemps(is, temps...);
}

template <typename... Args>
 void BaseTerminal::populateAndExecute(std::istream &is, std::ostream &os, std::function<void(Args...)> f,
                                                            typename std::remove_const<typename std::remove_reference<Args>::type>::type... temps)
{
    goPopulateTemps<typename std::remove_const<typename std::remove_reference<Args>::type>::type...>(is, temps...);

    conditionalExecute<Args...>(is, os, f, temps...);
}

//function that gets bound as type void to the console with a second function object that may have multiple arguments of various types
//this function gets called when the user enters the function name into the console.
template <typename... Args>
 void BaseTerminal::parse(std::istream &is, std::ostream &os, std::function<void(Args...)> f)
{

    //first we have to create a bunch of temp variables and pass them into the populateAndExecute function
    //the temp variables are needed to store the result.  There's no guarantee in c++ for argument evaluation order, so we can't just
    //skip the intermediate step of passing constructed temps into a second function
    populateAndExecute<Args...>(is, os, f, (makeTemp<typename std::remove_const<typename std::remove_reference<Args>::type>::type>())...);
}

 void BaseTerminal::bindCommand(const std::string &str, void (*fptr)(void), const std::string &help)
{
    commandTable[str] = [fptr](std::istream &, std::ostream &) { fptr(); };

    if (help.length())
        setHelpTopic(str, help);
}

template <typename... Args>
 void BaseTerminal::bindCommand(const std::string &str, void (*fptr)(Args...), const std::string &help)
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
 void BaseTerminal::bindCommand(const std::string &str, std::function<void(Args...)> fun, const std::string &help)
{
    commandTable[str] =
        [this, fun](std::istream &is, std::ostream &os) {
            this->parse<Args...>(is, os, fun);
        };

    if (help.length())
        setHelpTopic(str, help);
}

 void BaseTerminal::bindCommand(const std::string &str, ConsoleFunc fun, const std::string &help)
{
    if (help.length())
        setHelpTopic(str, help);

    commandTable[str] = fun;
}

 void BaseTerminal::setHelpTopic(const std::string &str, const std::string &data)
{
    helpTable[str] = data;
}

template <class T>
 void BaseTerminal::bindCVar(const std::string &str, T &var, const std::string &help)
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
 void BaseTerminal::printCvar(std::ostream &os, T *var)
{
    os << *var << std::endl;
}

template <class T>
 void BaseTerminal::assignDynamicVariable(std::istream &is, std::shared_ptr<T> var)
{
    is >> (*var);
}

template <class T>
 void BaseTerminal::writeDynamicVariable(std::ostream &os, std::shared_ptr<T> var)
{
    const T &deref = *(var);
    os << deref;
}

template <class T>
 void BaseTerminal::bindDynamicCVar(const std::string &var, const T &value, const std::string &help)
{
    helpTable[var] = help;
    bindDynamicCVar(var, value);
}

template <class T>
 void BaseTerminal::bindDynamicCVar(const std::string &var, const T &valueIn)
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

 void BaseTerminal::executeUntilEOF(std::istream &f, std::ostream &output)
{
    while (!f.eof())
    {
        commandExecute(f, output);
    }
}

 void BaseTerminal::executeFile(const std::string &x, std::ostream &output)
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

 void BaseTerminal::commandHelp(std::istream &is, std::ostream &os)
{
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
			COUTERR("No help available for topic: %s", x.c_str());
        }
    }
    else
    {
		COUTC("Type 'help' followed by the name of a command or variable to get help on that topic if available.");
    }
}

 void BaseTerminal::listHelp(std::ostream &os) const
{

	COUTC("Available help topics:");
    for (HelpTable::const_iterator it = helpTable.begin(); it != helpTable.end(); it++)
    {
		COUTY("  %s", it->first.c_str());
    }

    os << std::endl;
}

 void BaseTerminal::listCmd(std::ostream &os) const
{
    
	COUTC("Available commands:");
    for (CommandTable::const_iterator it = commandTable.begin(); it != commandTable.end(); it++)
    {
		COUTY("  %s", it->first.c_str());
    }

    os << std::endl;
}

 void BaseTerminal::listCVars(std::ostream &os) const
{
	COUTC("Bound console variables:");
    for (CVarReadTable::const_iterator it = cvarReadFTable.begin(); it != cvarReadFTable.end(); it++)
    {
		COUTY("  %s", it->first.c_str());
    }

    os << std::endl;
}

 void BaseTerminal::commandSet(std::istream &is, std::ostream &os)
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
 void BaseTerminal::commandEcho(std::istream &is, std::ostream &os)
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

 void BaseTerminal::commandExecute(const std::string &str, std::ostream &output)
{
    std::stringstream lineStream;
    lineStream.str(str);
    commandExecute(lineStream, output);
}

 void BaseTerminal::commandPrompt()
{
	std::clog << echo();

}
///reads a string from the input stream and executes the command associated with it, if there is one.  if not, reports an error.
 void BaseTerminal::commandExecute(std::istream &is, std::ostream &os)
{
    char ch;
	commandPrompt();
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

		COUTCMD("executing command '%s'", lineTemp.c_str());
        //os << echo() << lineTemp << std::endl;
		
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
			COUTERR("unknown command '%s'", x.c_str());
            //os << error() << "unknown command '" << x << "' - validate son!";
        }
        else
        {
            (it->second)(lineStream, os); //execute the command
        }

        os << '\n';
    }
}

void BaseTerminal::bindBasicCommands()
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

    bindCommand("runFile", [this](std::istream &is, std::ostream &os) {
        std::string f;
        is >> f;
        this->executeFile(f, os);
    },
                "runs the commands in a text file named by the argument");
}

 bool BaseTerminal::loadHistoryBuffer(const std::string &inFile)
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

 void BaseTerminal::saveHistoryBuffer(const std::string &outFile)
{
    if (history_buffer.size())
    {
        std::ofstream hfo(outFile);
        saveHistoryBuffer(hfo);
        hfo.close();
    }
}

 void BaseTerminal::loadHistoryBuffer(std::istream &inFile)
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

 void BaseTerminal::saveHistoryBuffer(std::ofstream &outfile)
{
    for (unsigned int i = 0; i < history_buffer.size(); i++)
    {
        outfile << history_buffer[i] << std::endl;
    }
}

 const std::deque<std::string> &BaseTerminal::historyBuffer() const
{
    return history_buffer;
}


*/


/*
 void BaseTerminal::dereferenceVariables(std::istream &is, std::ostream &os, std::string &str)
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

 BaseTerminal::BaseTerminal(size_t maxCapacity)
    : history_buffer(maxCapacity)
{
    bindBasicCommands();
}

 template <typename O, typename... Args>
 void  BaseTerminal::bindMemberCommand(const std::string &commandName, O &obj, void (O::*fptr)(Args...), const std::string &help)
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
 std::ostream &operator<<(std::ostream &instream, const BaseTerminal::DynamicVariable &var)
{
    const std::string &str = var;
    return instream << str;
}

/// dynamic variable definitions take a full line from the input stream
 std::istream &operator>>(std::istream &istream, BaseTerminal::DynamicVariable &var)
{
    return getline(istream, var);
}
*/