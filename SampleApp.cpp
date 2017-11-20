#include <Poco/Util/Application.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/AutoPtr.h>
#include <iostream>
#include <sstream>

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;

class SampleApp : public Application
{
public:
	SampleApp():_helpRequested(false)
	{
	}

protected:
	void initialize(Application &self)
    {
        loadConfiguration();
        Application::initialize(self);
    }

    void uninitialize()
    {
        Application::uninitialize();
    }

    void reinitialize(Application &self)
    {
        Application::reinitialize(self);
    }

    void defineOptions(OptionSet &options)
    {
        Application::defineOptions(options);

        options.addOption(
            Option("help", "h", "display help information on command lines arguments")
                .required(false)
                .repeatable(false)
                .callback(OptionCallback<SampleApp>(this, &SampleApp::handleHelp))
            );

        options.addOption(
                Option("define", "D", "define a configuration property")
                    .required(false)
                    .repeatable(true)
                    .argument("name=value")
                    .callback(OptionCallback<SampleApp>(this, &SampleApp::handleDefine))
            );

        options.addOption(
                Option("config-file", "f", "load configuration data from a file")
                    .required(false)
                    .repeatable(true)
                    .argument("filename")
                    .callback(OptionCallback<SampleApp>(this, &SampleApp::handleConfig))
            );

        options.addOption(
                Option("bind", "b", "bind option value to test.property")
                    .required(false)
                    .repeatable(false)
                    .argument("value")
                    .binding("test.property")
            );
    }

    void handleHelp(const std::string &name, const std::string &value)
    {
        _helpRequested = true;
        displayHelp();
        stopOptionsProcessing();
    }


    void displayHelp()
    {
        HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTTIONS");
        helpFormatter.setHeader("A sample application that demonstrates some of the features of the Poco::Util::Application class.");
        helpFormatter.format(std::cout);
    }

    void handleDefine(const std::string &namre, const std::string & value)
    {
        defineProperty(value);
    }

    void handleConfig(const std::string &name, const std::string &value)
    {
        loadConfiguration(value);
    }

    void defineProperty(const std::string &def)
    {
        std::string name;
        std::string value;
        std::string::size_type pos = def.find('=');
        if (pos != std::string::npos)
        {
            name.assign(def, 0, pos);
            value.assign(def, pos+1, def.length()-pos);
        }
        else
            name = def;
        config().setString(name, value);
    }

    int main(const ArgVec &args)
    {
        if (!_helpRequested)
        {
            logger().information("Command line:");
            std::ostringstream ostr;
            for (ArgVec::const_iterator it = argv().begin(); it != argv().end(); ++it)
            {
                ostr << *it << ' ';
            }
            logger().information(ostr.str());
            logger().information("Arguments to main():");
            for (ArgVec::const_iterator it = args.begin(); it != args.end(); ++it)
            {
                logger().information(*it);
            }
            logger().information("Application properties:");
            printProperties("");
        }
        return Application::EXIT_OK;
    }

    void printProperties(const std::string& base)
    {
        AbstractConfiguration::Keys keys;
        config().keys(base, keys);
        if (keys.empty())
        {
            if (config().hasProperty(base))
            {
                std::string msg;
                msg.append(base);
                msg.append(" = ");
                msg.append(config().getString(base));
                logger().information(msg);
            }
        }
        else
        {
            for (AbstractConfiguration::Keys::const_iterator it = keys.begin(); it != keys.end(); ++it)
            {
                std::string fullKey = base;
                if (!fullKey.empty()) fullKey += '.';
                fullKey.append(*it);
                printProperties(fullKey);
            }
        }
    }
    
private:
	bool _helpRequested;
};

POCO_APP_MAIN(SampleApp)
