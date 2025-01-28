#include <iostream>
#include <thread>
#include <filesystem>

void launch_easy(const std::string& base_path) {
    std::cout << "Launching Easy Image..." << std::endl;
    system((base_path + "\\easy.exe").c_str());
    std::cout << "Easy Image finished." << std::endl;
}

void launch_medium(const std::string& base_path) {
    std::cout << "Launching Medium Image..." << std::endl;
    system((base_path + "\\medium.exe").c_str());
    std::cout << "Medium Image finished." << std::endl;
}

int main() {
    // Get the directory of the running executable
    std::string base_path = std::filesystem::current_path().string() + "\\Debug";

    // Launch each executable in threads
    std::thread easy_thread(launch_easy, base_path);
    std::thread medium_thread(launch_medium, base_path);

    // Wait for all threads to finish
    easy_thread.join();
    medium_thread.join();

    std::cout << "All images finished." << std::endl;

    return 0;
}
