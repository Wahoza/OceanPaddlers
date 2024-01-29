#pragma once

#include <SFML/System.hpp>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>

namespace mmt_gd
{
/**
 * \brief print error message with file and line number
 */
#define ffErrorMsg(Text)                                                              \
    {                                                                                 \
        sf::err() << __FILE__ << " Line: " << __LINE__ << " " << (Text) << std::endl; \
    }


#ifdef _DEBUG
/**
 * \brief print error message and assert
 */
#define ffAssertMsg(Expression, Text)                                                     \
    {                                                                                     \
        if (!(Expression))                                                                \
        {                                                                                 \
            sf::err() << __FILE__ << " Line: " << __LINE__ << " " << (Text) << std::endl; \
            assert(Expression);                                                           \
        }                                                                                 \
    }
#else
#define ffAssertMsg(Expression, Text)
#endif


#define PROFILING _DEBUG && true ///< set false to deactivate Profiling
#ifdef PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION()  PROFILE_SCOPE(__FUNCTION__)
#else
#define PROFILE_SCOPE(name)
#endif

/**
 * \brief View Profiling results in Google Chrome chrome://tracing/
 */
struct ProfileResult
{
    const std::string name;
    long long         start, end;
    uint32_t          threadID;
};

class Instrumentor
{
    std::string   m_sessionName = "None";
    std::ofstream m_outputStream;
    int           m_profileCount = 0;
    std::mutex    m_lock;
    bool          m_activeSession = false;

    Instrumentor() = default;

public:
    static Instrumentor& instance()
    {
        static Instrumentor instance;
        return instance;
    }

    ~Instrumentor()
    {
        endSession();
    }

    void beginSession(const std::string& name, const std::string& filepath = "results.json")
    {
        if (m_activeSession)
        {
            endSession();
        }
        m_activeSession = true;
        m_outputStream.open(filepath);
        writeHeader();
        m_sessionName = name;
    }

    void endSession()
    {
        if (!m_activeSession)
        {
            return;
        }
        m_activeSession = false;
        writeFooter();
        m_outputStream.close();
        m_profileCount = 0;
    }

    void writeProfile(const ProfileResult& result)
    {
        std::lock_guard lock(m_lock);

        if (m_profileCount++ > 0)
        {
            m_outputStream << ",";
        }

        std::string name = result.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_outputStream << "{";
        m_outputStream << R"("cat":"function",)";
        m_outputStream << "\"dur\":" << result.end - result.start << ',';
        m_outputStream << R"("name":")" << name << "\",";
        m_outputStream << R"("ph":"X",)";
        m_outputStream << "\"pid\":0,";
        m_outputStream << "\"tid\":" << result.threadID << ",";
        m_outputStream << "\"ts\":" << result.start;
        m_outputStream << "}";
    }

    void writeHeader()
    {
        m_outputStream << R"({"otherData": {},"traceEvents":[)";
    }

    void writeFooter()
    {
        m_outputStream << "]}";
    }
};

class InstrumentationTimer
{
    ProfileResult m_result;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
    bool                                                        m_stopped{false};

public:
    explicit InstrumentationTimer(const std::string& name) : m_result({name, 0, 0, 0})


    {
        m_startTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!m_stopped)
        {
            stop();
        }
    }

    void stop()
    {
        const auto endTimepoint = std::chrono::high_resolution_clock::now();

        m_result.start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
        m_result.end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
        m_result.threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::instance().writeProfile(m_result);

        m_stopped = true;
    }
};
} // namespace mmt_gd
