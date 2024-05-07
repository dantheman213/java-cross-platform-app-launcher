#include "JavaLauncher.h"

#include <string>
#if WIN32
#include <windows.h>
#endif

#include "MessageBox.h"

// EDIT THESE VALUES
const int MINIMUM_JAVA_MAJOR_VERSION_REQUIRED = 21;
const std::string JAR_PATH = "app.jar"; // relative path
//

#if WIN32
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
    int main() {
#endif
    LaunchJar(JAR_PATH);
    return 0;
}
