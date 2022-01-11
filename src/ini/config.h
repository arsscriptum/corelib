//==============================================================================
//
//   config.h - exported_h
//
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================

#ifndef __INI_CONFIG_H__
#define __INI_CONFIG_H__


#include <string>
#include <vector>


class ConfigValue
{
public:
	
	ConfigValue(void) : key(), value() {}
	std::string key;
	std::string value;

	std::string to_string(void) const;
};

class ConfigSection
{
public:
	explicit ConfigSection(const std::string &section_name);

	std::vector<ConfigValue> section_members;
	std::string to_string(void) const;
	std::string get_name(void) const;

	bool has_member(const std::string &name) const;
	bool has_members(const std::vector<std::string> &names) const;

	std::string get_member(const std::string &member) const;

private:
	std::string section_name;
};



class Config
{
public:
	Config(void);
	virtual ~Config(void) {}

	bool loadConfigStream( std::string &config_data);
	bool loadConfigFile( std::string config_file) ;
	std::vector<ConfigSection> get_sections(void) const;
	ConfigSection get_section(const std::string &section_name) const;
	bool has_section(const std::string &section_name) const;

	std::string get_section_member(const std::string &section, const std::string &member,
		const std::string &default_value = "") const;

	std::string to_string(void) const;

	bool status(void) const;

	void clear(void);

protected:
	std::vector<ConfigSection> sections;
};


#endif //__INI_CONFIG_H__