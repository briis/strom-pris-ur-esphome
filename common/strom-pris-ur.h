class StromPrisUtils : public Component
{
public:
    void setup() override
    {
        // This will be called by ESPHome when the component is set up
    }
    static std::string datetimeToString(const esphome::ESPTime &datetime);
    static int extractAndAdjustHour(const std::string &dateTime);
    static std::array<std::string, 2> findHighestValueIndex(const std::vector<std::string> &dataPoints);
    static std::array<std::string, 2> findLowestValueIndex(const std::vector<std::string> &dataPoints);
    static std::string getReportStart();
};

std::string StromPrisUtils::datetimeToString(const esphome::ESPTime &datetime)
{
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", std::localtime(&datetime.timestamp));
    return std::string(buffer);
}

std::string StromPrisUtils::getReportStart()
{
    std::time_t now = std::time(nullptr);
    std::tm tm = *std::gmtime(&now);
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    std::time_t startOfDay = std::mktime(&tm);
    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S.000Z", std::gmtime(&startOfDay));
    return std::string(buffer);
}

int StromPrisUtils::extractAndAdjustHour(const std::string &dateTime)
{
    // Extract the hour part from the dateTime string
    int hour = std::stoi(dateTime.substr(11, 2));

    // Adjust the hour
    if (hour == 23)
    {
        hour = 0;
    }
    else
    {
        hour += 1;
    }

    return hour;
}
std::array<std::string, 2> StromPrisUtils::findLowestValueIndex(const std::vector<std::string> &dataPoints)
{
    int minIndex = -1;
    double minValue = std::numeric_limits<double>::max();

    for (size_t i = 0; i < dataPoints.size(); ++i)
    {
        double value = std::stod(dataPoints[i]);
        if (value != -1 && value < minValue)
        {
            minValue = value;
            minIndex = i;
        }
    }

    if (minIndex == -1)
    {
        return {"No valid data", "No valid data"};
    }

    return {std::to_string(minIndex + 1), std::to_string(minValue)};
}

std::array<std::string, 2> StromPrisUtils::findHighestValueIndex(const std::vector<std::string> &dataPoints)
{
    int maxIndex = -1;
    double maxValue = std::numeric_limits<double>::lowest();

    for (size_t i = 0; i < dataPoints.size(); ++i)
    {
        double value = std::stod(dataPoints[i]);
        if (value != -1 && value > maxValue)
        {
            maxValue = value;
            maxIndex = i;
        }
    }

    if (maxIndex == -1)
    {
        return {"No valid data", "No valid data"};
    }

    return {std::to_string(maxIndex + 1), std::to_string(maxValue)};
}

// To convert a string to a int, you can use std::stoi
// To convert a string to a double, you can use std::stod