//
// Created by Gusts Kaksis on 02/01/2024.
//

#ifndef TESTS_UTILITIES_HPP
#define TESTS_UTILITIES_HPP

#include <jni.h>
#include <android/log.h>
#include <vector>
#include <thread>

void copyArgs(JNIEnv *env, jobjectArray jargv, std::vector<std::vector<char>>& storage, std::vector<char*>& ptrs)
{
    int argc = env->GetArrayLength(jargv);
    ptrs.resize(argc + 1, nullptr);
    storage.resize(argc + 1);
    // args include the executable name, so we cheat it here with empty entry
    storage[0].resize(1);
    storage[0][0] = 0;
    ptrs[0] = storage[0].data();
    for (int i = 0; i < argc; ++i)
    {
        auto el = env->GetObjectArrayElement(jargv, i);
        if (auto str = static_cast<jstring>(el))
        {
            jboolean isCopy = JNI_FALSE;
            auto size = env->GetStringUTFLength(str);
            const auto* chars = env->GetStringUTFChars(str, &isCopy);
            storage[i + 1].resize(size + 1);
            auto* ptr = storage[i + 1].data();
            ptrs[i + 1] = ptr;
            if (size)
            {
                std::memcpy(ptr, chars, static_cast<std::size_t>(size));
            }
            ptr[size] = 0;
        }
    }
}

class LogCapture
{
public:
    LogCapture()
        : thread(std::bind(&LogCapture::runCapture, this))
    {}
    ~LogCapture()
    {
        std::unique_lock lock { mutex };
        isRunning = false;
        if (thread.joinable())
        {
            lock.unlock();
            thread.join();
        }

    }
private:
    std::thread thread;
    bool isRunning { true };
    std::mutex mutex;

    void runCapture()
    {
        std::unique_lock lock { mutex };
        int pipes[2];
        pipe(pipes);
        dup2(pipes[1], STDOUT_FILENO);
        FILE *inputFile = fdopen(pipes[0], "r");
        char readBuffer[256];
        char* res = nullptr;
        while (isRunning || res)
        {
            lock.unlock();
            res = fgets(readBuffer, sizeof(readBuffer), inputFile);
            __android_log_write(2, "stderr", readBuffer);
            lock.lock();
        }
    }
};

#endif //TESTS_UTILITIES_HPP
