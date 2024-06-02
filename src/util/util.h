#pragma

namespace nasal::util {

bool is_windows();
bool is_linux();
bool is_macos();
bool is_x86();
bool is_amd64();
bool is_x86_64();
bool is_arm();
bool is_aarch64();
bool is_ia64();
bool is_powerpc();
bool is_superh();
const char* get_platform();
const char* get_arch();

}