#include "FileReverser/Big5FileReverser.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <chrono>

int main()
{
    const char* sourcePath = "input.txt";
    const char* goldenPath = "input_golden.txt";

    {
        const char* destinationPath = "input_mmap.txt";
        std::filesystem::copy(sourcePath, destinationPath, std::filesystem::copy_options::overwrite_existing);
        auto mmapReverser = Big5FileReverser::MakeMmapReverser(destinationPath);

        auto start = std::chrono::high_resolution_clock::now();
        if (mmapReverser->process())
        {
            // Processing succeeded
        }
        else
        {
            std::cerr << "Mmap processing failed." << std::endl;
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Mmap processing time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms" << std::endl;

        std::ifstream a(destinationPath), b(goldenPath);
        if (std::equal(std::istreambuf_iterator<char>(a), {}, std::istreambuf_iterator<char>(b))) 
        {
            std::cout << "Mmap processing result matches the golden file." << std::endl;
        }
        else 
        {
            std::cerr << "Mmap processing result does not match the golden file." << std::endl;
        }

        std::filesystem::remove(destinationPath); // Clean up the copied file
    }

    {
        const char* destinationPath = "input_stream.txt";
        std::filesystem::copy(sourcePath, destinationPath, std::filesystem::copy_options::overwrite_existing);
        auto streamReverser = Big5FileReverser::MakeStreamReverser(destinationPath);

        auto start = std::chrono::high_resolution_clock::now();
        if (streamReverser->process())
        {
            // Processing succeeded
        }
        else
        {
            std::cerr << "Stream processing failed." << std::endl;
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Stream processing time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms" << std::endl;

        std::ifstream a(destinationPath), b(goldenPath);
        if (std::equal(std::istreambuf_iterator<char>(a), {}, std::istreambuf_iterator<char>(b))) 
        {
            std::cout << "Stream processing result matches the golden file." << std::endl;
        }
        else 
        {
            std::cerr << "Stream processing result does not match the golden file." << std::endl;
        }

        std::filesystem::remove(destinationPath); // Clean up the copied file
    }

    return 0;
}
