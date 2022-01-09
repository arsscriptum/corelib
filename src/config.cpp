//==============================================================================
//
//   config.cpp
//
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================


#include "config.h"
#include <algorithm>
#include "string_utilities.h"
#include "file_utilities.h"

std::string ConfigValue::to_string(void) const
{
	return key + "=" + value;
}


void Config::clear(void)
{
	this->sections.clear();
}


Config::Config(void) :
	sections()
{
}

bool Config::loadConfigFile( std::string config_file) 
{
	std::string iniData = getFileContents(config_file);
	if (iniData.length() == 0) {
		return false;
	}
	return Config::loadConfigStream(iniData);
}

bool Config::loadConfigStream( std::string &config_data)  
{
	if (config_data.empty())
	{
		/* No config data, cannot be valid */
		return false;
	}

	const std::vector<std::string> lines = StringHelpers::getInstance()->split_string(config_data, "\n");
	if (lines.size() < 2)
	{
		/* not enough lines in config file to be meaningful */
		return false;
	}

	this->sections.clear();

	ConfigSection *current_section = nullptr;

	for (std::string line : lines)
	{
		line = StringHelpers::getInstance()->trim_string_remove_new_line(line);
		if (line.size())
		{
			if (line[0] == '[')
			{
				/* this is a (new) section */
				const std::string section_name = StringHelpers::getInstance()->get_string_between_two_chars(line, '[', ']');

				if (section_name.size())
				{
					if (current_section)
					{
						this->sections.push_back(*current_section);
						delete current_section;
					}
					current_section = new ConfigSection(section_name);
				}
			}
			else if ((line[0] != '#') && (line[0] != ';'))       /* do not process comment lines */
			{
				/* only process lines when we have a section */
				if (current_section)
				{
					/* this can be a key value pair, find out */
					const std::vector<std::string> splitted = StringHelpers::getInstance()->split_string_once(line, '=');
					if (splitted.size() == 2)
					{
						/* yes this is a key = value line */
						ConfigValue cv;
						
						cv.key = StringHelpers::getInstance()->getInstance()->trim_string(splitted[0]); /* trim everything before = */
						const std::string value = splitted[1];
						/* and also strip comments and trim after = */
						const std::vector<std::string> value_split = StringHelpers::getInstance()->split_string_once(value, '#');
						const std::string value_without_comment = value_split[0];
						cv.value = StringHelpers::getInstance()->getInstance()->trim_string(value_without_comment);

						current_section->section_members.push_back(cv);
					}
				}
			}
		}
	}

	if (current_section)
	{
		this->sections.push_back(*current_section);
		delete current_section;

		return true;
	}

	return false;

}

std::vector<ConfigSection> Config::get_sections(void) const
{
	return this->sections;
}

ConfigSection Config::get_section(const std::string &section_name) const
{
	auto it = std::find_if(sections.begin(), sections.end(), [&section_name](const ConfigSection &section)
	{
		return section.get_name() == section_name;
	});

	if (it != sections.end())
	{
		return *it;
	}

	return ConfigSection("");       // TODO decide if we want to throw an exception
}

bool Config::has_section(const std::string &section_name) const
{
	if (section_name.empty())
	{
		return false;
	}

	const ConfigSection &s = get_section(section_name);
	if (s.get_name() == section_name)
	{
		return true;
	}

	return false;
}

std::string Config::get_section_member(const std::string &section, const std::string &member, const std::string &default_value) const
{
	const ConfigSection config_section = get_section(section);
	if (!config_section.get_name().empty())
	{
		for (const ConfigValue &value : config_section.section_members)
		{
			if (value.key == member)
			{
				return value.value;
			}
		}
	}

	return default_value;
}

std::string Config::to_string(void) const
{
	std::string data;

	for (const ConfigSection &section : this->sections)
	{
		data += section.to_string();
	}

	return data;
}

bool Config::status(void) const
{
	return !this->sections.empty();
}


bool ConfigSection::has_member(const std::string &name) const
{
	return std::any_of(this->section_members.begin(), this->section_members.end(), [&name](const ConfigValue &v)
	{
		return v.key == name;
	});
}


bool ConfigSection::has_members(const std::vector<std::string> &names) const
{
	for (const std::string &item : names)
	{
		if (this->has_member(item) == false)
		{
			return false;
		}
	}

	return true;
}


std::string ConfigSection::get_member(const std::string &member) const
{
	for (const ConfigValue &v : this->section_members)
	{
		if (v.key == member)
		{
			return v.value;
		}
	}

	return "";
}


ConfigSection::ConfigSection(const std::string &section_name) :
	section_members(),
	section_name(section_name)
{
}

std::string ConfigSection::to_string(void) const
{
	std::string data;

	data = "[" + this->section_name + "]\n";
	for (const ConfigValue &item : this->section_members)
	{
		data += item.to_string() + "\n";
	}
	data += "\n";

	return data;
}

std::string ConfigSection::get_name(void) const
{
	return this->section_name;
}