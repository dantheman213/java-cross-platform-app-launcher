#include <string>

extern const int MINIMUM_JAVA_MAJOR_VERSION_REQUIRED;
extern const std::string JAR_PATH;

bool IsJavaVersionCorrect();
void LaunchJar(const std::string& jar_path);
std::string ExecuteCommandAndGetOutput(const std::string& command);